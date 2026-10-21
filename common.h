
#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>
#include <set>

using namespace std;

/*
 * Parse tree node structure.
 * Each node stores an atom or logical operator.
 */
struct Node {
    char value;
    Node* left;
    Node* right;
};

// Common helper functions
bool isOperator(char c);
bool isBinaryOperator(char c);

// Task 1: Infix to prefix
string infixToPrefix(string infix);

// Task 2: Prefix to parse tree
Node* prefixToTree(string prefix);

// Task 3: Tree traversals
void inorderRecursive(Node* root);
void inorderIterative(Node* root);

void preorderRecursive(Node* root);
void preorderIterative(Node* root);

void postorderRecursive(Node* root);
void postorderIterative(Node* root);

// Task 4: Tree height
int heightRecursive(Node* root);
int heightIterative(Node* root);

// Task 5: Truth table and evaluation
void collectAtoms(Node* root, set<char>& atoms);

bool evaluate(Node* root, map<char, bool>& values);

void printTruthTable(Node* root);

// Memory cleanup
void deleteTree(Node* root);

#endif
