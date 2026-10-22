/**
 * @file task2_prefix_to_tree.h
 * @brief Task 2 - build a rooted binary parse tree from a prefix expression.
 */
#ifndef LOGIC_TASK2_H
#define LOGIC_TASK2_H

#include "common.h"
#include <string>

/**
 * @brief Builds the parse tree recursively (left to right, one call per token).
 *
 * The first token is the root. `~` takes one operand, `+ * >` take two, atoms are leaves.
 *
 * @param prefix space-separated prefix expression, e.g. `* + p q ~ r`
 * @return the root of the tree (free it with freeTree())
 * @throws std::invalid_argument if the expression is malformed
 * @par Complexity O(n) time, O(h) call-stack space (h = tree height).
 * @warning Recursion depth equals the tree height; a very deep tree can overflow the stack.
 */
Node *prefixToTreeRecursive(const std::string &prefix);

/**
 * @brief Builds the parse tree iteratively (scan right to left with an explicit stack).
 *
 * @param prefix space-separated prefix expression
 * @return the root of the tree (free it with freeTree())
 * @throws std::invalid_argument if the expression is malformed
 * @par Complexity O(n) time, O(n) space on the heap stack; no recursion, so no stack overflow.
 */
Node *prefixToTreeIterative(const std::string &prefix);

#endif
