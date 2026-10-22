/**
 * @file dimacs.h
 * @brief Reader for DIMACS `.cnf` benchmark files and their conversion to an infix formula.
 *
 * This is the glue between the SAT 2002 data set and Tasks 1-6: a CNF file is turned into one big
 * fully-parenthesized infix formula such as `(((x1+(~x2))*(x2+x3))*...)`.
 */
#ifndef LOGIC_DIMACS_H
#define LOGIC_DIMACS_H

#include <string>
#include <vector>

/** @brief A CNF formula as read from a DIMACS file. */
struct CnfInfo {
    std::size_t numVars = 0;                 ///< Number of variables declared on the `p cnf` line
    std::size_t numClauses = 0;              ///< Number of clauses actually read
    std::vector<std::vector<int>> clauses;   ///< Each clause is a list of non-zero literals (k or -k)
};

/**
 * @brief Reads a DIMACS CNF file.
 *
 * Lines starting with `c` are comments, `p cnf <vars> <clauses>` is the header, and every clause is a
 * list of integers terminated by 0 (clauses may span several lines). A `%` line ends the data
 * (used by some SATLIB files). Empty clauses are ignored.
 *
 * @param path path of the `.cnf` file (decompress `.gz`/`.bz2` files first)
 * @throws std::runtime_error if the file cannot be opened or contains no clauses
 */
CnfInfo readDimacs(const std::string &path);

/**
 * @brief Converts a CNF into a fully parenthesized infix formula.
 *
 * Variable k becomes the atom `xk`; a negative literal becomes `(~xk)`. Literals of a clause are
 * OR-ed (`+`) and clauses are AND-ed (`*`). Both joins are built as a BALANCED binary tree, so the
 * resulting parse tree has height O(log n) instead of O(n) - this keeps the recursive algorithms
 * from overflowing the call stack on large benchmark files.
 *
 * @par Complexity O(total literals) time and space.
 */
std::string cnfToInfix(const CnfInfo &cnf);

#endif
