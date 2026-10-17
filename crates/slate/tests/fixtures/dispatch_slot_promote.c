#include <stdio.h>

struct Pair {
  int a;
  int b;
};

static void bump(int *p) { *p += 10; }

// @check-fn-begin
static int counted(int n) {
  int i = 0;
  int total = 0;
again:
  total += i;
  i++;
  if (i < n) goto again;
  return total;
}
// @check-fn-end

// @check-fn-begin
static int escaped(int n) {
  int x = n;
  int k = 0;
top:
  bump(&x);
  k++;
  if (k < 3) goto top;
  return x;
}
// @check-fn-end

// @check-fn-begin
static int record(int n) {
  struct Pair p = {1, 2};
  int k = 0;
loop:
  p.a += p.b;
  if (++k < n) goto loop;
  return p.a;
}
// @check-fn-end

// @check-fn-begin
static int aligned(int n) {
  _Alignas(32) int v = 1;
  int k = 0;
loop:
  v *= 2;
  if (++k < n) goto loop;
  return v;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d %d\n", counted(5), escaped(1), record(4), aligned(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn counted(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             total += i;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             if !(i < n) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn escaped(n: i32) -> i32 {
// CHECK-NEXT:     let mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = n;
// CHECK-NEXT:         k = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             bump(unsafe { &mut (*std::ptr::addr_of_mut!(x)) });
// CHECK-NEXT:             k += 1;
// CHECK-NEXT:             if !(k < 3) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return x;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn record(n: i32) -> i32 {
// CHECK-NEXT:     let mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: std::mem::MaybeUninit<Pair> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut Pair = std::ptr::addr_of_mut!(p) as *mut Pair;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_0]], Pair { a: 1, b: 2 });
// CHECK-NEXT:         k = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             (*[[SLATE_SLOT_0]]).a += (*[[SLATE_SLOT_0]]).b;
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = k + 1;
// CHECK-NEXT:             k = [[V_0]];
// CHECK-NEXT:             if !([[V_0]] < n) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return (*[[SLATE_SLOT_0]]).a;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn aligned(n: i32) -> i32 {
// CHECK-NEXT:     let mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut v: std::mem::MaybeUninit<__SlateAlign32<i32>> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(v) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_0]], 1 as i32);
// CHECK-NEXT:         k = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             *[[SLATE_SLOT_0]] *= 2;
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = k + 1;
// CHECK-NEXT:             k = [[V_0]];
// CHECK-NEXT:             if !([[V_0]] < n) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return *[[SLATE_SLOT_0]];
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
