#ifndef POLLARD_RHO_H
#define POLLARD_RHO_H

#include "template.h"

using namespace std;

/**
 * Pollard's Rho & Miller-Rabin Factorization
 * * USAGE:
 * PollardRho pr;
 * vector<uint64_t> factors = pr.solve(n);
 * * COMPLEXITY: O(n^(1/4)) expected
 * * REQUIRES: C++17 (for std::gcd), GCC/Clang (for __uint128_t)
 */
struct PollardRho {
    using u64 = uint64_t;
    using u128 = __uint128_t;

    mt19937_64 rng;

    PollardRho() {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }

    // Main API: Returns sorted vector of prime factors
    vector<u64> solve(u64 n) {
        vector<u64> fac;
        recurse(n, fac);
        sort(fac.begin(), fac.end());
        return fac;
    }

private:
    // Modular Multiplication (a * b) % m using 128-bit integer
    u64 mul_mod(u64 a, u64 b, u64 m) {
        return (u128)a * b % m;
    }

    // Modular Exponentiation (a ^ e) % m
    u64 pow_mod(u64 a, u64 e, u64 m) {
        u64 r = 1;
        while (e) {
            if (e & 1) r = mul_mod(r, a, m);
            a = mul_mod(a, a, m);
            e >>= 1;
        }
        return r;
    }

    // Miller-Rabin Primality Test (Deterministic for u64)
    bool is_prime(u64 n) {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        // Deterministic bases for 64-bit integers
        static const u64 bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        
        u64 d = n - 1;
        int s = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            s++;
        }

        for (u64 a : bases) {
            if (n <= a) break;
            u64 x = pow_mod(a, d, n);
            if (x == 1 || x == n - 1) continue;
            bool composite = true;
            for (int i = 1; i < s; ++i) {
                x = mul_mod(x, x, n);
                if (x == n - 1) {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }

    // Pollard's Rho Algorithm to find a non-trivial factor
    u64 get_factor(u64 n) {
        if (n % 2 == 0) return 2;
        
        // Loop until a factor is found
        while (true) {
            u64 x = uniform_int_distribution<u64>(2, n - 1)(rng);
            u64 y = x;
            u64 c = uniform_int_distribution<u64>(1, n - 1)(rng);
            u64 d = 1;
            
            auto f = [&](u64 val) { return (mul_mod(val, val, n) + c) % n; };

            while (d == 1) {
                x = f(x);
                y = f(f(y));
                d = std::__gcd((x > y ? x - y : y - x), n);
            }
            if (d != n) return d;
        }
    }

    // Recursive Decomposition
    void recurse(u64 n, vector<u64>& fac) {
        if (n == 1) return;
        if (is_prime(n)) {
            fac.push_back(n);
            return;
        }
        u64 d = get_factor(n);
        recurse(d, fac);
        recurse(n / d, fac);
    }
};

#endif

/*
Usage:

void solve() {
    long long n;
    cin >> n;

    PollardRho pr;
    vector<unsigned long long> factors = pr.solve(n);

    cout << "Prime Factors: ";
    for (auto p : factors) {
        cout << p << " ";
    }
    cout << "\n";
}
*/