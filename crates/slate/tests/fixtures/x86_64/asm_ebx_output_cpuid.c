typedef unsigned int U32;

int cpuid_leaf7_ebx_nonzero(void) {
  U32 f7a, f7b, f7c;
  __asm__("cpuid" : "=a"(f7a), "=b"(f7b), "=c"(f7c) : "a"(7), "c"(0) : "edx");
  return (int)(f7b != 0 || f7a != 0 || f7c != 0);
}

int main(void) { return cpuid_leaf7_ebx_nonzero(); }
