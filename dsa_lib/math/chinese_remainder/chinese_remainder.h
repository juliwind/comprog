#ifndef CHINESE_REMAINDER_H
#define CHINESE_REMAINDER_H

#include "template.h"

/**
 * Chinese Remainder Theorem (Generalized)
 * * USAGE:
 * // Solve system: x = a[i] (mod m[i])
 * auto res = CRT::solve(remainders, moduli);
 * if (res.second == -1) cout << "No Solution";
 * else cout << res.first << " (mod " << res.second << ")";
 * * FEATURES:
 * - Handles non-coprime moduli (Generalized CRT).
 * - Detects no solution cases.
 * - Overflow safe (uses __int128 internally).
 * * COMPLEXITY: O(N * log(LCM))
 */
struct CRT {
    using ll = long long;
    using u128 = __int128_t;

    // Extended Euclidean Algorithm
    // Finds x, y such that ax + by = gcd(a, b)
    static ll ext_gcd(ll a, ll b, ll &x, ll &y) {
        if (b == 0) {
            x = 1; y = 0;
            return a;
        }
        ll x1, y1;
        ll g = ext_gcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return g;
    }

    // Normalizes x into range [0, m-1]
    static ll norm(ll x, ll m) {
        x %= m;
        if (x < 0) x += m;
        return x;
    }

    /**
     * Solves the system of congruences:
     * x = a[0] (mod m[0])
     * x = a[1] (mod m[1])
     * ...
     * @return pair {x, LCM} where x is the smallest non-negative solution.
     * Returns {-1, -1} if no solution exists.
     */
    static pair<ll, ll> solve(const vector<ll>& a, const vector<ll>& m) {
        if (a.empty() || m.empty() || a.size() != m.size()) return {0, 1};

        ll x = 0;
        ll lcm = 1;

        for (size_t i = 0; i < a.size(); ++i) {
            ll a_i = norm(a[i], m[i]);
            ll m_i = m[i];

            // We want to merge:
            // X_new = x (mod lcm)
            // X_new = a_i (mod m_i)
            //
            // This means X_new = x + k * lcm
            // x + k * lcm = a_i (mod m_i)
            // k * lcm = a_i - x (mod m_i)
            
            ll p, q;
            ll g = ext_gcd(lcm, m_i, p, q);
            
            // Check existence: (a_i - x) must be divisible by gcd(lcm, m_i)
            if ((a_i - x) % g != 0) return {-1, -1};

            // Solve for k in: k * (lcm/g) = (a_i - x)/g (mod m_i/g)
            // Let target = (a_i - x) / g
            // We have p * lcm + q * m_i = g  =>  p * (lcm/g) + q * (m_i/g) = 1
            // So inverse of (lcm/g) modulo (m_i/g) is p.
            
            ll m_div_g = m_i / g;
            ll target = (a_i - x) / g;
            
            // k = target * p (mod m_div_g)
            // Use u128 to prevent overflow during multiply
            ll k = (ll)((u128)norm(target, m_div_g) * norm(p, m_div_g) % m_div_g);

            // Update x and lcm
            // x_new = x + k * lcm
            x = x + (ll)((u128)k * lcm); // k < m_i/g, so k*lcm < m_i/g * lcm = LCM(lcm, m_i)
            lcm = lcm * m_div_g;         // New LCM is LCM(lcm, m_i)
            
            x = norm(x, lcm);
        }

        return {x, lcm};
    }
    
    // Convenience wrapper for single merge
    // Returns {new_remainder, new_modulus} or {-1, -1}
    static pair<ll, ll> merge(ll a1, ll m1, ll a2, ll m2) {
        return solve({a1, a2}, {m1, m2});
    }
};

#endif

/*
Usage Example:

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n), m(n);
    for(int i = 0; i < n; ++i) cin >> a[i];
    for(int i = 0; i < n; ++i) cin >> m[i];

    // Example:
    // x = 2 (mod 3)
    // x = 3 (mod 5)
    // x = 2 (mod 7)
    
    auto result = CRT::solve(a, m);
    
    if (result.second == -1) {
        cout << "No Solution\n";
    } else {
        cout << result.first << " " << result.second << "\n";
        // Output: 23 105 (meaning x = 23 mod 105)
    }
}
*/