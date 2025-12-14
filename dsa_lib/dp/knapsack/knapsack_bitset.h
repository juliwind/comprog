#ifndef KNAPSACK_BITSET_H
#define KNAPSACK_BITSET_H

#include "template.h"

/**
 * Optimized Bitset Knapsack Struct
 * * USAGE: 
 * // 1. Define global solver (efficient memory reuse)
 * Knapsack<200005> ks; 
 * * void solve() {
 * // 2. Solve for specific items
 * ks.solve(weights, counts);
 * if (ks.possible(target)) ...
 * }
 * * COMPLEXITY: O(Target * N / 64)
 */
template<int MAX_W>
struct Knapsack {
    bitset<MAX_W> dp;

    // Default Constructor: useful for global declaration
    Knapsack() { dp.reset(); }

    // Constructor 1: Direct weights and counts
    Knapsack(const vector<int>& weights, const vector<int>& counts) {
        solve(weights, counts);
    }

    // Constructor 2: Raw item list {2, 5, 2, 2, 5}
    // Auto-compresses to: 2->3, 5->2
    Knapsack(vector<int> items) { // Pass-by-value to sort internally
        sort(items.begin(), items.end());
        vector<int> w, c;
        if (!items.empty()) {
            w.push_back(items[0]);
            c.push_back(1);
            for (size_t i = 1; i < items.size(); ++i) {
                if (items[i] == items[i-1]) {
                    c.back()++;
                } else {
                    w.push_back(items[i]);
                    c.push_back(1);
                }
            }
        }
        solve(w, c);
    }

    // Main Solver
    void solve(const vector<int>& weights, const vector<int>& counts) {
        dp.reset();
        dp[0] = 1;
        int n = weights.size();
        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            int c = counts[i];
            if (w == 0) continue;
            if (w >= MAX_W) continue;
            
            for (int k = 1; c > 0; k <<= 1) {
                int take = min(k, c);
                long long shift = 1LL * w * take;
                
                if (shift < MAX_W) {
                    dp |= (dp << shift);
                }
                c -= take;
            }
        }
    }

    bool possible(int k) const {
        if (k < 0 || k >= MAX_W) return false;
        return dp[k];
    }
};

#endif

/*
Usage:

void solve(){
    int n, k;
    cin >> n >> k;
    
    // CASE A: You have raw items (e.g., 3, 3, 5, 10, 3)
    vi items(n);
    for(int &x : items) cin >> x;

    // The template arg <200005> must be >= k
    Knapsack<200005> ks(items); 

    if(ks.possible(k)) cout << "YES\n";
    else cout << "NO\n";

    // CASE B: You have weights and counts (e.g., weight 5 appears 3 times)
    // vi weights = {5, 10};
    // vi counts = {3, 1};
    // Knapsack<200005> ks2(weights, counts);
}
*/