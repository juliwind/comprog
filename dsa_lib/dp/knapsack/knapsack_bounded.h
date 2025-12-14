#ifndef KNAPSACK_BOUNDED_H
#define KNAPSACK_BOUNDED_H

#include "template.h"

/**
 * Optimized Bounded Knapsack Struct (Value Maximization)
 * * USAGE: 
 * // 1. Define solver with Max Capacity
 * BoundedKnapsack<200005> ks; 
 * * void solve() {
 * // 2. Solve for specific items
 * ks.solve(weights, values, counts);
 * cout << ks.query(capacity) << endl;
 * }
 * * COMPLEXITY: O(Capacity * N * log(Count))
 */
template<int MAX_W, typename T = long long>
struct BoundedKnapsack {
    T dp[MAX_W];

    // Default Constructor: useful for global declaration
    BoundedKnapsack() { reset(); }

    // Constructor 1: Direct weights, values, and counts
    BoundedKnapsack(const vector<int>& weights, const vector<T>& values, const vector<int>& counts) {
        solve(weights, values, counts);
    }

    // Constructor 2: Raw item list {{weight, value}, {weight, value}...}
    // Auto-compresses duplicate items (same weight AND same value)
    BoundedKnapsack(vector<pair<int, T>> items) { 
        sort(items.begin(), items.end());
        vector<int> w, c;
        vector<T> v;
        
        if (!items.empty()) {
            w.push_back(items[0].first);
            v.push_back(items[0].second);
            c.push_back(1);
            
            for (size_t i = 1; i < items.size(); ++i) {
                if (items[i] == items[i-1]) {
                    c.back()++;
                } else {
                    w.push_back(items[i].first);
                    v.push_back(items[i].second);
                    c.push_back(1);
                }
            }
        }
        solve(w, v, c);
    }

    // Reset DP table
    void reset() {
        // Use fill instead of memset for non-POD types support, though usually T=long long
        fill(dp, dp + MAX_W, 0); 
    }

    // Main Solver
    void solve(const vector<int>& weights, const vector<T>& values, const vector<int>& counts) {
        reset();
        int n = weights.size();
        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            T val = values[i];
            int c = counts[i];

            // Optimizations
            if (w == 0 || w >= MAX_W) continue;

            // Binary Decomposition (1, 2, 4...)
            for (int k = 1; c > 0; k <<= 1) {
                int take = min(k, c);
                int dw = w * take;
                T dv = val * take;

                // Standard 0/1 Knapsack Loop (Reverse)
                // We go down to 'dw' because indices lower than that can't fit this item
                for (int j = MAX_W - 1; j >= dw; --j) {
                    if (dp[j - dw] + dv > dp[j]) {
                        dp[j] = dp[j - dw] + dv;
                    }
                }
                c -= take;
            }
        }
    }

    // Query: Max value for capacity 'k'
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
    
    // CASE A: Raw items (e.g., pairs of {weight, value})
    // vector<pair<int, ll>> items(n);
    // for(auto &x : items) cin >> x.first >> x.second;
    // BoundedKnapsack<10005> ks(items);
    // cout << ks.query(capacity) << "\n";

    // CASE B: Distinct weights/values with counts
    vi w(n), c(n);
    vl v(n);
    for(int i=0; i<n; ++i) cin >> w[i] >> v[i] >> c[i];

    // Template arg <10005> must be > capacity
    BoundedKnapsack<10005> ks(w, v, c); 

    cout << ks.query(capacity) << "\n";
}
*/