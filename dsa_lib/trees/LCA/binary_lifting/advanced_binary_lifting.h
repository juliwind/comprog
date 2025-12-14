#ifndef BINARY_LIFTING_H
#define BINARY_LIFTING_H

#include "template.h"

/**
 * Advanced Binary Lifting / LCA Struct
 * * USAGE:
 * BinaryLifting<long long> bl(n, adj);
 * int lca = bl.lca(u, v);
 * auto info = bl.query_path(u, v);
 * cout << info.sum << " " << info.mn << endl;
 * * FEATURES:
 * - LCA, Distance, K-th Ancestor
 * - Path Aggregates (Sum, Min, Max)
 * - Edge weights supported (stored on child node)
 * * COMPLEXITY:
 * - Build: O(N log N)
 * - Query: O(log N)
 */
template<typename T = long long>
struct BinaryLifting {
    
    // Aggregate Node
    struct Node {
        T sum;
        T mn;
        T mx;
        
        // Identity for merging (0 for sum, infinity for min/max)
        static Node identity() {
            return {0, std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest()};
        }
        
        static Node merge(const Node& a, const Node& b) {
            return {
                a.sum + b.sum,
                std::min(a.mn, b.mn),
                std::max(a.mx, b.mx)
            };
        }
    };

    int n, LOG;
    vector<int> depth;
    vector<vector<int>> up;
    vector<vector<Node>> jump;

    // Constructor: automatically builds table from adjacency list
    // adj should be vector<vector<pair<int, T>>> where T is weight type
    BinaryLifting(int _n, const vector<vector<pair<int, T>>>& adj, int root = 0) : n(_n) {
        LOG = 0;
        while ((1 << LOG) <= n) LOG++;
        LOG++; // Safety buffer
        
        depth.assign(n, 0);
        up.assign(n, vector<int>(LOG));
        jump.assign(n, vector<Node>(LOG)); // Default init
        
        // Root has weight 0 edge to itself
        dfs(root, root, 0, 0, adj);
    }

    void dfs(int u, int p, int d, T w, const vector<vector<pair<int, T>>>& adj) {
        depth[u] = d;
        up[u][0] = p;
        jump[u][0] = {w, w, w};
        
        for (int i = 1; i < LOG; i++) {
            int parent = up[u][i - 1];
            up[u][i] = up[parent][i - 1];
            jump[u][i] = Node::merge(jump[u][i - 1], jump[parent][i - 1]);
        }
        
        for (auto& edge : adj[u]) {
            int v = edge.first;
            T weight = edge.second;
            if (v != p) {
                dfs(v, u, d + 1, weight, adj);
            }
        }
    }

    // --- Standard Queries ---

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
        // Lift u to same depth as v
        for (int i = LOG - 1; i >= 0; i--) {
            if (depth[u] - (1 << i) >= depth[v]) {
                u = up[u][i];
            }
        }
        
        if (u == v) return u;
        
        // Lift both until just below LCA
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    int kth_ancestor(int u, int k) {
        if (k > depth[u]) return -1;
        for (int i = 0; i < LOG; i++) {
            if ((k >> i) & 1) u = up[u][i];
        }
        return u;
    }

    // Finds the node K steps away from u towards v
    int kth_node_on_path(int u, int v, int k) {
        int lc = lca(u, v);
        int d_u = depth[u] - depth[lc];
        int d_v = depth[v] - depth[lc];
        
        if (k <= d_u) return kth_ancestor(u, k);
        k -= d_u;
        if (k <= d_v) return kth_ancestor(v, d_v - k);
        return -1;
    }

    // --- Path Aggregate Query ---
    
    Node query_path(int u, int v) {
        int lc = lca(u, v);
        Node res = Node::identity();

        // Helper to lift and merge
        auto lift = [&](int& curr, int target_depth) {
            for (int i = LOG - 1; i >= 0; i--) {
                if (depth[curr] - (1 << i) >= target_depth) {
                    res = Node::merge(res, jump[curr][i]);
                    curr = up[curr][i];
                }
            }
        };

        // Merge path u -> lca
        lift(u, depth[lc]);
        // Merge path v -> lca
        lift(v, depth[lc]);
        
        // Note: Edge weights are stored on the CHILD.
        // The edge (lca, parent_of_lca) is NOT included, which is correct for paths.
        return res;
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, q;
    cin >> n >> q;

    // 1. Build Adjacency List (Pairs: {neighbor, weight})
    // Note: Template.h defines vvpl as vector<vector<pair<int, long long>>>
    vvpl adj(n);
    for(int i = 0; i < n - 1; ++i) {
        int u, v; ll w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // 2. Initialize
    BinaryLifting<long long> bl(n, adj);

    // 3. Queries
    while(q--) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        
        int l = bl.lca(u, v);
        int d = bl.dist(u, v);
        auto info = bl.query_path(u, v);
        
        cout << "LCA: " << l + 1 << "\n";
        cout << "Dist: " << d << "\n";
        cout << "Min Edge: " << info.mn << "\n";
    }
}
*/