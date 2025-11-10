struct DSU {
    unordered_map<ll,ll> nxt;
    ll find(ll x){
        ll y = x;
        while(true){
            auto it = nxt.find(y);
            if(it == nxt.end()) break;
            y = it->second;
        }
        ll z = x;
        while(true){
            auto it = nxt.find(z);
            if(it == nxt.end()) break;
            ll nz = it->second;
            if(nz == y) break;
            it->second = y;
            z = nz;
        }
        return y;
    }
    void occupy(ll x){
        nxt[x] = find(x+1);
    }
};