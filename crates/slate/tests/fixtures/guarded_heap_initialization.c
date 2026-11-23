#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @check-fn-begin
static void fill_loop(int flag) {
  int *p = NULL;
  if (flag)
    p = malloc(4 * sizeof(int));
  if (p) {
    for (int i = 0; i < 4; ++i)
      p[i] = i;
    printf("%d\n", p[3]);
    free(p);
  }
}
// @check-fn-end

// @check-fn-begin
static void zero_fill(int flag) {
  int *p = NULL;
  if (flag)
    p = malloc(4 * sizeof(int));
  if (p) {
    memset(p, 0, 4 * sizeof(int));
    printf("%d\n", p[3]);
    free(p);
  }
}
// @check-fn-end

// @check-fn-begin
static void zeroed_allocation(int flag) {
  int *p = NULL;
  if (flag)
    p = calloc(4, sizeof(int));
  if (p) {
    printf("%d\n", p[3]);
    free(p);
  }
}
// @check-fn-end

// @check-fn-begin
static int grow_prefix(int flag) {
  int result = 0;
  int *p = NULL;
  if (flag)
    p = malloc(sizeof(int));
  if (p) {
    p[0] = 41;
    int *q = realloc(p, 4 * sizeof(int));
    if (!q) {
      int saved = p[0];
      free(p);
      return saved;
    }
    p = q;
    p[1] = 42;
    result = p[1];
    free(p);
  }
  return result;
}
// @check-fn-end

// @check-fn-begin
static int grow_sparse(int flag) {
  int result = 0;
  int *p = NULL;
  if (flag)
    p = calloc(1, sizeof(int));
  if (p) {
    int *q = realloc(p, 4 * sizeof(int));
    if (!q) {
      int saved = p[0];
      free(p);
      return saved;
    }
    p = q;
    p[3] = p[0];
    p[3] = 43;
    result = p[3];
    free(p);
  }
  return result;
}
// @check-fn-end

int main(void) {
  fill_loop(1);
  zero_fill(1);
  zeroed_allocation(1);
  printf("%d %d\n", grow_prefix(1), grow_sparse(1));
  printf("%d %d\n", grow_prefix(0), grow_sparse(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn fill_loop(flag: i32) {
// CHECK-NEXT:     let mut p: Option<Vec<i32>> = None;
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = Vec::try_alloc(4);
// CHECK-NEXT:     }
// CHECK-NEXT:     if let Some(p) = p.as_mut() {
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             p.push(i);
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", p[3]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn zero_fill(flag: i32) {
// CHECK-NEXT:     let mut p: Option<Vec<i32>> = None;
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = Vec::try_alloc(4);
// CHECK-NEXT:     }
// CHECK-NEXT:     if let Some(p) = p.as_mut() {
// CHECK-NEXT:         p.resize(4, 0);
// CHECK-NEXT:         println!("{}", p[3]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn zeroed_allocation(flag: i32) {
// CHECK-NEXT:     let mut p: Option<Vec<i32>> = None;
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = Vec::try_filled(4, 0);
// CHECK-NEXT:     }
// CHECK-NEXT:     if let Some(p) = p.as_mut() {
// CHECK-NEXT:         println!("{}", p[3]);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn grow_prefix(flag: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     let mut p: Option<Vec<i32>> = None;
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = Vec::try_alloc(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     if let Some(p) = p.as_mut() {
// CHECK-NEXT:         p.push(41i32);
// CHECK-NEXT:         let q: bool = p.try_reserve_len(4).is_none();
// CHECK-NEXT:         if q {
// CHECK-NEXT:             let saved: i32 = p[0];
// CHECK-NEXT:             return saved;
// CHECK-NEXT:         }
// CHECK-NEXT:         p.push(42i32);
// CHECK-NEXT:         result = p[1];
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn grow_sparse(flag: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     let mut p: Option<Vec<core::mem::MaybeUninit<i32>>> = None;
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = Vec::try_filled(1, core::mem::MaybeUninit::new(0));
// CHECK-NEXT:     }
// CHECK-NEXT:     if let Some(p) = p.as_mut() {
// CHECK-NEXT:         let q: bool = p.try_resize_with(4, core::mem::MaybeUninit::uninit).is_none();
// CHECK-NEXT:         if q {
// CHECK-NEXT:             let saved: i32 = unsafe { p[0].assume_init_read() };
// CHECK-NEXT:             return saved;
// CHECK-NEXT:         }
// CHECK-NEXT:         p[3] = core::mem::MaybeUninit::new(unsafe { p[0].assume_init_read() });
// CHECK-NEXT:         p[3].write(43i32);
// CHECK-NEXT:         result = unsafe { p[3].assume_init_read() };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// SLATE-FILECHECK-END
