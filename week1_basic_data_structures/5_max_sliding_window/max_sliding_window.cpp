#include <iostream>
#include <vector>
#include <limits>

using std::cin;
using std::cout;
using std::vector;
using std::max;

class StackWithMax {
    vector<int> stack, max;

public:
    void push(int value) {
        stack.push_back(value);
        if (max.empty()) {
            max.push_back(value);
        } else {
            max.push_back(std::max(value, max.back()));
        }
    }

    void pop() {
        if (!stack.empty()) {
            stack.pop_back();
            max.pop_back();
        }
    }

    int get_max() const {
        if (stack.empty()) return -std::numeric_limits<int>::infinity();
        return max.back();
    }

    int back() const {
        return stack.back();
    }

    bool empty() const {
        return stack.empty();
    }
};

void max_sliding_window(vector<int> const & A, int w) {
    // Use 2 stacks to store the sliding window's elements
    StackWithMax stack1, stack2;

    // Fill the first w-1 elements in stack1
    for (size_t i = 0; i < w - 1; ++i) {
        stack1.push(A[i]);
    }

    // Iterate moving the sliding window
    int back, window_max;
    for (size_t i = w - 1; i < A.size(); ++i) {
        // Add the next element to complete w elements in the sliding window
        stack1.push(A[i]);

        // If stack2 is empty, move all the elements from stack1 to stack2
        if (stack2.empty()) {
            while (!stack1.empty()) {
                back = stack1.back();
                stack2.push(back);
                stack1.pop();
            }
        }

        // Get the maximum value from stack1 and stack2
        window_max = max(stack1.get_max(), stack2.get_max());
        cout << window_max << " ";

        // Remove the mostleft value from the sliding window
        stack2.pop();
    }
    return;
}

int main() {
    int n = 0;
    cin >> n;

    vector<int> A(n);
    for (size_t i = 0; i < n; ++i)
        cin >> A.at(i);

    int w = 0;
    cin >> w;

    max_sliding_window(A, w);

    return 0;
}
