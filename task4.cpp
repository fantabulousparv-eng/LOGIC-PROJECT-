
#include <algorithm>
#include <queue>
#include "common.h"
using namespace std;



// Task 4.1: Recursive height
int heightRecursive(Node* root) {
    if (root == nullptr) return 0;

    return 1 + max(heightRecursive(root->left),
                   heightRecursive(root->right));
}

// Task 4.2: Iterative height using BFS
int heightIterative(Node* root) {
    if (root == nullptr) return 0;

    queue<Node*> q;
    q.push(root);

    int height = 0;

    while (!q.empty()) {
        int levelSize = (int)q.size();
        height++;

        for (int i = 0; i < levelSize; i++) {
            Node* node = q.front();
            q.pop();

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    return height;
}
