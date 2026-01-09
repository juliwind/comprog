
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vii = vector<vector<int>>;
using vl = vector<long long>;
using vll = vector<vector<long long>>;
using vc = vector<char>;
using vpl = vector<pair<int, long long>>;
using vvpl = vector<vector<pair<int, long long>>>;
#define fast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
typedef long long ll;typedef long double ld;typedef pair<int,int> pii;
#define PB push_back
#define all(a) (a).begin(), (a).end()
const ll mod = 1e9+7, N = 2e6+7, M = 2e6+7, INF = INT_MAX/10;
ll powe(ll x, ll y){ x = x%mod, y=y%(mod-1);ll ans = 1;while(y>0){if (y&1){ans = (1ll * x * ans)%mod;}y>>=1;x = (1ll * x * x)%mod;}return ans;}


vi zf(vi s) {
    int n = s.size();
    vi z(n);
    for(int i = 1, l = 0, r = 0; i < n; ++i) {
        if(i <= r)
            z[i] = min(r - i + 1, z[i - l]);
        while(i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if(i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

vi p2z(vi p) {
    int n = p.size();
    vi s(n);
    s[0] = 0; 
    int next_unique_char = 1;
    for(int i = 1; i < n; ++i) {
        if(p[i] > 0) {
            s[i] = s[p[i] - 1];
        } 
        else {
            s[i] = next_unique_char++;
        }
    }
    return zf(s);
}

vi z2p(vi z) {
    int n = z.size();
    vi pi(n, 0);
    for(int i = 1; i < n; ++i) {
        if(z[i] > 0) {
            int end_pos = i + z[i] - 1;
            pi[end_pos] = max(pi[end_pos], z[i]);
        }
    }
    for(int i = n - 1; i > 0; --i) {
        pi[i - 1] = max(pi[i - 1], pi[i] - 1);
    }
    return pi;
}






void solve(){
    string s; int n;
    cin >> s >> n;
    vi a(n);
    for(int i = 0; i < n; ++i) cin >> a[i];
    vi ans;
    if(s == "p2z") ans = p2z(a);
    else ans = z2p(a);
    for(auto x : ans) cout << x << ' ';
    cout << '\n';
}



signed main(){
    fast;
    int t = 1;
    //cin >> t;
    while(t--){
        solve();
    }
    return 0;
}
