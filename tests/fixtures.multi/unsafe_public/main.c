#include <stdio.h>

int read_ptr(int *p);

int main(void) {
  int value = 7;
  printf("%d\n", read_ptr(&value));
  return 0;
}
// LOWERING-DAG: use crate::pointers::read_ptr;
// LOWERING-LABEL: {{^}}fn main() {
// LOWERING-DAG: unsafe { read_ptr(
// LOWERING: {{^}}}
