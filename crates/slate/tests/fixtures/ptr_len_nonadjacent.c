#include <stdio.h>

static int sum_items(int *items, int printable, int length) {
  printf("this is another number: %d\n", printable);
  int total = 0;
  for (int i = 0; i < length; i++) {
    total += items[i];
  }
  return total;
}

int main(void) {
  int values[] = {2, 4, 6, 8};
  printf("%d\n", sum_items(values, 5, 4));
  return 0;
}
// REWRITES-DAG: fn sum_items(items: &[i32], printable: i32, length: i32) -> i32
// REWRITES-DAG: println!("this is another number: {}", printable);
// REWRITES-DAG: sum_items(values.as_slice(), 5, 4)
// REWRITES-NOT: let printable: i32 = items.len() as i32;
// REWRITES-NOT: fn sum_items(items: &[i32])
