/**
 * @file task6_analysis.h
 * @brief Task 6 - measures output, running time and memory of every task and writes an HTML report.
 */
#ifndef LOGIC_TASK6_H
#define LOGIC_TASK6_H

#include "task5_evaluate.h"
#include <string>
#include <vector>

/** @brief Running time of one algorithm. */
struct StageMetric {
    std::string name;   ///< e.g. "Task 3: in-order (recursive)"
    double ms;          ///< wall-clock milliseconds
};

/** @brief Everything measured for one formula / benchmark file. */
struct FileMetrics {
    std::string name;                 ///< file name or formula label
    bool ok = false;                  ///< false if processing failed (see error)
    std::string error;                ///< error message when ok == false
    std::size_t numVars = 0;          ///< distinct atoms in the tree
    std::size_t numClauses = 0;       ///< clauses (0 for plain formulas)
    std::size_t infixChars = 0;       ///< length of the infix input
    std::size_t nodes = 0;            ///< parse-tree nodes
    int height = 0;                   ///< tree height in levels (Task 4)
    std::size_t treeBytes = 0;        ///< nodes * sizeof(Node) (lower bound, heap strings/allocator overhead excluded)
    long peakRssKb = -1;              ///< process peak resident memory after this file (-1 = unknown)
    bool traversalsAgree = false;     ///< recursive == iterative for all three traversals
    bool roundTrip = false;           ///< in-order output equals the original infix input
    bool evalAgree = false;           ///< recursive and iterative evaluation gave the same value
    bool sampleValue = false;         ///< value of the formula under the (fixed-seed) random assignment
    TruthTableResult table;           ///< truth-table summary (computed == false when skipped)
    std::vector<StageMetric> stages;  ///< timing of each algorithm
};

/**
 * @brief Runs the whole pipeline (Tasks 1-5) on one formula and measures it.
 * @param name          label shown in the report
 * @param infix         fully parenthesized infix formula
 * @param maxVars       truth table is only built when the formula has at most this many atoms
 * @param useRecursive  false = skip the recursive variants (for extremely deep trees)
 */
FileMetrics analyseFormula(const std::string &name, const std::string &infix, unsigned maxVars, bool useRecursive = true);

/** @brief Process peak memory (resident set) in KB, or -1 if the platform does not support it. */
long peakRssKb();

/**
 * @brief Writes a styled HTML page with the results, timings, memory use and complexity analysis.
 * @param results one entry per analysed file / formula
 * @param path    output file (e.g. report.html)
 */
void writeHtmlReport(const std::vector<FileMetrics> &results, const std::string &path);

#endif
