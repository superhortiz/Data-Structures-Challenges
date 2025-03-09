#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
    char c;
    int size;
    Vertex* left;
    Vertex* right;
    Vertex* parent;

    Vertex(char c, Vertex* left, Vertex* right) : c(c), size(1), left(left), right(right), parent(NULL) {}
};

void update(Vertex* v) {
    if (v == NULL) return;

    // Update size attribute
    v->size = 1 + (v->left != NULL ? v->left->size : 0) + (v->right != NULL ? v->right->size : 0);

    // Update left parent connection
    if (v->left != NULL) {
        v->left->parent = v;
    }

    // Update right parent connection
    if (v->right != NULL) {
        v->right->parent = v;
    }
}

void small_rotation(Vertex* v) {
    Vertex* parent = v->parent;
    if (parent == NULL) {
        return;
    }
    Vertex* grandparent = v->parent->parent;
    if (parent->left == v) {
        Vertex* m = v->right;
        v->right = parent;
        parent->left = m;
    } else {
        Vertex* m = v->left;
        v->left = parent;
        parent->right = m;
    }
    update(parent);
    update(v);
    v->parent = grandparent;
    if (grandparent != NULL) {
        if (grandparent->left == parent) {
            grandparent->left = v;
        } else {
            grandparent->right = v;
        }
    }
}

void big_rotation(Vertex* v) {
    if (v->parent->left == v && v->parent->parent->left == v->parent) {
        // Zig-zig
        small_rotation(v->parent);
        small_rotation(v);
    } else if (v->parent->right == v && v->parent->parent->right == v->parent) {
        // Zig-zig
        small_rotation(v->parent);
        small_rotation(v);
    } else {
        // Zig-zag
        small_rotation(v);
        small_rotation(v);
    }  
}

// Makes splay of the given vertex and makes
// it the new root.
void splay(Vertex*& root, Vertex* v) {
    if (v == NULL) return;
    while (v->parent != NULL) {
        if (v->parent->parent == NULL) {
            small_rotation(v);
            break;
        }
        big_rotation(v);
    }
    root = v;
}

// Returns the size of the subtree rooted at the given node
int get_size(Vertex*& node) {
	if (node == NULL)
		return 0;
	return node->size;
}

// Finds the node corresponding to the given rank (0-based) in the in-order traversal
// of the tree with the given root. Splays the last visited (deepest) node
// to the root of the tree.
Vertex* find_rank(Vertex*& root, int rank) {
    Vertex* node = root;

    while (node != NULL) {
		if (get_size(node->left) > rank) {
			node = node->left;
		} else if (get_size(node->left) < rank) {
			rank -= get_size(node->left) + 1;
			node = node->right;
		} else {
            break;
        }
    }
    splay(root, node);
    return node;
}

// Splits the tree in 2 by the given node right
void split(Vertex* root, int rank, Vertex*& left, Vertex*& right) {
    right = find_rank(root, rank);
    splay(root, right);

    // If right is null, the node of rank is larger than all elements in the tree
    if (right == NULL) {
        left = root;
        return;
    }

    // Divide the tree
    left = right->left;
    right->left = NULL;
    if (left != NULL) {
        left->parent = NULL;
    }

    // Update the metadata of the split trees to ensure consistency
    update(left);
    update(right);
}

Vertex* merge(Vertex* left, Vertex* right) {
    // If any of the trees is empty, we just return the other tree
    if (left == NULL) return right;
    if (right == NULL) return left;

    // Look for the minimum key in the right tree
    Vertex* min_right = right;
    while (min_right->left != NULL) {
        min_right = min_right->left;
    }

    // Bring the minimum key to the top
    splay(right, min_right);

    // Connect the trees
    right->left = left;

    // Update and return the new root of the merged trees
    update(right);
    return right;
}

// Builds a balanced splay tree from the string 's'
// using the range [lo, hi] as indices.
Vertex* build_tree(const std::string &s, int lo, int hi) {
    // Base case: return NULL if the range is invalid
    if (lo > hi) return NULL;

    // Find the middle index to make the tree balanced
    int mid = lo + (hi - lo) / 2;

    // Recursively build the left and right subtrees
    Vertex* left = build_tree(s, lo, mid - 1);
    Vertex* right = build_tree(s, mid + 1, hi);

    // Create a new node for the current character
    Vertex* node = new Vertex(s[mid], left, right);

    // Update the node's metadata (size, parent pointers, etc.)
    update(node);
    return node;
}

// Performs an in-order traversal of the tree and appends the characters to the buffer
void in_order(std::vector<char>& buffer, Vertex* &node) {
    if (node == NULL) return;
    in_order(buffer, node->left);
    buffer.push_back(node->c);
    in_order(buffer, node->right);
}

class Rope {
	std::string s;
    Vertex* root;

public:
	Rope(const std::string &s) : s(s) {
        root = build_tree(s, 0, s.size() - 1);
    }

    // Processes the substring defined by [i, j] and re-inserts it starting at position k
	void process(int i, int j, int k) {
        Vertex* left = NULL;
        Vertex* middle = NULL;
        Vertex* right = NULL;

        // Split the tree into two parts: left (before i) and middle (starting from i)
        split(root, i, left, middle);

        // Split the middle part into two: middle (range [i, j]) and right (after j)
        split(middle, j + 1 - get_size(left), middle, right);

        // Remove the range [i, j] by merging left and right
        root = merge(left, right);

        // Split the updated tree into two parts: left (before k) and right (starting from k)
        split(root, k, left, right);

        // Reinsert the middle part (range [i, j]) at position k by merging left, middle, and right
        root = merge(merge(left, middle), right);
	}

	std::string result() {
        std::vector<char> buffer;
        in_order(buffer, root);
        s = std::string(buffer.begin(), buffer.end());
		return s;
	}
};

int main() {
	std::ios_base::sync_with_stdio(0);
	std::string s;
	std::cin >> s;
	Rope rope(s);

	int actions;
	std::cin >> actions;
	for (int action_index = 0; action_index < actions; ++action_index) {
		int i, j, k;
		std::cin >> i >> j >> k;
		rope.process(i, j, k);
	}
	std::cout << rope.result() << std::endl;
}
