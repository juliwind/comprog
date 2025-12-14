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
const ll mod = 1e9 + 7, N = 2e5 + 7, INF = INT_MAX;
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

const int LOG = 20;
int up[N][LOG];
int depth[N];
vi adj[N];

// initialize, when zero indexed: dfs_lca(0, 0, 0)
void dfs_lca(int u, int p, int d) {
  depth[u] = d;
  up[u][0] = p;
  for (int i = 1; i < LOG; i++) {
    up[u][i] = up[up[u][i - 1]][i - 1];
  }
  for (int v : adj[u]) {
    if (v != p) {
      dfs_lca(v, u, d + 1);
    }
  }
}

// kth ancestor of u
int kth_ancestor(int u, int k) {
  if (depth[u] < k)
    return -1;
  for (int i = 0; i < LOG; i++) {
    if ((k >> i) & 1) {
      u = up[u][i];
    }
  }
  return u;
}

// lca of u, v
int get_lca(int u, int v) {
  if (depth[u] < depth[v])
    swap(u, v);
  u = kth_ancestor(u, depth[u] - depth[v]);
  if (u == v)
    return u;
  for (int i = LOG - 1; i >= 0; i--) {
    if (up[u][i] != up[v][i]) {
      u = up[u][i];
      v = up[v][i];
    }
  }
  return up[u][0];
}

// returns distance between u, v
int get_dist(int u, int v) {
  return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

// true, if node x is on path u - v
bool on_path(int u, int v, int x) {
  int lca = get_lca(u, v);
  return (get_dist(lca, x) + get_dist(x, u) == get_dist(lca, u)) ||
         (get_dist(lca, x) + get_dist(x, v) == get_dist(lca, v));
}

int get_kth_on_path(int u, int v, int k) {
  int l = get_lca(u, v);
  int d = depth[u] - depth[l];
  if (k <= d)
    return kth_ancestor(u, k);
  return kth_ancestor(v, get_dist(u, v) - k);
}

int bfs(int start, int n) {
  vi d(n, -1);
  queue<int> q;
  q.push(start);
  d[start] = 0;
  int far = start;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    if (d[u] > d[far])
      far = u;
    for (int v : adj[u]) {
      if (d[v] == -1) {
        d[v] = d[u] + 1;
        q.push(v);
      }
    }
  }
  return far;
}

void solve() {
  int n, q;
  cin >> n >> q;
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    --u;
    --v;
    adj[u].PB(v);
    adj[v].PB(u);
  }
  dfs_lca(0, 0, 0);
  int d1 = bfs(0, n);
  int d2 = bfs(d1, n);
  while (q--) {
    int v, c;
    cin >> v >> c;
    --v;
    int dist1 = get_dist(v, d1);
    int dist2 = get_dist(v, d2);
    if (c <= dist1)
      cout << get_kth_on_path(v, d1, c) + 1 << "\n";
    else if (c <= dist2)
      cout << get_kth_on_path(v, d2, c) + 1 << "\n";
    else
      cout << "-1\n";
  }
}

signed main() {
  fast;
  int t = 1;
  // cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
