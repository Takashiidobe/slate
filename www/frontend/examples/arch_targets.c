int printf(const char *, ...);

#if defined(__x86_64__) || defined(_M_X64)
int arch_code(void) { return 64; }
#elif defined(__i386__) || defined(_M_IX86)
int arch_code(void) { return 86; }
#elif defined(__aarch64__) || defined(_M_ARM64)
int arch_code(void) { return 128; }
#elif defined(__arm__) || defined(_M_ARM)
int arch_code(void) { return 32; }
#elif defined(__powerpc64__) || defined(__PPC64__)
int arch_code(void) { return 640; }
#elif defined(__powerpc__) || defined(__POWERPC__) || defined(_M_PPC)
int arch_code(void) { return 320; }
#elif defined(__wasm64__)
int arch_code(void) { return 6400; }
#elif defined(__wasm32__)
int arch_code(void) { return 3200; }
#elif defined(_M_RISCV64)
int arch_code(void) { return 645; }
#elif defined(_M_RISCV32)
int arch_code(void) { return 325; }
#else
int arch_code(void) { return 0; }
#endif

int main(void) {
  printf("%d\n", arch_code());
  return 0;
}
