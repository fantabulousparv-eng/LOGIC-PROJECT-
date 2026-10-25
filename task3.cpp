
#include <stack>
#include <iostream>
#include "common.h"

using namespace std;

// Task 3.1: Recursive inorder
void inorderRecursive(Node* root) {
    if (root == nullptr) return;

    if (root->value == "~") {
        cout << "~";
        inorderRecursive(root->right);
    }
    else if (root->value == "+" ||
             root->value == "*" ||
             root->value == ">") {
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

// Task 3.2: Iterative inorder
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

// Task 3.3: Recursive preorder
void preorderRecursive(Node* root) {
    if (root == nullptr) return;

    cout << root->value << " ";
    preorderRecursive(root->left);
    preorderRecursive(root->right);
}

// Task 3.4: Iterative preorder
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

// Task 3.5: Recursive postorder
void postorderRecursive(Node* root) {
    if (root == nullptr) return;

    postorderRecursive(root->left);
    postorderRecursive(root->right);
    cout << root->value << " ";
}

// Task 3.6: Iterative postorder
void postorderIterative(Node* root) {
    if (root == nullptr) return;

    stack<Node*> first, second;
    first.push(root);

    while (!first.empty()) {
        Node* node = first.top();
        first.pop();

        second.push(node);

        if (node->left) first.push(node->left);
        if (node->right) first.push(node->right);
    }

    while (!second.empty()) {
        cout << second.top()->value << " ";
        second.pop();
    }
}
