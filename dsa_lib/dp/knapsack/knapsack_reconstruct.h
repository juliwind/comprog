#ifndef KNAPSACK_RECONSTRUCT_H
#define KNAPSACK_RECONSTRUCT_H

#include "template.h"

/**
 * Reconstructable Knapsack Struct
 * * USAGE:
 * KnapsackReconstruct<MAX_W> ks(weights, counts);
 * if (ks.possible(target)) {
 * // Returns map: { original_item_index -> quantity_used }
 * map<int, int> solution = ks.reconstruct(target);
 * }
 * * MEMORY: Uses sizeof(int) * MAX_W bytes (32x more than bitset).
 * * COMPLEXITY: O(Target * N * log(Count))
 */
template<int MAX_W>
struct KnapsackReconstruct {
    // Stores the index of the decomposed item used to reach sum 's'.
    // -1 = Unreachable, -2 = Start
    int from_item[MAX_W];
    
    // Metadata for decomposed items (Binary parts: 1, 2, 4...)
    struct DecomposedItem {
        int weight;      // The total weight of this batch (unit_weight * count)
        int original_idx;// Index in the user's input vector
        int count;       // How many copies this batch represents
    };
    vector<DecomposedItem> items_meta;

    // Constructors
    KnapsackReconstruct() { reset(); }

    KnapsackReconstruct(const vector<int>& weights, const vector<int>& counts) {
        solve(weights, counts);
    }

    void reset() {
        fill(from_item, from_item + MAX_W, -1);
        from_item[0] = -2; 
        items_meta.clear();
    }

    void solve(const vector<int>& weights, const vector<int>& counts) {
        reset();
        int n = weights.size();
        int current_id = 0; // ID for each decomposed part

        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            int c = counts[i];

            if (w == 0 || w >= MAX_W) continue;

            // Binary Decomposition (1, 2, 4...)
            for (int k = 1; c > 0; k <<= 1) {
                int take = min(k, c);
                int batch_weight = w * take;
                
                items_meta.push_back({batch_weight, i, take});

                // Standard 0/1 DP (Reverse order to prevent reusing same batch)
                for (int s = MAX_W - 1; s >= batch_weight; --s) {
                    // If current sum 's' is not yet reached, but 's - weight' is:
                    if (from_item[s] == -1 && from_item[s - batch_weight] != -1) {
                        from_item[s] = current_id;
                    }
                }
                
                c -= take;
                current_id++;
            }
        }
    }

    // Check reachability
    bool possible(int k) const {
        if (k < 0 || k >= MAX_W) return false;
        return from_item[k] != -1;
    }

    map<int, int> reconstruct(int k) {
        map<int, int> usage;
        if (!possible(k)) return usage;

        while (k > 0) {
            int id = from_item[k];

            if (id < 0) break; 

            const auto& item = items_meta[id];
            usage[item.original_idx] += item.count;
            
            k -= item.weight;
        }
        return usage;
    }
};

#endif

/*
Usage:

void solve() {
    int n, target;
    cin >> n >> target;
    
    vi w(n), c(n);
    for(int i=0; i<n; ++i) cin >> w[i] >> c[i];

    // Must be large enough for target
    KnapsackReconstruct<200005> ks(w, c);

    if (ks.possible(target)) {
        cout << "Possible\n";
        
        map<int, int> result = ks.reconstruct(target);
        
        cout << "Items used:\n";
        for(auto p : result) {
            // p.first is index in original vectors 'w' and 'c'
            // p.second is how many were taken
            cout << "Index " << p.first << ": Taken " << p.second << "\n";
        }
    } else {
        cout << "Impossible\n";
    }
}
*/