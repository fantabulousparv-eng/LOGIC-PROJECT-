/** @file task5_evaluate.cpp @brief Implementation of Task 5. */
#include "task5_evaluate.h"
#include "task3_traversals.h"
#include <ostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

vector<string> collectAtoms(Node *root)
{
    vector<string> names;
    unordered_map<string, int> index;
    if (!root)
        return names;
    vector<Node *> st;
    st.push_back(root);
    while (!st.empty())
    { // pre-order => atoms numbered left to right
        Node *n = st.back();
        st.pop_back();
        if (!opSymbol(n))
        {
            auto it = index.find(n->val);
            if (it == index.end())
            {
                it = index.emplace(n->val, static_cast<int>(names.size())).first;
                names.push_back(n->val);
            }
            n->atomIdx = it->second;
        }
        if (n->right)
            st.push_back(n->right);
        if (n->left)
            st.push_back(n->left);
    }
    return names;
}

/** @brief Applies connective @p op to already-evaluated operands. */
static inline bool apply(char op, bool a, bool b)
{
    switch (op)
    {
    case '+':
        return a || b;
    case '*':
        return a && b;
    default:
        return !a || b; // '>' implication
    }
}

bool evaluateRecursive(const Node *n, const vector<char> &vals)
{
    char op = opSymbol(n);
    if (!op)
    {
        if (n->atomIdx < 0)
            throw logic_error("atoms not indexed - call collectAtoms() first");
        return vals[n->atomIdx] != 0;
    }
    if (op == '~')
        return !evaluateRecursive(n->left, vals);
    bool a = evaluateRecursive(n->left, vals);
    bool b = evaluateRecursive(n->right, vals);
    return apply(op, a, b);
}

bool evaluateOrdered(const vector<const Node *> &order, const vector<char> &vals, vector<char> &stack)
{
    stack.clear();
    for (const Node *n : order)
    {
        char op = opSymbol(n);
        if (!op)
        {
            if (n->atomIdx < 0)
                throw logic_error("atoms not indexed - call collectAtoms() first");
            stack.push_back(vals[n->atomIdx]);
        }
        else if (op == '~')
        {
            stack.back() = !stack.back();
        }
        else
        {
            bool b = stack.back() != 0;
            stack.pop_back();
            bool a = stack.back() != 0;
            stack.pop_back();
            stack.push_back(apply(op, a, b));
        }
    }
    return stack.back() != 0;
}

bool evaluateIterative(const Node *root, const vector<char> &vals)
{
    vector<char> stack;
    return evaluateOrdered(postOrderNodes(root), vals, stack);
}

TruthTableResult truthTable(Node *root, ostream *out, unsigned maxVars, unsigned long long maxPrintRows)
{
    TruthTableResult res;
    vector<string> atoms = collectAtoms(root);
    res.numVars = atoms.size();
    if (atoms.size() > maxVars || atoms.size() >= 63)
        return res; // too large: skipped

    res.computed = true;
    res.rows = 1ULL << atoms.size();
    const size_t n = atoms.size();
    vector<const Node *> order = postOrderNodes(root);
    vector<char> vals(n), scratch;
    scratch.reserve(order.size());

    if (out)
    {
        for (const string &a : atoms)
            *out << a << '\t';
        *out << "| result\n";
    }
    for (unsigned long long mask = 0; mask < res.rows; ++mask)
    {
        for (size_t i = 0; i < n; ++i)
            vals[i] = !((mask >> (n - 1 - i)) & 1ULL); // first row = all T
        bool r = evaluateOrdered(order, vals, scratch);
        if (r)
            ++res.satisfying;
        if (out && mask < maxPrintRows)
        {
            for (size_t i = 0; i < n; ++i)
                *out << (vals[i] ? 'T' : 'F') << '\t';
            *out << "| " << (r ? 'T' : 'F') << '\n';
        }
    }
    if (out && res.rows > maxPrintRows)
        *out << "... (" << (res.rows - maxPrintRows) << " more rows not printed)\n";
    res.satisfiable = res.satisfying > 0;
    res.tautology = res.satisfying == res.rows;
    return res;
}
