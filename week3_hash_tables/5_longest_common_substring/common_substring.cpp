#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;
typedef unsigned long long ull;
const int R = 256, m1 = 1000000007, m2 = 1000000009;

struct Answer {
	size_t i, j, len;
};

bool check_common_substring(const string &s, const string &t, size_t l, Answer &ans,
		vector<ull> &h1_s, vector<ull> &h1_t, vector<ull> &h2_s, vector<ull> &h2_t, vector<ull> &y1, vector<ull> &y2) {
	
	// Initialize hash maps for storing hash values of substrings of length l from string s
	static unordered_set<ull> hash_table_s1;
	static unordered_map<ull, size_t> hash_table_s2;

	// Clear the reused hash maps
	hash_table_s1.clear();
	hash_table_s2.clear();

	// Iterate over s, storing the hash values
	for (size_t i = 0; i < s.size() - l + 1; ++i) {
		hash_table_s1.insert((h1_s[i + l] - y1[l] * h1_s[i] % m1 + m1) % m1);
		hash_table_s2[(h2_s[i + l] - y2[l] * h2_s[i] % m2 + m2) % m2] = i;
	}

	// Iterate over t, comparing the stored hash values
	static ull hash_val;
	for (size_t i = 0; i < t.size() - l + 1; ++i) {
		hash_val = (h1_t[i + l] - y1[l] * h1_t[i] % m1 + m1) % m1;
		if (hash_table_s1.count(hash_val) > 0) {
			hash_val = (h2_t[i + l] - y2[l] * h2_t[i] % m2 + m2) % m2;
			if (hash_table_s2.count(hash_val) > 0) {
				ans = {hash_table_s2[hash_val], i, l};
				return true;
			}
		}
	}
	return false;
}

Answer solve(const string &s, const string &t) {
	// Precompute hash values for all prefixes of s and t
	size_t max_len = max(s.size(), t.size());
	vector<ull> h1_s(s.size() + 1, 0), h1_t(t.size() + 1, 0),
				h2_s(s.size() + 1, 0), h2_t(t.size() + 1, 0),
				y1(max_len + 1, 1), y2(max_len + 1, 1);

	for (size_t i = 0; i < s.size(); ++i) {
		h1_s[i + 1] = (h1_s[i] * R + s[i]) % m1;
		h2_s[i + 1] = (h2_s[i] * R + s[i]) % m2;
	}

	for (size_t i = 0; i < t.size(); ++i) {
		h1_t[i + 1] = (h1_t[i] * R + t[i]) % m1;
		h2_t[i + 1] = (h2_t[i] * R + t[i]) % m2;
	}

	// Precompute y = x^l
	for (size_t i = 1; i <= max_len; ++i) {
		y1[i] = (R * y1[i - 1]) % m1;
		y2[i] = (R * y2[i - 1]) % m2;
	}

	Answer ans = {0, 0, 0};
	size_t lo = 1, hi = min(s.size(), t.size());
	size_t i, j, mid;
	bool common_substring = false;
	while (lo <= hi) {
		mid = lo + (hi - lo) / 2;
		common_substring = check_common_substring(s, t, mid, ans, h1_s, h1_t, h2_s, h2_t, y1, y2);
		if (common_substring) {
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(0);
	string s, t;
	while (cin >> s >> t) {
		auto ans = solve(s, t);
		cout << ans.i << " " << ans.j << " " << ans.len << "\n";
	}
}
