#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <stack>
#include <map>
#include <cmath>
#include <cctype>
#include <sstream>
#include <iomanip>
using namespace std;

// Function to tokenize an expression
vector<string> tokenizeExpression(const string& expression) {
    vector<string> tokens;
    regex pattern("\\d+\\.\\d+|\\d+|[a-zA-Z]+|[+\\*\\/\\^\\(\\)]|-");
    
    auto words_begin = sregex_iterator(expression.begin(), expression.end(), pattern);
    auto words_end = sregex_iterator();
    
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        tokens.push_back(i->str());
    }
    
    return tokens;
}

// Function to check if a token is an operator
bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

// Function to check if a token is an operand (number or variable)
bool isOperand(const string& token) {
    if (token.empty()) return false;
    
    // Check if it's a number (integer or decimal)
    bool isNumber = true;
    int dotCount = 0;
    
    for (char c : token) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) {
                isNumber = false;
                break;
            }
        } else if (!isdigit(c)) {
            isNumber = false;
            break;
        }
    }
    
    if (isNumber) return true;
    
    // Check if it's a variable (alphabetic characters)
    for (char c : token) {
        if (!isalpha(c)) return false;
    }
    
    return true;
}

// Function to evaluate a simple expression (for constant folding)
double evaluateSimpleExpression(double a, const string& op, double b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    if (op == "^") return pow(a, b);
    return 0.0;
}

// Check if a token is a numeric constant
bool isNumeric(const string& token) {
    if (token.empty()) return false;
    
    bool hasDecimal = false;
    for (size_t i = 0; i < token.size(); i++) {
        if (token[i] == '.' && !hasDecimal) {
            hasDecimal = true;
        } else if (!isdigit(token[i])) {
            return false;
        }
    }
    
    return true;
}

// Function to optimize expression using constant folding
string optimizeExpression(const string& expression) {
    vector<string> tokens = tokenizeExpression(expression);
    vector<string> outputQueue;
    stack<string> operatorStack;
    
    // Define operator precedence
    map<string, int> precedence = {
        {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}
    };
    
    // Convert to postfix notation (Shunting Yard algorithm)
    for (const string& token : tokens) {
        if (isOperand(token)) {
            outputQueue.push_back(token);
        }
        else if (token == "(") {
            operatorStack.push(token);
        }
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty() && operatorStack.top() == "(") {
                operatorStack.pop(); // Discard the left parenthesis
            }
        }
        else if (isOperator(token)) {
            while (!operatorStack.empty() && operatorStack.top() != "(" &&
                   ((precedence[token] <= precedence[operatorStack.top()]))) {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);
        }
    }
    
    // Pop remaining operators to output queue
    while (!operatorStack.empty()) {
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }
    
    // Evaluate the postfix expression with constant folding
    stack<string> evaluationStack;
    
    for (const string& token : outputQueue) {
        if (isOperand(token)) {
            evaluationStack.push(token);
        }
        else if (isOperator(token)) {
            if (evaluationStack.size() >= 2) {
                string right = evaluationStack.top();
                evaluationStack.pop();
                
                string left = evaluationStack.top();
                evaluationStack.pop();
                
                // If both operands are constants, fold them
                if (isNumeric(left) && isNumeric(right)) {
                    double a = stod(left);
                    double b = stod(right);
                    double result = evaluateSimpleExpression(a, token, b);
                    
                    // If result is an integer, remove decimal point
                    if (result == static_cast<int>(result)) {
                        evaluationStack.push(to_string(static_cast<int>(result)));
                    } else {
                        evaluationStack.push(to_string(result));
                    }
                } else {
                    // If operands involve variables, keep the expression
                    evaluationStack.push("(" + left + " " + token + " " + right + ")");
                }
            }
        }
    }
    
    // Format the result expression (remove unnecessary parentheses)
    string result = evaluationStack.top();
    if (!result.empty() && result.front() == '(' && result.back() == ')') {
        result = result.substr(1, result.length() - 2);
    }
    
    return result;
}

int main() {
    cout << "=== Code Optimization Using Constant Folding ===" << endl << endl;
    
    // Test cases from the practical requirements
    vector<string> testExpressions = {
        "5 + x - 3 * 2",
        "2 + 3 * 4 - 1",
        "x + (3 * 5) - 2",
        "(22 / 7) * r * r"
    };
    
    cout << "Sample input output:" << endl;
    cout << setw(30) << left << "Input" << "Output" << endl;
    cout << string(60, '-') << endl;
    
    for (const string& expr : testExpressions) {
        string optimized = optimizeExpression(expr);
        cout << setw(30) << left << expr << optimized << endl;
    }
    
    // Interactive mode
    cout << "\nEnter your own expressions (type 'exit' to quit):" << endl;
    string userInput;
    
    while (true) {
        cout << "\nExpression: ";
        getline(cin, userInput);
        
        if (userInput == "exit") break;
        
        string optimized = optimizeExpression(userInput);
        cout << "Optimized: " << optimized << endl;
    }
    
    return 0;
} 