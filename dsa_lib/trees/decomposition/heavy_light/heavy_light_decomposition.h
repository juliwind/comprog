#ifndef HEAVY_LIGHT_DECOMPOSITION_H
#define HEAVY_LIGHT_DECOMPOSITION_H

#include "template.h"

/**
 * Heavy-Light Decomposition
 * Flattens tree into array where heavy paths are contiguous.
 * * USAGE:
 * // 1. Build
 * HLD hld(n, adj);
 * // 2. Path Query (Decomposes path u-v into O(log N) segments)
 * hld.process_path(u, v, [&](int l, int r) {
 * ans = max(ans, seg_tree.query(l, r));
 * });
 * // 3. Subtree Query (Subtree u is one contiguous segment)
 * auto [l, r] = hld.get_subtree(u);
 * ans = seg_tree.query(l, r);
 * * COMPLEXITY: O(N) build, O(log^2 N) path query
 */
struct HLD {
    int n;
    vector<int> parent;   // Parent of node
    vector<int> depth;    // Depth of node
    vector<int> heavy;    // Heavy child of node (-1 if none)
    vector<int> head;     // Head of the heavy chain the node belongs to
    vector<int> pos;      // Position in the flattened array (0-indexed)
    vector<int> sz;       // Subtree size
    int cur_pos;          // Current position for DFS traversal

    // Constructor: builds HLD from adjacency list
    // Assumes 0-indexed graph
    HLD(int _n, const vector<vector<int>>& adj, int root = 0) : n(_n) {
        parent.assign(n, -1);
        depth.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, 0);
        pos.assign(n, 0);
        sz.assign(n, 0);
        
        cur_pos = 0;
        
        // 1. Calculate sizes and find heavy edges
        dfs_sz(root, adj);
        
        // 2. Decompose chains and assign positions
        dfs_hld(root, adj);
    }

    // First pass: compute subtree sizes and identify heavy children
    void dfs_sz(int u, const vector<vector<int>>& adj) {
        sz[u] = 1;
        int max_sz = 0;
        for (int v : adj[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                depth[v] = depth[u] + 1;
                dfs_sz(v, adj);
                sz[u] += sz[v];
                
                // If this child is heavier, mark it
                if (sz[v] > max_sz) {
                    max_sz = sz[v];
                    heavy[u] = v;
                }
            }
        }
    }

    // Second pass: decompose into chains
    void dfs_hld(int u, const vector<vector<int>>& adj) {
        pos[u] = cur_pos++;
        
        // Go down heavy edge first (keeps chain contiguous)
        if (heavy[u] != -1) {
            head[heavy[u]] = head[u];
            dfs_hld(heavy[u], adj);
        }
        
        // Go down light edges
        for (int v : adj[u]) {
            if (v != parent[u] && v != heavy[u]) {
                head[v] = v; // New chain starts here
                dfs_hld(v, adj);
            }
        }
    }

    // LCA: Finds Lowest Common Ancestor
    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) u = parent[head[u]];
            else v = parent[head[v]];
        }
        return depth[u] < depth[v] ? u : v;
    }

    // Process Path: Apply a function 'op' to all segments on the path from u to v.
    // 'op' takes [l, r] inclusive in the flattened array.
    // Handles non-commutative operations correctly if op handles direction? 
    // This implementation is for commutative queries (Order doesn't strictly matter for segments).
    template<typename U>
    void process_path(int u, int v, U op) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            // Process segment from head[u] to u
            op(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        // Process last segment on same chain
        op(pos[u], pos[v]); 
    }

    // Variant for EDGE updates/queries (skips LCA node in final segment)
    template<typename U>
    void process_path_edges(int u, int v, U op) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            op(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        if (u != v) op(pos[u] + 1, pos[v]); // Skip u (LCA)
    }

    // Get Subtree Range: Returns [l, r] inclusive in flattened array
    pair<int, int> get_subtree(int u) {
        return {pos[u], pos[u] + sz[u] - 1};
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, q;
    cin >> n >> q;
    
    // 1. Read Tree
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v; --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 2. Build HLD
    HLD hld(n, adj);

    // 3. Build Segment Tree (on HLD flattened positions)
    // Note: If you have initial node values, map them: flat_val[pos[u]] = val[u]
    SegTree<long long> st(n); 
    
    while(q--) {
        int type; cin >> type;
        if (type == 1) { // Update node u
            int u, val; cin >> u >> val; --u;
            st.update(hld.pos[u], val);
        } else if (type == 2) { // Query Path u-v
            int u, v; cin >> u >> v; --u; --v;
            long long ans = 0; // Identity
            
            hld.process_path(u, v, [&](int l, int r) {
                ans = max(ans, st.query(l, r)); // Combine segments
            });
            cout << ans << "\n";
        } else if (type == 3) { // Query Subtree u
            int u; cin >> u; --u;
            auto [l, r] = hld.get_subtree(u);
            cout << st.query(l, r) << "\n";
        }
    }
}
*/