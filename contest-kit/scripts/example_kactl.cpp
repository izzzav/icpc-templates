#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#include "graph/LCA.h"               
#include "geometry/InsidePolygon.h"  

int main() {
    // дерево: 0-1, 0-2, 1-3, 1-4 (корень 0)
    vector<vi> g(5);
    vector<pii> edges = {{0, 1}, {0, 2}, {1, 3}, {1, 4}};
    for (auto [a, b] : edges) g[a].push_back(b), g[b].push_back(a);
    LCA lca(g);
    cout << lca.lca(3, 4) << ' ' << lca.lca(3, 2) << '\n';  // 1 0

    typedef Point<ll> Pt;
    vector<Pt> square = {Pt(0, 0), Pt(4, 0), Pt(4, 4), Pt(0, 4)};
    cout << inPolygon(square, Pt(2, 2)) << ' ' << inPolygon(square, Pt(5, 5)) << '\n';  // 1 0
}
