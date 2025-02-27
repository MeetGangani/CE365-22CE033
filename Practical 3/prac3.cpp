#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <iomanip>
using namespace std;

// Global sets and maps
set<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

set<string> ops = {
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||",
    "!", "&", "|", "^", "~", "<<", ">>", "+=", "-=", "*=", "/=", "%="
};

set<string> puncs = {"(", ")", "{", "}", "[", "]", ";", ","};

map<string, int> symTable;
map<int, vector<string>> errList;

// Function declarations
bool isChar(const string& tok);
bool isStr(const string& tok);
bool isNum(const string& tok);
bool isIdentifier(const string& tok);
vector<string> tokenize(const string& line);
void processLine(const string& line, int line_no);
void addErr(int line_no, const string& err);
void printSymbolTable();
void printErrors();

bool isChar(const string& tok) {
    return tok.length() >= 3 && tok[0] == '\'' && tok[tok.length()-1] == '\'';
}

bool isStr(const string& tok) {
    return tok.length() >= 2 && tok[0] == '\"' && tok[tok.length()-1] == '\"';
}

void addErr(int line_no, const string& err) {
    errList[line_no].push_back(err);
}

bool isNum(const string& tok) {
    try {
        // Handle floating point
        if (tok.find('.') != string::npos) {
            size_t pos;
            stod(tok, &pos);
            return pos == tok.length();
        }
        // Handle integers
        size_t pos;
        stol(tok, &pos);
        return pos == tok.length();
    }
    catch (...) {
        return false;
    }
}

bool isIdentifier(const string& tok) {
    if (tok.empty()) return false;
    if (!isalpha(tok[0]) && tok[0] != '_') return false;
    for (char c : tok) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

vector<string> tokenize(const string& line) {
    vector<string> toks;
    string currTok;
    bool inStr = false, inChr = false;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        char nextC = (i < line.length() - 1) ? line[i + 1] : ' ';

        if (c == '\"' && !inChr) {
            inStr = !inStr;
            currTok += c;
            if (!inStr) {
                toks.push_back(currTok);
                currTok.clear();
            }
            continue;
        }

        if (c == '\'' && !inStr) {
            inChr = !inChr;
            currTok += c;
            if (!inChr) {
                toks.push_back(currTok);
                currTok.clear();
            }
            continue;
        }

        if (inStr || inChr) {
            currTok += c;
            continue;
        }

        // Check for two-character operators
        string maybeOp = string(1, c) + string(1, nextC);
        if (ops.find(maybeOp) != ops.end()) {
            if (!currTok.empty()) {
                toks.push_back(currTok);
                currTok.clear();
            }
            toks.push_back(maybeOp);
            i++; // Skip next character
            continue;
        }

        if (isspace(c)) {
            if (!currTok.empty()) {
                toks.push_back(currTok);
                currTok.clear();
            }
        }
        else if (ops.find(string(1, c)) != ops.end() || puncs.find(string(1, c)) != puncs.end()) {
            if (!currTok.empty()) {
                toks.push_back(currTok);
                currTok.clear();
            }
            toks.push_back(string(1, c));
        }
        else {
            currTok += c;
        }
    }

    if (!currTok.empty()) {
        toks.push_back(currTok);
    }

    return toks;
}

void processLine(const string& line, int line_no) {
    vector<string> tokens = tokenize(line);

    for (const string& tok : tokens) {
        if (tok.empty()) continue;

        if (keywords.find(tok) != keywords.end()) {
            cout << "Keyword: " << tok << endl;
        }
        else if (ops.find(tok) != ops.end()) {
            cout << "Operator: " << tok << endl;
        }
        else if (puncs.find(tok) != puncs.end()) {
            cout << "Punctuation: " << tok << endl;
        }
        else if (isNum(tok)) {
            cout << "Number: " << tok << endl;
        }
        else if (isChar(tok)) {
            cout << "Character Constant: " << tok << endl;
        }
        else if (isStr(tok)) {
            cout << "String Constant: " << tok << endl;
        }
        else if (isIdentifier(tok)) {
            cout << "Identifier: " << tok << endl;
            symTable[tok] = line_no;
        }
        else {
            addErr(line_no, "Invalid token: " + tok);
        }
    }
}

void printSymbolTable() {
    cout << "\nSymbol Table:" << endl;
    cout << "=========================" << endl;
    cout << left << setw(15) << "Variable" << setw(10) << "Line no" << endl;
    cout << "=========================" << endl;

    for (const auto& entry : symTable) {
        cout << left << setw(15) << entry.first << setw(10) << entry.second << endl;
    }
}

void printErrors() {
    if (errList.empty()) {
        cout << "\nNo errors found." << endl;
        return;
    }

    cout << "\nErrors Found:" << endl;
    cout << "=========================" << endl;

    for (const auto& entry : errList) {
        for (const string& err : entry.second) {
            cout << "Line " << entry.first << ": " << err << endl;
        }
    }
}

int main() {
    ifstream file("test.c");
    if (!file.is_open()) {
        cout << "File not Found!" << endl;
        return 1;
    }

    cout << " test.c" << endl;
    int line_no = 1;
    bool inComment = false;
    string line;

    while (getline(file, line)) {
        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) {
            line_no++;
            continue;
        }

        if (inComment) {
            size_t endComment = line.find("*/");
            if (endComment != string::npos) {
                string afterComment = line.substr(endComment + 2);
                afterComment.erase(0, afterComment.find_first_not_of(" \t"));
                
                if (!afterComment.empty()) {
                    processLine(afterComment, line_no);
                }
                inComment = false;
            }
            line_no++;
            continue;
        }

        if (line.substr(0, 2) == "//") {
            cout << "Comment at line " << line_no << ": " << line << endl;
            line_no++;
            continue;
        }

        size_t commentStart = line.find("/*");
        if (commentStart != string::npos) {
            string beforeComment = line.substr(0, commentStart);
            beforeComment.erase(0, beforeComment.find_first_not_of(" \t"));
            
            if (!beforeComment.empty()) {
                processLine(beforeComment, line_no);
            }

            cout << "Comment at line " << line_no << ": ";
            size_t commentEnd = line.find("*/", commentStart);
            if (commentEnd != string::npos) {
                cout << line.substr(commentStart, commentEnd - commentStart + 2) << endl;
                
                string afterComment = line.substr(commentEnd + 2);
                afterComment.erase(0, afterComment.find_first_not_of(" \t"));
                
                if (!afterComment.empty()) {
                    processLine(afterComment, line_no);
                }
            }
            else {
                cout << line.substr(commentStart) << endl;
                inComment = true;
            }
            line_no++;
            continue;
        }

        processLine(line, line_no);
        line_no++;
    }

    printSymbolTable();
    printErrors();
    
    file.close();
    return 0;
}