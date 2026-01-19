struct Tree {
    int n, LOG;
    vvi up, adj;
    vi depth;
 
    Tree(int n) {
        this->n = n;
        LOG = 0;
        while ((1 << LOG) <= n) LOG++;
 
        adj.resize(n + 1);
        up.assign(n + 1, vi(LOG));
        depth.assign(n + 1, 0);         
    }
 
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    void dfs(int v, int p) {
        up[v][0] = p;
        for (int i = 1; i < LOG; i++)
            up[v][i] = up[ up[v][i-1] ][i-1];
 
        for (int u : adj[v]) {
            if (u == p) continue;
            depth[u] = depth[v] + 1;
            dfs(u, v);
        }
    }
 
    void build(int root = 1) {
        dfs(root, root);
    }
 
    int kth_ancestor(int v, int k) {
        for (int i = 0; i < LOG; i++)
            if (k & (1 << i))
                v = up[v][i];
        return v;
    }
 
    int lca(int a, int b) {
        if (depth[a] < depth[b])
            swap(a, b);
 
        a = kth_ancestor(a, depth[a] - depth[b]);
        if (a == b) return a;
 
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[a][i] != up[b][i]) {
                a = up[a][i];
                b = up[b][i];
            }
        }
        return up[a][0];
    }
 
    int dist(int u, int v) {
        int c = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[c];
    }
};
 
