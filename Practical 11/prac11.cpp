#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

using namespace std;

// Structure to store quadruple entries
struct Quad {
    string op;     // Operator
    string val1;   // First operand
    string val2;   // Second operand
    string res;    // Result variable
};

// Function to check if a character is an operator
bool isOp(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

// Function to return operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to convert infix expression to postfix
string toPostfix(string expr) {
    stack<char> ops;  // Stack for operators
    string result;     // Store postfix result
    string number;     // Temporary variable for multi-digit numbers

    for (char ch : expr) {
        if (isdigit(ch)) {
            number += ch;  // Collect digits together
        } else {
            if (!number.empty()) {
                result += number + " ";  // Add number to postfix
                number = "";
            }
            if (isOp(ch)) {
                while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                    result += ops.top();
                    result += " ";
                    ops.pop();
                }
                ops.push(ch);
            }
        }
    }

    // Add the last collected number
    if (!number.empty()) {
        result += number + " ";
    }

    // Pop remaining operators
    while (!ops.empty()) {
        result += ops.top();
        result += " ";
        ops.pop();
    }

    return result;
}

// Function to generate quadruple table
vector<Quad> makeQuadruples(string postfix) {
    vector<Quad> quads;
    stack<string> values;
    stringstream ss(postfix);
    string token;
    int tempCount = 1;

    while (ss >> token) {
        if (isdigit(token[0])) {
            values.push(token); // Push numbers to stack
        } else { // Operator case
            string val2 = values.top();
            values.pop();
            string val1 = values.top();
            values.pop();

            string tempVar = "t" + to_string(tempCount++);
            quads.push_back({token, val1, val2, tempVar});
            values.push(tempVar);
        }
    }

    return quads;
}

// Function to display quadruples
void showQuadruples(vector<Quad> quads) {
    cout << "Operator\tOperand1\tOperand2\tResult" << endl;
    for (Quad q : quads) {
        cout << q.op << "\t\t" << q.val1 << "\t\t" << q.val2 << "\t\t" << q.res << endl;
    }
}

int main() {
    string expr;
    cout << "Enter an arithmetic expression (e.g., 9+42*8): ";
    cin >> expr;

    string postfixExpr = toPostfix(expr);
    vector<Quad> quads = makeQuadruples(postfixExpr);

    cout << "\nQuadruple Table:\n";
    showQuadruples(quads);

    return 0;
}
