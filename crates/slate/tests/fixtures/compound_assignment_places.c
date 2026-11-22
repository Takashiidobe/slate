#include <stdio.h>

struct counter {
  int hits;
  unsigned flags;
};

static int next(void) {
  static int n = 3;
  return n++;
}

// @check-fn-begin
static int through_pointer(int *p, int n) {
  *p *= next();
  p[n] -= 2;
  return *p + p[n];
}
// @check-fn-end

// @check-fn-begin
static unsigned fields(struct counter *c) {
  c->hits += 4;
  c->flags |= 1u << 3;
  c->flags >>= 1;
  return (unsigned)c->hits + c->flags;
}
// @check-fn-end

// @check-fn-begin
static int promoted(char c, short s) {
  c += 1;
  s *= 2;
  return c + s;
}
// @check-fn-end

// @check-fn-begin
static int increments(int *a, int i) {
  int before = i++;
  ++i;
  a[i++] += 10;
  i = i + 1;
  return before + i;
}
// @check-fn-end

// @check-fn-begin
static int volatile_kept(int v) {
  volatile int x = v;
  x += 5;
  return x;
}
// @check-fn-end

int main(void) {
  int values[6] = {1, 2, 3, 4, 5, 6};
  struct counter counter = {1, 2};
  printf("%d\n", through_pointer(values, 2));
  printf("%u\n", fields(&counter));
  printf("%d\n", promoted(126, 300));
  printf("%d %d\n", increments(values, 1), values[3]);
  printf("%d\n", volatile_kept(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn through_pointer(p: &mut [i32], n__0: i32) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = next();
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = p.as_mut_ptr();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] *= [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     p[n__0 as usize] -= 2;
// CHECK-NEXT:     p[0usize] + p[n__0 as usize]
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn fields(c: &mut counter) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*c).hits += 4;
// CHECK-NEXT:         (*c).flags |= 1u32 << 3i32;
// CHECK-NEXT:         (*c).flags >>= 1i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     ((*c).hits as u32).wrapping_add((*c).flags)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn promoted(mut c: i8, mut s: i16) -> i32 {
// CHECK-NEXT:     c = ((c as i32) + 1) as i8;
// CHECK-NEXT:     s = ((s as i32) * 2) as i16;
// CHECK-NEXT:     (c as i32) + (s as i32)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn increments(a: &mut [i32], mut i: i32) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     i = [[V_0]] + 1;
// CHECK-NEXT:     let before: i32 = [[V_0]];
// CHECK-NEXT:     i += 1;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     i = [[V_1]] + 1;
// CHECK-NEXT:     a[[[V_1]] as usize] += 10;
// CHECK-NEXT:     i += 1;
// CHECK-NEXT:     before + i
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn volatile_kept(v: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = v;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write_volatile(
// CHECK-NEXT:             core::ptr::addr_of_mut!(x),
// CHECK-NEXT:             core::ptr::read_volatile(core::ptr::addr_of!(x)) + 5,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { core::ptr::read_volatile(core::ptr::addr_of!(x)) }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
