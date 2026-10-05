static inline int header_log2(unsigned n) { return 31 - __builtin_clz(n); }
