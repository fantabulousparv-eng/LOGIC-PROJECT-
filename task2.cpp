
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include "common.h"
#include <stdexcept>

using namespace std;

// Task 2: Construct a tree from single-character prefix notation
Node* prefixToTree(string prefix) {
    stack<Node*> st;

    for (int i = static_cast<int>(prefix.size()) - 1; i >= 0; i--) {
        char c = prefix[i];

        if (isspace(static_cast<unsigned char>(c))) {
            continue;
        }

        Node* node = new Node;
        node->value = string(1, c);
        node->left = nullptr;
        node->right = nullptr;

        if (c == '~') {
            if (st.empty())
                throw runtime_error("Invalid prefix expression");

            node->right = st.top();
            st.pop();
        }
        else if (c == '+' || c == '*' || c == '>') {
            if (st.size() < 2)
                throw runtime_error("Invalid prefix expression");

            node->left = st.top();
            st.pop();

            node->right = st.top();
            st.pop();
        }

        st.push(node);
    }

    if (st.size() != 1)
        throw runtime_error("Invalid prefix expression");

    Node* root = st.top();
    st.pop();
    return root;
}

void deleteTree(Node* root) {
    if (root == nullptr) return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
