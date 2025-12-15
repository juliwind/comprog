#ifndef EXTENDED_EUCLIDEAN_H
#define EXTENDED_EUCLIDEAN_H

#include "template.h"

/**
 * Extended Euclidean Algorithm Tools
 * * USAGE:
 * // 1. Basic: ax + by = gcd(a, b)
 * long long x, y;
 * long long g = ExGCD::solve(a, b, x, y);
 * * // 2. Modular Inverse: ax = 1 (mod m)
 * long long inv = ExGCD::mod_inverse(a, m);
 * * // 3. Diophantine: ax + by = c
 * auto res = ExGCD::solve_diophantine(a, b, c);
 * if (res.has_solution) cout << res.x << " " << res.y;
 * * * COMPLEXITY: O(log(min(a, b)))
 */
struct ExGCD {
    using ll = long long;
    using u128 = __int128_t;

    // --- 1. Basic Extended GCD ---
    // Finds {g, x, y} such that a*x + b*y = g = gcd(a, b)
    // Note: coefficients x and y may be negative.
    static ll solve(ll a, ll b, ll &x, ll &y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        ll x1, y1;
        ll g = solve(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return g;
    }

    // --- 2. Safe Modular Inverse ---
    // Returns x in [0, m-1] such that a*x = 1 (mod m)
    // Returns -1 if inverse does not exist (gcd(a, m) != 1)
    static ll mod_inverse(ll a, ll m) {
        ll x, y;
        ll g = solve(a, m, x, y);
        if (g != 1) return -1;
        return (x % m + m) % m;
    }

    // --- 3. Linear Diophantine Equation Solver ---
    // Solves a*x + b*y = c
    struct DiophantineResult {
        bool has_solution;
        ll x, y; // One specific solution
        ll g;    // gcd(a, b)
        
        // General solution form:
        // x_k = x + k * (b/g)
        // y_k = y - k * (a/g)
    };

    static DiophantineResult solve_diophantine(ll a, ll b, ll c) {
        ll x, y;
        ll g = solve(abs(a), abs(b), x, y);
        
        // No solution if c is not divisible by gcd(|a|, |b|)
        if (c % g != 0) {
            return {false, 0, 0, g};
        }

        // Scale the solution from "a*x + b*y = g" to "... = c"
        // Use __int128 to prevent overflow during scaling
        x = (ll)((u128)x * (c / g));
        y = (ll)((u128)y * (c / g));

        // Adjust signs if inputs were negative
        if (a < 0) x = -x;
        if (b < 0) y = -y;

        return {true, x, y, g};
    }

    // --- 4. Minimum Positive X Solution ---
    // Finds solution to ax + by = c where x is the smallest non-negative integer.
    // Useful for problems like "minimum coins to pay amount C"
    static pair<ll, ll> min_non_negative_x(ll a, ll b, ll c) {
        auto res = solve_diophantine(a, b, c);
        if (!res.has_solution) return {-1, -1};

        ll x = res.x;
        ll y = res.y;
        ll g = res.g;
        
        // Step size for general solutions
        // x_new = x + k * (b/g)
        ll step_x = b / g;
        ll step_y = a / g; // Note: y formula is y - k * (a/g)

        // Shift x to be minimal non-negative
        // We want smallest x >= 0
        // If x is negative, we add step_x until positive
        // If x is large positive, we subtract step_x
        
        // Note: Sign of step_x depends on sign of b. 
        // We ensure logic works regardless of signs.
        if (step_x == 0) {
            // Special case b=0: x is fixed
            if (x < 0) return {-1, -1};
            return {x, y};
        }

        // Normalize x using modulo arithmetic
        ll k = x / step_x;
        x -= k * step_x;
        y += k * step_y;
        
        if (x < 0) {
            if (step_x > 0) { x += step_x; y -= step_y; }
            else            { x -= step_x; y += step_y; }
        } else {
             // If we can reduce x further while staying >= 0 (if step_x < 0 logic inverted)
             if (step_x > 0 && x >= step_x) { x -= step_x; y += step_y; }
             if (step_x < 0 && x >= -step_x) { x += step_x; y -= step_y; }
        }
        
        return {x, y};
    }
};

#endif

/*
Usage Example:

void solve() {
    long long a, b, c;
    cin >> a >> b >> c;

    // 1. Solve ax + by = c
    auto res = ExGCD::solve_diophantine(a, b, c);
    
    if (!res.has_solution) {
        cout << "Impossible\n";
    } else {
        cout << "Specific solution: x=" << res.x << ", y=" << res.y << "\n";
        
        // 2. Find min non-negative x
        auto min_res = ExGCD::min_non_negative_x(a, b, c);
        if (min_res.first != -1) {
            cout << "Min x >= 0: " << min_res.first << "\n";
        }
    }

    // 3. Modular Inverse
    long long m = 1e9 + 7;
    long long inv = ExGCD::mod_inverse(a, m);
    if (inv != -1) cout << "Inverse of " << a << " mod " << m << " is " << inv << "\n";
}
*/