/** @file common.cpp @brief Implementation of the shared helpers declared in common.h. */
#include "common.h"
#include <cctype>
#include <sstream>

using namespace std;

bool isOperatorToken(const string &t)
{
    return t == "~" || t == "+" || t == "*" || t == ">";
}

vector<string> splitWhitespace(const string &s)
{
    vector<string> tokens;
    istringstream iss(s);
    string t;
    while (iss >> t)
        tokens.push_back(t);
    return tokens;
}

string stripSpaces(const string &s)
{
    string r;
    r.reserve(s.size());
    for (char c : s)
        if (!isspace(static_cast<unsigned char>(c)))
            r += c;
    return r;
}

void freeTree(Node *root)
{
    if (!root)
        return;
    vector<Node *> st;
    st.push_back(root);
    while (!st.empty())
    {
        Node *n = st.back();
        st.pop_back();
        if (n->left)
            st.push_back(n->left);
        if (n->right)
            st.push_back(n->right);
        delete n;
    }
}

size_t countNodes(const Node *root)
{
    if (!root)
        return 0;
    size_t count = 0;
    vector<const Node *> st;
    st.push_back(root);
    while (!st.empty())
    {
        const Node *n = st.back();
        st.pop_back();
        ++count;
        if (n->left)
            st.push_back(n->left);
        if (n->right)
            st.push_back(n->right);
    }
    return count;
}
