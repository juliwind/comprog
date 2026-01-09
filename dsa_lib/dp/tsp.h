#ifndef TRAVELLING_SALESMAN_H
#define TRAVELLING_SALESMAN_H

#include "template.h"

/**
 * Travelling Salesman Problem (TSP) & Hamiltonian Path Solver using Bitmask DP
 * * * USAGE:
 * // 1. Define solver (Max Nodes usually <= 20)
 * TSP<20> tsp(n); 
 * * // 2. Add edges (u, v, cost) - supports directed/undirected
 * for(auto& e : edges) tsp.add_edge(e.u, e.v, e.w);
 * * // 3. Solve
 * // A. Round Trip (Cycle): Start -> All Nodes -> Start
 * long long cost = tsp.solve_tour(0); 
 * * // B. Open Path: Start -> All Nodes -> Specific End (or -1 for any)
 * long long cost = tsp.solve_path(0, -1);
 * * // 4. Get the actual path
 * vector<int> path = tsp.reconstruct_path();
 * * * COMPLEXITY: 
 * Time: O(N^2 * 2^N)
 * Space: O(N * 2^N)
 */
template<int MAX_N, typename T = long long>
struct TSP {
    T dp[1 << MAX_N][MAX_N];
    int parent[1 << MAX_N][MAX_N];
    T dist[MAX_N][MAX_N];
    int n;
    int start_node;
    int end_node_req;
    bool is_tour;
    const T INF = numeric_limits<T>::max() / 4;

    TSP(int nodes) : n(nodes) {
        for(int i = 0; i < MAX_N; ++i) {
            for(int j = 0; j < MAX_N; ++j) {
                dist[i][j] = (i == j) ? 0 : INF;
            }
        }
    }

    void add_edge(int u, int v, T w, bool directed = false) {
        dist[u][v] = min(dist[u][v], w);
        if (!directed) dist[v][u] = min(dist[v][u], w);
    }

    void _reset_dp() {
        for(int mask = 0; mask < (1 << n); ++mask) {
            for(int i = 0; i < n; ++i) {
                dp[mask][i] = INF;
                parent[mask][i] = -1;
            }
        }
    }

    void _run_dp(int start) {
        _reset_dp();
        start_node = start;
        dp[1 << start][start] = 0;
        for (int mask = 1; mask < (1 << n); ++mask) {
            for (int u = 0; u < n; ++u) {
                if (!((mask >> u) & 1) || dp[mask][u] == INF) continue;
                for (int v = 0; v < n; ++v) {
                    if ((mask >> v) & 1) continue;
                    if (dist[u][v] != INF) {
                        int next_mask = mask | (1 << v);
                        if (dp[mask][u] + dist[u][v] < dp[next_mask][v]) {
                            dp[next_mask][v] = dp[mask][u] + dist[u][v];
                            parent[next_mask][v] = u;
                        }
                    }
                }
            }
        }
    }

    // Solve Round Trip: Start -> ... -> Start
    T solve_tour(int start = 0) {
        is_tour = true;
        _run_dp(start);
        T min_cost = INF;
        int full_mask = (1 << n) - 1;
        int best_last = -1;
        for (int i = 0; i < n; ++i) {
            if (i == start) continue;
            if (dp[full_mask][i] != INF && dist[i][start] != INF) {
                T current_total = dp[full_mask][i] + dist[i][start];
                if (current_total < min_cost) {
                    min_cost = current_total;
                    best_last = i;
                }
            }
        }
        end_node_req = best_last; 
        return min_cost;
    }

    // 2. Solve Hamiltonian Path: Start -> ... -> End
    // Pass end = -1 to allow ending at any node
    T solve_path(int start = 0, int end = -1) {
        is_tour = false;
        _run_dp(start);

        int full_mask = (1 << n) - 1;
        
        if (end != -1) {
            end_node_req = end;
            return dp[full_mask][end];
        } else {
            T min_cost = INF;
            int best_end = -1;
            for (int i = 0; i < n; ++i) {
                if (dp[full_mask][i] < min_cost) {
                    min_cost = dp[full_mask][i];
                    best_end = i;
                }
            }
            end_node_req = best_end;
            return min_cost;
        }
    }

    // Reconstruct the path based on the last solve() call
    vector<int> reconstruct_path() {
        vector<int> path;
        int curr = end_node_req;
        int mask = (1 << n) - 1;
        if (curr == -1 || dp[mask][curr] == INF) return {};
        while (curr != -1) {
            path.push_back(curr);
            int prev = parent[mask][curr];
            mask ^= (1 << curr);
            curr = prev;
        }
        reverse(path.begin(), path.end());
        if (is_tour && !path.empty()) {
            path.push_back(start_node);
        }
        return path;
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, m;
    cin >> n >> m;

    // 1. Initialize (Template param > N)
    TSP<20> tsp(n);

    // 2. Input Graph
    for(int i=0; i<m; ++i) {
        int u, v; long long w;
        cin >> u >> v >> w;
        // Adjust for 0-indexing if input is 1-based
        --u; --v; 
        tsp.add_edge(u, v, w, false); // false = undirected
    }

    // 3. Solve
    // Example: Find min cost cycle starting at 0
    long long ans = tsp.solve_tour(0);

    if (ans >= tsp.INF) {
        cout << "-1\n";
    } else {
        cout << ans << "\n";
        
        // 4. Print Path
        vector<int> path = tsp.reconstruct_path();
        for(int x : path) cout << x + 1 << " "; // +1 for 1-based output
        cout << "\n";
    }
}
*/