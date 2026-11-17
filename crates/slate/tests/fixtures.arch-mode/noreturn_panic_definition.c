__attribute__((noreturn)) void panic(const char *fmt, ...);

static volatile int halted;

void panic(const char *fmt, ...) {
  halted = fmt[0];
  for (;;)
    ;
}
