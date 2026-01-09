#ifndef ROLLING_HASH_H
#define ROLLING_HASH_H

#include "template.h"

/**
 * Polynomial Rolling Hash
 * * USAGE:
 * // 1. Define solver (Base 31, Mod 1e9+7 is a common default)
 * PolyHash<31, 1000000007> h(string);
 * // 2. Query Substring Hash (0-based, inclusive)
 * long long val = h.query(l, r);
 * * * COMPLEXITY: O(N) build, O(1) query
 */
template<long long BASE = 31, long long MOD = 1000000007>
struct PolyHash {
    vector<long long> h, p;

    PolyHash() {}

    // Constructor: Precomputes hashes and powers
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

/* * DOUBLE HASH HELPER
* Usage: DoubleHash dh(string); dh.query(l, r);
*/
struct DoubleHash {
    PolyHash<31, 1000000007> h1;
    PolyHash<37, 1000000009> h2;

    DoubleHash(const string& s) : h1(s), h2(s) {}

    pair<long long, long long> query(int l, int r) {
        return {h1.query(l, r), h2.query(l, r)};
    }
};

#endif