/**
 * @file task3_traversals.h
 * @brief Task 3 - in-order, pre-order and post-order traversals (recursive and iterative).
 *
 * In-order output is a fully parenthesized infix formula, e.g. `((p+q)*(~r))`.
 * Pre-order and post-order output is a space-separated token list.
 */
#ifndef LOGIC_TASK3_H
#define LOGIC_TASK3_H

#include "common.h"
#include <string>
#include <vector>

/** @brief Recursive in-order traversal producing the parenthesized infix formula. @par Complexity O(n) time, O(h) stack. */
std::string inOrderRecursive(const Node *root);
/** @brief Iterative in-order traversal (explicit work-stack) producing the same infix formula. @par Complexity O(n) time, O(h) space. */
std::string inOrderIterative(const Node *root);

/** @brief Recursive pre-order traversal (node, left, right). @par Complexity O(n) time, O(h) stack. */
std::string preOrderRecursive(const Node *root);
/** @brief Iterative pre-order traversal using a stack. @par Complexity O(n) time, O(h) space. */
std::string preOrderIterative(const Node *root);

/** @brief Recursive post-order traversal (left, right, node). @par Complexity O(n) time, O(h) stack. */
std::string postOrderRecursive(const Node *root);
/** @brief Iterative post-order traversal using a stack and a "last visited" pointer. @par Complexity O(n) time, O(h) space. */
std::string postOrderIterative(const Node *root);

/**
 * @brief Returns the nodes in post-order (iteratively). Used by Task 5 for fast bottom-up evaluation.
 * @par Complexity O(n) time, O(n) space.
 */
std::vector<const Node *> postOrderNodes(const Node *root);

#endif
