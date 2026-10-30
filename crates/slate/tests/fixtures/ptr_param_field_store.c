#include <stdio.h>

typedef struct {
  int a;
  int b;
} pair_t;

// @check-fn-begin
static void fill(pair_t *out, int x, int y) {
  out->a = x;
  out->b = y;
}
// @check-fn-end

int main(void) {
  pair_t p;
  fill(&p, 3, 4);
  printf("%d %d\n", p.a, p.b);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn fill(out: &mut __SlateRecord_ptr_param_field_store_c_6, x: i32, y: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(out as *mut __SlateRecord_ptr_param_field_store_c_6)).a = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(out as *mut __SlateRecord_ptr_param_field_store_c_6)).b = y;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
