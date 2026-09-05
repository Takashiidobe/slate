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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     (*{{arg[0-9]+}}).cells[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     (*({{arg[0-9]+}} as *mut Row)).cells[({{__v[0-9]+}} as usize)] = 9;
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
