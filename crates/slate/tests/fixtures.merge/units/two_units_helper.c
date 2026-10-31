int shared_common;

int helper(int x) {
#ifdef ALPHA
    x += 1;
#endif
    return x * shared_common;
}
