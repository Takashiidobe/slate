#include <stdio.h>

struct Row {
  int v[3];
};

struct Tail {
  int n;
  int data[1];
};

static int table[4] = {10, 20, 30, 40};

// @check-fn-begin
static int sum4(void) {
  int a[4] = {1, 2, 3, 4};
  int s = 0;
  for (int i = 0; i < 4; i++) s += a[i];
  return s;
}
// @check-fn-end

// @check-fn-begin
static int constant_slots(void) {
  int a[3] = {0, 0, 0};
  a[0] = 4;
  a[2] = a[0] + 1;
  return a[0] * a[2];
}
// @check-fn-end

// @check-fn-begin
static int member_and_global(int k) {
  struct Row r = {{1, 2, 3}};
  r.v[1] = table[k];
  return r.v[0] + r.v[1] + r.v[2];
}
// @check-fn-end

// @check-fn-begin
static int grid(void) {
  int g[2][3];
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++) g[i][j] = i * 3 + j;
  return g[1][2] - g[0][1];
}
// @check-fn-end

// @check-fn-begin
static int doubled(void) {
  int a[3] = {1, 2, 3};
  for (int i = 0; i < 3; i++) a[i] = a[i] * 2;
  return a[0] + a[1] + a[2];
}
// @check-fn-end

// @check-fn-begin
static int aliased(void) {
  int a[3] = {1, 2, 3};
  int *q = a;
  int s = 0;
  for (int i = 0; i < 3; i++) {
    s += a[i];
    q[2] = 10;
  }
  return s;
}
// @check-fn-end

// @check-fn-begin
static int through_pointer(const int *p, struct Tail *t) {
  return p[1] + t->data[0];
}
// @check-fn-end

int main(void) {
  int b[2] = {5, 6};
  struct Tail t = {1, {7}};
  printf("%d %d %d %d %d\n", sum4(), constant_slots(), member_and_global(2), grid(), through_pointer(b, &t));
  printf("%d %d\n", doubled(), aliased());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn sum4() -> i32 {
// CHECK-NEXT:     let a: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in a.0.iter().copied() {
// CHECK-NEXT:         s += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn constant_slots() -> i32 {
// CHECK-NEXT:     let mut a: [i32; 3] = [0, 0, 0];
// CHECK-NEXT:     a[0] = 4;
// CHECK-NEXT:     a[2] = a[0] + 1;
// CHECK-NEXT:     a[0] * a[2]
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn member_and_global(k: i32) -> i32 {
// CHECK-NEXT:     let mut r: Row = Row { v: [1, 2, 3] };
// CHECK-NEXT:     r.v[1] = unsafe { table.0[k as usize] };
// CHECK-NEXT:     r.v[0] + r.v[1] + r.v[2]
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn grid() -> i32 {
// CHECK-NEXT:     let mut g: __SlateAlign16<{{\[\[}}i32; 3]; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     for i in 0 as i32..2 as i32 {
// CHECK-NEXT:         for j in 0 as i32..3 as i32 {
// CHECK-NEXT:             g.0[i as usize][j as usize] = i * 3 + j;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     g.0[1][2] - g.0[0][1]
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn doubled() -> i32 {
// CHECK-NEXT:     let mut a: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         a[i as usize] *= 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     a[0] + a[1] + a[2]
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn aliased() -> i32 {
// CHECK-NEXT:     let mut a: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     let q: *mut i32 = a.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         s += a[i as usize];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { q.offset(2 as isize) } = 10;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn through_pointer(p: *const i32, t: &mut Tail) -> i32 {
// CHECK-NEXT:     (unsafe { *unsafe { p.offset(1 as isize) } })
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*(t as *mut Tail)).data.as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                     .offset(0 as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
