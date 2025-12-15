#ifndef SEGTREE_ITERATIVE_H
#define SEGTREE_ITERATIVE_H

#include "template.h"

/**
 * Iterative Segment Tree (Point Update / Range Query)
 * * USAGE:
 * // 1. Define solver
 * SegTree<long long> st(n);
 * // 2. Update and Query
 * st.update(pos, val);
 * long long sum = st.query(l, r);
 * // 3. Binary Search on Tree (Find first index in [l,r] satisfying pred)
 * int idx = st.find_first(l, r, [&](long long x){ return x >= 10; });
 * * COMPLEXITY: O(log N) ops, O(N) memory
 */
template<typename T>
struct SegTree {
    int n;
    vector<T> tree;
    T identity_element;

    // --- CUSTOMIZE MERGE LOGIC ---
    // Example: Range Max
    // Change merge() to max(a,b) and identity_element to -INF
    T merge(const T& a, const T& b) {
        return max(a, b); 
    }
    // -----------------------------

    // Default Constructor
    SegTree() : n(0), identity_element(0) {}

    // Constructor 1: Size and Identity
    SegTree(int _n, T identity = 0) : n(_n), identity_element(identity) {
        tree.assign(2 * n, identity_element);
    }

    // Constructor 2: From vector
    SegTree(const vector<T>& v, T identity = 0) : n(v.size()), identity_element(identity) {
        tree.assign(2 * n, identity_element);
        for (int i = 0; i < n; ++i) tree[n + i] = v[i];
        for (int i = n - 1; i > 0; --i) tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
    }

    // Point Update: set val at pos (0-indexed)
    void update(int pos, T val) {
        for (tree[pos += n] = val; pos > 1; pos >>= 1) {
            tree[pos >> 1] = merge(tree[pos], tree[pos ^ 1]);
        }
    }
    
    // Helper: Add val to existing pos
    void add(int pos, T val) {
        // Note: For 'add', ensure merge() logic makes sense (e.g. sum)
        // If merge is max, this might need to be tree[n+pos] + val
        T current = tree[n + pos]; 
        update(pos, current + val);
    }

    // Range Query [l, r] inclusive
    T query(int l, int r) {
        T res_l = identity_element, res_r = identity_element;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res_l = merge(res_l, tree[l++]);
            if (r & 1) res_r = merge(tree[--r], res_r);
        }
        return merge(res_l, res_r);
    }

    /**
     * O(log N) Tree Search
     * Finds the first index 'i' in [l, r] such that 'pred(tree[i])' is true.
     * Returns -1 if not found.
     * * Useful for: "First element >= X in range"
     * * Predicate: bool f(T node_val)
     */
    template<typename U>
    int find_first(int l, int r, U pred) {
        // 1. Validate range and full range check
        if (l > r) return -1;
        
        // 2. Optimization: If the combined range doesn't satisfy, quit early
        // (Optional, saves time if often not found)
        T region_val = query(l, r);
        if (!pred(region_val)) return -1;

        // 3. Search Logic
        l += n; r += n + 1;
        int found_idx = -1;

        // We decompose [l, r) into O(log N) nodes. 
        // We must process them strictly Left-to-Right.
        // The standard loop processes 'l' Left-to-Right, but 'r' Right-to-Left.
        // We handle 'l' nodes immediately. We store 'r' nodes to check later.
        vector<int> r_nodes; 

        // Phase 1: Collect/Check covering nodes
        while (l < r) {
            if (l & 1) {
                if (pred(tree[l])) {
                    found_idx = l;
                    break; 
                }
                l++;
            }
            if (r & 1) {
                r_nodes.push_back(--r);
            }
            l >>= 1; r >>= 1;
        }

        // If not found in 'l' loop, check 'r' nodes (in reverse order added = Left-to-Right)
        if (found_idx == -1) {
            for (int i = r_nodes.size() - 1; i >= 0; --i) {
                if (pred(tree[r_nodes[i]])) {
                    found_idx = r_nodes[i];
                    break;
                }
            }
        }

        if (found_idx == -1) return -1;

        // Phase 2: Drill down to the leaf
        // We found a covering node 'found_idx' that satisfies pred. 
        // Now find the specific leaf inside it.
        while (found_idx < n) {
            found_idx <<= 1; // Try left child
            if (!pred(tree[found_idx])) {
                found_idx++; // Go right if left doesn't satisfy
            }
        }
        
        return found_idx - n;
    }
};

#endif

/*
Usage Example:

void solve() {
    int n, q;
    cin >> n >> q;
    
    // Example: Max Segment Tree
    vector<ll> a(n);
    for(auto& x : a) cin >> x;
    
    // Note: Ensure 'merge' is max(a,b) and identity is -INF inside struct
    SegTree<ll> st(a, -1e18); 

    while(q--) {
        int type; cin >> type;
        if (type == 1) { // Update
            int pos, v; cin >> pos >> v;
            st.update(pos-1, v);
        } else if (type == 2) { // Query Max
            int l, r; cin >> l >> r;
            cout << st.query(l-1, r-1) << "\n";
        } else if (type == 3) { // Find First >= X
            int l, r, x; cin >> l >> r >> x;
            auto pred = [&](ll val) { return val >= x; };
            cout << st.find_first(l-1, r-1, pred) + 1 << "\n";
        }
    }
}
*/