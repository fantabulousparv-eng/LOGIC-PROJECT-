#include <iostream>
#include <string>
#include "common.h"

using namespace std;

int main() {
    string infix;

    cout << "Enter a fully parenthesized formula: ";
    getline(cin, infix);

    // Task 1
    string prefix = infixToPrefix(infix);
    cout << "Prefix: " << prefix << '\n';

    // Task 2
    Node* root = prefixToTree(prefix);

    // Task 3
    cout << "In-order: ";
    inorderRecursive(root);
    cout << '\n';

    // Task 4
    cout << "Tree height: "
         << heightRecursive(root) << '\n';

    // Task 5
    printTruthTable(root);

    deleteTree(root);

    return 0;
}