#ifndef SUFFIX_AUTOMATON_H
#define SUFFIX_AUTOMATON_H

#include "template.h"

/**
 * Suffix Automaton (SAM) Structure
 * * USAGE:
 * // 1. Define SAM (Size must be > length of string)
 * SuffixAutomaton<100005> sam;
 * * * void solve() {
 * // 2. Build
 * sam.build(s);
 * * // 3. Queries
 * cout << sam.total_distinct_substrings() << endl;
 * cout << sam.count_occurrences("pattern") << endl;
 * cout << sam.longest_common_substring(other_string) << endl;
 * }
 * * * COMPLEXITY:
 * Build: O(N) Time, O(N * Sigma) Memory
 * distinct_substrings: O(N)
 * count_occurrences: O(P) where P is pattern length
 * longest_common_substring: O(M) where M is other string length
 */
template<int MAX_N, int ALPHABET = 26, char BASE_CHAR = 'a'>
struct SuffixAutomaton {
    struct State {
        int len, link;
        int next[ALPHABET];
        long long cnt = 0;
        long long paths = 0;
        bool is_clone = false;
        int first_pos = -1;

        void clear() {
            len = 0; link = -1;
            cnt = 0; paths = 0;
            is_clone = false; first_pos = -1;
            memset(next, -1, sizeof(next));
        }
    };

    State st[MAX_N * 2];
    int sz, last;
    bool built_counts;

    SuffixAutomaton() { reset(); }

    SuffixAutomaton(const string& s) {
        reset();
        build(s);
    }

    void reset() {
        for(int i = 0; i <= sz; ++i) st[i].clear();
        sz = 1;
        last = 0;
        st[0].link = -1;
        built_counts = false;
    }

    void extend(char c, int pos_idx) {
        int cur = sz++;
        st[cur].clear();
        st[cur].len = st[last].len + 1;
        st[cur].first_pos = pos_idx;
        st[cur].cnt = 1;
        
        int p = last;
        int code = c - BASE_CHAR;
        
        while (p != -1 && st[p].next[code] == -1) {
            st[p].next[code] = cur;
            p = st[p].link;
        }

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[code];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone] = st[q];
                st[clone].len = st[p].len + 1;
                st[clone].cnt = 0;
                st[clone].is_clone = true; 
                
                while (p != -1 && st[p].next[code] == q) {
                    st[p].next[code] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void build(const string& s) {
        reset();
        for (int i = 0; i < s.size(); ++i) {
            extend(s[i], i);
        }
    }

    // Count Occurrences of states
    // Propagates counts up the suffix link tree.
    // Must be called before querying specific substring counts if not simple matching.
    void propogate_counts() {
        if (built_counts) return;
        vector<int> nodes(sz);
        iota(nodes.begin(), nodes.end(), 0);
        sort(nodes.begin(), nodes.end(), [&](int a, int b) {
            return st[a].len > st[b].len;
        });

        for (int u : nodes) {
            if (st[u].link != -1) {
                st[st[u].link].cnt += st[u].cnt;
            }
        }
        built_counts = true;
    }

    // Returns number of times pattern P appears in S
    long long count_occurrences(const string& p) {
        propogate_counts();
        int curr = 0;
        for (char c : p) {
            int code = c - BASE_CHAR;
            if (st[curr].next[code] == -1) return 0;
            curr = st[curr].next[code];
        }
        return st[curr].cnt;
    }

    // Total Distinct Substrings
    // Sum of (len(u) - len(link(u))) for all states
    long long total_distinct_substrings() {
        long long ans = 0;
        for (int i = 1; i < sz; ++i) {
            ans += st[i].len - st[st[i].link].len;
        }
        return ans;
    }

    // Longest Common Substring
    // Returns length and ending position in S
    pair<int, int> longest_common_substring_info(const string& t) {
        int v = 0, l = 0, best_len = 0, best_pos = 0;
        for (int i = 0; i < t.size(); ++i) {
            int code = t[i] - BASE_CHAR;
            while (v != 0 && st[v].next[code] == -1) {
                v = st[v].link;
                l = st[v].len;
            }
            if (st[v].next[code] != -1) {
                v = st[v].next[code];
                l++;
            }
            if (l > best_len) {
                best_len = l;
                best_pos = st[v].first_pos; 
            }
        }
        return {best_len, best_pos};
    }

    int longest_common_substring(const string& t) {
        return longest_common_substring_info(t).first;
    }

    // Smallest Cyclic Shift
    // finds the lexicographically smallest string among all possible cyclic rotations
    // S must be the string used to build SAM. This effectively computes for S + S.
    // Note: To use this, build(S + S) and pass length of original S.
    string smallest_cyclic_shift(int n) {
        int curr = 0;
        string res = "";
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < ALPHABET; ++k) {
                if (st[curr].next[k] != -1) {
                    curr = st[curr].next[k];
                    res += (char)(BASE_CHAR + k);
                    break;
                }
            }
        }
        return res;
    }

    // K-th Lexicographical Substring
    // finds the k-th smallest distinct substring
    // Precompute paths
    void calc_paths(int u = 0) {
        if (st[u].paths > 0) return;
        st[u].paths = 1;       
        for (int i = 0; i < ALPHABET; ++i) {
            if (st[u].next[i] != -1) {
                calc_paths(st[u].next[i]);
                st[u].paths += st[st[u].next[i]].paths;
            }
        }
    }

    // 1-based index k. Returns empty string if k is too large.
    string get_kth_substring(long long k) {
        vector<long long> dp(sz, 0);
        auto get_cnt = [&](auto&& self, int u) -> long long {
            if (dp[u]) return dp[u];
            dp[u] = 1;
            for(int i=0; i<ALPHABET; ++i) {
                if (st[u].next[i] != -1) dp[u] += self(self, st[u].next[i]);
            }
            return dp[u];
        };
        get_cnt(get_cnt, 0);

        if (k >= dp[0]) return ""; 

        int curr = 0;
        string res = "";
        while (k > 0) {
            for (int i = 0; i < ALPHABET; ++i) {
                if (st[curr].next[i] != -1) {
                    int next_state = st[curr].next[i];
                    long long cnt = dp[next_state];
                    if (k <= cnt) {
                        k--;
                        res += (char)(BASE_CHAR + i);
                        curr = next_state;
                        break;
                    } else {
                        k -= cnt;
                    }
                }
            }
        }
        return res;
    }
};

#endif

/*
Usage Example:

void solve() {
    string s, t;
    cin >> s;
    
    // 1. Initialize
    SuffixAutomaton<100005> sam(s); // MAX_N should be >= s.length()

    // 2. Count distinct substrings
    cout << "Distinct Substrings: " << sam.total_distinct_substrings() << endl;

    // 3. Count occurrences of a specific pattern
    // (Internally propagates counts on first call)
    cin >> t;
    cout << "Occurrences of " << t << ": " << sam.count_occurrences(t) << endl;

    // 4. LCS with another string
    string other; cin >> other;
    cout << "LCS: " << sam.longest_common_substring(other) << endl;
    
    // 5. K-th lexicographical substring
    long long k; cin >> k;
    cout << "K-th substring: " << sam.get_kth_substring(k) << endl;
}
*/