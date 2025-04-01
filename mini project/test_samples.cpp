#include <iostream>
#include <vector>
using namespace std;

// Sample C++ code with various loop constructs for testing the Loop Syntax Analyzer

int main() {
    // Valid for loop with initialization, condition, and iteration
    for (int i = 0; i < 10; i++) {
        cout << "Count: " << i << endl;
    }
    
    // Valid for loop with multiple initializations
    for (int i = 0, j = 10; i < j; i++, j--) {
        cout << "i = " << i << ", j = " << j << endl;
    }
    
    // Valid for loop with empty parts (infinite loop with break)
    for (;;) {
        static int count = 0;
        if (count++ > 5) break;
        cout << "Infinite loop iteration: " << count << endl;
    }
    
    // Invalid for loop - missing semicolon
    for (int x = 0; x < 10; x++) {
        cout << "This loop has a syntax error" << endl;
    }

    for (int x = 0x < 10; x++) {
        cout << "This loop has a syntax error" << endl;
    }
    
    // Valid while loop
    int counter = 0;
    while (counter < 5) {
        cout << "While loop: " << counter << endl;
        counter++;
    }
    
    // Invalid while loop - unbalanced parentheses
    int errorCounter = 0;
    while (errorCounter < 10) {
        cout << "This while loop has a syntax error" << endl;
        errorCounter++;
    }
    
    // Valid do-while loop
    int doCounter = 0;
    do {
        cout << "Do-while loop: " << doCounter << endl;
        doCounter++;
    } while (doCounter < 3);
    
    // Invalid do-while loop - missing semicolon
    int doErrorCounter = 0;
    do {
        cout << "This do-while loop has a syntax error" << endl;
        doErrorCounter++;
    } while (doErrorCounter < 3);
    
    // Nested loops - valid
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "Nested loops: i=" << i << ", j=" << j << endl;
        }
    }
    
    // For loop with complex initialization and iteration
    vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        cout << "Vector element: " << *it << endl;
    }
    
    // Range-based for loop (C++11)
    for (const auto& num : numbers) {
        cout << "Range-based for: " << num << endl;
    }
    
    return 0;
} 