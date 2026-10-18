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
// CHECK-LABEL: {{^}}fn get(p: &mut P, slot: &i32) -> i32 {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *((*(p as *mut P)).u.apSub.as_mut_ptr() as *mut *mut B)
// CHECK-NEXT:             .offset(*(slot as *const i32) as isize)
// CHECK-NEXT:     }) == std::ptr::null_mut::<B>()
// CHECK-NEXT:     {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         (*(*((*(p as *mut P)).u.apSub.as_mut_ptr() as *mut *mut B)
// CHECK-NEXT:             .offset(*(slot as *const i32) as isize)))
// CHECK-NEXT:             .n
// CHECK-NEXT:     }) + unsafe { (*(*(p as *mut P)).next).v }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
