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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn fill(mut out: &mut {{__SlateRecord[0-9]+}}, mut x: i32, mut y: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(out as *mut [[SLATERECORD_0:__SlateRecord[0-9]+]])).a = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(out as *mut [[SLATERECORD_0]])).b = y;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
