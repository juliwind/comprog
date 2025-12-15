#ifndef SEGTREE_LAZY_H
#define SEGTREE_LAZY_H

#include "template.h"

/**
 * Recursive Lazy Segment Tree (Range Update / Range Query)
 * * USAGE:
 * // 1. Define solver
 * LazySegTree<long long> st(n);
 * // 2. Updates and Queries
 * st.update(l, r, val); 
 * cout << st.query(l, r) << "\n";
 * int idx = st.find_first(l, r, predicate);
 * * COMPLEXITY: O(log N)
 * * MEMORY: 4 * N
 */
template<typename T>
struct LazySegTree {
    // --- CUSTOMIZE DATA & TAGS ---
    // CURRENT: Range Add / Range Sum
    
    T identity_val = 0;       // Identity for query (0 for sum, -INF for max)
    T identity_tag = 0;       // Identity for lazy tag (0 for add, -1 for set)

    // Merge two child node values
    T merge_nodes(const T& a, const T& b) {
        return a + b;
    }

    // Apply a tag to a node
    // len: range length (needed for Sum, ignore for Max/Min)
    void apply_tag(T& node_val, T& node_tag, const T& update_val, int len) {
        node_val += update_val * len; // Sum: val += add * len
        node_tag += update_val;       // Add: tag += add
        
        // FOR RANGE SET / RANGE MAX:
        // node_val = update_val;
        // node_tag = update_val;
    }
    // -----------------------------

    int n;
    vector<T> tree, lazy;

    // Default Constructor
    LazySegTree() : n(0) {}

    // Constructor 1: Size only
    LazySegTree(int _n) : n(_n) {
        tree.assign(4 * n, identity_val);
        lazy.assign(4 * n, identity_tag);
    }

    // Constructor 2: Initial vector
    LazySegTree(const vector<T>& v) : n(v.size()) {
        tree.assign(4 * n, identity_val);
        lazy.assign(4 * n, identity_tag);
        build(v, 1, 0, n - 1);
    }

    void push(int node, int start, int end) {
        if (lazy[node] == identity_tag) return;
        int mid = (start + end) >> 1;
        apply_tag(tree[node << 1], lazy[node << 1], lazy[node], mid - start + 1);
        apply_tag(tree[node << 1 | 1], lazy[node << 1 | 1], lazy[node], end - mid);
        lazy[node] = identity_tag;
    }

    void build(const vector<T>& v, int node, int start, int end) {
        if (start == end) {
            tree[node] = v[start];
            return;
        }
        int mid = (start + end) >> 1;
        build(v, node << 1, start, mid);
        build(v, node << 1 | 1, mid + 1, end);
        tree[node] = merge_nodes(tree[node << 1], tree[node << 1 | 1]);
    }

    // Range Update [l, r] inclusive
    void update(int l, int r, T val) {
        update(1, 0, n - 1, l, r, val);
    }

    void update(int node, int start, int end, int l, int r, T val) {
        if (l > end || r < start) return;
        if (l <= start && end <= r) {
            apply_tag(tree[node], lazy[node], val, end - start + 1);
            return;
        }
        push(node, start, end);
        int mid = (start + end) >> 1;
        update(node << 1, start, mid, l, r, val);
        update(node << 1 | 1, mid + 1, end, l, r, val);
        tree[node] = merge_nodes(tree[node << 1], tree[node << 1 | 1]);
    }

    // Range Query [l, r] inclusive
    T query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    T query(int node, int start, int end, int l, int r) {
        if (l > end || r < start) return identity_val;
        if (l <= start && end <= r) return tree[node];
        push(node, start, end);
        int mid = (start + end) >> 1;
        return merge_nodes(query(node << 1, start, mid, l, r),
                           query(node << 1 | 1, mid + 1, end, l, r));
    }

    // TREE WALK: Find first index i in [l, r] where pred(tree[i]) is true
    // Returns -1 if not found.
    template<typename U>
    int find_first(int l, int r, U pred) {
        return find_first(1, 0, n - 1, l, r, pred);
    }

    template<typename U>
    int find_first(int node, int start, int end, int l, int r, U pred) {
        // Optimization: if range outside [l,r] or aggregate fails pred, skip
        if (l > end || r < start || !pred(tree[node])) return -1;
        if (start == end) return start;
        
        push(node, start, end);
        int mid = (start + end) >> 1;
        
        // Try left child first
        int res = find_first(node << 1, start, mid, l, r, pred);
        if (res != -1) return res;
        
        // Try right child
        return find_first(node << 1 | 1, mid + 1, end, l, r, pred);
    }
};

#endif

/*
Usage:

void solve(){
    int n, q;
    cin >> n >> q;
    
    // CASE A: Build from vector
    // vector<ll> a(n);
    // for(auto &x : a) cin >> x;
    // LazySegTree<ll> st(a);

    // CASE B: Build empty (0s)
    LazySegTree<ll> st(n); 

    while(q--) {
        int type; cin >> type;
        if (type == 1) { // Range Add
            int l, r, v; cin >> l >> r >> v;
            st.update(l-1, r-1, v); // 0-indexed
        } else if (type == 2) { // Range Sum
            int l, r; cin >> l >> r;
            cout << st.query(l-1, r-1) << "\n";
        } else if (type == 3) { // Find First >= X
            int l, r, x; cin >> l >> r >> x;
            auto pred = [&](ll val) { return val >= x; };
            // Note: For find_first to work reliably, tree usually needs to be Max/Min
            cout << st.find_first(l-1, r-1, pred) << "\n";
        }
    }
}
*/