#include <stdio.h>

struct Pair {
  int left;
  int right;
};

static int counter = 5;
static int table[4] = {1, 2, 3, 4};
static int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};
static struct Pair pair = {8, 9};
static char name[] = "slate";

static int *counter_ptr = &counter;
static int *table_ptr = table;
static int *table_third = &table[2];
static int *table_end = table + 4;
static int (*table_whole)[4] = &table;
static int *grid_row = grid[1];
static int *pair_right = &pair.right;
static struct Pair *pair_ptr = &pair;
static char *name_ptr = name;
static const char *name_tail = name + 2;
static int *const fixed_counter = &counter;
static int **counter_ptr_ptr = &counter_ptr;

static int *slots[3] = {&counter, &table[1], &pair.left};

struct Cursor {
  const char *label;
  int *value;
  struct Pair *pair;
};

static struct Cursor cursors[2] = {
    {"counter", &counter, &pair},
    {"table", table + 3, 0},
};

int main(void) {
  *counter_ptr += 1;
  table_ptr[0] = 10;
  *table_third += 20;
  (*table_whole)[3] = 40;
  grid_row[2] = 60;
  *pair_right = 90;
  pair_ptr->left = 80;
  name_ptr[0] = 'S';
  **counter_ptr_ptr += 100;
  *slots[1] = 22;
  *slots[2] += 1;
  *cursors[1].value += 400;
  int sum = 0;
  for (int *it = table; it != table_end; ++it) {
    sum += *it;
  }
  printf("%d %d %d %d %d\n", counter, *fixed_counter, table[0], table[1],
         table[2]);
  printf("%d %d %d %d\n", table[3], grid[1][2], pair.left, pair.right);
  printf("%s %s %d\n", name, name_tail, sum);
  printf("%s %d %d %s %d\n", cursors[0].label, *cursors[0].value,
         cursors[0].pair->right, cursors[1].label, cursors[1].pair == 0);
  return sum % 50;
}
