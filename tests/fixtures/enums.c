#include <stdio.h>

enum Basic {
  BasicZero,
  BasicOne,
  BasicFive = 5,
  BasicSix,
  BasicNegative = -2,
  BasicNegativeNext
};

int main(void) {
  printf("%d\n", BasicZero);
  printf("%d\n", BasicOne);
  printf("%d\n", BasicFive);
  printf("%d\n", BasicSix);
  printf("%d\n", BasicNegative);
  printf("%d\n", BasicNegativeNext);
  return 0;
}
// REWRITES-DAG: enum Basic
// REWRITES-DAG: BasicFive = 5
// REWRITES-DAG: BasicNegative = -2
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: Basic::BasicZero as i32
// REWRITES-DAG: Basic::BasicOne as i32
// REWRITES-DAG: Basic::BasicFive as i32
// REWRITES-DAG: Basic::BasicSix as i32
// REWRITES-DAG: Basic::BasicNegative as i32
// REWRITES-DAG: Basic::BasicNegativeNext as i32
// REWRITES-NOT: println!("{}", 5);
// REWRITES: {{^}}}
