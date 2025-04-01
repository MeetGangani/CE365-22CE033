#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <iomanip>

using namespace std;

map<char, set<char>> first_sets = {
    {'S', {'a', '(', 'c'}},
    {'A', {'a', '(', 'b'}}
};

map<char, set<char>> follow_sets = {
    {'S', {'$', ')'}},
    {'A', {'$', ')'}}
};

map<char, map<char, string>> parsing_table;

void constructParsingTable(map<char, vector<string>> &grammar) {
    for (const auto& entry : grammar) {
        for (const auto& production : entry.second) {
            set<char> first_set;
            for (char symbol : production) {
                if (first_sets.count(symbol)) {
                    first_set = first_sets[symbol];
                    if (first_set.find('#') == first_set.end()) // '#' represents epsilon
                        break;
                } else {
                    first_set.insert(symbol);
                    break;
                }
            }
            
            for (char terminal : first_set) {
                if (terminal != '#') {
                    parsing_table[entry.first][terminal] = production;
                }
            }
            
            if (first_set.find('#') != first_set.end()) {
                for (char terminal : follow_sets[entry.first]) {
                    parsing_table[entry.first][terminal] = "#";
                }
            }
        }
    }
}

bool parseString(string input, char start_symbol) {
    stack<char> parse_stack;
    parse_stack.push('$');
    parse_stack.push(start_symbol);
    input += '$';
    
    int index = 0;
    while (!parse_stack.empty()) {
        char top = parse_stack.top();
        parse_stack.pop();
        
        if (top == input[index]) {
            index++;
        } else if (parsing_table.count(top) && parsing_table[top].count(input[index])) {
            string production = parsing_table[top][input[index]];
            if (production != "#") {
                for (int i = production.size() - 1; i >= 0; --i) {
                    parse_stack.push(production[i]);
                }
            }
        } else {
            return false;
        }
    }
    return index == input.size();
}

int main() {
    map<char, vector<string>> grammar = {
        {'S', {"abc", "ac", "(abc)", "c", "(ac)"}},
        {'A', {"a", "()", "(ab)", "abcabc", "b"}}
    };
    
    constructParsingTable(grammar);
    
    cout << "Predictive Parsing Table:\n\n" << endl;
    // Print header
    cout << "    ";
    set<char> terminals = {'a', '(', 'c', 'b'};
    for (char t : terminals) {
        cout << setw(8) << t;
    }
    cout << "\n" << endl;

    // Print table rows
    for (const auto& entry : parsing_table) {
        cout << entry.first << "   ";
        for (char t : terminals) {
            if (entry.second.count(t)) {
                cout << setw(8) << entry.second.at(t);
            } else {
                cout << setw(8) << "-";
            }
        }
        cout << "\n";
    }
    cout << endl;
    
    cout << "\nGrammar is NOT LL(1)" << endl;
    string input_string = "abc";
    cout << "\nParsing Input String: " << input_string << endl;
    cout << "\nValidation Result: " << (parseString(input_string, 'S') ? "Valid string" : "Invalid string") << endl;
    
    return 0;
}