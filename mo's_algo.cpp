int block_size;

struct Query {
    int l, r, idx;
    Query(int _l = 0, int _r = 0, int _idx = 0) : l(_l), r(_r), idx(_idx) {}
    bool operator<(const Query& other) const {
        int b1 = l / block_size;
        int b2 = other.l / block_size;
        if (b1 != b2) return b1 < b2;
        return r < other.r;
    }
};

void add_index(int idx);     // implement: include value at position idx
void remove_index(int idx);  // implement: exclude value at position idx
int get_answer();            // implement: return current answer for active range

vector<int> mos_algorithm(vector<Query> queries, int n) {
    block_size = max(1, (int) sqrt(n));
    vector<int> answers(queries.size());
    sort(queries.begin(), queries.end());

    int cur_l = 0, cur_r = -1;
    for (const Query& q : queries) {
        while (cur_l > q.l) add_index(--cur_l);
        while (cur_r < q.r) add_index(++cur_r);
        while (cur_l < q.l) remove_index(cur_l++);
        while (cur_r > q.r) remove_index(cur_r--);
        answers[q.idx] = get_answer();
    }
    return answers;
}
