#include <stdio.h>

static int sum(int *items, int len) {
  int total = 0;
  for (int i = 0; i < len; i++) {
    total += items[i];
  }
  return total;
}

static void bump(int *items, int len) {
  for (int i = 0; i < len; i++) {
    items[i] += 1;
  }
}

int main(void) {
  int values[4] = {2, 4, 6, 8};
  printf("%d\n", sum(values, 4));
  bump(values, 4);
  printf("%d %d\n", values[0], values[3]);
  return 0;
}
// REWRITES-DAG: fn sum(items: &[i32], len: i32) -> i32
// REWRITES-DAG: fn bump(mut items: &mut [i32], len: i32) {
// REWRITES-DAG: total += items[(i as usize)];
// REWRITES-DAG: items[(i as usize)] = items[(i as usize)] + 1;
// REWRITES-DAG: sum(values.as_slice(), 4)
// REWRITES-DAG: bump(values.as_mut_slice(), 4)
// REWRITES-NOT: __slate_item
// REWRITES-NOT: .offset(
// REWRITES-NOT: items.as_mut_ptr()
