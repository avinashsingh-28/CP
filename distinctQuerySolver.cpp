#include <bits/stdc++.h>
using namespace std;

struct FenwickTree {
    int n;
    vector<int> bit;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n + 1, 0);
    }

    void update(int index, int value) {
        for (; index <= n; index += index & -index) {
            bit[index] += value;
        }
    }

    int query(int index) const {
        int sum = 0;
        for (; index > 0; index -= index & -index) {
            sum += bit[index];
        }
        return sum;
    }

    int rangeQuery(int l, int r) const {
        return query(r) - query(l - 1);
    }
};

struct Query {
    int l, r, id;

    bool operator<(const Query& other) const {
        return r < other.r;
    }
};

struct DistinctQuerySolver {
    int n, q;
    vector<int> arr;
    vector<Query> queries;

    DistinctQuerySolver(int n, int q, vector<int> arr, vector<pair<int,int>> rawQueries)
        : n(n), q(q), arr(arr) {

        for (int i = 0; i < q; i++) {
            queries.push_back({rawQueries[i].first + 1,
                               rawQueries[i].second + 1,
                               i});
        }
    }

    vector<int> solve() {
        unordered_map<int, int> compress;
        int id = 0;
        for (int &x : arr) {
            if (!compress.count(x)) {
                compress[x] = id++;
            }
            x = compress[x];
        }

        vector<int> lastIndex(id, -1);
        FenwickTree fenwick(n);

        sort(queries.begin(), queries.end());

        vector<int> answer(q);
        int qi = 0;

        for (int i = 0; i < n; i++) {
            int value = arr[i];

            if (lastIndex[value] != -1) {
                fenwick.update(lastIndex[value] + 1, -1);
            }

            lastIndex[value] = i;
            fenwick.update(i + 1, 1);

            while (qi < q && queries[qi].r == i + 1) {
                answer[queries[qi].id] =
                    fenwick.rangeQuery(queries[qi].l, queries[qi].r);
                qi++;
            }
        }
        return answer;
    }
};
