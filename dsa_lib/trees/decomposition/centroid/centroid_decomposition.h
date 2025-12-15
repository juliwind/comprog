#ifndef CENTROID_DECOMPOSITION_H
#define CENTROID_DECOMPOSITION_H

#include "template.h"

/**
 * Centroid Decomposition (Complete)
 * Handles "Closest Marked Node" type queries efficiently.
 * * USAGE:
 * // 1. Build
 * CentroidDecomposition cd(n, adj);
 * // 2. Update: Mark node 'u' as active (red)
 * cd.update(u);
 * // 3. Query: Distance to closest active node
 * int dist = cd.query(u);
 * * COMPLEXITY: 
 * - Build: O(N log N)
 * - Update/Query: O(log^2 N)
 */
struct CentroidDecomposition {
    int n;
    
    // Centroid Tree Structure
    vector<int> parent;       // Parent in Centroid Tree (-1 for root)
    vector<int> sub_sz;       // Subtree size helper
    vector<bool> visited;     // "Removed" nodes
    const vector<vector<int>>& adj;

    // Solver Specific Data (Customize here)
    // Stores min distance to a "red" node in the subtree of centroid 'c'
    vector<int> min_dist;     

    // --- Internal LCA for Distances ---
    // Minimal binary lifting needed for O(log N) distance queries
    int LOG;
    vector<int> depth;
    vector<vector<int>> up;

    CentroidDecomposition(int _n, const vector<vector<int>>& _adj) 
        : n(_n), adj(_adj) {
        
        // 1. Init Centroid Structures
        parent.assign(n, -1);
        sub_sz.assign(n, 0);
        visited.assign(n, false);
        
        // Init Solver Data (Infinity initially)
        min_dist.assign(n, 1e9); 

        // 2. Init LCA / Distances
        LOG = 0;
        while ((1 << LOG) <= n) LOG++;
        depth.assign(n, 0);
        up.assign(n, vector<int>(LOG));
        dfs_lca(0, 0, 0); // Build LCA table

        // 3. Build Centroid Tree
        build(0, -1);
    }

    // --- LCA Logic ---
    void dfs_lca(int u, int p, int d) {
        depth[u] = d;
        up[u][0] = p;
        for (int i = 1; i < LOG; i++) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        for (int v : adj[u]) {
            if (v != p) dfs_lca(v, u, d + 1);
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

    int get_dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }

    // --- Centroid Build Logic ---
    void calc_subtree_size(int u, int p) {
        sub_sz[u] = 1;
        for (int v : adj[u]) {
            if (v != p && !visited[v]) {
                calc_subtree_size(v, u);
                sub_sz[u] += sub_sz[v];
            }
        }
    }

    int find_centroid(int u, int p, int total_nodes) {
        for (int v : adj[u]) {
            if (v != p && !visited[v] && sub_sz[v] > total_nodes / 2) {
                return find_centroid(v, u, total_nodes);
            }
        }
        return u;
    }

    void build(int u, int p) {
        calc_subtree_size(u, -1);
        int centroid = find_centroid(u, -1, sub_sz[u]);

        parent[centroid] = p;
        visited[centroid] = true;

        for (int v : adj[centroid]) {
            if (!visited[v]) {
                build(v, centroid);
            }
        }
    }

    // --- Updates & Queries ---

    // Update: Mark node 'u' (paint red)
    void update(int u) {
        int curr = u;
        while (curr != -1) {
            int d = get_dist(u, curr);
            min_dist[curr] = min(min_dist[curr], d);
            curr = parent[curr];
        }
    }

    // Query: Shortest distance from 'u' to any marked node
    int query(int u) {
        int curr = u;
        int ans = 1e9;
        while (curr != -1) {
            int d = get_dist(u, curr);
            // Result is distance to centroid + closest marked node in that centroid's domain
            ans = min(ans, d + min_dist[curr]);
            curr = parent[curr];
        }
        // If ans > n, it means no nodes are marked
        return (ans > n) ? -1 : ans; 
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, m;
    cin >> n >> m;
    
    // 1. Read Graph
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v; --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 2. Build CD
    CentroidDecomposition cd(n, adj);

    // 3. Process Queries
    // Type 1: Paint node u
    // Type 2: Ask closest painted node to u
    while(m--) {
        int t, v; cin >> t >> v; --v;
        if(t == 1) {
            cd.update(v);
        } else {
            cout << cd.query(v) << "\n";
        }
    }
}
*/