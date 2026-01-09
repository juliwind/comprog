#ifndef LCP_H
#define LCP_H

#include "template.h"

/**
 * Suffix Array and LCP Array Construction
 * * USAGE:
 * // 1. Define solver with Max Length
 * LCP<200005> sa;
 * * void solve() {
 * // 2. Build for a string
 * sa.build(s);
 * * // 3. Queries
 * cout << sa.count_distinct_substrings() << endl;
 * cout << sa.query(idx1, idx2) << endl; // LCP of suffixes starting at idx1 and idx2
 * }
 * * COMPLEXITY: Build O(N log N), Query O(1)
 */
template<int MAX_N>
struct LCP {
    int sa[MAX_N];
    int rk[MAX_N];
    int lcp[MAX_N];
    int st[MAX_N][20];  
    int lg[MAX_N];
    
    int n;
    string S;

    LCP() { 
        lg[1] = 0;
        for (int i = 2; i < MAX_N; i++) lg[i] = lg[i/2] + 1;
    }

    void reset() {
        fill(sa, sa + MAX_N, 0);
        fill(rk, rk + MAX_N, 0);
        fill(lcp, lcp + MAX_N, 0);
    }

    void build(const string& str) {
        S = str;
        n = S.size();
        construct_sa();
        construct_lcp();
        build_sparse_table();
    }

    void construct_sa() {
        for (int i = 0; i < n; i++) sa[i] = i, rk[i] = S[i];

        for (int k = 1; k < n; k <<= 1) {
            auto cmp = [&](int i, int j) {
                if (rk[i] != rk[j]) return rk[i] < rk[j];
                int ri = (i + k < n) ? rk[i + k] : -1;
                int rj = (j + k < n) ? rk[j + k] : -1;
                return ri < rj;
            };
            sort(sa, sa + n, cmp);

            vector<int> tmp_rk(n);
            tmp_rk[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                tmp_rk[sa[i]] = tmp_rk[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
            }
            for (int i = 0; i < n; i++) rk[i] = tmp_rk[i];
            if (rk[sa[n-1]] == n - 1) break;
        }
    }

    void construct_lcp() {
        for (int i = 0, k = 0; i < n; i++) {
            if (rk[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[rk[i] + 1];
            while (i + k < n && j + k < n && S[i + k] == S[j + k]) k++;
            lcp[rk[i] + 1] = k;
            if (k > 0) k--;
        }
    }

    void build_sparse_table() {
        for (int i = 0; i < n; i++) st[i][0] = lcp[i];
        for (int j = 1; j < 20; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[i][j] = min(st[i][j-1], st[i + (1 << (j-1))][j-1]);
            }
        }
    }

    int get_min(int L, int R) const {
        int j = lg[R - L + 1];
        return min(st[L][j], st[R - (1 << j) + 1][j]);
    }

    // Query LCP of two arbitrary suffixes
    // Returns length of common prefix between S[i...] and S[j...]
    int query(int i, int j) const {
        if (i == j) return n - i;
        int ri = rk[i], rj = rk[j];
        if (ri > rj) swap(ri, rj);
        return get_min(ri + 1, rj);
    }

    // Count Distinct Substrings
    long long count_distinct_substrings() const {
        long long total = 0;
        for (int i = 0; i < n; i++) {
            total += (n - sa[i]) - lcp[i]; 
        }
        return total;
    }

    // Longest Repeated Substring
    // Returns {length, starting_index}
    pair<int, int> longest_repeated_substring() const {
        int max_len = 0;
        int idx = 0;
        for(int i = 1; i < n; ++i) {
            if (lcp[i] > max_len) {
                max_len = lcp[i];
                idx = sa[i];
            }
        }
        return {max_len, idx};
    }

    // String Matching
    // Count occurrences of pattern P in S in O(|P| * log N)
    int count_occurrences(const string& P) {
        int m = P.size();
                int L = 0, R = n, lb = n;
        while(L < R) {
            int mid = (L + R) / 2;
            if (S.substr(sa[mid], m) >= P) {
                lb = mid; R = mid;
            } else L = mid + 1;
        }
        if (lb == n || S.substr(sa[lb], m) != P) return 0;
        L = lb; R = n; int ub = n;
        while(L < R) {
            int mid = (L + R) / 2;
            if (S.substr(sa[mid], m) > P) {
                ub = mid; R = mid;
            } else L = mid + 1;
        }

        return ub - lb;
    }
};

#endif