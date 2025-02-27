#include <iostream>
#include <string>
using namespace std;

int main() {
    // ... existing code ...
    cout << "Enter Test CASE Number : ";
    int test;
    cin >> test;

    if (test != 3) {
        int no_input;
        cout << "Enter Number of input symbols : ";
        cin >> no_input;

        char input[100];  // Using fixed array instead of dynamic
        // cout << "Enter Input symbols:\n";

        for (int i = 0; i < no_input; i++) {
            cout << "Enter input symbol " << (i + 1) << ": ";
            cin >> input[i];
        }

        cout << "\nEnter number of states: ";
        int no_state;
        cin >> no_state;

        // Check Valid initial State
        int init;
        do {
            cout << "Enter initial state: ";
            cin >> init;

            if(init > 0 && init <= no_state) {
                break;
            }
            cout << "Invalid initial state.\n";
            cout << "Enter Again........\n";
        } while (true);

        cout << "Enter number of accepting states: ";
        int no_accept;
        cin >> no_accept;

        int accept[100];  // Using fixed array
        for (int i = 0; i < no_accept; i++) {
            do {
                cout << "Enter accepting state " << (i + 1) << ": ";
                cin >> accept[i];
                if(accept[i] > 0 && accept[i] <= no_state) {
                    break;
                }
                cout << "Invalid Accepting state\n";
                cout << "Enter Again........\n";
            } while(true);
        }

        // Transition Table
        int TT[100][100];  // Using fixed size 2D array
        cout << "Enter Transition Table:\n";

        for (int i = 0; i < no_state; i++) {
            for (int j = 0; j < no_input; j++) {
                do {
                    cout << "State " << (i + 1) << " to " << input[j] << ": ";
                    cin >> TT[i][j];
                    if(TT[i][j] > 0 && TT[i][j] <= no_state) {
                        break;
                    }
                    cout << "Invalid State\n";
                    cout << "Enter Again........\n";
                } while(true);
            }
            cout << endl;
        }

        // Adding dead state transitions
        for (int j = 0; j < no_input; j++) {
            TT[no_state][j] = no_state + 1;
        }

        int ch;
        do {
            cout << "\n1. Enter String \n";
            cout << "2. Exit\n";
            cout << "Enter your choice: ";
            cin >> ch;

            if (ch == 2) break;

            if (ch != 1) {
                cout << "Invalid Choice.\n";
                continue;
            }

            cin.ignore();  // Clear input buffer
            string str;
            cout << "Enter string: ";
            getline(cin, str);

            int current = init;

            for (int i = 0; i < str.length(); i++) {
                bool found = false;
                for (int j = 0; j < no_input; j++) {
                    if (str[i] == input[j]) {
                        current = TT[current - 1][j];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    current = no_state;
                    break;
                }
            }

            bool valid = false;
            if (current != no_state + 1) {
                for (int i = 0; i < no_accept; i++) {
                    if (accept[i] == current) {
                        valid = true;
                        break;
                    }
                }
            }

            // cout << current << endl;
            if (valid) {
                cout << "Valid String\n";
            } else {
                cout << "Invalid String\n";
            }

        } while (true);

    } else {
        // Test case 3 implementation
        int init = 1;
        int no_input = 36;
        char input[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        int no_state = 3;
        int no_accept = 1;
        int accept[] = {2};

        // Transition Table
        int TT[4][36] = {
            {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3},
            {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
            {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
            {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
        };

        int ch;
        do {
            cout << "\n1. Enter String \n";
            cout << "2. Exit\n";
            cout << "Enter your choice: ";
            cin >> ch;

            if (ch == 2) break;

            if (ch != 1) {
                cout << "Invalid Choice.\n";
                continue;
            }

            cin.ignore();
            string str;
            cout << "Enter string: ";
            getline(cin, str);

            int current = init;

            for (int i = 0; i < str.length(); i++) {
                bool found = false;
                for (int j = 0; j < no_input; j++) {
                    if (str[i] == input[j]) {
                        current = TT[current - 1][j];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    current = no_state;
                    break;
                }
            }

            bool valid = false;
            if (current != no_state + 1) {
                for (int i = 0; i < no_accept; i++) {
                    if (accept[i] == current) {
                        valid = true;
                        break;
                    }
                }
            }

            if (valid) {
                cout << "Valid String\n";
            } else {
                cout << "Invalid String\n";
            }

        } while (true);
    }
    return 0;
}