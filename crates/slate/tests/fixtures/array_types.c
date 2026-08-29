#include <stdio.h>

static int sum_char_array(void) {
  char values[3];
  values[0] = 2;
  values[1] = 5;
  values[2] = values[0] + values[1];
  return values[2];
}

static double pick_double_array(int index) {
  double values[3];
  values[0] = 1.25;
  values[1] = 2.50;
  values[2] = values[0] + values[1];
  return values[index];
}

int main(void) {
  printf("%d\n", sum_char_array());
  printf("%.2f\n", pick_double_array(2));
  return 0;
}
// REWRITES-DAG: values[((index as i64) as usize)]
// REWRITES-NOT: (0 as usize)
// REWRITES-NOT: (1 as usize)
// REWRITES-NOT: (2 as usize)
// REWRITES-NOT: ((values[0] as i32) + (values[1] as i32)) as i8
