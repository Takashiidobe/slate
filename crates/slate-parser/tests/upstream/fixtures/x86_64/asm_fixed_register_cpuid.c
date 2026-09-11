typedef unsigned int U32;

static int cpuid_leaf1_nonzero(void) {
  U32 f1a, f1c, f1d;
  __asm__("cpuid\n\t" : "=a"(f1a), "=c"(f1c), "=d"(f1d) : "a"(1) : "ebx");
  return (int)((f1c | f1d) != 0);
}

int main(void) { return cpuid_leaf1_nonzero(); }


