struct segtree {

    int size, no_op = 0;
    vector<int> values;

    segtree(int n) {
        size = 1;
        while (size < n) size *= 2;
        values.assign(2 * size, no_op); 
    }

    void build(vector<int> &a, int x, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < (int)a.size()) values[x] = a[lx];
            return;
        }
        int m = (lx + rx) / 2;
        build(a, 2 * x + 1, lx, m);
        build(a, 2 * x + 2, m, rx);
        values[x] = merge(values[2 * x + 1], values[2 * x + 2]);
    }

    void build(vector<int> &a) {
        build(a, 0, 0, size);
    }

    int merge(int a, int b) {
        return a + b;
    }

    void set(int i, int b, int x, int lx, int rx) {
        if (rx - lx == 1) {
            values[x] = b;
            return;
        }
        int m = (lx + rx) / 2;
        if (i < m) set(i, b, 2 * x + 1, lx, m);
        else set(i, b, 2 * x + 2, m, rx);
        values[x] = merge(values[2 * x + 1], values[2 * x + 2]);
    }

    void set(int i, int x) {
        set(i, x, 0, 0, size);
    }

    int calc(int l, int r, int x, int lx, int rx) {
        if (lx >= r || l >= rx) return 0;
        if (l <= lx && r >= rx) return values[x];
        int m = (lx + rx) / 2;
        int value_left = calc(l, r, 2 * x + 1, lx, m);
        int value_right = calc(l, r, 2 * x + 2, m, rx);
        return merge(value_left, value_right);
    }

    int calc(int l, int r) {
        return calc(l, r, 0, 0, size);
    }
};
