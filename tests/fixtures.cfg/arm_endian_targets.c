int printf(const char *, ...);

#if defined(__ARMEB__) || defined(__AARCH64EB__)
static int arm_endian_code(void) { return 100; }
#elif defined(__ARMEL__) || defined(__AARCH64EL__)
static int arm_endian_code(void) { return 200; }
#else
static int arm_endian_code(void) { return 0; }
#endif

int main(void) {
  printf("%d\n", arm_endian_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(any(all(target_arch = "arm", target_endian = "big"), all(target_arch = "aarch64", target_endian = "big")))]
// DIRECTIVES-DAG: #[cfg(any(all(target_arch = "arm", target_endian = "little"), all(target_arch = "aarch64", target_endian = "little")))]
// DIRECTIVES-DAG: #[cfg(not(any(any(all(target_arch = "arm", target_endian = "big"), all(target_arch = "aarch64", target_endian = "big")), any(all(target_arch = "arm", target_endian = "little"), all(target_arch = "aarch64", target_endian = "little")))))]
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 100;
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 200;
// DIRECTIVES-DAG: let _v{{[0-9]+}}: i32 = 0;
