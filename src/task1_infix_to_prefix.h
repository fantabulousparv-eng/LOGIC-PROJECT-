/**
 * @file task1_infix_to_prefix.h
 * @brief Task 1 - convert an infix propositional formula into prefix notation.
 */
#ifndef LOGIC_TASK1_H
#define LOGIC_TASK1_H

#include <string>
#include <vector>

/**
 * @brief Splits an infix formula into tokens.
 *
 * Tokens are: `(`, `)`, the connectives `~ + * >`, and atoms (runs of letters,
 * digits and underscores, e.g. `p`, `x12`). Whitespace is ignored.
 *
 * @param infix the infix formula
 * @return the token list
 * @throws std::invalid_argument on an unexpected character
 */
std::vector<std::string> tokenizeInfix(const std::string &infix);

/**
 * @brief Converts an infix formula to prefix notation.
 *
 * Algorithm (reverse / swap / shunting-yard / reverse):
 *  1. reverse the token list and swap every `(` with `)`;
 *  2. convert the result to postfix with a stack (operator precedence ~ > * > + > >);
 *  3. reverse the postfix list.
 *
 * @param infix a well-formed, fully parenthesized infix formula, e.g. `((p+q)*(~r))`
 * @return space-separated prefix expression, e.g. `* + p q ~ r`
 * @throws std::invalid_argument on unbalanced parentheses or a bad character
 * @par Complexity O(n) time, O(n) space.
 */
std::string infixToPrefix(const std::string &infix);

#endif
