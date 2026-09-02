#include <stdio.h>

struct Row {
  int cells[2];
};

static void set_cell(struct Row *row) {
  // @lowering-begin
  // @rewrite-begin
  row->cells[1] = 9;
  // @rewrite-end
  // @lowering-end
}

static int *mutable_identity(const int *value) { return (int *)value; }

int main(void) {
  struct Row row = {{3, 4}};
  set_cell(&row);
  int value                 = 7;
  *mutable_identity(&value) = 11;
  printf("%d %d\n", row.cells[1], value);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = 9;
// LOWERING-DAG: let {{_v[0-9]+}}: i64 = 1;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     (*{{arg[0-9]+}}).cells[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     (*({{arg[0-9]+}} as *mut Row)).cells[({{_v[0-9]+}} as usize)] = 9;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
