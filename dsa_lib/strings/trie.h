#ifndef TRIE_H
#define TRIE_H

#include "template.h"
#include <cstring>
#include <string>

/**
 * Static Array-based Trie (Prefix Tree)
 * * USAGE: 
 * // 1. Define Trie with Max Nodes (Total characters in all strings)
 * // Optional: Alphabet size (26) and Base char ('a')
 * Trie<100005, 26, 'a'> trie; 
 * * void solve() {
 * // 2. Build or Insert
 * trie.insert("apple");
 * trie.insert("app");
 * * // 3. Query
 * cout << trie.count_exact("app") << endl; // 1
 * cout << trie.count_prefix("ap") << endl; // 2
 * cout << trie.kth_element(1) << endl;     // "app" (lexicographical)
 * }
 * * COMPLEXITY: 
 * Insert/Search: O(L) where L is string length
 * Space: O(MAX_NODES * ALPHABET_SIZE)
 */
template<int MAX_NODES, int K = 26, int BASE_CHAR = 'a'>
struct Trie {
    int next[MAX_NODES][K];
    int cnt_leaf[MAX_NODES];
    int cnt_sub[MAX_NODES];
    int nodes_count;

    Trie() { reset(); }

    Trie(const vector<string>& words) {
        reset();
        for (const string& s : words) insert(s);
    }

    void reset() {
        nodes_count = 1;
        memset(next[0], -1, sizeof(next[0]));
        cnt_leaf[0] = 0;
        cnt_sub[0] = 0;
    }

    int new_node() {
        int u = nodes_count++;
        memset(next[u], -1, sizeof(next[u]));
        cnt_leaf[u] = 0;
        cnt_sub[u] = 0;
        return u;
    }

    // Insert a string
    void insert(const string& s) {
        int u = 0;
        cnt_sub[u]++;
        for (char c : s) {
            int v = c - BASE_CHAR;
            if (next[u][v] == -1) next[u][v] = new_node();
            u = next[u][v];
            cnt_sub[u]++;
        }
        cnt_leaf[u]++;
    }

    // Erase a string (Lazy delete: decrements counts)
    // Returns true if deleted, false if string didn't exist
    bool erase(const string& s) {
        if (count_exact(s) == 0) return false;
        int u = 0;
        cnt_sub[u]--;
        for (char c : s) {
            int v = c - BASE_CHAR;
            u = next[u][v];
            cnt_sub[u]--;
        }
        cnt_leaf[u]--;
        return true;
    }

    // Count how many times string s was inserted
    int count_exact(const string& s) const {
        int u = 0;
        for (char c : s) {
            int v = c - BASE_CHAR;
            if (next[u][v] == -1) return 0;
            u = next[u][v];
        }
        return cnt_leaf[u];
    }

    // Count how many words have s as a prefix
    int count_prefix(const string& s) const {
        int u = 0;
        for (char c : s) {
            int v = c - BASE_CHAR;
            if (next[u][v] == -1) return 0;
            u = next[u][v];
        }
        return cnt_sub[u];
    }

    // Returns the length of the Longest Common Prefix of s in the trie
    int lcp_length(const string& s) const {
        int u = 0, len = 0;
        for (char c : s) {
            int v = c - BASE_CHAR;
            if (next[u][v] == -1 || cnt_sub[next[u][v]] == 0) break;
            u = next[u][v];
            len++;
        }
        return len;
    }

    // Find the k-th lexicographically smallest string (1-based index)
    // Returns "" if k > total words
    string kth_element(int k) const {
        if (k > cnt_sub[0]) return "";
        string res = "";
        int u = 0;
        while (true) {
            if (cnt_leaf[u] >= k) return res;
            k -= cnt_leaf[u];
            for (int i = 0; i < K; ++i) {
                int v = next[u][i];
                if (v != -1 && cnt_sub[v] > 0) {
                    if (cnt_sub[v] >= k) {
                        res += (char)(BASE_CHAR + i);
                        u = v;
                        goto next_level;
                    } else {
                        k -= cnt_sub[v];
                    }
                }
            }
            break;
            next_level:;
        }
        return res;
    }
};

#endif

/*
Usage:

void solve(){
    int n, q;
    cin >> n >> q;

    // Template args: <MaxNodes> (approx Sum of lengths)
    Trie<200005> trie; 
    
    for(int i=0; i<n; ++i) {
        string s; cin >> s;
        trie.insert(s);
    }

    while(q--) {
        int type; string s;
        cin >> type;
        if (type == 1) { // Insert
            cin >> s; trie.insert(s);
        } else if (type == 2) { // Query Prefix Count
            cin >> s; cout << trie.count_prefix(s) << "\n";
        } else if (type == 3) { // K-th smallest string
            int k; cin >> k;
            cout << trie.kth_element(k) << "\n";
        }
    }
}
*/