#include <iostream>
#include <vector>

using namespace std;
typedef unsigned long long ull;
const int R = 256, m1 = 1000000007, m2 = 1000000009;

class Solver {
	string s;
	vector<ull> h1, h2, y1, y2;

public:	
	Solver(string s) : s(s), h1(s.size() + 1, 0), h2(s.size() + 1, 0), y1(s.size() + 1, 1), y2(s.size() + 1, 1) {
		// Precompute hash values for all prefixes
		// We use 2 hash functions to reduce the probabilities of collisions to (n/m1) * (n/m2)
		for (size_t i = 0; i < s.size(); ++i) {
			h1[i + 1] = (h1[i] * R + s[i]) % m1;
			h2[i + 1] = (h2[i] * R + s[i]) % m2;
		}

		// Precompute y = x^l
		for (size_t i = 1; i <= s.size(); ++i) {
			y1[i] = (R * y1[i - 1]) % m1;
			y2[i] = (R * y2[i - 1]) % m2;
		}
	}

	bool ask(int a, int b, int l) {
		// Use the first hashing to compare substrings
		ull h1_a = (h1[a + l] - y1[l] * h1[a] % m1 + m1) % m1;
		ull h1_b = (h1[b + l] - y1[l] * h1[b] % m1 + m1) % m1;
		if (h1_a != h1_b) return false;

		// Use the second hashing to compare substrings
		ull h2_a = (h2[a + l] - y2[l] * h2[a] % m2 + m2) % m2;
		ull h2_b = (h2[b + l] - y2[l] * h2[b] % m2 + m2) % m2;
		return h2_a == h2_b;
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0);

	string s;
	int q;
	cin >> s >> q;
	Solver solver(s);
	for (int i = 0; i < q; i++) {
		int a, b, l;
		cin >> a >> b >> l;
		cout << (solver.ask(a, b, l) ? "Yes\n" : "No\n");
	}
}
