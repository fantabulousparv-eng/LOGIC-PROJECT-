/** @file task4_height.cpp @brief Implementation of Task 4. */
#include "task4_height.h"
#include <algorithm>
#include <queue>

using namespace std;

int heightRecursive(const Node *root)
{
    if (!root)
        return 0;
    return 1 + max(heightRecursive(root->left), heightRecursive(root->right));
}

int heightIterative(const Node *root)
{
    if (!root)
        return 0;
    int levels = 0;
    queue<const Node *> q;
    q.push(root);
    while (!q.empty())
    {
        size_t width = q.size(); // all nodes currently queued belong to one level
        ++levels;
        for (size_t i = 0; i < width; ++i)
        {
            const Node *n = q.front();
            q.pop();
            if (n->left)
                q.push(n->left);
            if (n->right)
                q.push(n->right);
        }
    }
    return levels;
}
