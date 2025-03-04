#include <iostream>
#include <set>
#include <map>
#include <string>
using namespace std;

// Maps to store FIRST and FOLLOW sets
map<char, set<char>> first;
map<char, set<char>> follow;

// Function to compute FIRST sets
void computeFirst() {
    bool changed;
    
    // Initialize FIRST sets for terminals
    first['a'].insert('a');
    first['b'].insert('b');
    first['c'].insert('c');
    first['('].insert('(');
    first[')'].insert(')');
    
    // Add epsilon ('e') to A and B's FIRST sets
    first['A'].insert('e');  // 'e' represents epsilon
    first['B'].insert('e');  // 'e' represents epsilon
    
    do {
        changed = false;
        
        // S → A B C | D
        for (char x : first['A']) {
            if (x != 'e' && first['S'].insert(x).second) changed = true;
        }
        if (first['A'].count('e')) {
            for (char x : first['B']) {
                if (x != 'e' && first['S'].insert(x).second) changed = true;
            }
            if (first['B'].count('e')) {
                for (char x : first['C']) {
                    if (first['S'].insert(x).second) changed = true;
                }
            }
        }
        for (char x : first['D']) {
            if (first['S'].insert(x).second) changed = true;
        }
        
        // C → ( S ) | c
        first['C'].insert('(');
        first['C'].insert('c');
        
        // D → A C
        for (char x : first['A']) {
            if (x != 'e' && first['D'].insert(x).second) changed = true;
        }
        if (first['A'].count('e')) {
            for (char x : first['C']) {
                if (first['D'].insert(x).second) changed = true;
            }
        }
        
    } while (changed);
}

// Function to compute FOLLOW sets
void computeFollow() {
    bool changed;
    
    // Add $ to FOLLOW(S)
    follow['S'].insert('$');
    
    do {
        changed = false;
        
        // S → A B C | D
        for (char x : follow['S']) {
            if (follow['C'].insert(x).second) changed = true;
        }
        if (first['C'].count('e')) {
            for (char x : follow['S']) {
                if (follow['B'].insert(x).second) changed = true;
            }
        }
        for (char x : first['B']) {
            if (x != 'e' && follow['A'].insert(x).second) changed = true;
        }
        for (char x : first['C']) {
            if (x != 'e' && follow['B'].insert(x).second) changed = true;
        }
        
        // C → ( S )
        follow['S'].insert(')');
        
        // D → A C
        for (char x : follow['D']) {
            if (follow['C'].insert(x).second) changed = true;
        }
        for (char x : first['C']) {
            if (x != 'e' && follow['A'].insert(x).second) changed = true;
        }
        if (first['C'].count('e')) {
            for (char x : follow['D']) {
                if (follow['A'].insert(x).second) changed = true;
            }
        }
        
    } while (changed);
}

// Function to print sets in the required format
void printSet(char nonTerminal, const set<char>& s, bool isFirst = true) {
    if (isFirst) {
        cout << "First(" << nonTerminal << ") = {";
    } else {
        cout << "Follow(" << nonTerminal << ") = {";
    }
    
    bool first = true;
    for (char c : s) {
        if (c == 'e') continue;  // Skip epsilon for First sets as per output format
        if (!first) cout << ", ";
        cout << c;
        first = false;
    }
    cout << "}" << endl;
}

int main() {
    cout << "Grammar:" << endl;
    cout << "S → A B C | D" << endl;
    cout << "A → a | ε" << endl;
    cout << "B → b | ε" << endl;
    cout << "C → ( S ) | c" << endl;
    cout << "D → A C" << endl;
    cout << endl;
    
    computeFirst();
    computeFollow();
    
    // Print FIRST sets
    printSet('S', first['S']);
    printSet('A', first['A']);
    printSet('B', first['B']);
    printSet('C', first['C']);
    printSet('D', first['D']);
    
    // Print FOLLOW sets
    printSet('S', follow['S'], false);
    printSet('A', follow['A'], false);
    printSet('B', follow['B'], false);
    printSet('C', follow['C'], false);
    printSet('D', follow['D'], false);
    
    return 0;
}
