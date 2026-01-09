#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include "template.h"

/**
 * Aho-Corasick Automaton
 * * USAGE:
 * // 1. Define solver
 * // <Max Total Characters, Alphabet Size, Base Char>
 * AhoCorasick<200005, 26, 'a'> ac;
 * * void solve() {
 * // 2. Insert patterns
 * ac.insert("he");
 * ac.insert("she");
 * ac.insert("his");
 * ac.insert("hers");
 * * // 3. Build failure links & DFA
 * ac.build();
 * * // 4. Query
 * cout << ac.query_total_occurrences("ushers") << endl; // Returns count of all matches
 * * // 5. Access internal state for DP
 * // ac.tr[u][c] gives the next state from u with char c
 * }
 * * * COMPLEXITY: 
 * - Build: O(Sum of pattern lengths * Alphabet)
 * - Query: O(Text length)
 */
template<int MAX_N, int ALPHA = 26, char MIN_CHAR = 'a'>
struct AhoCorasick {
    int tr[MAX_N][ALPHA];
    int fail[MAX_N];
    int end_count[MAX_N];
    int accum_count[MAX_N];
    vector<int> end_indices[MAX_N];
    int nodes_count;

    AhoCorasick() { reset(); }

    AhoCorasick(const vector<string>& patterns) {
        reset();
        for (int i = 0; i < patterns.size(); ++i) {
            insert(patterns[i], i);
        }
        build();
    }

    void reset() {
        for (int i = 0; i <= nodes_count; ++i) {
            fill(tr[i], tr[i] + ALPHA, 0);
            fail[i] = 0;
            end_count[i] = 0;
            accum_count[i] = 0;
            end_indices[i].clear();
        }
        nodes_count = 1;
        fill(tr[0], tr[0] + ALPHA, 0);
        nodes_count = 1; 
    }

    // Insert a pattern into the Trie
    // id is optional, used if you need to track which specific pattern matched
    void insert(const string& s, int id = -1) {
        int u = 0;
        for (char c : s) {
            int idx = c - MIN_CHAR;
            if (!tr[u][idx]) {
                tr[u][idx] = nodes_count++;
            }
            u = tr[u][idx];
        }
        end_count[u]++;
        if(id != -1) end_indices[u].push_back(id);
    }

    void build() {
        queue<int> q;
        for (int i = 0; i < ALPHA; ++i) {
            if (tr[0][i]) {
                q.push(tr[0][i]);
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            accum_count[u] = end_count[u] + accum_count[fail[u]];
            for (int i = 0; i < ALPHA; ++i) {
                if (tr[u][i]) {
                    fail[tr[u][i]] = tr[fail[u]][i];
                    q.push(tr[u][i]);
                } else {
                    tr[u][i] = tr[fail[u]][i];
                }
            }
        }
    }

    // Returns the total number of pattern occurrences in the text
    long long query_total_occurrences(const string& text) {
        int u = 0;
        long long ans = 0;
        for (char c : text) {
            u = tr[u][c - MIN_CHAR];
            ans += accum_count[u];
        }
        return ans;
    }

    // Returns a map { pattern_id : count } for all patterns found
    // Requires insert() to have been called with IDs
    map<int, int> query_pattern_counts(const string& text) {
        int u = 0;
        map<int, int> counts;
        for (char c : text) {
            u = tr[u][c - MIN_CHAR];
            int temp = u;
            while (temp > 0) {
                if (end_count[temp] > 0) {
                    for (int id : end_indices[temp]) {
                        counts[id]++;
                    }
                }
                temp = fail[temp];
            }
        }
        return counts;
    }
    int next_state(int u, char c) const {
        return tr[u][c - MIN_CHAR];
    }
};

#endif

/*
Usage:

void solve() {
    int n;
    cin >> n;
    
    // Note: MAX_N should be sum of lengths of all patterns + buffer
    // Be mindful of stack size; define globally if MAX_N is large.
    AhoCorasick<200005> ac; 

    vector<string> patterns(n);
    for(int i=0; i<n; ++i) {
        cin >> patterns[i];
        ac.insert(patterns[i], i); // Store index for identification
    }
    
    ac.build();

    string text;
    cin >> text;

    // 1. Total Occurrences
    cout << "Total matches: " << ac.query_total_occurrences(text) << "\n";

    // 2. Specific Pattern Counts
    map<int, int> counts = ac.query_pattern_counts(text);
    for(auto const& [id, count] : counts) {
        cout << "Pattern " << patterns[id] << ": " << count << "\n";
    }
}
*/