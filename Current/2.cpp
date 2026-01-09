
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



template<long long BASE = 31, long long MOD = 1000000007>
struct PolyHash {
    vector<long long> h, p;

    PolyHash() {}

    PolyHash(const string& s) {
        build(s);
    }

    void build(const string& s) {
        int n = s.length();
        h.resize(n);
        p.resize(n);
        
        p[0] = 1;
        h[0] = s[0] % MOD;
        
        for (int i = 1; i < n; ++i) {
            p[i] = (p[i - 1] * BASE) % MOD;
            h[i] = (h[i - 1] * BASE + s[i]) % MOD;
        }
    }

    // Returns hash of substring s[l...r] inclusive
    long long query(int l, int r) const {
        if (l > r) return 0;
        long long res = h[r];
        if (l > 0) {
            res = (res - (h[l - 1] * p[r - l + 1]) % MOD);
            if (res < 0) res += MOD;
        }
        return res;
    }
};

struct DoubleHash {
    PolyHash<31, 1000000007> h1;
    PolyHash<37, 1000000009> h2;

    DoubleHash(const string& s) : h1(s), h2(s) {}

    pair<long long, long long> query(int l, int r) {
        return {h1.query(l, r), h2.query(l, r)};
    }
};







void solve(){
    int n, k, q; cin >> n >> k >> q;
    set<pair<ll,ll>> athletes;
    string s;
    for(int i = 0; i < n; ++i) {
        cin >> s;
        DoubleHash dh(s);
        athletes.insert(dh.query(0, k - 1));
    }
    int matches = 0;
    string genes = "ACGT";
    ll mod1 = 1000000007;
    ll mod2 = 1000000009;
    for(int i = 0; i < q; ++i) {
        cin >> s;
        DoubleHash dh(s);
        pair<ll,ll> cur  = dh.query(0, k-1);
        if(athletes.count(cur)) {
            ++matches;
            continue;
        }
        bool found = false;
        for(int j = 0; j < k && !found; ++j) {
            char ori = s[j];
            for(char c : genes) {
                if(c == ori) continue;
                ll p1 = dh.h1.p[k-1-j];
                ll p2 = dh.h2.p[k-1-j];
                ll n1 = (cur.first-(1LL * ori *p1) %mod1 + mod1)%mod1;
                n1 = (n1 + (1LL* c * p1) % mod1) % mod1;
                ll n2 = (cur.second - (1LL * ori *p2) % mod2 +mod2) % mod2;
                n2 = (n2 + (1LL *c * p2) % mod2) % mod2;
                if(athletes.count({n1, n2})) {
                    found = true;
                    break;
                }
            }
            if (found) matches++;
        }
    }
    cout << matches << '\n';
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
