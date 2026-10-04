int printf(const char *, ...);

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

int main(void) {
  printf("%d\n", os_code());
  return 0;
}
