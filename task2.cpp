#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
using namespace std;

// This is the code for task 2 Task 2:
// Write a function to convert the prefix expression into a rooted binary parse tree.

/**
 * @brief Represents a node in a propositional-logic parse tree.
 *
 * Operator nodes store their operands in the child pointers; atom nodes are
 * leaves. The unary operator `~` uses only the left child.
 */

struct Node
{
    string val; // Atom name or one of: ~ + * >
    Node *left;
    Node *right; // nullptr for atoms and for the unary '~'

    /**
     * @brief Creates a node with the given atom or operator.
     * @param v The atom name or operator symbol.
     */

    Node(const string &v) : val(v), left(nullptr), right(nullptr) {} //used in intialiaxation
};

/**
 * @brief Checks whether a token is a supported logical operator.
 * @param t The token to check.
 * @return `true` if `t` is `~`, `+`, `*`, or `>`; otherwise `false`.
 */

bool isOperator(const string &t)
{
    return t == "~" || t == "+" || t == "*" || t == ">"; // if not a connectiveee then an atom
}

/**
 * @brief Splits a whitespace-separated prefix expression into tokens.
 * @param prefix The prefix expression to tokenizer1.
 * @return The tokens in their original order.
 */


vector<string> tokenizer1(const string &prefix)
{
    vector<string> tokens;
    istringstream iss(prefix);
    string t;
    while (iss >> t)
        tokens.push_back(t); //cobersion to tokens using puch back
    return tokens;
}

/**
 * @brief Builds a parse-tree subtree from prefix tokens, recursively.
 * @param tokens The prefix tokens to consume.
 * @param idx The current token position; advanced as tokens are consumed.
 * @return The root of the subtree, or `nullptr` if no token remains.
 */

Node *buildTreeRec(const vector<string> &tokens, size_t &idx)
{
    //idx is the index 
    if (idx >= tokens.size())
        return nullptr; // malformed input guard

    Node *node = new Node(tokens[idx++]);

    if (node->val == "~")
    { // unary: one child as neagtion has always one child
        node->left = buildTreeRec(tokens, idx); //this causes recursion
    }
    else if (isOperator(node->val))
    { 
        // binary: two children other things have two left and right children
        node->left = buildTreeRec(tokens, idx);
        node->right = buildTreeRec(tokens, idx);
    } // else: atom, leaf
    return node;
}

/**
 * @brief Converts a whitespace-separated prefix expression into a parse tree.
 * @param prefix The expression containing atoms and logical operators.
 * @return The root of the constructed tree, or `nullptr` if the input is empty.
 */

Node *prefixToTreeRec(const string &prefix)
{
    vector<string> tokens = tokenizer1(prefix);
    size_t idx = 0;
    return buildTreeRec(tokens, idx);
}

/**
 * @brief Frees a parse tree and all of its subtrees.
 * @param root The root of the tree to free; may be `nullptr`.
 */

void freeTree(Node *root)
{
    if (!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

/**
 * @brief Prints a parse tree as a fully parenthesized infix expression.
 * @param n The root of the tree to print; may be `nullptr`.
 */

void printInfxx(const Node *n)
{
    if (!n)
        return;
    if (n->val == "~")
    {
        cout << "(~";
        printInfxx(n->left);
        cout << ")";
    }
    else if (isOperator(n->val))
    {
        cout << "(";
        printInfxx(n->left);
        cout << n->val;
        printInfxx(n->right);
        cout << ")";
    }
    else
    {
        cout << n->val;
    }
}

/**
 * @brief Builds and prints a sample parse tree, then releases its memory.
 * @return `0` when the program completes successfully.
 */

int main()
{
    string prefix = "* + p q ~ r"; // ((p+q)*(~r))

    Node *t1 = prefixToTreeRec(prefix);
    printInfxx(t1);
    cout << "\n"; 

    //freeTree(t1); //this free's the tree

    return 0;
}