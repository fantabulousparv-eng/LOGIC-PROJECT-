#include<iostream>
#include <stack>
#include <string>
#include <cctype>
#include "common.h"
using namespace std;


// Task 2: Construct parse tree from prefix expression
Node* prefixToTree(string prefix) {
    stack<Node*> st;

    for (int i = (int)prefix.size() - 1; i >= 0; i--) {
        char c = prefix[i];

        if (isspace(static_cast<unsigned char>(c)))
            continue;

        Node* node = new Node;
        node->value = c;
        node->left = nullptr;
        node->right = nullptr;

        if (c == '~') {
            node->right = st.top();
            st.pop();
        }
        else if (c == '+' || c == '*' || c == '>') {
            node->left = st.top();
            st.pop();

            node->right = st.top();
            st.pop();
        }

        st.push(node);
    }

    Node* root = st.top();
    st.pop();

    return root;
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

