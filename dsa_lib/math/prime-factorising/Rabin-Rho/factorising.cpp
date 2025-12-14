#include <bits/stdc++.h>
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
const ll N = 2e6+7, M = 2e6+7, INF = INT_MAX/10;
//ll powe(ll x, ll y){ x = x%mod, y=y%(mod-1);ll ans = 1;while(y>0){if (y&1){ans = (1ll * x * ans)%mod;}y>>=1;x = (1ll * x * x)%mod;}return ans;}



/*
factor number up to 10^18 with Miller–Rabin primality test, Pollard’s Rho algorithm
O(n^(1/4))
*/


mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

using u64 = uint64_t;
using u128 = __uint128_t;

u64 mul_mod(u64 a, u64 b, u64 m){ return (u128)a*b % m; }
u64 pow_mod(u64 a, u64 e, u64 m){
    u64 r = 1;
    while(e){
        if(e&1) r = mul_mod(r,a,m);
        a = mul_mod(a,a,m);
        e >>= 1;
    }
    return r;
}

bool isPrime64(u64 n){
    if(n < 2) return false;
    for(u64 p: {2u,3u,5u,7u,11u,13u,17u,19u,23u,29u,31u,37u}){
        if(n%p==0) return n==p;
    }
    u64 d = n-1, s = 0;
    while((d&1)==0){ d >>= 1; ++s; }
    // Deterministic for 64-bit
    for(u64 a: {2u,3u,5u,7u,11u,13u,17u}){
        if(a % n == 0) continue;
        u64 x = pow_mod(a, d, n);
        if(x==1 || x==n-1) continue;
        bool comp = true;
        for(u64 i=1;i<s;i++){
            x = mul_mod(x,x,n);
            if(x==n-1){ comp=false; break; }
        }
        if(comp) return false;
    }
    return true;
}

u64 f_rho(u64 x, u64 c, u64 mod){
    return (mul_mod(x,x,mod) + c) % mod;
}

u64 pollard(u64 n){
    if((n&1)==0) return 2;
    if(n%3==0) return 3;
    uniform_int_distribution<u64> dist_x(2, n-2), dist_c(1, n-1);
    while(true){
        u64 x = dist_x(rng), y = x, c = dist_c(rng);
        if(c>=n) c %= n;
        if(c==0) c = 1;
        u64 d = 1;
        while(d==1){
            x = f_rho(x,c,n);
            y = f_rho(f_rho(y,c,n),c,n);
            u64 diff = x>y? x-y : y-x;
            d = __gcd(diff, n);
        }


        if(d!=n) return d;
    }
}

void factor_rec(u64 n, vector<u64>& fac){
    if(n==1) return;
    if(isPrime64(n)){ fac.push_back(n); return; }
    u64 d = pollard(n);
    factor_rec(d, fac);
    factor_rec(n/d, fac);
}
 

void solve(){
	unsigned long long n;
    cin >> n;
    vector<u64> fac;
    factor_rec(n, fac);
    sort(fac.begin(), fac.end());
    for(size_t i=0;i<fac.size();++i){
        if(i) cout << ' ';
        cout << fac[i];
    }
    cout << '\n';
}




signed main(){
    fast;
    int t = 1;
    cin >> t;
    while(t--){
    	solve();
    }
    return 0;
}
