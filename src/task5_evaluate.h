/**
 * @file task5_evaluate.h
 * @brief Task 5 - bottom-up evaluation of a formula and generation of its truth table.
 */
#ifndef LOGIC_TASK5_H
#define LOGIC_TASK5_H

#include "common.h"
#include <iosfwd>
#include <string>
#include <vector>

/**
 * @brief Finds the distinct atoms (in order of first appearance, left to right) and stores each
 *        atom's column number in Node::atomIdx.
 * @param root root of the tree
 * @return the atom names; the position of a name equals its atomIdx
 * @par Complexity O(n) expected time, O(n) space.
 */
std::vector<std::string> collectAtoms(Node *root);

/**
 * @brief Recursive bottom-up evaluation (children first, then the connective).
 * @param root tree whose atoms were indexed by collectAtoms()
 * @param vals truth value of every atom, indexed by atomIdx
 * @throws std::logic_error if an atom has not been indexed
 * @par Complexity O(n) time, O(h) stack.
 */
bool evaluateRecursive(const Node *root, const std::vector<char> &vals);

/**
 * @brief Evaluates a tree given as a post-order node list, using a reusable value stack.
 * @param order nodes in post-order (see postOrderNodes())
 * @param vals  truth value of every atom, indexed by atomIdx
 * @param stack scratch stack (reused between calls to avoid reallocations)
 * @par Complexity O(n) time, O(h) space.
 */
bool evaluateOrdered(const std::vector<const Node *> &order, const std::vector<char> &vals, std::vector<char> &stack);

/** @brief Iterative bottom-up evaluation (post-order list + value stack). @par Complexity O(n) time, O(n) space. */
bool evaluateIterative(const Node *root, const std::vector<char> &vals);

/** @brief Summary of a truth-table run. */
struct TruthTableResult {
    bool computed = false;               ///< false if skipped because there were too many atoms
    std::size_t numVars = 0;             ///< number of distinct atoms
    unsigned long long rows = 0;         ///< 2^numVars
    unsigned long long satisfying = 0;   ///< rows where the formula is true
    bool tautology = false;              ///< true in every row
    bool satisfiable = false;            ///< true in at least one row
};

/**
 * @brief Computes the complete truth table of the formula (rows start with all-T, last row all-F).
 * @param root          formula tree
 * @param out           stream to print the table to, or nullptr to only count
 * @param maxVars       refuse (computed = false) if the formula has more atoms than this
 * @param maxPrintRows  print at most this many rows (the whole table is still evaluated)
 * @par Complexity O(2^n * m) time for n atoms and m tree nodes, O(m) space.
 */
TruthTableResult truthTable(Node *root, std::ostream *out, unsigned maxVars = 20, unsigned long long maxPrintRows = 1024);

#endif
