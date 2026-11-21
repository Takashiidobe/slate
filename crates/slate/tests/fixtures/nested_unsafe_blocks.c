#include <stdio.h>

struct B {
  int n;
};

struct P {
  union {
    struct B *apSub[4];
    int x;
  } u;
  struct P *next;
  int v;
};

// @check-fn-begin
static int get(struct P *p, int *slot) {
  if (p->u.apSub[*slot] == 0)
    return -1;
  return p->u.apSub[*slot]->n + p->next->v;
}
// @check-fn-end

int main(void) {
  struct B b = {5};
  struct P q = {{{0}}, 0, 3};
  struct P p = {{{&b, 0, 0, 0}}, &q, 1};
  int s = 0, t = 1;
  printf("%d %d\n", get(&p, &s), get(&p, &t));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn get(p: &P, slot: &i32) -> i32 {
// CHECK-NEXT:     if unsafe { *((*p).u.apSub.as_ptr() as *mut *mut B).offset(*slot as isize) }
// CHECK-NEXT:         .is_null() {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { (*(*((*p).u.apSub.as_ptr() as *mut *mut B).offset(*slot as isize))).n })
// CHECK-NEXT:         + unsafe { (*(*p).next).v }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
