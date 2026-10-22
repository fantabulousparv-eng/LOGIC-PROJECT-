/**
 * @file common.h
 * @brief Shared parse-tree data structure and small helpers used by every task.
 */
#ifndef LOGIC_COMMON_H
#define LOGIC_COMMON_H

#include <cstddef>
#include <string>
#include <vector>

/**
 * @brief One node of the binary parse tree of a propositional formula.
 *
 * - Atom (leaf):      both children are nullptr.
 * - Negation `~`:     the single operand is stored in @c left, @c right is nullptr.
 * - `+`, `*`, `>`:    @c left is the first operand, @c right the second.
 */
struct Node {
    std::string val;   ///< Atom name or one of the connectives ~ + * >
    Node *left;        ///< Left (or only) child
    Node *right;       ///< Right child (nullptr for atoms and for ~)
    int atomIdx;       ///< Column of this atom in the truth table (-1 = not indexed yet, set by Task 5)

    /** @brief Creates a leaf node holding @p v. */
    explicit Node(const std::string &v) : val(v), left(nullptr), right(nullptr), atomIdx(-1) {}
};

/**
 * @brief Returns the connective symbol of a node ('~','+','*','>') or 0 if the node is an atom.
 * @note Inline and branch-light because it sits in the hot loop of the truth-table evaluation.
 */
inline char opSymbol(const Node *n) {
    if (n->val.size() == 1) {
        char c = n->val[0];
        if (c == '~' || c == '+' || c == '*' || c == '>') return c;
    }
    return 0;
}

/** @brief True if @p t is one of the connectives ~ + * > */
bool isOperatorToken(const std::string &t);

/** @brief Splits @p s on whitespace into tokens. @par Complexity O(n) time, O(n) space. */
std::vector<std::string> splitWhitespace(const std::string &s);

/** @brief Returns @p s with all whitespace removed. */
std::string stripSpaces(const std::string &s);

/**
 * @brief Frees an entire tree iteratively (safe for very deep trees).
 * @par Complexity O(n) time, O(h) extra space.
 */
void freeTree(Node *root);

/** @brief Counts the nodes of a tree iteratively. @par Complexity O(n) time, O(h) space. */
std::size_t countNodes(const Node *root);

#endif
