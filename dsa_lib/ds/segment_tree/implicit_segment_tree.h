#ifndef IMPLICIT_SEGTREE_H
#define IMPLICIT_SEGTREE_H

#include "template.h"

/**
 * Implicit (Dynamic) Segment Tree
 * Handles huge coordinates (up to 10^18) with sparse updates.
 * * USAGE:
 * // 1. Define Range [0, 1e9]
 * ImplicitSegTree<long long> st(0, 1e9);
 * // 2. Range Update & Query
 * st.update(l, r, val);
 * cout << st.query(l, r) << "\n";
 * * MEMORY: O(Q * log(Range))
 * * COMPLEXITY: O(log(Range)) per op
 */
template<typename T>
struct ImplicitSegTree {
    // --- CUSTOMIZE LOGIC ---
    // Default: Range Add / Range Sum
    
    T identity_val = 0;
    T identity_tag = 0;

    T merge(const T& a, const T& b) {
        return a + b;
    }

    // Apply tag to a node covering range [l, r]
    void apply_tag(T& val, T& tag, T change, long long l, long long r) {
        val += change * (r - l + 1); // Sum: val += add * len
        tag += change;               // Add: tag += add
        
        // FOR MAX/SET:
        // val = change; tag = change;
    }
    // -----------------------

    struct Node {
        T val;
        T lazy;
        int l = -1, r = -1; // Indices in the pool (-1 = null)
        Node(T v, T tag) : val(v), lazy(tag) {}
    };

    long long min_idx, max_idx;
    vector<Node> pool;

    // Constructor: Define the full coordinate range [L, R]
    ImplicitSegTree(long long L, long long R) : min_idx(L), max_idx(R) {
        pool.reserve(500000); // Optional: Reserve memory to prevent reallocations
        create_node();        // Create root at index 0
    }

    int create_node() {
        pool.emplace_back(identity_val, identity_tag);
        return pool.size() - 1;
    }

    // Lazy Propagation
    void push(int idx, long long l, long long r) {
        if (pool[idx].lazy == identity_tag) return;
        
        long long mid = l + (r - l) / 2;
        
        // Create children if they don't exist
        if (pool[idx].l == -1) pool[idx].l = create_node();
        if (pool[idx].r == -1) pool[idx].r = create_node();

        // Apply tag to children
        apply_tag(pool[pool[idx].l].val, pool[pool[idx].l].lazy, pool[idx].lazy, l, mid);
        apply_tag(pool[pool[idx].r].val, pool[pool[idx].r].lazy, pool[idx].lazy, mid + 1, r);

        pool[idx].lazy = identity_tag;
    }

    // Range Update
    void update(long long ql, long long qr, T val) {
        update(0, min_idx, max_idx, ql, qr, val);
    }

    void update(int idx, long long l, long long r, long long ql, long long qr, T val) {
        if (l > qr || r < ql) return;
        if (l >= ql && r <= qr) {
            apply_tag(pool[idx].val, pool[idx].lazy, val, l, r);
            return;
        }
        
        push(idx, l, r);
        long long mid = l + (r - l) / 2;
        
        // Create children on demand if recursing
        if (pool[idx].l == -1) pool[idx].l = create_node();
        update(pool[idx].l, l, mid, ql, qr, val);
        
        if (pool[idx].r == -1) pool[idx].r = create_node();
        update(pool[idx].r, mid + 1, r, ql, qr, val);

        // Pull up
        T res_l = (pool[idx].l == -1) ? identity_val : pool[pool[idx].l].val;
        T res_r = (pool[idx].r == -1) ? identity_val : pool[pool[idx].r].val;
        pool[idx].val = merge(res_l, res_r);
    }

    // Range Query
    T query(long long ql, long long qr) {
        return query(0, min_idx, max_idx, ql, qr);
    }

    T query(int idx, long long l, long long r, long long ql, long long qr) {
        if (l > qr || r < ql || idx == -1) return identity_val;
        if (l >= ql && r <= qr) return pool[idx].val;

        push(idx, l, r);
        long long mid = l + (r - l) / 2;
        
        return merge(query(pool[idx].l, l, mid, ql, qr),
                     query(pool[idx].r, mid + 1, r, ql, qr));
    }
};

#endif

/*
Usage:

void solve() {
    // 1. Define large range, e.g., 0 to 10^9 or 10^18
    // Note: Use long long for coordinates
    long long N = 1e9 + 7;
    ImplicitSegTree<long long> st(0, N);

    int q;
    cin >> q;
    while(q--) {
        int type; cin >> type;
        if (type == 1) { // Update [l, r]
            long long l, r, v;
            cin >> l >> r >> v;
            st.update(l, r, v);
        } else { // Query [l, r]
            long long l, r;
            cin >> l >> r;
            cout << st.query(l, r) << "\n";
        }
    }
}
*/