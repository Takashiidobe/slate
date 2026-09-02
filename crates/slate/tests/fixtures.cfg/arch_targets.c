int printf(const char *, ...);

// @rewrite-fn-begin
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
// @rewrite-fn-end

int main(void) {
  printf("%d\n", arch_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(target_arch = "x86_64")]
// DIRECTIVES-DAG: #[cfg(target_arch = "x86")]
// DIRECTIVES-DAG: #[cfg(target_arch = "aarch64")]
// DIRECTIVES-DAG: #[cfg(target_arch = "arm")]
// DIRECTIVES-DAG: #[cfg(target_arch = "powerpc64")]
// DIRECTIVES-DAG: #[cfg(target_arch = "powerpc")]
// DIRECTIVES-DAG: #[cfg(target_arch = "wasm64")]
// DIRECTIVES-DAG: #[cfg(target_arch = "wasm32")]
// DIRECTIVES-DAG: #[cfg(target_arch = "riscv64")]
// DIRECTIVES-DAG: #[cfg(target_arch = "riscv32")]

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn arch_code() -> i32 {
// REWRITES-DAG:     64
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
