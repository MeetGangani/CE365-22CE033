#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

map<char, set<char>> firstSets, followSets;
map<char, vector<string>> productions;
char startSymbol;

// Compute FIRST set
void computeFirst(char symbol) {
    if (!isupper(symbol)) {  // If terminal, return (ignored in output)
        firstSets[symbol].insert(symbol);
        return;
    }

    for (string rule : productions[symbol]) {
        bool containsNull = true;

        for (char ch : rule) {
            computeFirst(ch); // Recursively compute FIRST

            // Add all non-null FIRST elements of ch to FIRST(symbol)
            for (char firstVal : firstSets[ch]) {
                if (firstVal != '#')
                    firstSets[symbol].insert(firstVal);
            }

            // Stop if null ('#') is not in FIRST(ch)
            if (firstSets[ch].count('#') == 0) {
                containsNull = false;
                break;
            }
        }

        // If all symbols in rule can be null, add null ('#') to FIRST(symbol)
        if (containsNull) {
            firstSets[symbol].insert('#');
        }
    }
}

// Compute FOLLOW set
void computeFollow(char symbol) {
    if (symbol == startSymbol)
        followSets[symbol].insert('$'); // Start symbol gets $

    for (auto &prod : productions) {
        char lhs = prod.first;

        for (string rule : prod.second) {
            for (size_t i = 0; i < rule.size(); i++) {
                if (rule[i] == symbol) {
                    bool isLast = (i == rule.size() - 1);

                    if (!isLast) { // If not last symbol
                        char nextSymbol = rule[i + 1];

                        // Add FIRST(nextSymbol) to FOLLOW(symbol), excluding null ('#')
                        for (char firstVal : firstSets[nextSymbol]) {
                            if (firstVal != '#')
                                followSets[symbol].insert(firstVal);
                        }

                        // If FIRST(nextSymbol) contains null, add FOLLOW(LHS)
                        if (firstSets[nextSymbol].count('#') || isLast) {
                            computeFollow(lhs);
                            for (char followVal : followSets[lhs]) {
                                followSets[symbol].insert(followVal);
                            }
                        }
                    } else { // If last symbol, inherit FOLLOW(LHS)
                        computeFollow(lhs);
                        for (char followVal : followSets[lhs]) {
                            followSets[symbol].insert(followVal);
                        }
                    }
                }
            }
        }
    }
}

// Display FIRST and FOLLOW sets (excluding terminals & # entry)
void displaySets(string setName, map<char, set<char>> &sets, bool excludeTerminals) {
    cout << setName << " sets:\n";
    for (auto &entry : sets) {
        if (excludeTerminals && !isupper(entry.first)) continue; // Skip terminals

        cout << entry.first << " = { ";
        for (char ch : entry.second) {
            cout << ch << " ";
        }
        cout << "}\n";
    }
}

int main() {
    // Define grammar productions
    productions = {
        {'S', {"ABC", "D"}},
        {'A', {"a", "#"}},
        {'B', {"b", "#"}},
        {'C', {"(S)", "c"}},
        {'D', {"AC"}}
    };

    startSymbol = 'S'; // Define start symbol

    // Compute FIRST sets
    for (auto &prod : productions) {
        computeFirst(prod.first);
    }

    // Compute FOLLOW sets
    for (auto &prod : productions) {
        computeFollow(prod.first);
    }

    // Display results (excluding terminals in FIRST set)
    displaySets("First", firstSets, true);
    displaySets("Follow", followSets, false);

    return 0;
}
