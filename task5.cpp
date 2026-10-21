
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include "common.h"
using namespace std;


// Task 5.1: Collect propositional atoms
void collectAtoms(Node* root, set<char>& atoms) {
    if (root == nullptr) return;

    if (root->value != '~' &&
        root->value != '+' &&
        root->value != '*' &&
        root->value != '>') {
        atoms.insert(root->value);
    }

    collectAtoms(root->left, atoms);
    collectAtoms(root->right, atoms);
}

// Task 5.2: Evaluate the formula
bool evaluate(Node* root, map<char, bool>& values) {
    if (root->value == '~')
        return !evaluate(root->right, values);

    if (root->value != '+' &&
        root->value != '*' &&
        root->value != '>') {
        return values[root->value];
    }

    bool left = evaluate(root->left, values);
    bool right = evaluate(root->right, values);

    if (root->value == '+') return left || right;
    if (root->value == '*') return left && right;
    if (root->value == '>') return !left || right;

    return false;
}

// Task 5.3: Print the complete truth table
void printTruthTable(Node* root) {
    set<char> atomSet;
    collectAtoms(root, atomSet);

    vector<char> atoms(atomSet.begin(), atomSet.end());
    int k = (int)atoms.size();

    if (k > 20) {
        cout << "Too many atoms for truth-table generation.\n";
        return;
    }

    unsigned long long rows = 1ULL << k;
    map<char, bool> values;

    for (char c : atoms) cout << c << "\t";
    cout << "Result\n";

    for (unsigned long long mask = 0; mask < rows; mask++) {
        for (int j = 0; j < k; j++) {
            bool value = (mask >> (k - 1 - j)) & 1ULL;
            values[atoms[j]] = value;
            cout << (value ? "T" : "F") << "\t";
        }

        cout << (evaluate(root, values) ? "T" : "F") << "\n";
    }
}
