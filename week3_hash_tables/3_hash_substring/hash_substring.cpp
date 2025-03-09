#include <iostream>
#include <string>
#include <vector>

using std::string;
typedef unsigned long long ull;
const int R = 256, p = 997;

struct Data {
    string pattern, text;
};

Data read_input() {
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

void print_occurrences(const std::vector<int>& output) {
    for (size_t i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

ull get_hash(const string& s, size_t len) {
    ull h = 0;
    for (size_t i = 0; i < len; ++i)
        h = (R * h + s[i]) % p;
    return h;
}

bool are_equal(const string& s, const string& t, size_t start) {
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] != t[start + i])
            return false;
    }
    return true;
}

std::vector<int> get_occurrences(const Data& input) {
    const string& s = input.pattern, t = input.text;
    std::vector<int> ans;

    // Get hash values for pattern and the first window in text
    ull hash_pattern = get_hash(s, s.size());
    ull hash_window = get_hash(t, s.size());

    // Precompute y = x^(|pattern|-1)
    int y = 1;
    for (size_t i = 0; i < s.size() - 1; ++i)
        y = (R * y) % p;

    // Slide the pattern over text
    for (size_t i = 0; i <= t.size() - s.size(); ++i) {
        // Check if the strings are equal
        if (hash_pattern == hash_window && are_equal(s, t, i))
            ans.push_back(i);

        if (i < t.size() - s.size()) {
            // Remove the influence of the outgoing character (the character at i-M)
            hash_window = (hash_window - y * t[i] % p + p) % p;

            // Add the influence of the incoming character (the character at i)
            hash_window = (hash_window * R + t[i + s.size()]) % p;
        }
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences(read_input()));
    return 0;
}