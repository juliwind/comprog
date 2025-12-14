#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vii = vector<vector<int>>;
using vl = vector<long long>;
using vll = vector<vector<long long>>;
using vc = vector<char>;
#define fast                                                                   \
  ios::sync_with_stdio(0);                                                     \
  cin.tie(0);                                                                  \
  cout.tie(0);
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
#define F first
#define S second
#define PB push_back
#define all(a) (a).begin(), (a).end()
const ll mod = 1e9 + 7, N = 2e6 + 7, M = 2e6 + 7, INF = INT_MAX / 10;
ll powe(ll x, ll y) {
  x = x % mod, y = y % (mod - 1);
  ll ans = 1;
  while (y > 0) {
    if (y & 1) {
      ans = (1ll * x * ans) % mod;
    }
    y >>= 1;
    x = (1ll * x * x) % mod;
  }
  return ans;
}

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    int val;        // value in this position
    int prio;       // heap priority
    int sz;         // subtree size
    ll sum;         // subtree sum
    int mx;         // subtree max
    bool rev;       // lazy flag: reverse this subtree
    Node *l, *r;
    Node(int v, int p) : val(v), prio(p), sz(1), sum(v), mx(v),
                         rev(false), l(nullptr), r(nullptr) {}
};

int get_sz(Node* t) { return t ? t->sz : 0; }
ll  get_sum(Node* t) { return t ? t->sum : 0; }
int get_mx(Node* t) { return t ? t->mx : (int)-2e9; }

void pull(Node* t) {
    if (!t) return;
    t->sz  = 1 + get_sz(t->l) + get_sz(t->r);
    t->sum = t->val + get_sum(t->l) + get_sum(t->r);
    t->mx  = max(t->val, max(get_mx(t->l), get_mx(t->r)));
}

// lazy propagation: currently only reverse flag.
// extend here if you need range add / assign etc.
void push(Node* t) {
    if (!t || !t->rev) return;
    t->rev = false;
    swap(t->l, t->r);
    if (t->l) t->l->rev ^= 1;
    if (t->r) t->r->rev ^= 1;
}

// split by size: left has first k elements [0..k-1] (0-indexed)
void split(Node* t, int k, Node* &a, Node* &b) {
    if (!t) {
        a = b = nullptr;
        return;
    }
    push(t);
    if (get_sz(t->l) >= k) {
        // entire needed prefix lies in left subtree
        split(t->l, k, a, t->l);
        b = t;
        pull(b);
    } else {
        // take left subtree + this node, and some from right
        split(t->r, k - get_sz(t->l) - 1, t->r, b);
        a = t;
        pull(a);
    }
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

// ------------- basic 0-indexed operations on positions -------------

// insert value v at position pos (0 <= pos <= size), shifting [pos..] right
void insert_pos(Node* &root, int pos, int v) {
    Node *L, *R;
    split(root, pos, L, R);
    Node* nd = new Node(v, (int)rng());
    root = merge(merge(L, nd), R);
}

// erase element at position pos (0 <= pos < size)
void erase_pos(Node* &root, int pos) {
    Node *A, *B, *C;
    split(root, pos, A, B);      // A: [0..pos-1], B: [pos..]
    split(B, 1, B, C);           // B: [pos], C: [pos+1..]
    // optional: delete B's subtree to free memory
    root = merge(A, C);
}

// get value at position pos (0 <= pos < size)
int get_at(Node* root, int pos) {
    while (root) {
        push(root);
        int left_sz = get_sz(root->l);
        if (pos == left_sz) return root->val;
        if (pos < left_sz) root = root->l;
        else {
            pos -= left_sz + 1;
            root = root->r;
        }
    }
    return -1; // should not happen for valid pos
}

// set value at position pos to v
void set_at(Node* &root, int pos, int v) {
    Node *A, *B, *C;
    split(root, pos, A, B);          // A: [0..pos-1], B: [pos..]
    split(B, 1, B, C);               // B: [pos]
    if (B) {
        push(B);
        B->val = v;
        pull(B);
    }
    root = merge(merge(A, B), C);
}

// ------------- range queries/updates on [l, r] (0-indexed, inclusive) -------------

// range sum on [l, r]
ll range_sum(Node* &root, int l, int r) {
    Node *A, *B, *C;
    split(root, r + 1, A, C);    // A: [0..r], C: [r+1..]
    split(A, l, A, B);           // B: [l..r]
    ll ans = get_sum(B);
    root = merge(merge(A, B), C);
    return ans;
}

// range max on [l, r]
int range_max(Node* &root, int l, int r) {
    Node *A, *B, *C;
    split(root, r + 1, A, C);
    split(A, l, A, B);
    int ans = get_mx(B);
    root = merge(merge(A, B), C);
    return ans;
}

// reverse [l, r]
void range_reverse(Node* &root, int l, int r) {
    Node *A, *B, *C;
    split(root, r + 1, A, C);
    split(A, l, A, B);
    if (B) B->rev ^= 1;
    root = merge(merge(A, B), C);
}

// move [l, r] so it occupies positions [pos, pos + (r - l)] in the final array
// all indices 0-based, inclusive; assume 0 <= l <= r, 0 <= pos, pos + (r-l) < size
void range_move(Node* &root, int l, int r, int pos) {
    if (!root) return;
    if (pos >= l && pos <= r) return; // moving inside itself -> no change

    int len = r - l + 1;

    if (pos < l) {
        // original: P [0..pos-1], Q [pos..l-1], B [l..r], R [r+1..]
        // final:    P, B, Q, R
        Node *P, *Q, *B, *R, *temp, *temp2;
        split(root, pos, P, temp);              // P, (Q+B+R)
        split(temp, l - pos, Q, temp2);         // Q, (B+R)
        split(temp2, len, B, R);                // B, R
        root = merge(P, merge(B, merge(Q, R)));
    } else { // pos > r
        // original: P [0..l-1], B [l..r], Q [r+1..pos+len-1], R [pos+len..]
        // final:    P, Q, B, R
        Node *P, *B, *Q, *R, *temp, *temp2;
        split(root, l, P, temp);                // P, (B+Q+R)
        split(temp, len, B, temp2);             // B, (Q+R)
        int qlen = pos - l;                     // |Q|
        split(temp2, qlen, Q, R);               // Q, R
        root = merge(P, merge(Q, merge(B, R)));
    }
}

void collect(Node *t, vi &out) {
  if (!t)
    return;
  push(t);
  collect(t->l, out);
  out.push_back(t->val);
  collect(t->r, out);
}

// ===================== Problem solve() using the treap =====================

void solve() {
    int n, q;
    if (!(cin >> n >> q)) return;

    Node* root = nullptr;

    // build initial array: just merge at the end (0-indexed)
    for (int i = 0; i < n; ++i) {
        int v;
        cin >> v;
        Node* nd = new Node(v, (int)rng());
        root = merge(root, nd);
    }

    while (q--) {
        char type;
        cin >> type;
        if (type == 'i') {
            int pos, val;
            cin >> pos >> val;
            // input: "insert val after position pos", pos in [0..m] (1-based except 0)
            // treap is 0-indexed "insert at position idx (before idx)"
            int idx = pos;               // pos=0 -> idx=0 (front), pos=k -> after index k-1 => idx=k
            insert_pos(root, idx, val);
        } else if (type == 'd') {
            int pos;
            cin >> pos;
            int idx = pos - 1;           // 1-based -> 0-based
            erase_pos(root, idx);
        } else if (type == 'a') {
            int pos;
            cin >> pos;
            int idx = pos - 1;
            cout << get_at(root, idx) << '\n';
        } else if (type == 'u') {
            int pos, val;
            cin >> pos >> val;
            int idx = pos - 1;
            set_at(root, idx, val);
        } else if (type == 's') {
            int l, r;
            cin >> l >> r;
            int L = l - 1, R = r - 1;
            cout << range_sum(root, L, R) << '\n';
        } else if (type == 'x') {
            int l, r;
            cin >> l >> r;
            int L = l - 1, R = r - 1;
            cout << range_max(root, L, R) << '\n';
        } else if (type == 'm') {
            int l, r, pos;
            cin >> l >> r >> pos;
            int L = l - 1, R = r - 1;
            int P = pos - 1;             // desired starting index in 0-based
            range_move(root, L, R, P);
        } else if (type == 'r') {
            int l, r;
            cin >> l >> r;
            int L = l - 1, R = r - 1;
            range_reverse(root, L, R);
        }
    }

    vi res;
    collect(root, res);
    for (int i = 0; i < (int)res.size(); ++i) {
        cout << res[i] << ' ';
    }
    cout << '\n';
}

signed main() {
  fast;
  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
