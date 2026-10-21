
#include <stack>
#include <iostream>
#include "common.h"
using namespace std;



// Task 3.1: Recursive in-order
void inorderRecursive(Node* root) {
    if (root == nullptr) return;

    if (root->value == '~') {
        cout << "~";
        inorderRecursive(root->right);
    }
    else if (root->value == '+' ||
             root->value == '*' ||
             root->value == '>') {
        cout << "(";
        inorderRecursive(root->left);
        cout << root->value;
        inorderRecursive(root->right);
        cout << ")";
    }
    else {
        cout << root->value;
    }
}

// Task 3.2: Iterative in-order
void inorderIterative(Node* root) {
    stack<Node*> st;
    Node* current = root;

    while (current != nullptr || !st.empty()) {
        while (current != nullptr) {
            st.push(current);
            current = current->left;
        }

        current = st.top();
        st.pop();

        cout << current->value << " ";
        current = current->right;
    }
}

// Task 3.3: Recursive pre-order
void preorderRecursive(Node* root) {
    if (root == nullptr) return;

    cout << root->value << " ";
    preorderRecursive(root->left);
    preorderRecursive(root->right);
}

// Task 3.4: Iterative pre-order
void preorderIterative(Node* root) {
    if (root == nullptr) return;

    stack<Node*> st;
    st.push(root);

    while (!st.empty()) {
        Node* node = st.top();
        st.pop();

        cout << node->value << " ";

        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);
    }
}

// Task 3.5: Recursive post-order
void postorderRecursive(Node* root) {
    if (root == nullptr) return;

    postorderRecursive(root->left);
    postorderRecursive(root->right);
    cout << root->value << " ";
}

// Task 3.6: Iterative post-order
void postorderIterative(Node* root) {
    if (root == nullptr) return;

    stack<Node*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        Node* node = s1.top();
        s1.pop();

        s2.push(node);

        if (node->left) s1.push(node->left);
        if (node->right) s1.push(node->right);
    }

    while (!s2.empty()) {
        cout << s2.top()->value << " ";
        s2.pop();
    }
}
