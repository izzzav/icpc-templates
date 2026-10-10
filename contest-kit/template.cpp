
#include <bits/stdc++.h>

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

int permToInt(vi& v) {
	int use = 0, i = 0, r = 0;
	for(int x:v) r = r * ++i + __builtin_popcount(use & -(1<<x)),
		use |= 1 << x;
	return r;
}

void solve() { 
    vi v{1, 3, 2, 4};
    cout << permToInt(v);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int t = 1;
    // cin >> t;
    while (t--) solve();
}
