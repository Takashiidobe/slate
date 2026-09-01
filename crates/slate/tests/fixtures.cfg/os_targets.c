int printf(const char *, ...);

#if defined(_WIN32)
static int os_code(void) { return 10; }
#elif defined(__ANDROID__)
static int os_code(void) { return 25; }
#elif defined(__linux__)
static int os_code(void) { return 20; }
#elif defined(__APPLE__)
static int os_code(void) { return 30; }
#elif defined(__FreeBSD__)
static int os_code(void) { return 35; }
#else
static int os_code(void) { return 40; }
#endif

int main(void) {
  printf("%d\n", os_code());
  return 0;
}
// DIRECTIVES-DAG: #[cfg(windows)]
// DIRECTIVES-DAG: #[cfg(target_os = "android")]
// DIRECTIVES-DAG: #[cfg(target_os = "linux")]
// DIRECTIVES-DAG: #[cfg(target_vendor = "apple")]
// DIRECTIVES-DAG: #[cfg(target_os = "freebsd")]
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}10;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}25;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}20;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}30;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}35;
// DIRECTIVES-DAG: {{__retval = |let _v[0-9]+: i32 = |return }}40;
