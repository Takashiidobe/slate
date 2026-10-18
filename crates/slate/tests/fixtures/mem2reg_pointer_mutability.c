#include <stdio.h>

struct Row {
  int cells[2];
};

// @check-fn-begin
static void set_cell(struct Row *row) {
  row->cells[1] = 9;
}
// @check-fn-end

static int *mutable_identity(const int *value) { return (int *)value; }

int main(void) {
  struct Row row = {{3, 4}};
  set_cell(&row);
  int value                 = 7;
  *mutable_identity(&value) = 11;
  printf("%d %d\n", row.cells[1], value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn set_cell(row: &mut Row) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((*(row as *mut Row)).cells.as_mut_ptr() as *mut i32).offset(1 as isize) = 9;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
