struct NumberTheory {
    int N;
    vector<int> spf;          
    vector<int> primes;
    vector<bool> isPrime;

    NumberTheory(int n) {
        N = n;
        spf.resize(N + 1);
        isPrime.assign(N + 1, true);
        build_sieve();
    }

    void build_sieve() {
        isPrime[0] = isPrime[1] = false;
        for (int i = 2; i <= N; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || (long long)i * p > N) break;
                spf[i * p] = p;
                isPrime[i * p] = false;
            }
        }
    }

    bool is_prime(int x) {
        return x >= 2 && spf[x] == x;
    }

    vector<pair<int,int>> factorize(int x) {
        vector<pair<int,int>> res;
        while (x > 1) {
            int p = spf[x], cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            res.push_back({p, cnt});
        }
        return res;
    }

    vector<int> get_divisors(int x) {
        vector<int> divisors = {1};
        auto f = factorize(x);

        for (auto [p, cnt] : f) {
            int sz = divisors.size();
            long long cur = 1;
            for (int i = 1; i <= cnt; i++) {
                cur *= p;
                for (int j = 0; j < sz; j++)
                    divisors.push_back(divisors[j] * cur);
            }
        }
        return divisors;
    }
};
