#include <stddef.h>
#include <stdio.h>

struct inner {
  int x;
  double d;
};

struct outer {
  char tag;
  struct inner in;
  int pair[2];
};

struct __attribute__((packed)) wire {
  char c;
  int i;
};

#pragma pack(push, 2)
struct half {
  char c;
  int i;
};
#pragma pack(pop)

union either {
  int i;
  float f;
};

// @check-fn-begin
static void bump(int *p) { *p += 1; }
// @check-fn-end

// @check-fn-begin
static void scale(double *p) { *p *= 2.0; }
// @check-fn-end

// @check-fn-begin
static void mark(char *p) { *p = 'w'; }
// @check-fn-end

// @check-fn-begin
static void bump_packed(int *p) { *p += 10; }
// @check-fn-end

// @check-fn-begin
static void bump_half(int *p) { *p += 20; }
// @check-fn-end

// @check-fn-begin
static void bump_union(int *p) { *p += 30; }
// @check-fn-end

// @check-fn-begin
static char owner_tag(double *d) {
  struct inner *owner = (struct inner *)((char *)d - offsetof(struct inner, d));
  return (char)('a' + owner->x);
}
// @check-fn-end

// @check-fn-begin
static void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}
// @check-fn-end

int main(void) {
  struct outer o = {'o', {1, 1.5}, {3, 4}};
  struct wire w = {'c', 5};
  struct half h = {'h', 6};
  union either e = {7};
  struct outer a = {'a', {8, 2.5}, {9, 10}};
  struct wire b = {'b', 11};
  struct inner c = {3, 0.5};
  bump(&o.in.x);
  scale(&o.in.d);
  mark(&w.c);
  bump_packed(&w.i);
  bump_half(&h.i);
  bump_union(&e.i);
  swap(&a.pair[0], &a.pair[1]);
  swap(&a.in.x, &b.i);
  printf("%c %d %.1f %d %d\n", o.tag, o.in.x, o.in.d, o.pair[0], o.pair[1]);
  printf("%c %d %c %d %d\n", w.c, w.i, h.c, h.i, e.i);
  int bi = b.i;
  printf("%d %d %d %d\n", a.in.x, a.pair[0], a.pair[1], bi);
  printf("%c\n", owner_tag(&c.d));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn bump(p: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = *p + 1;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn scale(p: &mut f64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = *p * 2.0f64;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn mark(p: &mut i8) {
// CHECK-NEXT:     *p = 119;
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bump_packed({{__v[0-9]+}}: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0:__v[0-9]+]] = *[[V_0]] + 10;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bump_half({{__v[0-9]+}}: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0:__v[0-9]+]] = *[[V_0]] + 20;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn bump_union(p: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = *p + 30;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn owner_tag(d: *mut f64) -> i8 {
// CHECK-NEXT:     let owner: *mut inner = (unsafe {
// CHECK-NEXT:         (d as *mut i8).offset(-(core::mem::offset_of!(inner, d) as isize))
// CHECK-NEXT:     }) as *mut inner;
// CHECK-NEXT:     (97 + unsafe { (*owner).x }) as i8
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn swap(a: *mut i32, b: *mut i32) {
// CHECK-NEXT:     let t: i32 = unsafe { *a };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *a = *b;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *b = t;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
