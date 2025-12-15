#ifndef PERSISTENT_SEGTREE_H
#define PERSISTENT_SEGTREE_H

#include "template.h"

/**
 * Persistent Segment Tree
 * * USAGE:
 * // 1. Build initial version (root 0)
 * PersistentSegTree<long long> pst(initial_vector);
 * // 2. Update creates NEW version
 * int root_v1 = pst.roots[0];
 * int root_v2 = pst.update(root_v1, pos, val);
 * // 3. Query any version
 * cout << pst.query(root_v1, l, r) << "\n";
 * cout << pst.query(root_v2, l, r) << "\n";
 * * COMPLEXITY: O(log N) per op
 * * MEMORY: O(N + Q log N)
 */
template<typename T>
struct PersistentSegTree {
    // --- CUSTOMIZE LOGIC ---
    // Default: Point Update / Range Sum
    
    T identity_val = 0;

    T merge(const T& a, const T& b) {
        return a + b;
    }
    // -----------------------

    struct Node {
        T val;
        int l, r;
    };

    int n;
    vector<Node> pool;
    vector<int> roots; // Stores root index of each version

    PersistentSegTree(const vector<T>& v) : n(v.size()) {
        pool.reserve(n * 4 + 200000); // Pre-allocate some space
        roots.push_back(build(v, 0, n - 1));
    }

    int new_node(T val) {
        pool.push_back({val, -1, -1});
        return pool.size() - 1;
    }

    int new_node(int l, int r) {
        // Pull value from children immediately
        T val_l = (l == -1) ? identity_val : pool[l].val;
        T val_r = (r == -1) ? identity_val : pool[r].val;
        pool.push_back({merge(val_l, val_r), l, r});
        return pool.size() - 1;
    }

    // Build initial tree
    int build(const vector<T>& v, int l, int r) {
        if (l == r) return new_node(v[l]);
        int mid = l + (r - l) / 2;
        return new_node(build(v, l, mid), build(v, mid + 1, r));
    }

    // Update: Returns NEW root index
    int update(int prev_root, int pos, T val) {
        return update(prev_root, 0, n - 1, pos, val);
    }

    int update(int node, int l, int r, int pos, T val) {
        if (l == r) return new_node(val);
        
        int mid = l + (r - l) / 2;
        int new_l = pool[node].l;
        int new_r = pool[node].r;

        if (pos <= mid) {
            new_l = update(pool[node].l, l, mid, pos, val);
        } else {
            new_r = update(pool[node].r, mid + 1, r, pos, val);
        }
        
        return new_node(new_l, new_r);
    }

    // Range Query on specific version
    T query(int root, int ql, int qr) {
        return query(root, 0, n - 1, ql, qr);
    }

    T query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l || node == -1) return identity_val;
        if (ql <= l && r <= qr) return pool[node].val;
        
        int mid = l + (r - l) / 2;
        return merge(query(pool[node].l, l, mid, ql, qr),
                     query(pool[node].r, mid + 1, r, ql, qr));
    }
    
    // K-th Smallest Number in Range (Standard Application)
    // REQUIRES: Tree built on Frequency Array (Sum Segment Tree)
    // Computes K-th smallest in subarray [L, R] using roots[time_R] - roots[time_L-1] logic.
    // 'val_range_L/R' are the bounds of values (e.g., 0 to 10^9)
    int query_kth(int root_L, int root_R, int val_l, int val_r, int k) {
        if (val_l == val_r) return val_l;
        
        int mid = val_l + (val_r - val_l) / 2;
        
        // Count elements in left subtree: count(root_R->left) - count(root_L->left)
        int l_node_L = (root_L == -1) ? -1 : pool[root_L].l;
        int l_node_R = (root_R == -1) ? -1 : pool[root_R].l;
        
        T count_left = (l_node_R == -1 ? 0 : pool[l_node_R].val) - 
                       (l_node_L == -1 ? 0 : pool[l_node_L].val);

        if (k <= count_left) {
            return query_kth(l_node_L, l_node_R, val_l, mid, k);
        } else {
            return query_kth((root_L == -1 ? -1 : pool[root_L].r), 
                             (root_R == -1 ? -1 : pool[root_R].r), 
                             mid + 1, val_r, k - count_left);
        }
    }
};

#endif

/*
Usage:

void solve() {
    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for(auto& x : a) cin >> x;

    // 1. Build Version 0
    PersistentSegTree<long long> pst(a);

    while(q--) {
        int type; cin >> type;
        if (type == 1) { // Update (Create new version)
            int k, pos, val; cin >> k >> pos >> val;
            // Create version (last_version + 1) based on version 'k'
            int new_root = pst.update(pst.roots[k], pos - 1, val);
            pst.roots.push_back(new_root);
        } else { // Query
            int k, l, r; cin >> k >> l >> r;
            cout << pst.query(pst.roots[k], l - 1, r - 1) << "\n";
        }
    }
}
*/