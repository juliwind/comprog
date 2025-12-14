#ifndef TREAP_H
#define TREAP_H

#include "template.h"

/**
 * Implicit Treap (Array with powerful range operations)
 * * USAGE:
 * Treap<int> t(initial_vector);
 * t.insert(pos, val);
 * t.reverse(l, r);
 * long long sum = t.query_sum(l, r);
 * * COMPLEXITY: O(log N) for all operations
 */
template<typename T>
struct Treap {
    struct Node {
        T val;
        int prio;
        int sz;
        long long sum;
        T mx;
        bool rev; // Lazy reverse flag
        Node *l, *r;

        Node(T v, int p) : val(v), prio(p), sz(1), sum(v), mx(v), 
                           rev(false), l(nullptr), r(nullptr) {}
    };

    Node* root;
    mt19937 rng;

    // Default Constructor
    Treap() : root(nullptr) {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }

    // Constructor from vector
    Treap(const vector<T>& v) : root(nullptr) {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
        for(const T& x : v) append(x);
    }

    // --- Helpers ---
    int get_sz(Node* t) { return t ? t->sz : 0; }
    long long get_sum(Node* t) { return t ? t->sum : 0; }
    T get_mx(Node* t) { return t ? t->mx : numeric_limits<T>::lowest(); }

    void pull(Node* t) {
        if (!t) return;
        t->sz = 1 + get_sz(t->l) + get_sz(t->r);
        t->sum = t->val + get_sum(t->l) + get_sum(t->r);
        t->mx = max({t->val, get_mx(t->l), get_mx(t->r)});
    }

    void push(Node* t) {
        if (!t || !t->rev) return;
        t->rev = false;
        swap(t->l, t->r);
        if (t->l) t->l->rev ^= 1;
        if (t->r) t->r->rev ^= 1;
    }

    void split(Node* t, int k, Node* &a, Node* &b) {
        if (!t) { a = b = nullptr; return; }
        push(t);
        if (get_sz(t->l) >= k) {
            split(t->l, k, a, t->l);
            b = t;
        } else {
            split(t->r, k - get_sz(t->l) - 1, t->r, b);
            a = t;
        }
        pull(t);
    }

    Node* merge(Node* a, Node* b) {
        if (!a || !b) return a ? a : b;
        if (a->prio > b->prio) {
            push(a);
            a->r = merge(a->r, b);
            pull(a);
            return a;
        } else {
            push(b);
            b->l = merge(a, b->l);
            pull(b);
            return b;
        }
    }

    // --- Core Operations (0-indexed) ---

    // Insert 'val' at 'pos' (0 <= pos <= size)
    void insert(int pos, T val) {
        Node *L, *R;
        split(root, pos, L, R);
        Node* nd = new Node(val, (int)rng());
        root = merge(merge(L, nd), R);
    }

    // Append to end (fast insert)
    void append(T val) {
        insert(get_sz(root), val);
    }

    // Erase element at 'pos'
    void erase(int pos) {
        Node *L, *mid, *R;
        split(root, pos, L, R);
        split(R, 1, mid, R);
        // delete mid; // Optional: free memory
        root = merge(L, R);
    }

    // Get value at 'pos'
    T get(int pos) {
        Node* curr = root;
        while (curr) {
            push(curr);
            int left_sz = get_sz(curr->l);
            if (pos == left_sz) return curr->val;
            if (pos < left_sz) curr = curr->l;
            else {
                pos -= left_sz + 1;
                curr = curr->r;
            }
        }
        return T(); // Should not happen
    }

    // Set value at 'pos'
    void set(int pos, T val) {
        Node *L, *mid, *R;
        split(root, pos, L, R);
        split(R, 1, mid, R);
        if (mid) {
            push(mid);
            mid->val = mid->sum = mid->mx = val; // Reset logic
            // Note: If T is complex, you might need manual pull logic here
            pull(mid); 
        }
        root = merge(merge(L, mid), R);
    }

    // Range Sum [l, r]
    long long query_sum(int l, int r) {
        Node *L, *mid, *R;
        split(root, r + 1, L, R);
        split(L, l, L, mid);
        long long ans = get_sum(mid);
        root = merge(merge(L, mid), R);
        return ans;
    }

    // Range Max [l, r]
    T query_max(int l, int r) {
        Node *L, *mid, *R;
        split(root, r + 1, L, R);
        split(L, l, L, mid);
        T ans = get_mx(mid);
        root = merge(merge(L, mid), R);
        return ans;
    }

    // Reverse range [l, r]
    void reverse(int l, int r) {
        Node *L, *mid, *R;
        split(root, r + 1, L, R);
        split(L, l, L, mid);
        if (mid) mid->rev ^= 1;
        root = merge(merge(L, mid), R);
    }

    // Move range [l, r] to new position 'pos'
    // 'pos' is the target index in the array *before* the move logic happens
    // (See standard CP problems "Cyclic Shift" or "Cut and Paste")
    void move(int l, int r, int pos) {
        if (!root || (pos >= l && pos <= r)) return;
        int len = r - l + 1;
        
        Node *P, *Q, *B, *R_part, *temp, *temp2;
        
        if (pos < l) {
            // Move block B=[l..r] to before l (at pos)
            // Original: P[0..pos-1], Q[pos..l-1], B[l..r], R_part[r+1..]
            // Target:   P, B, Q, R_part
            split(root, pos, P, temp);
            split(temp, l - pos, Q, temp2);
            split(temp2, len, B, R_part);
            root = merge(P, merge(B, merge(Q, R_part)));
        } else {
            // Move block B=[l..r] to after r (at pos)
            // Original: P[0..l-1], B[l..r], Q[r+1..pos+len-1], R_part[pos+len..]
            // Target:   P, Q, B, R_part
            split(root, l, P, temp);
            split(temp, len, B, temp2);
            int q_len = pos - l; 
            split(temp2, q_len, Q, R_part);
            root = merge(P, merge(Q, merge(B, R_part)));
        }
    }

    // Convert to vector
    vector<T> to_vector() {
        vector<T> res;
        auto dfs = [&](auto&& self, Node* t) -> void {
            if (!t) return;
            push(t);
            self(self, t->l);
            res.push_back(t->val);
            self(self, t->r);
        };
        dfs(dfs, root);
        return res;
    }
    
    int size() { return get_sz(root); }
};

#endif

/*
Usage Example:

void solve() {
    int n, q;
    cin >> n >> q;
    
    // 1. Build from vector
    vi a(n);
    for(int& x : a) cin >> x;
    Treap<int> t(a);

    // 2. Process Queries
    while(q--) {
        char type; cin >> type;
        if(type == 'I') { // Insert
            int pos, val; cin >> pos >> val;
            t.insert(pos, val);
        }
        else if(type == 'S') { // Sum [l, r]
            int l, r; cin >> l >> r;
            cout << t.query_sum(l, r) << "\n";
        }
        else if(type == 'R') { // Reverse [l, r]
            int l, r; cin >> l >> r;
            t.reverse(l, r);
        }
        else if(type == 'M') { // Move [l, r] to pos
            int l, r, pos; cin >> l >> r >> pos;
            t.move(l, r, pos);
        }
    }
    
    // 3. Print final array
    vi final_arr = t.to_vector();
    for(int x : final_arr) cout << x << " ";
    cout << "\n";
}
*/