#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <stack>
#include <iomanip>
#include <set>

using namespace std;

class LoopSyntaxAnalyzer {
private:
    struct LoopInfo {
        string type;           // "for", "while", "do-while"
        string initialization; // for initialization part
        string condition;      // condition for all loop types
        string iteration;      // for increment/decrement part
        int lineNumber;        // where the loop starts
        bool isValid;          // syntax validity
        string errorMessage;   // error description if invalid
    };

    vector<string> sourceCode;
    vector<LoopInfo> loops;
    
    // Helper function to trim whitespace
    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    }
    
    // Check balanced parentheses
    bool hasBalancedParentheses(const string& str) {
        stack<char> s;
        for (char c : str) {
            if (c == '(') {
                s.push(c);
            } else if (c == ')') {
                if (s.empty() || s.top() != '(') {
                    return false;
                }
                s.pop();
            }
        }
        return s.empty();
    }
    
    // Check balanced braces
    bool hasBalancedBraces(const string& str) {
        stack<char> s;
        for (char c : str) {
            if (c == '{') {
                s.push(c);
            } else if (c == '}') {
                if (s.empty() || s.top() != '{') {
                    return false;
                }
                s.pop();
            }
        }
        return s.empty();
    }
    
    // Extract content between outermost parentheses
    string extractParenthesesContent(const string& str) {
        size_t start = str.find('(');
        if (start == string::npos) return "";
        
        int level = 0;
        size_t end = 0;
        
        for (size_t i = start + 1; i < str.length(); i++) {
            if (str[i] == '(') {
                level++;
            } else if (str[i] == ')') {
                if (level == 0) {
                    end = i;
                    break;
                }
                level--;
            }
        }
        
        if (end <= start) return "";
        return str.substr(start + 1, end - start - 1);
    }
    
    // Analyze for loop
    void analyzeForLoop(const string& line, int lineNum) {
        LoopInfo loop;
        loop.type = "for";
        loop.lineNumber = lineNum;
        loop.isValid = true;
        
        // Check basic syntax - updated to be more lenient
        regex forPattern(R"(for\s*\(.*\).*?)");
        if (!regex_match(line, forPattern)) {
            loop.isValid = false;
            loop.errorMessage = "Invalid for loop syntax";
            loops.push_back(loop);
            return;
        }
        
        // Extract and check the for loop parts
        string content = extractParenthesesContent(line);
        if (content.empty()) {
            loop.isValid = false;
            loop.errorMessage = "Empty or invalid for loop parameters";
            loops.push_back(loop);
            return;
        }
        
        // Split into initialization, condition, iteration
        vector<string> parts;
        string part;
        int level = 0;
        bool inQuotes = false;
        
        for (char c : content) {
            if (c == '"' && (part.empty() || part.back() != '\\')) {
                inQuotes = !inQuotes;
            }
            
            if (!inQuotes) {
                if (c == '(') level++;
                else if (c == ')') level--;
            }
            
            if (c == ';' && level == 0 && !inQuotes) {
                parts.push_back(trim(part));
                part.clear();
            } else {
                part += c;
            }
        }
        
        if (!part.empty()) {
            parts.push_back(trim(part));
        }
        
        // A for loop should have 3 parts: initialization, condition, iteration
        if (parts.size() != 3) {
            loop.isValid = false;
            loop.errorMessage = "For loop should have 3 parts separated by semicolons";
            loops.push_back(loop);
            return;
        }
        
        loop.initialization = parts[0];
        loop.condition = parts[1];
        loop.iteration = parts[2];
        
        // Validate each part
        if (loop.initialization.empty() && loop.condition.empty() && loop.iteration.empty()) {
            // Empty for loop (for(;;)) is actually valid in C++
            loop.isValid = true;
        } else {
            // Check for common errors in each part
            if (!loop.initialization.empty()) {
                // Initialization typically contains variable declaration or assignment
                if (regex_search(loop.initialization, regex(R"(for\s*\()"))) {
                    loop.isValid = false;
                    loop.errorMessage = "Nested 'for' keyword in initialization part";
                }
            }
            
            // Condition can be any expression or empty
            if (!loop.condition.empty() && loop.condition.find_first_not_of(" \t") == string::npos) {
                loop.isValid = false;
                loop.errorMessage = "Invalid condition in for loop";
            }
        }
        
        loops.push_back(loop);
    }
    
    // Analyze while loop
    void analyzeWhileLoop(const string& line, int lineNum) {
        LoopInfo loop;
        loop.type = "while";
        loop.lineNumber = lineNum;
        loop.isValid = true;
        
        // Check basic syntax - updated to be more lenient
        regex whilePattern(R"(while\s*\(.*\).*?)");
        if (!regex_match(line, whilePattern)) {
            loop.isValid = false;
            loop.errorMessage = "Invalid while loop syntax";
            loops.push_back(loop);
            return;
        }
        
        // Extract and check the condition
        loop.condition = extractParenthesesContent(line);
        if (loop.condition.empty()) {
            loop.isValid = false;
            loop.errorMessage = "Empty or invalid while loop condition";
            loops.push_back(loop);
            return;
        }
        
        // Condition can be any non-empty expression
        if (loop.condition.find_first_not_of(" \t") == string::npos) {
            loop.isValid = false;
            loop.errorMessage = "Invalid condition in while loop";
        }
        
        loops.push_back(loop);
    }
    
    // Analyze do-while loop
    void analyzeDoWhileLoop(const string& line, int lineNum) {
        LoopInfo loop;
        loop.type = "do-while";
        loop.lineNumber = lineNum;
        loop.isValid = true;
        
        // For do-while, we check for the while part with a semicolon - updated to be more lenient
        regex doWhilePattern(R"(while\s*\(.*\)\s*;.*?)");
        if (!regex_match(line, doWhilePattern)) {
            loop.isValid = false;
            loop.errorMessage = "Invalid do-while loop syntax (missing semicolon)";
            loops.push_back(loop);
            return;
        }
        
        // Extract and check the condition
        loop.condition = extractParenthesesContent(line);
        if (loop.condition.empty()) {
            loop.isValid = false;
            loop.errorMessage = "Empty or invalid do-while loop condition";
            loops.push_back(loop);
            return;
        }
        
        // Condition can be any non-empty expression
        if (loop.condition.find_first_not_of(" \t") == string::npos) {
            loop.isValid = false;
            loop.errorMessage = "Invalid condition in do-while loop";
        }
        
        loops.push_back(loop);
    }

public:
    // Constructor
    LoopSyntaxAnalyzer() {}
    
    // Load source code from file
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to open file " << filename << endl;
            return false;
        }
        
        sourceCode.clear();
        string line;
        while (getline(file, line)) {
            sourceCode.push_back(line);
        }
        
        file.close();
        return true;
    }
    
    // Load source code from string (for testing)
    void loadFromString(const string& code) {
        sourceCode.clear();
        
        istringstream iss(code);
        string line;
        while (getline(iss, line)) {
            sourceCode.push_back(line);
        }
    }
    
    // Analyze the source code
    void analyze() {
        loops.clear();
        
        // Map to store line number information
        vector<size_t> lineStartPositions;
        lineStartPositions.push_back(0); // First line starts at position 0
        
        // Concatenate code and track line positions
        string concatenatedCode = "";
        for (const string& line : sourceCode) {
            concatenatedCode += line + "\n";
            lineStartPositions.push_back(concatenatedCode.length());
        }
        
        // Helper function to find line number from position
        auto getLineNumber = [&](size_t position) -> int {
            auto it = upper_bound(lineStartPositions.begin(), lineStartPositions.end(), position);
            return static_cast<int>(distance(lineStartPositions.begin(), it));
        };
        
        // Process valid for loops
        regex forLoopRegex(R"(for\s*\(\s*([^;]*?)\s*;\s*([^;]*?)\s*;\s*([^;]*?)\s*\))");
        smatch forMatches;
        string::const_iterator searchStart(concatenatedCode.cbegin());
        size_t currentPos = 0;
        
        // Store processed positions to avoid duplicate reporting
        set<size_t> processedPositions;
        
        while (regex_search(searchStart, concatenatedCode.cend(), forMatches, forLoopRegex)) {
            string fullMatch = forMatches.str();
            size_t matchPos = forMatches.position() + currentPos;
            
            // Skip if we've already processed this position
            if (processedPositions.find(matchPos) == processedPositions.end()) {
                processedPositions.insert(matchPos);
                
                // Skip if this is inside a string literal or comment
                bool isInString = false;
                bool isInComment = false;
                for (size_t i = 0; i < matchPos; i++) {
                    if (i > 0 && concatenatedCode[i-1] != '\\') {
                        if (concatenatedCode[i] == '"') isInString = !isInString;
                        else if (concatenatedCode[i] == '/' && i+1 < concatenatedCode.length() && concatenatedCode[i+1] == '/') 
                            isInComment = true;
                        else if (concatenatedCode[i] == '\n') isInComment = false;
                    }
                }
                
                if (!isInString && !isInComment) {
                    LoopInfo loop;
                    loop.type = "for";
                    loop.lineNumber = getLineNumber(matchPos);
                    loop.isValid = true;
                    
                    // Extract components
                    loop.initialization = trim(forMatches[1].str());
                    loop.condition = trim(forMatches[2].str());
                    loop.iteration = trim(forMatches[3].str());
                    
                    // Validate the loop
                    if (hasBalancedParentheses(fullMatch)) {
                        // Empty for loop is valid
                        if (loop.initialization.empty() && loop.condition.empty() && loop.iteration.empty()) {
                            loop.isValid = true;
                        }
                    } else {
                        loop.isValid = false;
                        loop.errorMessage = "Unbalanced parentheses in for loop";
                    }
                    
                    loops.push_back(loop);
                }
            }
            
            // Move to next search position
            currentPos += forMatches.position() + forMatches.length();
            searchStart = forMatches.suffix().first;
        }
        
        // Also handle range-based for loops (for (type var : container))
        regex rangeBasedForLoopRegex(R"(for\s*\(\s*([^:]*?)\s*:\s*([^)]*?)\s*\))");
        smatch rangeBasedForMatches;
        searchStart = concatenatedCode.cbegin();
        currentPos = 0;
        
        while (regex_search(searchStart, concatenatedCode.cend(), rangeBasedForMatches, rangeBasedForLoopRegex)) {
            string fullMatch = rangeBasedForMatches.str();
            size_t matchPos = rangeBasedForMatches.position() + currentPos;
            
            // Skip if we've already processed this position
            if (processedPositions.find(matchPos) == processedPositions.end()) {
                processedPositions.insert(matchPos);
                
                // Skip if this is inside a string literal or comment
                bool isInString = false;
                bool isInComment = false;
                for (size_t i = 0; i < matchPos; i++) {
                    if (i > 0 && concatenatedCode[i-1] != '\\') {
                        if (concatenatedCode[i] == '"') isInString = !isInString;
                        else if (concatenatedCode[i] == '/' && i+1 < concatenatedCode.length() && concatenatedCode[i+1] == '/') 
                            isInComment = true;
                        else if (concatenatedCode[i] == '\n') isInComment = false;
                    }
                }
                
                if (!isInString && !isInComment) {
                    LoopInfo loop;
                    loop.type = "for";
                    loop.lineNumber = getLineNumber(matchPos);
                    loop.isValid = true;
                    
                    // For range-based loops, store differently
                    loop.initialization = "range-based";
                    loop.condition = trim(rangeBasedForMatches[1].str()); // variable
                    loop.iteration = trim(rangeBasedForMatches[2].str());  // container
                    
                    if (!hasBalancedParentheses(fullMatch)) {
                        loop.isValid = false;
                        loop.errorMessage = "Unbalanced parentheses in range-based for loop";
                    }
                    
                    loops.push_back(loop);
                }
            }
            
            // Move to next search position
            currentPos += rangeBasedForMatches.position() + rangeBasedForMatches.length();
            searchStart = rangeBasedForMatches.suffix().first;
        }
        
        // Process invalid for loops (missing semicolons)
        regex invalidForLoopRegex(R"(for\s*\(\s*([^;:)]*)\s*\))");
        smatch invalidForMatches;
        searchStart = concatenatedCode.cbegin();
        currentPos = 0;
        
        while (regex_search(searchStart, concatenatedCode.cend(), invalidForMatches, invalidForLoopRegex)) {
            string fullMatch = invalidForMatches.str();
            size_t matchPos = invalidForMatches.position() + currentPos;
            
            // Skip if we've already processed this position
            if (processedPositions.find(matchPos) == processedPositions.end()) {
                // Check if it's a truly invalid loop (not matched by the valid patterns)
                smatch tempMatch1, tempMatch2;
                if (!regex_match(fullMatch, tempMatch1, forLoopRegex) && 
                    !regex_match(fullMatch, tempMatch2, rangeBasedForLoopRegex)) {
                    
                    // Skip if this is inside a string literal or comment
                    bool isInString = false;
                    bool isInComment = false;
                    for (size_t i = 0; i < matchPos; i++) {
                        if (i > 0 && concatenatedCode[i-1] != '\\') {
                            if (concatenatedCode[i] == '"') isInString = !isInString;
                            else if (concatenatedCode[i] == '/' && i+1 < concatenatedCode.length() && concatenatedCode[i+1] == '/') 
                                isInComment = true;
                            else if (concatenatedCode[i] == '\n') isInComment = false;
                        }
                    }
                    
                    if (!isInString && !isInComment) {
                        LoopInfo loop;
                        loop.type = "for";
                        loop.lineNumber = getLineNumber(matchPos);
                        loop.isValid = false;
                        loop.errorMessage = "Missing semicolon(s) in for loop";
                        loops.push_back(loop);
                        processedPositions.insert(matchPos);
                    }
                }
            }
            
            // Move to next search position
            currentPos += invalidForMatches.position() + invalidForMatches.length();
            searchStart = invalidForMatches.suffix().first;
        }
        
        // Process valid while loops
        regex whileLoopRegex(R"(while\s*\(\s*(.+?)\s*\))");
        smatch whileMatches;
        searchStart = concatenatedCode.cbegin();
        currentPos = 0;
        
        while (regex_search(searchStart, concatenatedCode.cend(), whileMatches, whileLoopRegex)) {
            string fullMatch = whileMatches.str();
            size_t matchPos = whileMatches.position() + currentPos;
            
            // Skip if this is inside a string literal or comment
            bool isInString = false;
            bool isInComment = false;
            bool isDoWhile = false;
            
            for (size_t i = 0; i < matchPos; i++) {
                if (i > 0 && concatenatedCode[i-1] != '\\') {
                    if (concatenatedCode[i] == '"') isInString = !isInString;
                    else if (concatenatedCode[i] == '/' && i+1 < concatenatedCode.length() && concatenatedCode[i+1] == '/') 
                        isInComment = true;
                    else if (concatenatedCode[i] == '\n') isInComment = false;
                }
            }
            
            // Check if this is part of a do-while by looking for "do" before the while
            size_t checkPos = matchPos;
            while (checkPos > 0) {
                checkPos--;
                if (concatenatedCode[checkPos] == '}') {
                    // Find matching opening brace
                    int braceLevel = 1;
                    while (checkPos > 0 && braceLevel > 0) {
                        checkPos--;
                        if (concatenatedCode[checkPos] == '}') braceLevel++;
                        else if (concatenatedCode[checkPos] == '{') braceLevel--;
                    }
                    
                    // Look for "do" before the opening brace
                    if (checkPos > 2) {
                        size_t doPos = concatenatedCode.rfind("do", checkPos);
                        if (doPos != string::npos && doPos > checkPos - 10) {
                            isDoWhile = true;
                            break;
                        }
                    }
                    break;
                }
                if (!isspace(concatenatedCode[checkPos])) break;
            }
            
            // Skip do-while loops, they'll be handled separately
            if (!isInString && !isInComment && !isDoWhile) {
                LoopInfo loop;
                loop.type = "while";
                loop.lineNumber = getLineNumber(matchPos);
                loop.isValid = true;
                
                // Extract condition
                loop.condition = trim(whileMatches[1].str());
                
                // Validate
                if (loop.condition.empty()) {
                    loop.isValid = false;
                    loop.errorMessage = "Empty condition in while loop";
                }
                else if (!hasBalancedParentheses(loop.condition)) {
                    loop.isValid = false;
                    loop.errorMessage = "Unbalanced parentheses in while loop condition";
                }
                
                loops.push_back(loop);
            }
            
            // Move to next search position
            currentPos += whileMatches.position() + whileMatches.length();
            searchStart = whileMatches.suffix().first;
        }
        
        // Process invalid while loops (missing closing parenthesis)
        regex invalidWhileLoopRegex(R"(while\s*\([^)]*\{)");
        smatch invalidWhileMatches;
        searchStart = concatenatedCode.cbegin();
        currentPos = 0;
        
        while (regex_search(searchStart, concatenatedCode.cend(), invalidWhileMatches, invalidWhileLoopRegex)) {
            string fullMatch = invalidWhileMatches.str();
            size_t matchPos = invalidWhileMatches.position() + currentPos;
            
            // Skip if this is inside a string literal or comment
            bool isInString = false;
            bool isInComment = false;
            for (size_t i = 0; i < matchPos; i++) {
                if (i > 0 && concatenatedCode[i-1] != '\\') {
                    if (concatenatedCode[i] == '"') isInString = !isInString;
                    else if (concatenatedCode[i] == '/' && i+1 < concatenatedCode.length() && concatenatedCode[i+1] == '/') 
                        isInComment = true;
                    else if (concatenatedCode[i] == '\n') isInComment = false;
                }
            }
            
            if (!isInString && !isInComment) {
                LoopInfo loop;
                loop.type = "while";
                loop.lineNumber = getLineNumber(matchPos);
                loop.isValid = false;
                loop.errorMessage = "Unbalanced parentheses in while loop";
                loops.push_back(loop);
            }
            
            // Move to next search position
            currentPos += invalidWhileMatches.position() + invalidWhileMatches.length();
            searchStart = invalidWhileMatches.suffix().first;
        }
        
        // Special handling for do-while loops - modified to be more accurate
        searchStart = concatenatedCode.cbegin();
        currentPos = 0;
        regex doRegex(R"(\bdo\s*\{)");
        smatch doMatches;
        
        // Keep track of do-while positions to avoid duplicates
        set<size_t> processedDoWhilePositions;
        
        while (regex_search(searchStart, concatenatedCode.cend(), doMatches, doRegex)) {
            size_t doPos = doMatches.position() + currentPos;
            
            // Skip if this is inside a string literal or comment
            bool isInString = false;
            bool isInComment = false;
            for (size_t i = 0; i < doPos; i++) {
                if (i > 0 && concatenatedCode[i-1] != '\\') {
                    if (concatenatedCode[i] == '"') isInString = !isInString;
                    else if (concatenatedCode[i] == '/' && i+1 < concatenatedCode.length() && concatenatedCode[i+1] == '/') 
                        isInComment = true;
                    else if (concatenatedCode[i] == '\n') isInComment = false;
                }
            }
            
            if (!isInString && !isInComment && processedDoWhilePositions.find(doPos) == processedDoWhilePositions.end()) {
                size_t openBracePos = doPos + doMatches.str().find('{');
                
                // Find matching closing brace
                int braceLevel = 1;
                size_t closeBracePos = openBracePos + 1;
                
                while (closeBracePos < concatenatedCode.length() && braceLevel > 0) {
                    if (concatenatedCode[closeBracePos] == '{') braceLevel++;
                    else if (concatenatedCode[closeBracePos] == '}') braceLevel--;
                    closeBracePos++;
                }
                
                if (braceLevel == 0) {
                    // Look for while after the closing brace
                    size_t whilePos = concatenatedCode.find("while", closeBracePos);
                    
                    if (whilePos != string::npos && whilePos < closeBracePos + 20) {
                        // This is likely a do-while loop
                        // Extract the condition
                        size_t condStartPos = concatenatedCode.find('(', whilePos);
                        if (condStartPos != string::npos) {
                            size_t condEndPos = condStartPos + 1;
                            int parenLevel = 1;
                            
                            while (condEndPos < concatenatedCode.length() && parenLevel > 0) {
                                if (concatenatedCode[condEndPos] == '(') parenLevel++;
                                else if (concatenatedCode[condEndPos] == ')') parenLevel--;
                                condEndPos++;
                            }
                            
                            if (parenLevel == 0) {
                                // Check for semicolon
                                bool hasSemicolon = false;
                                size_t semicolonPos = condEndPos;
                                while (semicolonPos < concatenatedCode.length() && 
                                       isspace(concatenatedCode[semicolonPos])) {
                                    semicolonPos++;
                                }
                                
                                if (semicolonPos < concatenatedCode.length() && 
                                    concatenatedCode[semicolonPos] == ';') {
                                    hasSemicolon = true;
                                }
                                
                                LoopInfo loop;
                                loop.type = "do-while";
                                loop.lineNumber = getLineNumber(doPos);
                                loop.isValid = hasSemicolon;
                                
                                if (!hasSemicolon) {
                                    loop.errorMessage = "Missing semicolon in do-while loop";
                                }
                                
                                // Extract condition
                                loop.condition = trim(concatenatedCode.substr(
                                    condStartPos + 1, condEndPos - condStartPos - 2));
                                
                                if (loop.condition.empty()) {
                                    loop.isValid = false;
                                    loop.errorMessage = "Empty condition in do-while loop";
                                }
                                
                                loops.push_back(loop);
                                processedDoWhilePositions.insert(doPos);
                            }
                        }
                    }
                }
            }
            
            // Move to next search position
            currentPos += doMatches.position() + doMatches.length();
            searchStart = doMatches.suffix().first;
        }
    }
    
    // Display analysis results
    void displayResults() {
        cout << "=== Loop Syntax Analysis Results ===" << endl;
        cout << "Total loops found: " << loops.size() << endl;
        
        int validCount = 0;
        for (const auto& loop : loops) {
            if (loop.isValid) validCount++;
        }
        
        cout << "Valid loops: " << validCount << endl;
        cout << "Invalid loops: " << (loops.size() - validCount) << endl << endl;
        
        cout << "Loop Details:" << endl;
        cout << string(80, '-') << endl;
        cout << left << setw(10) << "Line #" 
             << setw(10) << "Type" 
             << setw(20) << "Status" 
             << setw(40) << "Details/Error" << endl;
        cout << string(80, '-') << endl;
        
        for (const auto& loop : loops) {
            cout << left << setw(10) << loop.lineNumber 
                 << setw(10) << loop.type 
                 << setw(20) << (loop.isValid ? "Valid" : "Invalid") 
                 << setw(40) << (loop.isValid ? "" : loop.errorMessage) << endl;
            
            if (loop.isValid) {
                if (loop.type == "for") {
                    cout << "  Initialization: " << (loop.initialization.empty() ? "(empty)" : loop.initialization) << endl;
                    cout << "  Condition: " << (loop.condition.empty() ? "(empty)" : loop.condition) << endl;
                    cout << "  Iteration: " << (loop.iteration.empty() ? "(empty)" : loop.iteration) << endl;
                } else {
                    cout << "  Condition: " << loop.condition << endl;
                }
            }
            cout << string(80, '-') << endl;
        }
    }
    
    // Get count of valid and invalid loops
    pair<int, int> getLoopStats() {
        int validCount = 0;
        for (const auto& loop : loops) {
            if (loop.isValid) validCount++;
        }
        return {validCount, static_cast<int>(loops.size() - validCount)};
    }
};

int main() {
    LoopSyntaxAnalyzer analyzer;
    
    cout << "=== C++ Loop Syntax Analyzer ===" << endl << endl;
    
    // Menu
    while (true) {
        cout << "1. Analyze loops from a file" << endl;
        cout << "2. Exit" << endl;
        cout << "Select an option: ";
        
        int choice;
        cin >> choice;
        cin.ignore(); // Clear the newline character
        
        if (choice == 1) {
            string filename;
            cout << "Enter the C++ source file path: ";
            getline(cin, filename);
            
            if (analyzer.loadFromFile(filename)) {
                analyzer.analyze();
                analyzer.displayResults();
            }
        }
        else if (choice == 2) {
            cout << "Exiting..." << endl;
            break;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
        
        cout << endl;
    }
    
    return 0;
} 