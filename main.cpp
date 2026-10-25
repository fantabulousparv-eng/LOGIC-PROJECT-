
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "common.h"

using namespace std;

int main() {
    cout << "LOGIC IN CS - ASSIGNMENT 1, PART A\n";
    cout << "1. Enter a propositional formula\n";
    cout << "2. Load a SAT DIMACS CNF file\n";
    cout << "Choose input mode: ";

    int choice;

    if (!(cin >> choice)) {
        cerr << "Invalid choice.\n";
        return 1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Node* root = nullptr;

    try {
        if (choice == 1) {
            string infix;

            cout << "Enter a fully parenthesized formula: ";
            getline(cin, infix);

            // Task 1
            string prefix = infixToPrefix(infix);
            cout << "Prefix: " << prefix << '\n';

            // Task 2
            root = prefixToTree(prefix);
        }
        else if (choice == 2) {
            string filename;

            cout << "Enter the path to your .cnf file: ";
            getline(cin, filename);

            // Read the benchmark and build its tree.
            root = readDimacsCNF(filename);

            cout << "Prefix representation:\n";
            preorderRecursive(root);
            cout << '\n';
        }
        else {
            cout << "Please choose 1 or 2.\n";
            return 1;
        }

        // Tasks 3-5
        cout << "\n--- Task 3: Tree traversals ---\n";

        cout << "Inorder recursive: ";
        inorderRecursive(root);
        cout << '\n';

        cout << "Inorder iterative: ";
        inorderIterative(root);
        cout << '\n';

        cout << "Preorder recursive: ";
        preorderRecursive(root);
        cout << '\n';

        cout << "Preorder iterative: ";
        preorderIterative(root);
        cout << '\n';

        cout << "Postorder recursive: ";
        postorderRecursive(root);
        cout << '\n';

        cout << "Postorder iterative: ";
        postorderIterative(root);
        cout << '\n';

        cout << "\n--- Task 4: Tree height ---\n";
        cout << "Recursive height: "
             << heightRecursive(root) << '\n';
        cout << "Iterative height: "
             << heightIterative(root) << '\n';

        cout << "\n--- Task 5: Truth table ---\n";
        printTruthTable(root);

        deleteTree(root);
        root = nullptr;
    }
    catch (const exception& error) {
        cerr << "\nError: " << error.what() << '\n';
        deleteTree(root);
        return 1;
    }

    return 0;
}
