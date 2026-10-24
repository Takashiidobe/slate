#include <stdio.h>
#include <stdbool.h>

// @check-fn-begin
static int neq(unsigned *p, unsigned h) { if (!(p[h] != 0)) return 1; return 0; }
// @check-fn-end

// @check-fn-begin
static int nlt(int a, int b) { if (!(a < b)) return 1; return 0; }
// @check-fn-end

// @check-fn-begin
static int nne(int a, int b) { if (!(a != b)) return 1; return 0; }
// @check-fn-end

// @check-fn-begin
static int cond_ne0(int a, int b) { if ((a < b) != 0) return 1; return 0; }
// @check-fn-end

// @check-fn-begin
static int bool_ne0(bool a) { if (a != 0) return 1; return 0; }
// @check-fn-end

// @check-fn-begin
static int flag(int a) { bool r; if (a) { r = false; } else { r = true; } return r; }
// @check-fn-end

// @check-fn-begin
static int dneg(int a) { return !!(a < 3); }
// @check-fn-end

// @check-fn-begin
static int nneg(int a, int b) { return !(a == b && b > 2); }
// @check-fn-end

// @check-fn-begin
static int lit(int a) { bool r = true; if (a) r = false; return r; }
// @check-fn-end

int main(void) {
  unsigned v[2] = {0, 1};
  printf("%d %d %d %d %d %d %d %d %d\n", neq(v, 1), nlt(1, 2), nne(3, 3), cond_ne0(1, 2),
         bool_ne0(true), flag(0), dneg(1), nneg(1, 1), lit(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn neq(p: *mut u32, h: u32) -> i32 {
// CHECK-NEXT:     if (unsafe { *p.add(h as usize) }) == 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn nlt(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     if a >= b {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn nne(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     if a == b {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn cond_ne0(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     if a < b {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bool_ne0(a: bool) -> i32 {
// CHECK-NEXT:     if a {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn flag(a: i32) -> i32 {
// CHECK-NEXT:     let r: bool = a == 0;
// CHECK-NEXT:     r as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn dneg(a: i32) -> i32 {
// CHECK-NEXT:     (a < 3) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn nneg(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     !(a == b && b > 2) as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn lit(a: i32) -> i32 {
// CHECK-NEXT:     let mut r: bool = true;
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         r = false;
// CHECK-NEXT:     }
// CHECK-NEXT:     r as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
