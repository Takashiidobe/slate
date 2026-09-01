#include <stdio.h>
#include <stdlib.h>

static int sum_fixed(int *p) {
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += p[i];
  }
  return total;
}

static void scale_fixed(int *p) {
  for (int i = 0; i < 3; i++) {
    p[i] = p[i] * 2;
  }
}

static int sum3(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  return t;
}

static int mix(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  t += p[5];
  return t;
}

int main(void) {
  int arr[4] = {10, 20, 30, 40};
  // @lowering-begin
  // @rewrite-begin
  int r = sum_fixed(arr);
  // @rewrite-end
  // @lowering-end

  int a[3] = {1, 2, 3};
  // @rewrite-begin
  scale_fixed(a);
  // @rewrite-end

  int *m = malloc(3 * sizeof(int));
  m[0] = 5;
  m[1] = 6;
  m[2] = 7;
  // @rewrite-begin
  int r2 = sum3(m);
  // @rewrite-end
  free(m);

  int big[6] = {1, 2, 3, 4, 5, 6};
  // @rewrite-begin
  int r3 = mix(big);
  // @rewrite-end

  printf("%d %d %d %d %d\n", r, a[0], a[2], r2, r3);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i32 = arr.as_mut_ptr() as *mut i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = sum_fixed({{_v[0-9]+}});
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = arr.as_mut_ptr() as *mut i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = sum_fixed(unsafe { std::slice::from_raw_parts({{_v[0-9]+}} as *const i32, 4usize) });
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-DAG: scale_fixed(unsafe { std::slice::from_raw_parts_mut({{_v[0-9]+}} as *mut i32, 3usize) });
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = sum3(unsafe { std::slice::from_raw_parts({{_v[0-9]+}} as *const i32, 3usize) });
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i32 = big.as_mut_ptr() as *mut i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = mix({{_v[0-9]+}});
// SLATE-FILECHECK-END rewrites
