#include<bits/stdc++.h>

using namespace std;

int main() {
    int numofsymbol, numofstates, initialstate, numofas;

    cout << "Number of input symbols : ";
    cin >> numofsymbol;

    char inputsymbol[numofsymbol];
    cout << "Input symbols : ";
    for (int i = 0; i < numofsymbol; i++) {
        cin >> inputsymbol[i];
    }

    cout << "Enter number of states : ";
    cin >> numofstates;

    cout << "Initial state : ";
    cin >> initialstate;

    cout << "Number of accepting states : ";
    cin >> numofas;

    int accepting[numofas];
    cout << "Accepting states : ";
    for (int i = 0; i < numofas; i++) {
        cin >> accepting[i];
    }

    int transitiontable[numofstates + 1][numofsymbol];
    cout << "Transition table :\n";
    for (int i = 1; i <= numofstates; i++) {
        for (int j = 0; j < numofsymbol; j++) {
            cout << "state: " << i << " to " << inputsymbol[j] << " -> ";
            cin >> transitiontable[i][j];
        }
    }

    cout << "Input string : ";
    string Inputstring;
    cin >> Inputstring;

    int currentstate = initialstate;
    bool isValid = true;

    for (char c : Inputstring) {
        bool symbolFound = false;
        for (int j = 0; j < numofsymbol; j++) {
            if (c == inputsymbol[j]) {
                currentstate = transitiontable[currentstate][j];
                symbolFound = true;
                break;
            }
        }
        if (!symbolFound) {
            isValid = false;
            break;
        }
    }

    if (isValid) {
        isValid = false;
        for (int i = 0; i < numofas; i++) {
            if (currentstate == accepting[i]) {
                isValid = true;
                break;
            }
        }
    }
    if (isValid) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }

    return 0;
}