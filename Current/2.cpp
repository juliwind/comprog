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

vi adj[N];
ll w[N];
vector<pii> cuts;
int cut_cnt;

ll dfs(int u, int p, ll limit, bool fill) {
  ll sum = w[u];
  vector<pair<ll, int>> children;
  for (int v : adj[u]) {
    if (v != p) {
      children.PB({dfs(v, u, limit, fill), v});
    }
  }
  sort(all(children));
  for (auto x : children) {
    if (sum + x.F <= limit) {
      sum += x.F;
    } else {
      ++cut_cnt;
      if (fill)
        cuts.PB({u + 1, x.S + 1});
    }
  }
  return sum;
}

void solve() {
  int n, k;
  cin >> n >> k;
  ll l = 0, r = 0;
  for (int i = 0; i < n; ++i) {
    cin >> w[i];
    l = max(l, w[i]);
    r += w[i];
  }
  for (int i = 0; i < n; ++i)
    adj[i].clear();
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    --u;
    --v;
    adj[u].PB(v);
    adj[v].PB(u);
  }

  ll ans = r;
  while (l <= r) {
    ll mid = l + (r - l) / 2;
    cut_cnt = 0;
    dfs(0, -1, mid, 0);
    if (cut_cnt <= k) {
      ans = mid;
      r = mid - 1;
    } else {
      l = mid + 1;
    }
  }
  cut_cnt = 0;
  cuts.clear();
  dfs(0, -1, ans, 1);
  cout << cuts.size() << ' ' << ans << "\n";
  for (auto p : cuts)
    cout << p.F << ' ' << p.S << "\n";
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
