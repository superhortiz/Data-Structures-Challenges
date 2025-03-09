#include <cstdio>

// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
    int key;
    // Sum of all the keys in the subtree - remember to update
    // it after each operation that changes the tree.
    long long sum;
    Vertex* left;
    Vertex* right;
    Vertex* parent;

    Vertex(int key, long long sum, Vertex* left, Vertex* right, Vertex* parent) 
    : key(key), sum(sum), left(left), right(right), parent(parent) {}
};

void update(Vertex* v) {
    if (v == NULL) return;

    // Update sum attribute
    v->sum = v->key + (v->left != NULL ? v->left->sum : 0ll) + (v->right != NULL ? v->right->sum : 0ll);

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

// Searches for the given key in the tree with the given root
// and calls splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given key.
// Otherwise, returns a pointer to the node with the smallest
// bigger key (next value in the order).
// If the key is bigger than all keys in the tree, 
// returns NULL.
Vertex* find(Vertex*& root, int key) {
    Vertex* v = root;
    Vertex* last = root;
    Vertex* next = NULL;
    while (v != NULL) {
        if (v->key >= key && (next == NULL || v->key < next->key)) {
            next = v;
        }
        last = v;
        if (v->key == key) {
            break;      
        }
        if (v->key < key) {
            v = v->right;
        } else {
            v = v->left;
        }
    }
    splay(root, last);
    return next;
}

void split(Vertex* root, int key, Vertex*& left, Vertex*& right) {
    right = find(root, key);
    splay(root, right);

    // If right is null, the key is larger than all elements in the tree
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
    // If key is present in the tree, right will include it.
    // If the key is not present in the tree, right will be the root of the tree containing
    // the node with the closest key that was found during the find operation.
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

// Code that uses splay tree to solve the problem

Vertex* root = NULL;

void insert(int x) {
    // Using split and merge to add a new node in the tree
    Vertex* left = NULL;
    Vertex* right = NULL;
    Vertex* new_vertex = NULL;

    // Split the tree using the key of the new node
    split(root, x, left, right);
    if (right == NULL || right->key != x) {
        new_vertex = new Vertex(x, x, NULL, NULL, NULL);
    }

    // Merge the left tree with the new node, and both with the right tree
    root = merge(merge(left, new_vertex), right);
}

bool find(int x) {  
    // Find the node where the key x should be and splay the tree with the deepest visited node.
    // If the key is not in the tree, return the node with the smallest key larger than x.
    Vertex* node = find(root, x);

    // Check if the node is not null and its key matches x exactly
    if (node != NULL && node->key == x)
        return true;
    return false;
}

void erase(int x) {
    // Using split and merge to delete a node from the tree

    // If the key is not in the tree, then there is nothing to do
    if (!find(x)) return;

    // Split the tree into two parts: 
    // 'left' contains keys less than the key to remove,
    // 'right' contains keys greater than or equal to the key.
    Vertex* left = NULL;
    Vertex* right = NULL;
    split(root, x, left, right);

    // The node to delete is the root of the right tree. Remove and deallocate it.
    if (right != NULL) {
        Vertex* node_to_delete = right;
        right = right->right;
        // Deallocate memory
        delete node_to_delete;
        if (right != NULL)
            // If there is a remaining right subtree, detach it from the deleted node by resetting its parent
            right->parent = NULL;
    }

    // Merge the left and right trees and update the root
    root = merge(left, right);
}

long long sum(int from, int to) {
    // Using split and merge to get the sum of keys in the range
    Vertex* left = NULL;
    Vertex* middle = NULL;
    Vertex* right = NULL;

    // Split the tree into three parts:
    // 'left' contains keys strictly less than 'from',
    // 'middle' contains keys in the range [from, to],
    // and 'right' contains keys strictly greater than 'to'
    split(root, from, left, middle);
    split(middle, to + 1, middle, right);

    // Retrieve the sum of the range, stored in the 'sum' attribute of the root of the middle tree.
    long long ans = 0;
    if (middle != NULL) {
        ans = middle->sum;
    }

    // Restore the tree and return the answer
    root = merge(merge(left, middle), right);
    return ans;  
}

const int MODULO = 1000000001;

int main(){
    int n;
    scanf("%d", &n);
    int last_sum_result = 0;
    for (int i = 0; i < n; i++) {
        char buffer[10];
        scanf("%s", buffer);
        char type = buffer[0];
        switch (type) {
            case '+' : {
                int x;
                scanf("%d", &x);
                insert((x + last_sum_result) % MODULO);
                
            } break;
            case '-' : {
                int x;
                scanf("%d", &x);
                erase((x + last_sum_result) % MODULO);
                
            } break;            
            case '?' : {
                int x;
                scanf("%d", &x);
                printf(find((x + last_sum_result) % MODULO) ? "Found\n" : "Not found\n");
            } break;
            case 's' : {
                int l, r;
                scanf("%d %d", &l, &r);
                long long res = sum((l + last_sum_result) % MODULO, (r + last_sum_result) % MODULO);
                printf("%lld\n", res);
                last_sum_result = int(res % MODULO);
            }
        }
    }
    return 0;
}
