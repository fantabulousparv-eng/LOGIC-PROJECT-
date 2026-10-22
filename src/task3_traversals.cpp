/** @file task3_traversals.cpp @brief Implementation of Task 3. */
#include "task3_traversals.h"

using namespace std;

/** @brief Appends a token to @p out, inserting a single space between tokens. */
static void addToken(string &out, const string &tok)
{
    if (!out.empty())
        out += ' ';
    out += tok;
}

//  in-order 

static void inRec(const Node *n, string &out)
{
    if (!n)
        return;
    if (!n->left && !n->right)
    { // atom
        out += n->val;
    }
    else if (n->val == "~")
    { // unary: (~operand)
        out += "(~";
        inRec(n->left, out);
        out += ')';
    }
    else
    { // binary: (left op right)
        out += '(';
        inRec(n->left, out);
        out += n->val;
        inRec(n->right, out);
        out += ')';
    }
}

string inOrderRecursive(const Node *root)
{
    string out;
    inRec(root, out);
    return out;
}

string inOrderIterative(const Node *root)
{
    // Work item: either "expand this node" (n != nullptr) or "emit this literal" (n == nullptr).
    struct Item
    {
        const Node *n;
        const char *lit;
    };
    string out;
    if (!root)
        return out;
    vector<Item> st;
    st.push_back({root, nullptr});
    while (!st.empty())
    {
        Item it = st.back();
        st.pop_back();
        if (!it.n)
        {
            if (it.lit)
                out += it.lit;
            continue;
        }
        const Node *n = it.n;
        if (!n->left && !n->right)
        {
            out += n->val;
        }
        else if (n->val == "~")
        {
            // want: ( ~ left )  -> push in reverse order
            st.push_back({nullptr, ")"});
            st.push_back({n->left, nullptr});
            st.push_back({nullptr, "~"});
            st.push_back({nullptr, "("});
        }
        else
        {
            // want: ( left op right )
            st.push_back({nullptr, ")"});
            st.push_back({n->right, nullptr});
            st.push_back({nullptr, n->val.c_str()});
            st.push_back({n->left, nullptr});
            st.push_back({nullptr, "("});
        }
    }
    return out;
}

// pre-order 
static void preRec(const Node *n, string &out)
{
    if (!n)
        return;
    addToken(out, n->val);
    preRec(n->left, out);
    preRec(n->right, out);
}

string preOrderRecursive(const Node *root)
{
    string out;
    preRec(root, out);
    return out;
}

string preOrderIterative(const Node *root)
{
    string out;
    if (!root)
        return out;
    vector<const Node *> st;
    st.push_back(root);
    while (!st.empty())
    {
        const Node *n = st.back();
        st.pop_back();
        addToken(out, n->val);
        if (n->right)
            st.push_back(n->right); // right first so left is popped first
        if (n->left)
            st.push_back(n->left);
    }
    return out;
}

// post-order 

static void postRec(const Node *n, string &out)
{
    if (!n)
        return;
    postRec(n->left, out);
    postRec(n->right, out);
    addToken(out, n->val);
}

string postOrderRecursive(const Node *root)
{
    string out;
    postRec(root, out);
    return out;
}

vector<const Node *> postOrderNodes(const Node *root)
{
    vector<const Node *> order, st;
    const Node *cur = root, *last = nullptr;
    while (cur || !st.empty())
    {
        if (cur)
        {
            st.push_back(cur);
            cur = cur->left;
        }
        else
        {
            const Node *top = st.back();
            if (top->right && last != top->right)
            {
                cur = top->right; // go into the right subtree first
            }
            else
            {
                order.push_back(top); // both subtrees done -> visit
                last = top;
                st.pop_back();
            }
        }
    }
    return order;
}

string postOrderIterative(const Node *root)
{
    string out;
    for (const Node *n : postOrderNodes(root))
        addToken(out, n->val);
    return out;
}
