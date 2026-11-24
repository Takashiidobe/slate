typedef long double (*callback)(long double);

extern long double optional_step(long double) __attribute__((weak));

int main(void) {
  callback step = optional_step;
  return step != 0;
}
