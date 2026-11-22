#include <stdio.h>

static int *g;

__attribute__((noinline)) static void poke(void) { *g += 100; }

__attribute__((noinline)) static void stash(int *p) { g = p; }

// @check-fn-begin
static int bump(int *p) {
  int before = *p;
  poke();
  return *p - before;
}
// @check-fn-end

// @check-fn-begin
static int bump_stashed(int *p) {
  int before = *p;
  poke();
  return *p - before;
}
// @check-fn-end

static int *id(int *p) { return p; }

// @check-fn-begin
static void two(int *a, int *b) {
  *a = 1;
  *b = *b + 2;
}
// @check-fn-end

// @check-fn-begin
static int read_only(const int *p) { return *p + 1; }
// @check-fn-end

int main(void) {
  int x = 1, y = 0, z = 5, w = 7;
  g = &x;
  printf("%d\n", bump(&x));
  stash(&z);
  printf("%d\n", bump_stashed(&z));
  int *q = id(&y);
  two(&y, q);
  int *r = &w;
  two(&w, r);
  int k = 3;
  printf("%d %d %d %d %d\n", x, y, z, w, read_only(&k));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn bump(p: *mut i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *p };
// CHECK-NEXT:     poke();
// CHECK-NEXT:     (unsafe { *p }) - before
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bump_stashed(p: *mut i32) -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { *p };
// CHECK-NEXT:     poke();
// CHECK-NEXT:     (unsafe { *p }) - before
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn two(a: *mut i32, b: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *a = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *b += 2;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn read_only(p: &i32) -> i32 {
// CHECK-NEXT:     *p + 1
// CHECK: {{^}}}
// SLATE-FILECHECK-END
