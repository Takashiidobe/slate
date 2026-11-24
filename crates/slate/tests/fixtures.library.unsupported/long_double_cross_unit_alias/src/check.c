typedef long double (*callback)(long double);

extern long double alias_step(long double);
extern long double weak_step(long double);

static callback const steps[] = {alias_step, weak_step};

int check_long_double_aliases(void) {
  return (steps[0](1.0L) != 0x1.0000000000000002p0L) +
         2 * (steps[1](1.0L) != 0x1.0000000000000002p0L);
}
