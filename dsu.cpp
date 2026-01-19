struct DSU {
    vi parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) make_set(i);
    }

    void make_set(int a) {
        parent[a] = a;
        rank[a] = 0;
    }

    int find_set(int a) {
        if (a == parent[a]) return a;
        return parent[a] = find_set(parent[a]);
    }

    void union_sets(int a, int b) {
        int par_a = find_set(a);
        int par_b = find_set(b);
        if (par_a != par_b) {
            if (rank[par_a] < rank[par_b]) swap(par_a, par_b);
            parent[par_b] = par_a;
            if (rank[par_a] == rank[par_b]) rank[par_a]++;
        }
    }
};
