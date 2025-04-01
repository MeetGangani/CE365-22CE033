#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <regex>
#include <cmath>
#include <string>
#include <functional>

double evaluateExpression(const std::string& expression) {
    try {
        // Define operator precedence
        std::map<char, int> precedence = {
            {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'^', 3}
        };
        
        // Define operations
        std::map<char, std::function<double(double, double)>> operations = {
            {'+', [](double a, double b) { return a + b; }},
            {'-', [](double a, double b) { return a - b; }},
            {'*', [](double a, double b) { return a * b; }},
            {'/', [](double a, double b) { return a / b; }},
            {'^', [](double a, double b) { return pow(a, b); }}
        };
        
        auto applyOperation = [&](std::stack<char>& operators, std::stack<double>& values) {
            char op = operators.top();
            operators.pop();
            double right = values.top();
            values.pop();
            double left = values.top();
            values.pop();
            values.push(operations[op](left, right));
        };
        
        auto greaterPrecedence = [&](char op1, char op2) {
            return precedence[op1] > precedence[op2];
        };
        
        // Tokenize the expression - fixed regex
        std::vector<std::string> tokens;
        std::regex pattern("\\d+\\.\\d+|\\d+|[+\\*\\/\\^\\(\\)]|-");
        std::string::const_iterator start = expression.begin();
        std::smatch match;
        
        while (std::regex_search(start, expression.end(), match, pattern)) {
            tokens.push_back(match.str());
            start = match.suffix().first;
        }
        
        std::stack<double> values;
        std::stack<char> operators;
        
        for (const auto& token : tokens) {
            // Process numbers
            if (std::isdigit(token[0]) || (token.size() > 1 && token[0] == '.' && std::isdigit(token[1]))) {
                values.push(std::stod(token));
            }
            // Process operators
            else if (precedence.find(token[0]) != precedence.end()) {
                char op = token[0];
                while (!operators.empty() && operators.top() != '(' && 
                       greaterPrecedence(operators.top(), op)) {
                    applyOperation(operators, values);
                }
                operators.push(op);
            }
            // Process opening parenthesis
            else if (token == "(") {
                operators.push('(');
            }
            // Process closing parenthesis
            else if (token == ")") {
                while (!operators.empty() && operators.top() != '(') {
                    applyOperation(operators, values);
                }
                if (!operators.empty() && operators.top() == '(') {
                    operators.pop(); // Pop the '('
                }
            }
        }
        
        // Apply remaining operators
        while (!operators.empty()) {
            applyOperation(operators, values);
        }
        
        return values.top();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 0;
    }
}

int main() {
    std::vector<std::string> expressions = {
        "(3 + 5) * 2 ^ 3",
        "3 + 5 * 2",
        "3 + 5 * 2 ^ 2",
        "3 + (5 * 2)",
        "3 + 5 ^ 2 * 2",
        "3 * (5 + 2)",
        "(3 + 5) ^ 2",
        "3 ^ 2 * 3",
        "3 ^ 2 + 5 * 2",
        "(3 + 5 * 2 ^ 2 - 8) / 4 ^ 2 + 6"
    };
    
    for (const auto& expr : expressions) {
        std::cout << "Expression: " << expr << " -> Result: " << evaluateExpression(expr) << std::endl;
    }
    
    return 0;
}