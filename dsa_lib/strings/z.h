#ifndef Z_FUNCTION_H
#define Z_FUNCTION_H

#include "template.h"

/**
 * Z-Function Algorithm
 * * USAGE:
 * ZAlgo za(s);
 * // Access Z-values
 * cout << za.query(i) << endl;
 * * * COMPLEXITY: O(N)
 */
struct ZAlgo {
    vector<int> z;

    ZAlgo() {}

    ZAlgo(const string& s) {
        build(s);
    }

    void build(const string& s) {
        int n = s.length();
        z.assign(n, 0);
        int l = 0, r = 0;
        for (int i = 1; i < n; ++i) {
            if (i <= r) 
                z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                ++z[i];
            if (i + z[i] - 1 > r) {
                l = i;
                r = i + z[i] - 1;
            }
        }
        z[0] = n; 
    }

    int query(int i) const {
        if (i < 0 || i >= (int)z.size()) return 0;
        return z[i];
    }
};

#endif