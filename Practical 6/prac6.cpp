#include <iostream>
#include <string>
using namespace std;

bool isValid(string str) {
    // Base case: if string is just "a"
    if (str == "a") return true;
    
    // Check if string starts with '(' and ends with ')'
    if (str.length() >= 2 && str[0] == '(' && str[str.length()-1] == ')') {
        // Remove outer parentheses
        string inner = str.substr(1, str.length()-2);
        
        // Split the inner string by commas
        int pos = 0;
        string part;
        
        // If inner is empty, it's invalid
        if (inner.empty()) return false;
        
        // Check each part separated by commas
        while (pos < inner.length()) {
            int parenthesesCount = 0;
            int start = pos;
            
            // Find the end of current part (handling nested parentheses)
            while (pos < inner.length()) {
                if (inner[pos] == '(') parenthesesCount++;
                if (inner[pos] == ')') parenthesesCount--;
                if (parenthesesCount == 0 && inner[pos] == ',') break;
                pos++;
            }
            
            // Extract the current part
            part = inner.substr(start, pos - start);
            
            // Check if current part is valid
            if (!isValid(part)) return false;
            
            // Skip the comma
            pos++;
            
            // If we're at the end but there's a comma, it's invalid
            if (pos >= inner.length() && inner[inner.length()-1] == ',') 
                return false;
        }
        return true;
    }
    
    return false;
}

int main() {
    string input;
       
    cout << "\nEnter a string to check: ";
    getline(cin, input);
    
    if (isValid(input)) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }
    
    return 0;
}