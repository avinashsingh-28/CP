class SegTree {
private:
    int n;
    vector<int> tree;

    // change this according to the problem
    int identity = 0;
    int merge(int a, int b) {
        return a + b;
    }

    void build(int node, int l, int r, vector<int> &a) {
        if (l == r) {
            tree[node] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * node, l, m, a);
        build(2 * node + 1, m + 1, r, a);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

    int query(int node, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return identity;
        if (ql <= l && qr >= r) return tree[node];
        int m = (l + r) / 2;
        int left = query(2 * node, l, m, ql, qr);
        int right = query(2 * node + 1, m + 1, r, ql, qr);
        return merge(left, right);
    }

    void update(int node, int l, int r, int idx, int val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        int m = (l + r) / 2;
        if (idx <= m) update(2 * node, l, m, idx, val);
        else update(2 * node + 1, m + 1, r, idx, val);
        tree[node] = merge(tree[2 * node], tree[2 * node + 1]);
    }

public:
    SegTree(vector<int> &a) {
        n = a.size();
        tree.assign(4 * n, identity);
        build(1, 0, n - 1, a);
    }
    int query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }
    void update(int idx, int val) {
        update(1, 0, n - 1, idx, val);
    }
};
