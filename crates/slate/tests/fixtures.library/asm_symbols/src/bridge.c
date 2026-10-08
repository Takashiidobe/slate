extern int native_value[2];
extern int native_answer(void) __attribute__((visibility("hidden")));
int asm_symbols(void) {
  int *value;
  int (*answer)(void);
  __asm__("leaq %c1(%%rip), %0" : "=r"(value) : "i"(&native_value[1]));
  __asm__("leaq %P1(%%rip), %0" : "=r"(answer) : "i"(native_answer));
  return *value + answer();
}
