#ifndef SIMPLE_BINARY_LIFTING_H
#define SIMPLE_BINARY_LIFTING_H

#include "template.h"

/**
 * Simple Binary Lifting (LCA)
 * * USAGE:
 * LCA lca(n, adj);
 * int dist = lca.dist(u, v);
 * int ancestor = lca.kth_ancestor(u, k);
 * * COMPLEXITY:
 * - Build: O(N log N)
 * - Query: O(log N)
 */
struct LCA {
    int n, LOG;
    vector<int> depth;
    vector<vector<int>> up;

    // Constructor: automatically builds table from adjacency list
    LCA(int _n, const vector<vector<int>>& adj, int root = 0) : n(_n) {
        LOG = 0;
        while ((1 << LOG) <= n) LOG++;
        
        depth.assign(n, 0);
        up.assign(n, vector<int>(LOG));
        
        dfs(root, root, 0, adj);
    }

    void dfs(int u, int p, int d, const vector<vector<int>>& adj) {
        depth[u] = d;
        up[u][0] = p;
        for (int i = 1; i < LOG; i++) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1, adj);
            }
        }
    }

    // --- Queries ---

    int get_lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
        // Lift u to same depth as v
        u = kth_ancestor(u, depth[u] - depth[v]);
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
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }

    int kth_ancestor(int u, int k) {
        if (depth[u] < k) return -1;
        for (int i = 0; i < LOG; i++) {
            if ((k >> i) & 1) {
                u = up[u][i];
            }
        }
        return u;
    }

    // Returns the K-th node on the simple path from U to V
    // k=0 returns u, k=dist(u,v) returns v
    int kth_node_on_path(int u, int v, int k) {
        int lca = get_lca(u, v);
        int d_u = depth[u] - depth[lca];
        int d_v = depth[v] - depth[lca];
        
        if (k <= d_u) return kth_ancestor(u, k);
        k -= d_u;
        if (k <= d_v) return kth_ancestor(v, d_v - k);
        return -1;
    }

    // Checks if node 'x' lies on the simple path between u and v
    bool on_path(int u, int v, int x) {
        int lca = get_lca(u, v);
        return (dist(lca, x) + dist(x, u) == dist(lca, u)) ||
               (dist(lca, x) + dist(x, v) == dist(lca, v));
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, q;
    cin >> n >> q;
    
    // Unweighted Adjacency List (vii from template.h)
    vii adj(n); 
    for(int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        --u; --v; // 0-indexed
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    LCA tree(n, adj);

    while(q--) {
        int u, v; cin >> u >> v;
        --u; --v;
        cout << tree.dist(u, v) << "\n";
    }
}
*/