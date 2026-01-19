struct Node {
    Node *link[2];
    int ct = 0;
};
 
struct Trie {
    Node *root;
    Trie() : root(new Node()) {}
 
    void insert(int num) {
        Node *node = root;
        for (int i = 41; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!node -> link[bit]) {
                node -> link[bit] = new Node();
            } 
            node = node -> link[bit];
            node -> ct++;
        }
    }
 
    void remove(int num) {
        Node *node = root;
        for (int i = 41; i >= 0; i--) {
            int bit = (num >> i) & 1;
            node = node -> link[bit];
            node -> ct--;
        } 
    }
 
    int getMaxXOR(int num) {
        Node *node = root;
        int maxXor = 0;
        for (int i = 41; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (node->link[1 - bit] && node->link[1 - bit]->ct > 0) {
                maxXor |= (1ll << i);
                node = node->link[1 - bit];
            } else {
                node = node->link[bit];
            }
        }
        return maxXor;
    }

    int getXOR_lessThanL(int num, int l) {
        Node *node = root;
        int res = 0;
        for (int i = 31; i >= 0; i--) {
            int bit1 = (num >> i) & 1;
            int bit2 = (l >> i) & 1;
 
            if (bit2) {
                if (node -> link[bit1]) res += node -> link[bit1] -> ct;
                if (node -> link[1 ^ bit1]) node = node -> link[1 ^ bit1];
                else return res;
            } else {
                if (node -> link[bit1]) node = node -> link[bit1];
                else return res;
            }
        }
        return res;
    }
};
