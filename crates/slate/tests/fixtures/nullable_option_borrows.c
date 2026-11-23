#include <stdio.h>
#include <stddef.h>

// @check-fn-begin
static const int *find(const int *xs, int n, int key) {
  for (int i = 0; i < n; i++)
    if (xs[i] == key)
      return &xs[i];
  return NULL;
}
// @check-fn-end

// @check-fn-begin
static int lookup(const int *xs, int n, int key) {
  const int *hit = find(xs, n, key);
  if (!hit)
    return -1;
  return *hit;
}
// @check-fn-end

// @check-fn-begin
static int either(const int *xs, int n) {
  const int *hit = find(xs, n, 3);
  return hit ? *hit : 0;
}
// @check-fn-end

// @check-fn-begin
static const int *second(const int *xs) { return &xs[1]; }
// @check-fn-end

// @check-fn-begin
static int defensive(const int *p) {
  if (!p)
    return 0;
  return *p + 1;
}
// @check-fn-end

// @check-fn-begin
static int optional(const int *p) {
  if (p == NULL)
    return -7;
  return *p * 2;
}
// @check-fn-end

// @check-fn-begin
static void store(int *out, int value) {
  if (out)
    *out = value;
}
// @check-fn-end

// @check-fn-begin
static int chosen(int flag) {
  int a = 4;
  int b = 9;
  const int *p = NULL;
  if (flag)
    p = &a;
  else
    p = &b;
  return p ? *p : -1;
}
// @check-fn-end

// @check-fn-begin
static int run(int key) {
  int xs[3] = {1, 2, 3};
  int value = 6;
  int slot = 0;
  store(&slot, 5);
  store(NULL, 8);
  return lookup(xs, 3, key) + either(xs, 3) + *second(xs) + defensive(&value) + optional(NULL) +
         optional(&value) + slot;
}
// @check-fn-end

// @check-fn-begin
static const int *find_written(const int *xs, int n, int key) {
  for (int i = 0; i < n; i++)
    if (xs[i] == key)
      return &xs[i];
  return NULL;
}
// @check-fn-end

// @check-fn-begin
static int mutated(int key) {
  int xs[3] = {7, 8, 9};
  const int *hit = find_written(xs, 3, key);
  xs[0] = 1;
  return hit ? *hit + xs[0] : xs[0];
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", run(2), run(9));
  printf("%d %d\n", chosen(1), chosen(0));
  printf("%d %d\n", mutated(8), mutated(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn find(xs: &[i32], key: i32) -> Option<&i32> {
// CHECK-NEXT:     let n: i32 = xs.len() as i32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if xs[i as usize] == key {
// CHECK-NEXT:             return Some(&xs[i as usize]);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     None
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn lookup(xs: &[i32], key: i32) -> i32 {
// CHECK-NEXT:     let hit: Option<&i32> = find(xs, key);
// CHECK-NEXT:     if hit.is_none() {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     *hit.unwrap()
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn either(xs: &[i32]) -> i32 {
// CHECK-NEXT:     let hit: Option<&i32> = find(xs, 3i32);
// CHECK-NEXT:     if hit.is_some() { *hit.unwrap() } else { 0 }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn second(xs: &[i32]) -> &i32 {
// CHECK-NEXT:     &xs[1]
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn defensive(p: &i32) -> i32 {
// CHECK-NEXT:     if false {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     *p + 1
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn optional(p: Option<&i32>) -> i32 {
// CHECK-NEXT:     if p.is_none() {
// CHECK-NEXT:         return -7;
// CHECK-NEXT:     }
// CHECK-NEXT:     *p.unwrap() * 2
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn store(out: Option<&mut i32>, value: i32) {
// CHECK-NEXT:     let mut out = out;
// CHECK-NEXT:     if out.is_some() {
// CHECK-NEXT:         *out.as_deref_mut().unwrap() = value;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn chosen(flag: i32) -> i32 {
// CHECK-NEXT:     let mut a: i32 = 4;
// CHECK-NEXT:     let mut b: i32 = 9;
// CHECK-NEXT:     let mut p: *const i32 = core::ptr::null::<i32>();
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = core::ptr::addr_of_mut!(a) as *const i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         p = core::ptr::addr_of_mut!(b) as *const i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !p.is_null() { unsafe { *p } } else { -1 }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn run(key: i32) -> i32 {
// CHECK-NEXT:     let xs: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     let value: i32 = 6;
// CHECK-NEXT:     let mut slot: i32 = 0;
// CHECK-NEXT:     store(Some(&mut slot), 5i32);
// CHECK-NEXT:     store(None, 8i32);
// CHECK-NEXT:     lookup(xs.as_slice(), key) + either(xs.as_slice()) + *second(xs.as_slice())
// CHECK-NEXT:         + defensive(&value) + optional(None) + optional(Some(&value)) + slot
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn find_written(xs: *const i32, n: i32, key: i32) -> *const i32 {
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if (unsafe { *xs.offset(i as isize) }) == key {
// CHECK-NEXT:             return unsafe { xs.offset(i as isize) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     core::ptr::null::<i32>()
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn mutated(key: i32) -> i32 {
// CHECK-NEXT:     let mut xs: [i32; 3] = [7, 8, 9];
// CHECK-NEXT:     let hit: *const i32 = find_written(xs.as_mut_ptr() as *const i32, 3i32, key);
// CHECK-NEXT:     xs[0] = 1;
// CHECK-NEXT:     if !hit.is_null() { (unsafe { *hit }) + xs[0] } else { xs[0] }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
