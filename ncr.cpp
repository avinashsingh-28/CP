struct NCR {
    int maxN, MOD;
    vector<int> fact, invfact;

    NCR(int n, int m) {
        maxN = n;
        MOD = m;
        fact.resize(maxN + 1);
        invfact.resize(maxN + 1);
        build();
    }

    int binexp(int a, int b) {
        int res = 1;
        a %= MOD;
        while (b) {
            if (b & 1) res = (res * a) % MOD;
            a = (a * a) % MOD;
            b >>= 1;
        }
        return res;
    }

    void build() {
        fact[0] = 1;
        for (int i = 1; i <= maxN; i++)
            fact[i] = (fact[i - 1] * i) % MOD;
        invfact[maxN] = binexp(fact[maxN], MOD - 2);
        for (int i = maxN - 1; i >= 0; i--)
            invfact[i] = (invfact[i + 1] * (i + 1)) % MOD;
    }

    int ncr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * invfact[r] % MOD * invfact[n - r] % MOD;
    }
};
