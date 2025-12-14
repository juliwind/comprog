#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vii = vector<vector<int>>;
using vl = vector<long long>;
using vll = vector<vector<long long>>;
using vc = vector<char>;
using vpl = vector<pair<int, long long>>;
using vvpl = vector<vector<pair<int, long long>>>;
#define fast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
typedef long long ll; typedef long double ld; typedef pair<int, int> pii;
#define F first
#define S second
#define PB push_back
#define all(a) (a).begin(), (a).end()
const ll mod = 1e9+7, N = 2e6+7, M = 2e6+7, INF = INT_MAX/10;
ll powe(ll x, ll y){ x = x%mod, y=y%(mod-1);ll ans = 1;while(y>0){if (y&1){ans = (1ll * x * ans)%mod;}y>>=1;x = (1ll * x * x)%mod;}return ans;}

// --- DSU (0-indexed) ---
struct DSU {
    vector<int> parent, siz;
    DSU(int n) {
        parent.resize(n);
        iota(all(parent), 0);
        siz.assign(n, 1);
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i), root_j = find(j);
        if (root_i != root_j) {
            if (siz[root_i] < siz[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            siz[root_i] += siz[root_j];
            return true;
        }
        return false;
    }
};

// --- MST / MaxST Implementation (0-indexed) ---
struct Edge {
    int u, v;
    ll w;
    bool operator<(const Edge& other) const { return w < other.w; }
};

struct MST_Result {
    ll cost;                
    vvpl tree_adj;          
    vector<Edge> edges;     
    int components;         
};

MST_Result getMST(int n, const vvpl& adj, bool min_mst = true) {
    vector<Edge> all_edges;
    for (int u = 0; u < n; ++u) {
        for (auto& p : adj[u]) {
            int v = p.F;
            ll w = p.S;
            if (u < v) {
                all_edges.PB({u, v, w});
            }
        }
    }
    if (min_mst) sort(all(all_edges)); 
    else sort(all_edges.rbegin(), all_edges.rend());
    DSU dsu(n);
    MST_Result res;
    res.cost = 0;
    res.components = n;
    res.tree_adj.resize(n);
    for (const auto& e : all_edges) {
        if (dsu.unite(e.u, e.v)) {
            res.cost += e.w;
            res.edges.PB(e);
            res.tree_adj[e.u].PB({e.v, e.w});
            res.tree_adj[e.v].PB({e.u, e.w});
            res.components--;
        }
    }
    return res;
}

void solve(){
    int n, m;
    cin >> n >> m;
    
    // 0-indexed adjacency list
    vvpl adj(n); 
    
    for(int i = 0; i < m; ++i){
        int u, v;
        ll w;
        cin >> u >> v >> w;
        --u; --v; 
        
        adj[u].PB({v, w});
        adj[v].PB({u, w});
    }

    MST_Result minTree = getMST(n, adj, true);
    
    // Check connectivity for 0-indexed graph
    if (minTree.edges.size() < n - 1) {
        cout << "IMPOSSIBLE" << endl; 
    } else {
        cout << minTree.cost << endl;
    }
}

signed main(){
    fast;
    int t = 1;
    // cin >> t;
    while(t--){
        solve();
    }
    return 0;
}