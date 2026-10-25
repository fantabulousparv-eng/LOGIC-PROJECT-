
#ifndef COMMON_H
#define COMMON_H
/**
 * @file common.h
 * @brief Shared declarations for the Logic in CS assignment.
 *
 * Defines the binary expression-tree node structure and declares
 * functions used for parsing, tree traversal, height calculation,
 * variable collection, logical evaluation, truth-table generation,
 * DIMACS CNF input, and memory cleanup.
 */

#include <string>
#include <map>
#include <set>

using namespace std;

/**
 * @brief Represents a node in a propositional logic expression tree.
 *
 * A node stores an operator, propositional variable, or logical
 * constant. The left and right pointers refer to child nodes.
 */
struct Node {
    string value;  ///< Operator, variable name, or logical constant.
    Node* left;    ///< Pointer to the left child.
    Node* right;   ///< Pointer to the right child.
};


bool isOperator(char c);
bool isBinaryOperator(char c);

// Task 1
string infixToPrefix(string infix);

// Task 2
Node* prefixToTree(string prefix);

// Task 3
void inorderRecursive(Node* root);
void inorderIterative(Node* root);
void preorderRecursive(Node* root);
void preorderIterative(Node* root);
void postorderRecursive(Node* root);
void postorderIterative(Node* root);

// Task 4
int heightRecursive(Node* root);
int heightIterative(Node* root);

// Task 5
void collectAtoms(Node* root, set<string>& atoms);
bool evaluate(Node* root, map<string, bool>& values);
void printTruthTable(Node* root);

// SAT dataset reader
Node* readDimacsCNF(const string& filename);

// Memory cleanup
void deleteTree(Node* root);

#endif
