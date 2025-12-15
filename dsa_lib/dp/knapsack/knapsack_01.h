#ifndef KNAPSACK_01_H
#define KNAPSACK_01_H

#include "template.h"

/**
 * Standard 0/1 and Unbounded Knapsack Struct
 * * USAGE: 
 * // 1. Define solver
 * Knapsack01<200005> ks; 
 * * void solve() {
 * // 2. Solve 0/1 (default)
 * ks.solve(weights, values);
 * // OR Solve Unbounded (infinite items)
 * ks.solve_unbounded(weights, values);
 * cout << ks.query(capacity) << endl;
 * }
 * * COMPLEXITY: O(Capacity * N)
 */
template<int MAX_W, typename T = long long>
struct Knapsack01 {
    T dp[MAX_W];

    // Default Constructor
    Knapsack01() { reset(); }

    // Constructor 1: Direct weights and values
    // is_unbounded = false (default): 0/1 Knapsack
    // is_unbounded = true: Unbounded Knapsack
    Knapsack01(const vector<int>& weights, const vector<T>& values, bool is_unbounded = false) {
        if (is_unbounded) solve_unbounded(weights, values);
        else solve(weights, values);
    }

    // Constructor 2: Raw item pairs {{weight, value}, ...}
    Knapsack01(const vector<pair<int, T>>& items, bool is_unbounded = false) {
        vector<int> w; 
        vector<T> v;
        for(auto& p : items) {
            w.push_back(p.first);
            v.push_back(p.second);
        }
        if (is_unbounded) solve_unbounded(w, v);
        else solve(w, v);
    }

    void reset() {
        fill(dp, dp + MAX_W, 0);
    }

    // Standard 0/1 Knapsack: Each item at most once
    void solve(const vector<int>& weights, const vector<T>& values) {
        reset();
        int n = weights.size();
        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            T v = values[i];
            if (w == 0 || w >= MAX_W) continue; 
            for (int j = MAX_W - 1; j >= w; --j) {
                if (dp[j - w] + v > dp[j]) {
                    dp[j] = dp[j - w] + v;
                }
            }
        }
    }

    // Unbounded Knapsack: Each item infinite times
    void solve_unbounded(const vector<int>& weights, const vector<T>& values) {
        reset();
        int n = weights.size();
        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            T v = values[i];
            if (w == 0 || w >= MAX_W) continue;
            for (int j = w; j < MAX_W; ++j) {
                if (dp[j - w] + v > dp[j]) {
                    dp[j] = dp[j - w] + v;
                }
            }
        }
    }

    T query(int k) const {
        if (k < 0) return 0;
        if (k >= MAX_W) return dp[MAX_W - 1];
        return dp[k];
    }
};

#endif

/*
Usage:

void solve(){
    int n, capacity;
    cin >> n >> capacity;
    
    vi w(n); vl v(n);
    for(int i=0; i<n; ++i) cin >> w[i] >> v[i];

    // Template arg <10005> must be > capacity
    Knapsack01<10005> ks(w, v); 
    
    // For infinite items:
    // ks.solve_unbounded(w, v);

    cout << ks.query(capacity) << "\n";
}
*/