struct Node {
    Node *link[26];
    int ct = 0;
};

struct Trie {
    Node *root;

    Trie() { 
        root = new Node();
    }

    void insert(string &s) {
        Node *node = root;
        for (char c : s) {
            if (node->link[c - 'a'] == NULL) {
                node->link[c - 'a'] = new Node();
            }
            node = node->link[c - 'a'];
            node->ct++;
        }
    }
    
    void remove(string &s) {
        Node *node = root;
        for (char c : s) {
            node = node->link[c - 'a'];
            node -> ct--;
        }
    }

    int get(string &s) {
        int res = 0;
        Node *node = root;
        for (char c : s) {
            if (node->link[c - 'a'] == NULL) return res;
            node = node->link[c - 'a'];
            res += node->ct * 2;
        }
        return res;
    }
};
