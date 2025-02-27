#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <string>
using namespace std;

class FiniteAutomata {
private:
    int numStates;
    int initialState;
    set<int> acceptingStates;
    map<pair<int, char>, int> transitions;
    vector<char> inputSymbols;

public:
    void setup() {
        // Input symbols
        int numSymbols;
        cout << "Number of input symbols : ";
        cin >> numSymbols;
        
        cout << "Input symbols : ";
        for(int i = 0; i < numSymbols; i++) {
            char symbol;
            cin >> symbol;
            inputSymbols.push_back(symbol);
        }

        // States
        cout << "Enter number of states : ";
        cin >> numStates;

        // Initial state
        cout << "Initial state : ";
        cin >> initialState;

        // Accepting states
        int numAcceptingStates;
        cout << "Number of accepting states : ";
        cin >> numAcceptingStates;

        cout << "Accepting states : ";
        for(int i = 0; i < numAcceptingStates; i++) {
            int state;
            cin >> state;
            acceptingStates.insert(state);
        }

        // Transition table
        cout << "Transition table : \n";
        cin.ignore(); // Clear buffer
        
        for(int i = 0; i < numStates * numSymbols; i++) {
            string line;
            getline(cin, line);
            
            // Parse transition of form "1 to a -> 2"
            int fromState, toState;
            char symbol;
            
            // Extract first number (fromState)
            fromState = line[0] - '0';
            
            // Extract symbol (after "to ")
            symbol = line[5];
            
            // Extract last number (toState)
            toState = line[line.length() - 1] - '0';
            
            transitions[{fromState, symbol}] = toState;
        }
    }

    bool validateString(string input) {
        int currentState = initialState;
        
        for(char c : input) {
            if(transitions.find({currentState, c}) == transitions.end()) {
                return false;
            }
            currentState = transitions[{currentState, c}];
        }

        return acceptingStates.find(currentState) != acceptingStates.end();
    }
};

int main() {
    FiniteAutomata fa;
    fa.setup();

    string input;
    cout << "Input string : ";
    cin >> input;

    if(fa.validateString(input)) {
        cout << "Valid String" << endl;
    } else {
        cout << "Invalid String" << endl;
    }

    return 0;
}