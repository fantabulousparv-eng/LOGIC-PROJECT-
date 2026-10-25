
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include "common.h"

using namespace std;

// Task 5.1: Collect propositional atoms
void collectAtoms(Node* root, set<string>& atoms) {
    if (root == nullptr) return;

    if (root->value != "~" &&
        root->value != "+" &&
        root->value != "*" &&
        root->value != ">" &&
        root->value != "TRUE" &&
        root->value != "FALSE") {
        atoms.insert(root->value);
    }

    collectAtoms(root->left, atoms);
    collectAtoms(root->right, atoms);
}

// Task 5.2: Evaluate the formula
bool evaluate(Node* root, map<string, bool>& values) {
    if (root == nullptr) return false;

    if (root->value == "TRUE") return true;
    if (root->value == "FALSE") return false;

    if (root->value == "~")
        return !evaluate(root->right, values);

    if (root->value != "+" &&
        root->value != "*" &&
        root->value != ">") {
        return values[root->value];
    }

    bool left = evaluate(root->left, values);
    bool right = evaluate(root->right, values);

    if (root->value == "+") return left || right;
    if (root->value == "*") return left && right;
    if (root->value == ">") return !left || right;

    return false;
}

// Task 5.3: Print the complete truth table
void printTruthTable(Node* root) {
    set<string> atomSet;
    collectAtoms(root, atomSet);

    vector<string> atoms(atomSet.begin(), atomSet.end());
    int k = static_cast<int>(atoms.size());

    // Avoid impractically large truth tables.
    if (k > 16) {
        cout << "Truth table skipped: " << k
             << " variables would require 2^" << k
             << " rows.\n";
        return;
    }

    unsigned long long rows = 1ULL << k;
    map<string, bool> values;

    for (const string& atom : atoms)
        cout << atom << "\t";

    cout << "Result\n";

    for (unsigned long long mask = 0; mask < rows; mask++) {
        for (int j = 0; j < k; j++) {
            bool value = (mask >> (k - 1 - j)) & 1ULL;
            values[atoms[j]] = value;
            cout << (value ? "T" : "F") << "\t";
        }

        cout << (evaluate(root, values) ? "T" : "F") << '\n';
    }
}
