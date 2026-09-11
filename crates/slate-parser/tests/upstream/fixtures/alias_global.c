int        real_global = 12;
extern int alias_global __attribute__((alias("real_global")));

int main(void) {
  int result;
  result = alias_global;
  return result;
}


