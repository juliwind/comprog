#ifndef ROUND_POW2_H
#define ROUND_POW2_H

#include "template.h"

/**
 * Power of 2 Utilities
 * * USAGE:
 * int f = Pow2::floor(10);       // 8
 * ll  c = Pow2::ceil(10LL);      // 16
 * int r = Pow2::round(12);       // 16
 * * FEATURES:
 * - Auto-detects int vs long long
 * - Handles overflows (clamps to type MAX)
 * - Safe for non-positive inputs (returns 0)
 */
struct Pow2 {
    
    // --- Helper: Count Leading Zeros (Generic) ---
    static int clz(unsigned int x) { return __builtin_clz(x); }
    static int clz(unsigned long long x) { return __builtin_clzll(x); }

    // --- 1. Floor (Largest Power of 2 <= x) ---
    template <typename T>
    static T floor(T x) {
        static_assert(std::is_integral<T>::value, "Must be integral type");
        if (x <= 0) return 0;
        
        using U = typename std::make_unsigned<T>::type;
        U u = (U)x;
        int bits = std::numeric_limits<U>::digits;
        int shift = bits - 1 - clz(u);
        
        return (T)(U(1) << shift);
    }

    // --- 2. Ceil (Smallest Power of 2 >= x) ---
    template <typename T>
    static T ceil(T x) {
        static_assert(std::is_integral<T>::value, "Must be integral type");
        if (x <= 0) return 0;
        if (x == 1) return 1;
        
        using U = typename std::make_unsigned<T>::type;
        U u = (U)(x - 1);
        int bits = std::numeric_limits<U>::digits;
        int shift = bits - clz(u);
        
        // Check for overflow (next power of 2 > type_max)
        if (shift >= bits) return std::numeric_limits<T>::max();
        
        U cand = (U(1) << shift);
        // Additional safety check if T is signed and cand turned into sign bit (e.g. INT_MIN)
        if (cand > (U)std::numeric_limits<T>::max()) return std::numeric_limits<T>::max();
        
        return (T)cand;
    }

    // --- 3. Round (Nearest Power of 2) ---
    template <typename T>
    static T round(T x) {
        static_assert(std::is_integral<T>::value, "Must be integral type");
        if (x <= 0) return 0;
        
        T f = floor(x);
        T c = ceil(x);
        
        // Calculate distances safely
        // Use unsigned or wider type to prevent overflow during subtraction
        using CalcType = typename std::conditional<sizeof(T) <= 4, long long, unsigned long long>::type;
        
        CalcType dl = (CalcType)x - (CalcType)f;
        
        CalcType du;
        if (c == std::numeric_limits<T>::max()) {
            // If ceiling saturated, treat distance as infinite (or very large)
            du = std::numeric_limits<CalcType>::max();
        } else {
            du = (CalcType)c - (CalcType)x;
        }

        return (dl <= du) ? f : c;
    }
};

#endif

/*
Usage:

void solve() {
    int val_int = 13;
    long long val_ll = 4000000000LL;

    // 1. Floor
    cout << Pow2::floor(val_int) << "\n";    // 8
    cout << Pow2::floor(val_ll) << "\n";     // 2147483648 (2^31)

    // 2. Ceil
    cout << Pow2::ceil(val_int) << "\n";     // 16
    
    // 3. Round
    // 13 is closer to 16 (dist 3) than 8 (dist 5)
    cout << Pow2::round(13) << "\n";         // 16
    // 12 is equidistant to 8 (dist 4) and 16 (dist 4), logic prefers floor?
    // Let's check logic: dl(4) <= du(4) -> returns floor.
    cout << Pow2::round(12) << "\n";         // 8
}
*/