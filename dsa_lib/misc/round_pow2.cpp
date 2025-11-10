#include <bits/stdc++.h>

constexpr int U32_BITS  = std::numeric_limits<unsigned>::digits;
constexpr int U64_BITS  = std::numeric_limits<unsigned long long>::digits;


// ---------- int ----------

int pow2_floor_int(int x) {
    if (x <= 0) return 0;
    unsigned u = (unsigned)x;
    unsigned msb = U32_BITS - 1 - __builtin_clz(u);
    return (int)(1u << msb);
}

int pow2_ceil_int(int x) {
    if (x <= 0) return 0;
    if (x == 1) return 1;
    unsigned u = (unsigned)(x - 1);
    unsigned shift = U32_BITS - __builtin_clz(u);
    unsigned cand = (shift >= U32_BITS) ? 0u : (1u << shift);
    if (cand == 0u || cand > (unsigned)std::numeric_limits<int>::max())
        return std::numeric_limits<int>::max();
    return (int)cand;
}

int pow2_round_int(int x) {
    if (x <= 0) return 0;
    int f = pow2_floor_int(x);
    int c = pow2_ceil_int(x);
    long long dl = (long long)x - f;
    long long du = (c == std::numeric_limits<int>::max()) ? (long long)LLONG_MAX : (long long)c - x;
    return (dl <= du) ? f : c;
}


// ---------- long long ----------

long long pow2_floor_ll(long long x) {
    if (x <= 0) return 0;
    unsigned long long u = (unsigned long long)x;
    unsigned long long msb = U64_BITS - 1 - __builtin_clzll(u);
    return (long long)(1ull << msb);
}

long long pow2_ceil_ll(long long x) {
    if (x <= 0) return 0;
    if (x == 1) return 1;
    unsigned long long u = (unsigned long long)(x - 1);
    unsigned shift = U64_BITS - __builtin_clzll(u);
    unsigned long long cand = (shift >= U64_BITS) ? 0ull : (1ull << shift);
    if (cand == 0ull || cand > (unsigned long long)std::numeric_limits<long long>::max())
        return std::numeric_limits<long long>::max();    // clamp
    return (long long)cand;
}

long long pow2_round_ll(long long x) {
    if (x <= 0) return 0;
    long long f = pow2_floor_ll(x);
    long long c = pow2_ceil_ll(x);
    long long dl = x - f;
    long long du = (c == std::numeric_limits<long long>::max()) ? LLONG_MAX : c - x;
    return (dl <= du) ? f : c;
}

