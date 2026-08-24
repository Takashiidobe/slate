#include <stdio.h>
#include <stdlib.h>

struct Item {
  int key;
  int value;
};

static int cmp_int(const void *a, const void *b) {
  return *(const int *)a - *(const int *)b;
}

static int cmp_item(const void *a, const void *b) {
  const struct Item *ia = (const struct Item *)a;
  const struct Item *ib = (const struct Item *)b;
  return ia->key - ib->key;
}

int main(void) {
  int nums[5] = {4, 1, 5, 3, 2};
  int key     = 3;
  qsort(nums, 5, sizeof(int), cmp_int);
  int *hit = bsearch(&key, nums, 5, sizeof(int), cmp_int);

  struct Item items[4] = {{3, 30}, {1, 10}, {4, 40}, {2, 20}};
  struct Item needle   = {4, 0};
  qsort(items, 4, sizeof(struct Item), cmp_item);
  struct Item *found =
      bsearch(&needle, items, 4, sizeof(struct Item), cmp_item);

  printf("%d %d %d %d\n", nums[0], nums[4], hit ? *hit : -1,
         found ? found->value : -1);
  return 0;
}
// REWRITES-DAG: nums.as_mut_slice().sort_by(|__slate_a, __slate_b| __slate_a.cmp(__slate_b));
// REWRITES-DAG: nums.as_slice().binary_search_by(|__slate_probe| __slate_probe.cmp(&key))
// REWRITES-DAG: items.as_mut_slice().sort_by(|__slate_a, __slate_b| __slate_a.key.cmp(&__slate_b.key));
// REWRITES-DAG: items.as_slice().binary_search_by(|__slate_probe| __slate_probe.key.cmp(&needle.key))
// REWRITES-NOT: fn qsort(
// REWRITES-NOT: fn bsearch(
// REWRITES-NOT: unsafe { qsort(
// REWRITES-NOT: unsafe { bsearch(
