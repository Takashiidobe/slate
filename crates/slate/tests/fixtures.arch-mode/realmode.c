void cpuid_count(unsigned id, unsigned count, unsigned *a, unsigned *b, unsigned *c,
                 unsigned *d) {
  __asm__ volatile("cpuid"
                   : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                   : "0"(id), "2"(count));
}

long width_of_long(void) { return sizeof(long) * 8 + sizeof(void *); }
