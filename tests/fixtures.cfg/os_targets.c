int printf(const char *, ...);

// @rewrite-fn-begin
#if defined(_WIN32)
int os_code(void) { return 10; }
#elif defined(__ANDROID__)
int os_code(void) { return 25; }
#elif defined(__linux__)
int os_code(void) { return 20; }
#elif defined(__APPLE__)
int os_code(void) { return 30; }
#elif defined(__FreeBSD__)
int os_code(void) { return 35; }
#else
int os_code(void) { return 40; }
#endif
// @rewrite-fn-end

int main(void) {
  printf("%d\n", os_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(windows)]
// DIRECTIVES-DAG: #[cfg(target_os = "android")]
// DIRECTIVES-DAG: #[cfg(target_os = "linux")]
// DIRECTIVES-DAG: #[cfg(target_vendor = "apple")]
// DIRECTIVES-DAG: #[cfg(target_os = "freebsd")]

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn os_code() -> i32 {
// REWRITES-DAG:     20
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
