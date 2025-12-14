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
typedef long long ll;typedef long double ld;typedef pair<int,int> pii;
#define F first
#define S second
#define PB push_back
#define all(a) (a).begin(), (a).end()
const ll mod = 1e9+7, N = 2e5+7, INF = INT_MAX;
ll powe(ll x, ll y){ x = x%mod, y=y%(mod-1);ll ans = 1;while(y>0){if (y&1){ans = (1ll * x * ans)%mod;}y>>=1;x = (1ll * x * x)%mod;}return ans;}

/**
 * Advanced Binary Lifting Template
 * Supports: 
 * - Edge and Node weights
 * - Path Aggregates (Sum, Min, Max)
 * - Non-commutative Path Aggregates (left-to-right vs right-to-left)
 * - K-th ancestor and K-th node on path
 * - Distance and LCA
 */

struct BLNode {
    ll sum;
    ll mn;
    ll mx;    
    static BLNode merge(const BLNode& a, const BLNode& b) {
        return {a.sum + b.sum, min(a.mn, b.mn), max(a.mx, b.mx)};
    }
};

struct BinaryLifting {
    int n, LOG;
    vector<int> depth;
    vector<vector<int>> up;
    vector<vector<BLNode>> jump;
    BinaryLifting(int _n, const vvpl& adj, const vector<ll>& node_weights = {}, int root = 0) : n(_n) {
        LOG = 32 - __builtin_clz(n);
        depth.assign(n, 0);
        up.assign(n, vector<int>(LOG));
        jump.assign(n, vector<BLNode>(LOG));
        dfs(root, root, 0, 0, adj);
    }
    void dfs(int u, int p, int d, ll w, const vvpl& adj) {
        depth[u] = d;
        up[u][0] = p;
        jump[u][0] = {w, w, w};
        for (int i = 1; i < LOG; i++) {
            up[u][i] = up[up[u][i - 1]][i - 1];
            jump[u][i] = BLNode::merge(jump[u][i - 1], jump[up[u][i - 1]][i - 1]);
        }
        for (auto& edge : adj[u]) {
            if (edge.first != p) dfs(edge.first, u, d + 1, edge.second, adj);
        }
    }
    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        for (int i = LOG - 1; i >= 0; i--) {
            if (depth[u] - (1 << i) >= depth[v]) u = up[u][i];
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }
    int get_kth_ancestor(int u, int k) {
        if (k < 0 || k > depth[u]) return -1;
        for (int i = 0; i < LOG; i++) {
            if ((k >> i) & 1) u = up[u][i];
        }
        return u;
    }
    int get_kth_node_on_path(int u, int v, int k) {
        int lca = get_lca(u, v);
        int d_u_lca = depth[u] - depth[lca];
        int d_v_lca = depth[v] - depth[lca];
        if (k <= d_u_lca) return get_kth_ancestor(u, k);
        k -= d_u_lca;
        if (k <= d_v_lca) return get_kth_ancestor(v, d_v_lca - k);
        return -1;
    }
    int get_dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }
    BLNode query_path(int u, int v) {
        int lca = get_lca(u, v);
        BLNode res = {0, (ll)2e18, (ll)-2e18};
        
        auto walk = [&](int curr, int k) {
            for (int i = 0; i < LOG; i++) {
                if ((k >> i) & 1) {
                    res = BLNode::merge(res, jump[curr][i]);
                    curr = up[curr][i];
                }
            }
        };
        walk(u, depth[u] - depth[lca]);
        walk(v, depth[v] - depth[lca]);
        return res;
    }
};

void solve(){
    int n, q;
    cin >> n >> q;
    vvpl adj(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; ll w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // Precompute everything
    BinaryLifting bl(n, adj);

    while (q--) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        cout << "Min on path: " << bl.query_path(u, v).mn << "\n";
        cout << "Sum on path: " << bl.query_path(u, v).sum << "\n";
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
