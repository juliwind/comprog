#ifndef MST_H
#define MST_H

#include "template.h"

/**
 * Kruskal's MST Algorithm
 * * USAGE:
 * MST<long long> mst(n);
 * mst.add_edge(u, v, w);
 * mst.solve(); // or mst.solve(false) for MaxST
 * cout << mst.cost << endl;
 * * COMPLEXITY: O(E log E)
 */
template<typename T = long long>
struct MST {
    struct Edge {
        int u, v;
        T w;
        int id; // Optional: Original index of edge
        bool operator<(const Edge& other) const { return w < other.w; }
    };

    // --- Internal DSU for Kruskal's ---
    struct DSU {
        vector<int> parent, siz;
        DSU(int n) {
            parent.resize(n);
            iota(parent.begin(), parent.end(), 0);
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

    // --- Data Members ---
    int n;
    vector<Edge> all_edges;
    
    // --- Results ---
    T cost;
    vector<Edge> mst_edges;          // The edges chosen for MST
    vector<vector<pair<int, T>>> tree_adj; // Adjacency list of the MST
    int components;                  // 1 if fully connected

    // Constructors
    MST(int nodes) : n(nodes), cost(0), components(nodes) {}

    // Constructor from existing adjacency list
    MST(int nodes, const vector<vector<pair<int, T>>>& adj) : n(nodes) {
        for (int u = 0; u < n; ++u) {
            for (auto& p : adj[u]) {
                if (u < p.first) add_edge(u, p.first, p.second);
            }
        }
    }

    void add_edge(int u, int v, T w, int id = -1) {
        all_edges.push_back({u, v, w, id});
    }

    // Main Solver
    // min_mst = true (default) -> Minimum Spanning Tree
    // min_mst = false          -> Maximum Spanning Tree
    void solve(bool min_mst = true) {
        // 1. Sort Edges
        if (min_mst) sort(all_edges.begin(), all_edges.end());
        else sort(all_edges.rbegin(), all_edges.rend());

        // 2. Init Result containers
        cost = 0;
        components = n;
        mst_edges.clear();
        tree_adj.assign(n, {});
        DSU dsu(n);

        // 3. Kruskal's Loop
        for (const auto& e : all_edges) {
            if (dsu.unite(e.u, e.v)) {
                cost += e.w;
                mst_edges.push_back(e);
                tree_adj[e.u].push_back({e.v, e.w});
                tree_adj[e.v].push_back({e.u, e.w});
                components--;
            }
        }
    }

    // Check if graph is connected
    bool is_connected() const {
        return components == 1;
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, m;
    cin >> n >> m;

    MST<long long> mst(n);
    
    for(int i = 0; i < m; ++i) {
        int u, v; ll w;
        cin >> u >> v >> w;
        mst.add_edge(u-1, v-1, w); // 0-indexed
    }

    mst.solve(); // Calculate MST#include <limits>
#include <type_traits>

    if (!mst.is_connected()) {
        cout << "IMPOSSIBLE\n";
    } else {
        cout << mst.cost << "\n";
        
        // Example: Iterate over MST edges
        // for(auto& e : mst.mst_edges) cout << e.u << " " << e.v << endl;
    }
}
*/