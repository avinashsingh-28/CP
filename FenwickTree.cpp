class BIT {
private:
    int n;
    vector<int> bit;
public:
    BIT(int n) {
        this->n = n;
        bit.assign(n + 1, 0);
    }
    void add(int idx, int val) {
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }
    int sum(int idx) {
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }
};

class RangeBIT {
private:
    int n;
    BIT bit1, bit2;
public:
    RangeBIT(int n) : n(n), bit1(n), bit2(n) {}
    void add(int l, int r, int val) {
        bit1.add(l, val);
        bit1.add(r + 1, -val);
        bit2.add(l, val * (l - 1));
        bit2.add(r + 1, -val * r);
    }
    int sum(int idx) {
        return bit1.sum(idx) * idx - bit2.sum(idx);
    }
};
