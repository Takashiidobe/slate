#include <stdio.h>

static int add(int a, int unused) { return a + 1; }

static int get_used(int a, int b) { return a + b; }

static int remove_two(int a, int unused_a, int unused_b) { return a + 2; }

static int address_taken(int a, int unused) { return a + 4; }

int main(void) {
  int (*indirect)(int, int) = address_taken;
  printf("%d %d %d %d\n", add(5, 10), get_used(1, 2), remove_two(3, 4, 5),
         indirect(8, 9));
  return 0;
}
// REWRITES-DAG: fn add(a: i32) -> i32
// REWRITES-DAG: fn get_used(a: i32, b: i32) -> i32
// REWRITES-DAG: fn remove_two(a: i32) -> i32
// REWRITES-DAG: fn address_taken(a: i32, unused: i32) -> i32
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: add(5)
// REWRITES-DAG: get_used(1, 2)
// REWRITES-DAG: remove_two(3)
// REWRITES-DAG: indirect.unwrap()(8, 9)
// REWRITES-NOT: add(5, 10)
// REWRITES: {{^}}}
