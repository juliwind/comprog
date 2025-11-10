    #include <bits/stdc++.h>
    #include <climits>
    #include <queue>
    using namespace std;
    using vi = vector<int>;
    using vc = vector<char>;
    #define fast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    typedef long long ll;typedef long double ld;typedef pair<int,int> pii;
    #define F first
    #define S second
    #define PB push_back
    #define MP make_pair
    #define all(a) (a).begin(), (a).end()
    const ll mod = 1e9+7, N = 2e6+7, M = 2e6+7, INF = INT_MAX/10;
    ll powe(ll x, ll y){ x = x%mod, y=y%(mod-1);ll ans = 1;while(y>0){if (y&1){ans = (1ll * x * ans)%mod;}y>>=1;x = (1ll * x * x)%mod;}return ans;}
     
     
     
     
    void solve(){
    	 int n, k; cin >> n >> k;
        vi p(n);
        vector<vi> ch(n);
        p[0] = -1;
        for(int i = 1; i < n; ++i) {
            int v; cin >> v;
            --v;
            p[i] = v;
            ch[v].PB(i);
        }
        vi layer(n);
        int minl = INT_MAX;
        priority_queue<int> pq;
        pq.push(0);
        while(!pq.empty()) {
            int v = pq.top(); pq.pop();
            if(v == 0) layer[v] = 0;
            else layer[v] = layer[p[v]] + 1;
            if(ch[v].empty()) minl = min(minl, layer[v]);
            for(int u : ch[v]) pq.push(u);
        }
        vi lcount(n);
        for(int i = 0; i < n; ++i) ++lcount[layer[i]];
        vi sets;
        ll sum = 0;
        for(int i = 0; i <= minl; ++i) {
            sets.PB(lcount[i]);
            sum += lcount[i];
        }
        ll ones = n - sum;
        for(int i = 0; i < ones; ++i) sets.PB(1);
        static bitset<200001> dp;
        unordered_map<int,int> cnt;
        for(int w : sets) if(w <= k) ++cnt[w];
        dp.reset(); dp[0] = 1;
        for(auto x : cnt){
            int w = x.first, c = x.second;
            for(int b = 1; c > 0; b <<= 1){
                int take = min(b, c);
                ll sh = 1LL * w * take;
                if(sh <= k) dp |= (dp << sh);
                c -= take;
            }
        }
        if(dp[k]) cout << (minl + 1) << "\n";
        else cout << (minl) << "\n";
     
     
    }
     
     
     
     
    signed main(){
        fast;
        int t = 1;
        cin >> t;
        while(t--){
        	solve();
        }
        return 0;
    }`