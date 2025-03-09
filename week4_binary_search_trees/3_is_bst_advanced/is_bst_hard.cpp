#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Node {
    int key;
    int left;
    int right;

    Node() : key(0), left(-1), right(-1) {}
    Node(int key_, int left_, int right_) : key(key_), left(left_), right(right_) {}
};

void in_order(vector<int> &result, const vector<Node> &tree, const int node, bool &is_ordered, const int level, int &last_level_written) {
    // Base case: If the current node is null (-1) or if the tree is already deemed unordered, stop the recursion
    if (node == -1 || !is_ordered) return;

    // Recursively process the left subtree, increasing the level
    in_order(result, tree, tree[node].left, is_ordered, level + 1, last_level_written);

    // Check if the current node violates the Binary Search Tree (BST) property:
    // Case 1: The current key is less than or equal to the previous key in the result vector
    if (!result.empty() && result.back() > tree[node].key) {
        is_ordered = false;
    
    // Case 2: The current key is equal to the previous key, but the current node is on a higher level 
    // (indicating that the same key is incorrectly placed in the left subtree)
    } else if (!result.empty() && result.back() == tree[node].key && last_level_written > level) {
        is_ordered = false;

    // If the BST property is not violated, add the current key to the result vector and update the last written level
    } else {
        result.push_back(tree[node].key);
        last_level_written = level;
    }

    // Recursively process the right subtree, increasing the level
    in_order(result, tree, tree[node].right, is_ordered, level + 1, last_level_written);
}

bool IsBinarySearchTree(const vector<Node>& tree) {
    if (tree.size() == 0) return true;
    bool is_ordered = true;
    vector<int> result;
    int last_level_written = 0;
    in_order(result, tree, 0, is_ordered, 0, last_level_written);
    return is_ordered;
}

int main() {
    int nodes;
    cin >> nodes;
    vector<Node> tree;
    for (int i = 0; i < nodes; ++i) {
        int key, left, right;
        cin >> key >> left >> right;
        tree.push_back(Node(key, left, right));
    }
    if (IsBinarySearchTree(tree)) {
        cout << "CORRECT" << endl;
    } else {
        cout << "INCORRECT" << endl;
    }
    return 0;
}
