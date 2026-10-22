/** @file task2_prefix_to_tree.cpp @brief Implementation of Task 2. */
#include "task2_prefix_to_tree.h"
#include <stdexcept>

using namespace std;

/**
 * @brief Recursive helper: reads ONE complete prefix expression starting at @p idx.
 * @param tok tokens of the prefix expression
 * @param idx shared reading position (by reference so every call advances it for the next one)
 * @return root of the subtree built
 */
static Node *build(const vector<string> &tok, size_t &idx)
{
    if (idx >= tok.size())
        throw invalid_argument("prefix expression ended early");
    Node *node = new Node(tok[idx++]);
    if (node->val == "~")
    {
        node->left = build(tok, idx);
    }
    else if (isOperatorToken(node->val))
    {
        node->left = build(tok, idx);  // must run first
        node->right = build(tok, idx); // then this one
    } // else: atom -> leaf
    return node;
}

Node *prefixToTreeRecursive(const string &prefix)
{
    vector<string> tok = splitWhitespace(prefix);
    size_t idx = 0;
    Node *root = build(tok, idx);
    if (idx != tok.size())
    {
        freeTree(root);
        throw invalid_argument("extra tokens after a complete prefix expression");
    }
    return root;
}

Node *prefixToTreeIterative(const string &prefix)
{
    vector<string> tok = splitWhitespace(prefix);
    vector<Node *> st;

    auto fail = [&](Node *n, const char *msg)
    {
        delete n;
        for (Node *r : st)
            freeTree(r);
        throw invalid_argument(msg);
    };

    for (size_t i = tok.size(); i-- > 0;)
    {
        Node *n = new Node(tok[i]);
        if (tok[i] == "~")
        {
            if (st.empty())
                fail(n, "missing operand for ~");
            n->left = st.back();
            st.pop_back();
        }
        else if (isOperatorToken(tok[i]))
        {
            if (st.size() < 2)
                fail(n, "missing operand for a binary connective");
            n->left = st.back(); // first operand popped = left child
            st.pop_back();
            n->right = st.back();
            st.pop_back();
        }
        st.push_back(n);
    }
    if (st.size() != 1)
    {
        for (Node *r : st)
            freeTree(r);
        throw invalid_argument("malformed prefix expression");
    }
    return st.back();
}
