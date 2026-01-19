/*
======== Quick reference: what to set in the generic segtree template ========

For each semantic you must set:
  - no_operation        : value meaning "no pending operation" (stored in operations[])
  - neutral_element     : identity value for query (returned for no-overlap)
  - combine_ops(old,new,len)       : how to compose pending operations
  - apply_op_to_value(value,op,len): how applying op changes node.value (node aggregates whole segment)
  - calc_op(a,b)                   : how to merge two child node values

Also:
  - When pushing a node's pending op to children, pass child segment lengths:
      left_len  = m - lx
      right_len = rx - m
    (len is needed for sum-like operations).

-------------------------------------------------------------------------------
1) RANGE ADD + RANGE SUM (no modulo)
   - no_operation      = 0        // add 0 = no-op
   - neutral_element   = 0        // sum neutral

   // compose pending adds
   int combine_ops(int old_op, int new_op, int len) {
       return old_op + new_op;
   }
   // node.value stores SUM over the segment -> op affects sum by (op * len)
   int apply_op_to_value(int value, int pending, int len) {
       return value + pending * len;
   }
   // merge children sums
   int calc_op(int a, int b) {
       return a + b;
   }

   Notes:
   - init values[] from initial array (default zeros).
   - use 64-bit if sums may overflow.

-------------------------------------------------------------------------------
2) RANGE ADD + RANGE MIN
   - no_operation      = 0
   - neutral_element   = +INF    // choose INF large enough (e.g. 1e18 for long long)

   int combine_ops(int old_op, int new_op, int len) {
       return old_op + new_op;
   }
   // node.value stores MIN over the segment -> adding shifts min by pending
   int apply_op_to_value(int value, int pending, int len) {
       return value + pending;
   }
   int calc_op(int a, int b) {
       return min(a, b);
   }

   Notes:
   - values[] initialized from input (or INF for padding).
   - len is NOT used when applying to node.value (min is per-element shift).

-------------------------------------------------------------------------------
3) RANGE ASSIGN (set all to v) + RANGE MIN
   - no_operation      = NO_ASSIGN_SENTINEL
                         // pick a sentinel that can't be a valid assignment,
                         // e.g. NO_ASSIGN = INT_MIN or use an extra boolean flag array.
   - neutral_element   = +INF

   int combine_ops(int old_op, int new_op, int len) {
       // new assignment overrides previous
       if (new_op == NO_ASSIGN) return old_op;
       return new_op;
   }
   int apply_op_to_value(int value, int pending, int len) {
       if (pending == NO_ASSIGN) return value;
       return pending;  // whole segment becomes 'pending'
   }
   int calc_op(int a, int b) {
       return min(a, b);
   }

   Notes:
   - For assign you must ensure NO_ASSIGN cannot be confused with valid values,
     or use separate has_op[] boolean to mark pending assignments.

-------------------------------------------------------------------------------
4) RANGE MULTIPLY + RANGE SUM  (mod M)
   - no_operation      = 1        // multiplicative identity
   - neutral_element   = 0        // sum neutral
   - M                = 1e9+7 (or given modulus)

   int combine_ops(int old_op, int new_op, int len) {
       return (1LL * old_op * new_op) % M;
   }
   int apply_op_to_value(int value, int pending, int len) {
       return (1LL * value * pending) % M;
   }
   int calc_op(int a, int b) {
       int s = a + b;
       if (s >= M) s -= M;
       return s;
   }

   Notes:
   - Initial array for some problems (like the example) may be all ones; pass that array to build().
   - If you store count or sums differently, remember to use len where required.

-------------------------------------------------------------------------------
5) RANGE BITWISE OR updates + RANGE BITWISE AND queries
   - no_operation      = 0            // OR with 0 = no-op
   - neutral_element   = ALL_ONES     // e.g. (1LL<<31)-1 or (1LL<<30)-1 depending on bit-range
                                      // choose mask with 1s for all relevant bits (v < 2^30)

   int combine_ops(int old_op, int new_op, int len) {
       return old_op | new_op;
   }
   // node.value stores bitwise AND over the segment
   int apply_op_to_value(int value, int pending, int len) {
       return value | pending;   // (old_and | v)
   }
   int calc_op(int a, int b) {
       return a & b;
   }

   Notes:
   - Initialize leaves to 0 (problem states initial array zeros); padding leaves should be neutral_element (all-ones),
     so they don't change AND results.
   - The identity used: (x|v) & (y|v) = (x & y) | v, hence node AND becomes (old_and | v).

-------------------------------------------------------------------------------
General gotchas:
 - For sum-like node values, apply_op_to_value must use segment length (len).
 - For min/max/bitwise per-element aggregates, typically you do NOT multiply by len when applying op.
 - Always check and use the correct 'len' when pushing to children: (m - lx) and (rx - m).
 - After pushing, clear parent operation: operations[x] = no_operation.
 - get(i) should return values[x] at leaf (not operations[x]).
 - For padding indices (lx >= n) set values[x] = neutral_element during build.
*/


struct segtree {

    int size;
    vector<int> operations;
    vector<int> values;

    int no_operation = inf;
    int neutral_element = 0;

    void build(const vector<int> &a, int x, int lx, int rx, int n) {
        if (rx - lx == 1) {
            if (lx < n && !a.empty()) values[x] = a[lx];
            else values[x] = neutral_element;
            return;
        }
        int m = (lx + rx) / 2;
        build(a, 2 * x + 1, lx, m, n);
        build(a, 2 * x + 2, m, rx, n);
        values[x] = calc_op(values[2 * x + 1], values[2 * x + 2]);
    }

    void init(int n, const vector<int> &a = {}) {
        size = 1;
        while (size < n) size *= 2;
        operations.assign(2 * size, no_operation); 
        values.assign(2 * size, 0);
        build(a, 0, 0, size, n);
    }

    int combine_ops(int old_op, int new_op, int len) {
        return 0;
    }
    int apply_op_to_value(int value, int pending, int len) {
        return 0;
    }
    int calc_op(int a, int b) {
        return 0;
    }

    void apply_to_operations(int &target_op, int pending, int len) {
        target_op = combine_ops(target_op, pending, len);
    }
    void apply_to_values(int &target_val, int pending, int len) {
        target_val = apply_op_to_value(target_val, pending, len);
    }

    void propagate(int x, int lx, int rx) {
        if (operations[x] == no_operation) return;
        if (rx - lx == 1) return;
        int m = (lx + rx) / 2;
        apply_to_operations(operations[2*x+1], operations[x], m - lx);
        apply_to_operations(operations[2*x+2], operations[x], rx - m);
        apply_to_values(values[2*x+1], operations[x], m - lx);
        apply_to_values(values[2*x+2], operations[x], rx - m);
        operations[x] = no_operation;
    }

    void modify(int l, int r, int b, int x, int lx, int rx) {
        propagate(x, lx, rx);
        if (lx >= r || l >= rx) return;
        if (lx >= l && rx <= r) {
            apply_to_operations(operations[x], b, rx - lx);
            apply_to_values(values[x], b, rx - lx);
            return;
        } 
        int m = (lx + rx) / 2;
        modify(l, r, b, 2 * x + 1, lx, m);
        modify(l, r, b, 2 * x + 2, m, rx);
        values[x] = calc_op(values[2 * x + 1], values[2 * x + 2]);
    }
    void modify(int l, int r, int b) {
        return modify(l, r, b, 0, 0, size);
    }

    int calc(int l, int r, int x, int lx, int rx) {
        propagate(x, lx, rx);
        if (lx >= r || l >= rx) return neutral_element;
        if (lx >= l && rx <= r) return values[x];
        int m = (lx + rx) / 2;
        int m1 = calc(l, r, 2 * x + 1, lx, m);
        int m2 = calc(l, r, 2 * x + 2, m, rx);
        return calc_op(m1, m2);
    }
    int calc(int l, int r) {
        return calc(l, r, 0, 0, size);
    }
};
