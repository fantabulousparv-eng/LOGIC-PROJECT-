/** @file task1_infix_to_prefix.cpp @brief Implementation of Task 1. */
#include "task1_infix_to_prefix.h"
#include "common.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace std;

/** @brief Operator precedence used by the shunting-yard step (higher binds tighter). */
static int precedence(const string &op)
{
    if (op == "~")
        return 3;
    if (op == "*")
        return 2;
    if (op == "+")
        return 1;
    return 0; // '>'
}

vector<string> tokenizeInfix(const string &s)
{
    vector<string> tokens;
    size_t i = 0;
    while (i < s.size())
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (isspace(c))
        {
            ++i;
        }
        else if (c == '(' || c == ')' || c == '~' || c == '+' || c == '*' || c == '>')
        {
            tokens.emplace_back(1, static_cast<char>(c));
            ++i;
        }
        else if (isalnum(c) || c == '_')
        {
            size_t j = i;
            while (j < s.size() && (isalnum(static_cast<unsigned char>(s[j])) || s[j] == '_'))
                ++j;
            tokens.push_back(s.substr(i, j - i));
            i = j;
        }
        else
        {
            throw invalid_argument(string("unexpected character '") + static_cast<char>(c) + "'");
        }
    }
    return tokens;
}

string infixToPrefix(const string &infix)
{
    vector<string> tokens = tokenizeInfix(infix);

    // Step 1: reverse and swap parentheses
    reverse(tokens.begin(), tokens.end());
    for (string &t : tokens)
    {
        if (t == "(")
            t = ")";
        else if (t == ")")
            t = "(";
    }

    // Step 2: infix -> postfix (shunting-yard) on the reversed token list
    vector<string> postfix, stack;
    for (const string &t : tokens)
    {
        if (t == "(")
        {
            stack.push_back(t);
        }
        else if (t == ")")
        {
            while (!stack.empty() && stack.back() != "(")
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            if (stack.empty())
                throw invalid_argument("unbalanced parentheses");
            stack.pop_back(); // discard "("
        }
        else if (isOperatorToken(t))
        {
            while (!stack.empty() && stack.back() != "(" && precedence(stack.back()) > precedence(t))
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(t);
        }
        else
        {
            postfix.push_back(t); // atom
        }
    }
    while (!stack.empty())
    {
        if (stack.back() == "(")
            throw invalid_argument("unbalanced parentheses");
        postfix.push_back(stack.back());
        stack.pop_back();
    }

    // Step 3: reverse the postfix list -> prefix
    reverse(postfix.begin(), postfix.end());
    string out;
    for (size_t i = 0; i < postfix.size(); ++i)
    {
        if (i)
            out += ' ';
        out += postfix[i];
    }
    return out;
}
