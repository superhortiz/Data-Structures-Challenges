#include <iostream>
#include <vector>

using namespace std;
typedef unsigned long long ull;
const int R = 256, m = 1000000007;

vector<ull> get_prefixes(const string &text) {
	// Precompute hash values for all prefixes of the string text
	vector<ull> prefixes(text.size() + 1, 0);
	for (size_t i = 0; i < text.size(); ++i) {
		prefixes[i + 1] = (prefixes[i] * R + text[i]) % m;
	}
	return prefixes;
}

void number_mismatches(const vector<ull> &prefixes_text, const vector<ull> &prefixes_pattern, const vector<ull> &y,
					  const int index, const int lo, const int hi, int &n_mismatches) {

	// Use binary search to get the number of mismatches
	int l = hi - lo + 1;

	// If there is a match, there are no more mismatches in this substring
	ull hash_text = (prefixes_text[index + lo + l] - y[l] * prefixes_text[index + lo] % m + m) % m;
	ull hash_pattern = (prefixes_pattern[lo + l] - y[l] * prefixes_pattern[lo] % m + m) % m;
	if (hash_text == hash_pattern) return;

	// If lo == hi then there is a mismatch
	else if (lo >= hi) {
		n_mismatches++;
		return;
	}

	// Recurse in the 2 halves
	int mid = lo + (hi - lo) / 2;
	number_mismatches(prefixes_text, prefixes_pattern, y, index, lo, mid, n_mismatches);
	number_mismatches(prefixes_text, prefixes_pattern, y, index, mid + 1, hi, n_mismatches);
}

vector<int> solve(int k, const string &text, const string &pattern) {
	vector<int> pos;

	// Precompute hash values for all prefixes of text and pattern
	vector<ull> prefixes_text = get_prefixes(text);
	vector<ull> prefixes_pattern = get_prefixes(pattern);

	// Precompute y = x^l
	vector<ull> y(pattern.size() + 1, 1);
	for (size_t i = 1; i <= pattern.size(); ++i)
		y[i] = (R * y[i - 1]) % m;

	// Iterate over the string text
	int n_mismatches;
	for (int i = 0; i < text.size() - pattern.size() + 1; ++i) {
		n_mismatches = 0;

		// Get the number of mismatches
		number_mismatches(prefixes_text, prefixes_pattern, y, i, 0, pattern.size() - 1, n_mismatches);
		if (n_mismatches <= k)
			pos.push_back(i);
	}
	return pos;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(0);
	int k;
	string t, p;
	while (cin >> k >> t >> p) {
		auto a = solve(k, t, p);
		cout << a.size();
		for (int x : a)
			cout << " " << x;
		cout << "\n";
	}
}

