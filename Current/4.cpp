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

int parent[N];
int sz[N];
int h[N];
vi adj[N];

void make_set(int v) {
  parent[v] = v;
  sz[v] = 1;
}

int find_set(int v) {
  if (v == parent[v])
    return v;
  return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
  a = find_set(a);
  b = find_set(b);
  if (a != b) {
    if (sz[a] < sz[b])
      swap(a, b);
    parent[b] = a;
    sz[a] += sz[b];
  }
}

void solve() {
  int n;
  cin >> n;
  vector<pii> nodes(n);
  for (int i = 0; i < n; ++i) {
    cin >> h[i];
    nodes[i] = {h[i], i};
    make_set(i);
    adj[i].clear();
  }

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    --u;
    --v;
    adj[u].PB(v);
    adj[v].PB(u);
  }

  sort(all(nodes));

  ll ans = 0;
  int i = 0;
  while (i < n) {
    int j = i;
    while (j < n && nodes[j].F == nodes[i].F)
      ++j;
    for (int k = i; k < j; ++k) {
      int u = nodes[k].S;
      for (int v : adj[u]) {
        if (h[v] <= h[u]) {
          union_sets(u, v);
        }
      }
    }
    for (int k = i; k < j; ++k) {
      int u = nodes[k].S;
      ans += (ll)(sz[find_set(u)] - 1);
    }
    i = j;
  }

  cout << ans << "\n";
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
