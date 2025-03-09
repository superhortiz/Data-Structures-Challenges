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

void in_order(vector<int> &result, const vector<Node> &tree, const int node, bool &is_ordered) {
    if (node == -1 || !is_ordered) return;
    in_order(result, tree, tree[node].left, is_ordered);
    if (!result.empty() && result.back() > tree[node].key) {
        is_ordered = false;
    } else {
        result.push_back(tree[node].key);
    }
    in_order(result, tree, tree[node].right, is_ordered);
}

bool IsBinarySearchTree(const vector<Node>& tree) {
    if (tree.size() == 0) return true;
    bool is_ordered = true;
    vector<int> result;
    in_order(result, tree, 0, is_ordered);
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
