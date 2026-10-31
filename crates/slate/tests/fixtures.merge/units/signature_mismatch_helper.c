struct pair {
    int first;
#ifdef BETA
    long second;
#endif
};

#ifdef ALPHA
long widen(long x) {
    return x + 1;
}
#else
int widen(int x) {
    return x + 1;
}
#endif

struct pair make_pair(int x) {
    struct pair pair = {0};
    pair.first = x;
    return pair;
}

#if GAMMA == 3
long table_size = 3;
#else
int table_size = 4;
#endif
