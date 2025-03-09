#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

class HeapBuilder {
private:
    vector<int> data_;
    vector<pair<int, int>> swaps_;

    void WriteResponse() const {
        cout << swaps_.size() << "\n";
        for (int i = 0; i < swaps_.size(); ++i) {
            cout << swaps_[i].first << " " << swaps_[i].second << "\n";
        }
    }

    void ReadData() {
        int n;
        cin >> n;
        data_.resize(n);
        for(int i = 0; i < n; ++i)
            cin >> data_[i];
    }

    void Sink(int i) {
        // Repeat until we reach the end of the array
        while (2 * i + 1 < data_.size()) {
            // Get the left child
            int j = 2 * i + 1;

            // Find the smallest child
            if (j + 1 < data_.size() && data_[j] > data_[j + 1]) {
                j++;
            }

            // If parent is smaller than the children, stop sinking
            if (data_[i] <= data_[j]) break;

            // Swap the parent with the smallest child
            swap(data_[i], data_[j]);
            swaps_.push_back(make_pair(i, j));

            // Update the index
            i = j;
        }
    }

    void GenerateSwaps() {
        swaps_.clear();
        
        for (int i = (data_.size() - 1) / 2; i >= 0; --i) {
            Sink(i);
        }
    }

public:
    void Solve() {
        ReadData();
        GenerateSwaps();
        WriteResponse();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    HeapBuilder heap_builder;
    heap_builder.Solve();
    return 0;
}
