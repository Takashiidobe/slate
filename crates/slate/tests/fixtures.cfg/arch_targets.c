int printf(const char *, ...);

#if defined(__x86_64__) || defined(_M_X64)
static int arch_code(void) { return 64; }
#elif defined(__i386__) || defined(_M_IX86)
static int arch_code(void) { return 86; }
#elif defined(__aarch64__) || defined(_M_ARM64)
static int arch_code(void) { return 128; }
#elif defined(__arm__) || defined(_M_ARM)
static int arch_code(void) { return 32; }
#elif defined(__powerpc64__) || defined(__PPC64__)
static int arch_code(void) { return 640; }
#elif defined(__powerpc__) || defined(__POWERPC__) || defined(_M_PPC)
static int arch_code(void) { return 320; }
#elif defined(__wasm64__)
static int arch_code(void) { return 6400; }
#elif defined(__wasm32__)
static int arch_code(void) { return 3200; }
#elif defined(_M_RISCV64)
static int arch_code(void) { return 645; }
#elif defined(_M_RISCV32)
static int arch_code(void) { return 325; }
#else
static int arch_code(void) { return 0; }
#endif

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
// DIRECTIVES-DAG: #[cfg(not(any(target_arch = "x86_64", target_arch = "x86", target_arch = "aarch64", target_arch = "arm", target_arch = "powerpc64", target_arch = "powerpc", target_arch = "wasm64", target_arch = "wasm32", target_arch = "riscv64", target_arch = "riscv32")))]
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}64;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}86;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}128;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}32;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}640;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}320;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}6400;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}3200;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}645;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}325;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = }}0;
