#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <stack>
using namespace std;

// Function to check if a string is a valid variable name in C++
bool isValidVariable(const string &var) {
    regex varPattern("^[a-zA-Z_][a-zA-Z0-9_]*$");
    return regex_match(var, varPattern);
}

// Function to check if the string is a valid expression
bool isValidExpression(const string &expr) {
    if (expr.empty()) return true; // Empty expressions are valid in for loops
    
    // Simple validation - this could be expanded for more complex checking
    int parenCount = 0;
    for (char c : expr) {
        if (c == '(') parenCount++;
        if (c == ')') parenCount--;
        if (parenCount < 0) return false;
    }
    return parenCount == 0;
}

// Function to check if the given for loop is valid
bool checkForLoop(const string &code) {
    // Check if the statement starts with "for"
    regex forPattern("^\\s*for\\s*\\(.*\\)\\s*\\{.*\\}\\s*$");
    if (!regex_match(code, forPattern)) {
        cout << "Error: Invalid for loop syntax. Must follow 'for (init; condition; update) { body }' pattern." << endl;
        return false;
    }
    
    // Extract the parts inside the parentheses
    size_t openParen = code.find('(');
    size_t closeParen = code.find(')', openParen);
    if (openParen == string::npos || closeParen == string::npos) {
        cout << "Error: Missing or mismatched parentheses." << endl;
        return false;
    }
    
    string partsInside = code.substr(openParen + 1, closeParen - openParen - 1);
    
    // Split into three sections by semicolons
    vector<string> sections;
    size_t pos = 0, found;
    while ((found = partsInside.find(';', pos)) != string::npos) {
        sections.push_back(partsInside.substr(pos, found - pos));
        pos = found + 1;
    }
    sections.push_back(partsInside.substr(pos)); // Add the last part
    
    if (sections.size() != 3) {
        cout << "Error: For loop requires three sections (initialization; condition; update)." << endl;
        return false;
    }
    
    // Trim whitespace from sections
    for (string &section : sections) {
        section.erase(0, section.find_first_not_of(" \t"));
        section.erase(section.find_last_not_of(" \t") + 1);
    }
    
    // Check initialization section (can be empty, a declaration, or an expression)
    string init = sections[0];
    if (!init.empty()) {
        // Check if it's a variable declaration
        regex declPattern("^(int|float|double|char|long|short|unsigned|signed)(\\s+[a-zA-Z_][a-zA-Z0-9_]*)(\\s*=\\s*.+)?$");
        bool isDeclaration = regex_match(init, declPattern);
        
        if (!isDeclaration && !isValidExpression(init)) {
            cout << "Error: Invalid initialization in for loop." << endl;
            return false;
        }
    }
    
    // Check condition section (can be empty or a boolean expression)
    string condition = sections[1];
    if (!condition.empty() && !isValidExpression(condition)) {
        cout << "Error: Invalid condition in for loop." << endl;
        return false;
    }
    
    // Check update section (can be empty or an expression)
    string update = sections[2];
    if (!update.empty() && !isValidExpression(update)) {
        cout << "Error: Invalid update expression in for loop." << endl;
        return false;
    }
    
    // Check for balanced braces in the body
    size_t openBrace = code.find('{', closeParen);
    size_t closeBrace = code.find_last_of('}');
    
    if (openBrace == string::npos || closeBrace == string::npos || openBrace > closeBrace) {
        cout << "Error: Missing or mismatched braces for loop body." << endl;
        return false;
    }
    
    // Check for balanced braces in the body content
    string body = code.substr(openBrace, closeBrace - openBrace + 1);
    stack<char> braceStack;
    for (char c : body) {
        if (c == '{') braceStack.push(c);
        else if (c == '}') {
            if (braceStack.empty()) {
                cout << "Error: Mismatched closing brace in loop body." << endl;
                return false;
            }
            braceStack.pop();
        }
    }
    
    if (!braceStack.empty()) {
        cout << "Error: Unclosed braces in loop body." << endl;
        return false;
    }
    
    cout << "Valid for loop syntax." << endl;
    return true;
}

int main() {
    string code;
    cout << "Enter a C++ for loop: ";
    getline(cin, code);
    checkForLoop(code);
    return 0;
}
