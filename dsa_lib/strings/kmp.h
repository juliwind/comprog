#ifndef PREFIX_FUNCTION_H
#define PREFIX_FUNCTION_H

#include "template.h"

/**
 * Knuth-Morris-Pratt (KMP) Algorithm / Prefix Function
 * * USAGE:
 * KMP kmp(pattern);
 * // Get pi array
 * vector<int> pi = kmp.pi;
 * // Find pattern in text
 * vector<int> matches = kmp.find_in(text);
 * * * COMPLEXITY: O(N) build, O(M) search
 */
struct KMP {
    vector<int> pi;
    string P;

    KMP() {}

    KMP(const string& s) {
        build(s);
    }

    void build(const string& s) {
        P = s;
        int n = s.length();
        pi.assign(n, 0);
        for (int i = 1; i < n; i++) {
            int j = pi[i - 1];
            while (j > 0 && s[i] != s[j])
                j = pi[j - 1];
            if (s[i] == s[j])
                j++;
            pi[i] = j;
        }
    }

    // Returns 0-based start indices of all occurrences of P in text
    vector<int> find_in(const string& text) {
        vector<int> matches;
        int n = text.length();
        int m = P.length();
        if (m == 0) return matches;
        int j = 0;
        for (int i = 0; i < n; i++) {
            while (j > 0 && text[i] != P[j])
                j = pi[j - 1];
            if (text[i] == P[j])
                j++;
            if (j == m) {
                matches.push_back(i - m + 1);
                j = pi[j - 1];
            }
        }
        return matches;
    }
};

#endif