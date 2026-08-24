// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{}", sum);
// REWRITES-DAG: println!("{} {}", partial[1], partial[3]);
// REWRITES-DAG: println!("{}", s);
// REWRITES-DAG: let padded: [i8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: for i in 0..5 {
// REWRITES-DAG: sum += a[((i as i64) as usize)];
// REWRITES-NOT: if !(i < 5)
// REWRITES: {{^}}}

#include <stdio.h>

int main(void) {
  int  a[5]       = {1, 2, 3, 4, 5};
  int  partial[4] = {7, 8};
  char s[6]       = "hello";
  char padded[8]  = "hi";

  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += a[i];
  }
  printf("%d\n", sum);
  printf("%d %d\n", partial[1], partial[3]);
  printf("%s\n", s);
  printf("%s %d\n", padded, padded[4]);
  return 0;
}
