
#include <iostream>
#include <string>
#include <cctype>
#include "common.h"
using namespace std;

bool isBinaryOperator(char c) {
    return c == '+' || c == '*' || c == '>';
}

string expression;
int pos;

// Task 1: Convert infix expression to prefix
string parseExpression() {
    if (expression[pos] == '~') {
        pos++;
        return "~" + parseExpression();
    }

    if (expression[pos] == '(') {
        pos++;

        string left = parseExpression();
        char op = expression[pos++];

        string right = parseExpression();

        pos++; // Skip closing parenthesis

        return string(1, op) + left + right;
    }

    return string(1, expression[pos++]);
}

string infixToPrefix(string infix) {
    expression = "";

    for (char c : infix) {
        if (!isspace(static_cast<unsigned char>(c)))
            expression += c;
    }

    pos = 0;
    return parseExpression();
}
