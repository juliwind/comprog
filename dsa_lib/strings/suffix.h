#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include "template.h"

/**
 * Suffix Array + LCP Array Construction
 * * USAGE: 
 * // 1. Define solver (Template arg must be > string length)
 * SuffixArray<100005> sa; 
 * * void solve() {
 * // 2. Build SA and LCP
 * sa.build(s);
 * // 3. Access
 * // sa.p[i] -> Index of the i-th lexicographical suffix
 * // sa.lcp[i] -> LCP between suffix p[i-1] and p[i]
 * }
 * * COMPLEXITY: Build O(N log N), LCP O(N)
 */
template<int MAX_N>
struct SuffixArray {
    int p[MAX_N];
    int c[MAX_N];
    int lcp[MAX_N];
    int pn[MAX_N], cn[MAX_N], cnt[MAX_N];
    SuffixArray() {}
    void reset(int n) {
        fill(cnt, cnt + max(n + 1, 256), 0);
    }

    void build(string s) {
        s += "$";
        int n = s.size();
        reset(n);
        const int alpha = 256;
        fill(cnt, cnt + alpha, 0);
        for (int i = 0; i < n; i++) cnt[s[i]]++;
        for (int i = 1; i < alpha; i++) cnt[i] += cnt[i - 1];
        for (int i = 0; i < n; i++) p[--cnt[s[i]]] = i;
        c[p[0]] = 0;
        int classes = 1;
        for (int i = 1; i < n; i++) {
            if (s[p[i]] != s[p[i - 1]]) classes++;
            c[p[i]] = classes - 1;
        }
        for (int h = 0; (1 << h) < n; ++h) {
            for (int i = 0; i < n; i++) {
                pn[i] = p[i] - (1 << h);
                if (pn[i] < 0) pn[i] += n;
            }
            fill(cnt, cnt + classes, 0);
            for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
            for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
            cn[p[0]] = 0;
            classes = 1;
            for (int i = 1; i < n; i++) {
                pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
                if (cur != prev) ++classes;
                cn[p[i]] = classes - 1;
            }
            for (int i = 0; i < n; i++) c[i] = cn[i];
        }
        build_lcp(s, n);
    }

    void build_lcp(const string& s, int n) {
        vector<int> rank(n);
        for (int i = 0; i < n; i++) rank[p[i]] = i;
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = rank[i];
            if (pi == 0) {
                k = 0; 
                continue;
            }
            int j = p[pi - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            if (k > 0) k--;
        }
    }
};

#endif

/*
Usage:

void solve(){
    string s; 
    cin >> s;

    // 1. Declare (MAX_N > s.length())
    SuffixArray<100005> sa;
    
    // 2. Build
    sa.build(s);

    // 3. Use
    // Note: Because we added '$', the valid suffixes are usually 1 to n.
    // sa.p[0] is the sentinel suffix.
    int n = s.length(); // Original length
    
    cout << "Suffix Array:\n";
    for(int i = 1; i <= n; ++i) { // Start from 1 to skip sentinel
        cout << sa.p[i] << " ";
    }
    cout << "\nLCP Array:\n";
    for(int i = 1; i <= n; ++i) { // lcp[i] is between p[i] and p[i-1]
        cout << sa.lcp[i] << " ";
    }
    cout << "\n";
}
*/