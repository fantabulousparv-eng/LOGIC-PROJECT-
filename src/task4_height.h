/**
 * @file task4_height.h
 * @brief Task 4 - height of the parse tree (recursive and iterative).
 *
 * Convention: the height is the number of LEVELS, i.e. the number of nodes on the longest
 * root-to-leaf path. An empty tree has height 0 and a single atom has height 1.
 * (Height measured in edges is therefore `height - 1`.)
 */
#ifndef LOGIC_TASK4_H
#define LOGIC_TASK4_H

#include "common.h"

/** @brief Recursive height: `1 + max(height(left), height(right))`. @par Complexity O(n) time, O(h) stack. */
int heightRecursive(const Node *root);

/** @brief Iterative height using a level-order (BFS) queue, counting levels. @par Complexity O(n) time, O(w) space (w = max width). */
int heightIterative(const Node *root);

#endif
