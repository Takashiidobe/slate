#include <stdio.h>
#include <stdlib.h>

struct point {
  int x;
  int y;
};

static int counter(int start) {
  int total = start;
  void bump(int by) { total += by; }
  bump(2);
  bump(3);
  return total;
}

static int grandparent(int n) {
  int base = n * 10;
  int middle(int k) {
    int scale = k + 1;
    int inner(int j) { return base + scale * j; }
    return inner(k) + inner(1);
  }
  return middle(2) + middle(3);
}

static int parity(int n) {
  int calls = 0;
  auto int is_odd(int);
  int is_even(int k) {
    calls++;
    return k == 0 ? 1 : is_odd(k - 1);
  }
  int is_odd(int k) {
    calls++;
    return k == 0 ? 0 : is_even(k - 1);
  }
  return is_even(n) * 100 + calls;
}

static int aggregates(void) {
  int values[4] = {1, 2, 3, 4};
  struct point p = {5, 6};
  int *cursor = &values[1];
  void touch(void) {
    values[3] += p.x;
    p.y *= 2;
    cursor++;
    *cursor += 100;
  }
  touch();
  return values[0] + values[1] + values[2] + values[3] + p.x + p.y;
}

static int sum_vla(int n) {
  int data[n];
  for (int i = 0; i < n; i++)
    data[i] = i * i;
  int total(void) {
    int sum = 0;
    for (int i = 0; i < n; i++)
      sum += data[i];
    return sum;
  }
  return total();
}

static int with_goto(int n) {
  int acc = 0;
  void add(int v) { acc += v; }
again:
  add(n);
  if (--n > 0)
    goto again;
  return acc;
}

static int compare_ints(const void *a, const void *b) {
  int less(int x, int y) { return x < y; }
  int left = *(const int *)a;
  int right = *(const int *)b;
  return less(right, left) - less(left, right);
}

static void sort_with_nested(void) {
  int values[5] = {4, 1, 5, 2, 3};
  int ascending(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
  }
  qsort(values, 5, sizeof(int), ascending);
  for (int i = 0; i < 5; i++)
    printf("%d ", values[i]);
  qsort(values, 5, sizeof(int), compare_ints);
  for (int i = 0; i < 5; i++)
    printf("%d ", values[i]);
  printf("\n");
}

static int shadowed(int x) {
  int first(void) { return x + 1; }
  {
    int first_twice(void) { return first() * 2; }
    x = first_twice();
  }
  {
    int first(void) { return x - 1; }
    return first();
  }
}

static int loop_bound(void) {
  int limit = 3;
  int steps = 0;
  void shrink(void) { limit--; }
  for (int i = 0; i < limit; i++) {
    steps++;
    shrink();
  }
  return steps * 10 + limit;
}

int main(void) {
  printf("%d\n", counter(1));
  printf("%d\n", grandparent(1));
  printf("%d %d\n", parity(4), parity(5));
  printf("%d\n", aggregates());
  printf("%d\n", sum_vla(5));
  printf("%d\n", with_goto(4));
  sort_with_nested();
  printf("%d\n", shadowed(4));
  printf("%d\n", loop_bound());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::qsort;
// CHECK-NEXT: fn counter(mut total: i32) -> i32 {
// CHECK-NEXT:     counter__bump(unsafe { &mut (*std::ptr::addr_of_mut!(total)) }, 2i32);
// CHECK-NEXT:     counter__bump(unsafe { &mut (*std::ptr::addr_of_mut!(total)) }, 3i32);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn counter__bump(__slate_capture_0: &mut i32, by: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__slate_capture_0 as *mut i32) = *(__slate_capture_0 as *mut i32) + by;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn grandparent(n: i32) -> i32 {
// CHECK-NEXT:     let mut base: i32 = n * 10;
// CHECK-NEXT:     grandparent__middle(unsafe { &(*std::ptr::addr_of_mut!(base)) }, 2i32)
// CHECK-NEXT:         + grandparent__middle(unsafe { &(*std::ptr::addr_of_mut!(base)) }, 3i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn grandparent__inner(__slate_capture_0: &i32, __slate_capture_1: &i32, j: i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(__slate_capture_0 as *const i32) })
// CHECK-NEXT:         + (unsafe { *(__slate_capture_1 as *const i32) }) * j
// CHECK-NEXT: }
// CHECK-NEXT: fn grandparent__middle(__slate_capture_0: &i32, k: i32) -> i32 {
// CHECK-NEXT:     let mut scale: i32 = k + 1;
// CHECK-NEXT:     grandparent__inner(
// CHECK-NEXT:         __slate_capture_0,
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(scale)) },
// CHECK-NEXT:         k,
// CHECK-NEXT:     )
// CHECK-NEXT:         + grandparent__inner(
// CHECK-NEXT:             __slate_capture_0,
// CHECK-NEXT:             unsafe { &(*std::ptr::addr_of_mut!(scale)) },
// CHECK-NEXT:             1i32,
// CHECK-NEXT:         )
// CHECK-NEXT: }
// CHECK-NEXT: fn parity(n: i32) -> i32 {
// CHECK-NEXT:     let mut calls: i32 = 0;
// CHECK-NEXT:     parity__is_even(unsafe { &mut (*std::ptr::addr_of_mut!(calls)) }, n) * 100 + calls
// CHECK-NEXT: }
// CHECK-NEXT: fn parity__is_even(__slate_capture_0: &mut i32, k: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__slate_capture_0 as *mut i32) = *(__slate_capture_0 as *mut i32) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if k == 0 { 1 } else { parity__is_odd(__slate_capture_0, k - 1) };
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: fn parity__is_odd(__slate_capture_0: &mut i32, k: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__slate_capture_0 as *mut i32) = *(__slate_capture_0 as *mut i32) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0]]: i32 = if k == 0 { 0 } else { parity__is_even(__slate_capture_0, k - 1) };
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: fn aggregates() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     let mut p: point = point { x: 5, y: 6 };
// CHECK-NEXT:     let mut cursor: *mut i32 = unsafe { (values.0.as_mut_ptr() as *mut i32).add(1) };
// CHECK-NEXT:     aggregates__touch(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(values.0)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(cursor)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     values.0[0] + values.0[1] + values.0[2] + values.0[3] + p.x + p.y
// CHECK-NEXT: }
// CHECK-NEXT: fn aggregates__touch(
// CHECK-NEXT:     __slate_capture_0: &mut [i32; 4],
// CHECK-NEXT:     __slate_capture_1: &mut point,
// CHECK-NEXT:     __slate_capture_2: &mut *mut i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let [[V_0]]: *mut i32 = unsafe {
// CHECK-NEXT:         ((*(__slate_capture_0 as *mut [i32; 4])).as_mut_ptr() as *mut i32).add(3)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = *[[V_0]] + (*(__slate_capture_1 as *mut point)).x;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(__slate_capture_1 as *mut point)).y = (*(__slate_capture_1 as *mut point)).y
// CHECK-NEXT:             * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = unsafe { *(__slate_capture_2 as *mut *mut i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__slate_capture_2 as *mut *mut i32) = [[V_1]].add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: *mut i32 = unsafe { *(__slate_capture_2 as *mut *mut i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_2]] = *[[V_2]] + 100;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_vla(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = (n as i64) as u64;
// CHECK-NEXT:     let mut data: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_3]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_3]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(data.as_mut_ptr() as *mut i32).offset(i as isize) = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum_vla__total(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(n)) },
// CHECK-NEXT:         data.as_mut_ptr() as *mut [i32; 0],
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_vla__total(__slate_capture_4: &i32, __slate_capture_5: *mut [i32; 0]) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < unsafe { *(__slate_capture_4 as *const i32) } {
// CHECK-NEXT:         sum += unsafe { *(__slate_capture_5 as *mut i32).offset(i as isize) };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn with_goto(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut acc: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         acc = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             with_goto__add(&mut (*std::ptr::addr_of_mut!(acc)), n);
// CHECK-NEXT:             let [[V_3]]: i32 = n - 1;
// CHECK-NEXT:             n = [[V_3]];
// CHECK-NEXT:             if [[V_3]] <= 0 {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return acc;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn with_goto__add(__slate_capture_2: &mut i32, v: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__slate_capture_2 as *mut i32) = *(__slate_capture_2 as *mut i32) + v;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn compare_ints(a: *const (), b: *const ()) -> i32 {
// CHECK-NEXT:     let left: i32 = unsafe { *(a as *const i32) };
// CHECK-NEXT:     let right: i32 = unsafe { *(b as *const i32) };
// CHECK-NEXT:     compare_ints__less(right, left) - compare_ints__less(left, right)
// CHECK-NEXT: }
// CHECK-NEXT: fn compare_ints__less(x: i32, y: i32) -> i32 {
// CHECK-NEXT:     (x < y) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn sort_with_nested() {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         4i32, 1i32, 5i32, 2i32, 3i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         qsort(
// CHECK-NEXT:             (values.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             5u64 as usize,
// CHECK-NEXT:             (std::mem::size_of::<i32>() as u64) as usize,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:                     Option<
// CHECK-NEXT:                         unsafe extern "C" fn(
// CHECK-NEXT:                             *const core::ffi::c_void,
// CHECK-NEXT:                             *const core::ffi::c_void,
// CHECK-NEXT:                         ) -> i32,
// CHECK-NEXT:                     >,
// CHECK-NEXT:                 >(Some(sort_with_nested__ascending))
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..5i32 {
// CHECK-NEXT:         print!("{} ", values.0[i as usize]);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         qsort(
// CHECK-NEXT:             (values.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             5u64 as usize,
// CHECK-NEXT:             (std::mem::size_of::<i32>() as u64) as usize,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:                     Option<
// CHECK-NEXT:                         unsafe extern "C" fn(
// CHECK-NEXT:                             *const core::ffi::c_void,
// CHECK-NEXT:                             *const core::ffi::c_void,
// CHECK-NEXT:                         ) -> i32,
// CHECK-NEXT:                     >,
// CHECK-NEXT:                 >(Some(compare_ints))
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..5i32 {
// CHECK-NEXT:         print!("{} ", values.0[i as usize]);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn sort_with_nested__ascending(a: *const (), b: *const ()) -> i32 {
// CHECK-NEXT:     (unsafe { *(a as *const i32) }) - unsafe { *(b as *const i32) }
// CHECK-NEXT: }
// CHECK-NEXT: fn shadowed(mut x: i32) -> i32 {
// CHECK-NEXT:     x = shadowed__first_twice(unsafe { &(*std::ptr::addr_of_mut!(x)) });
// CHECK-NEXT:     shadowed__first_2(unsafe { &(*std::ptr::addr_of_mut!(x)) })
// CHECK-NEXT: }
// CHECK-NEXT: fn shadowed__first(__slate_capture_0: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(__slate_capture_0 as *const i32) }) + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn shadowed__first_twice(__slate_capture_0: &i32) -> i32 {
// CHECK-NEXT:     shadowed__first(__slate_capture_0) * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn shadowed__first_2(__slate_capture_0: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(__slate_capture_0 as *const i32) }) - 1
// CHECK-NEXT: }
// CHECK-NEXT: fn loop_bound() -> i32 {
// CHECK-NEXT:     let mut limit: i32 = 3;
// CHECK-NEXT:     let mut steps: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < limit {
// CHECK-NEXT:         steps += 1;
// CHECK-NEXT:         loop_bound__shrink(unsafe { &mut (*std::ptr::addr_of_mut!(limit)) });
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     steps * 10 + limit
// CHECK-NEXT: }
// CHECK-NEXT: fn loop_bound__shrink(__slate_capture_0: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__slate_capture_0 as *mut i32) = *(__slate_capture_0 as *mut i32) - 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", counter(1i32));
// CHECK-NEXT:     println!("{}", grandparent(1i32));
// CHECK-NEXT:     println!("{} {}", parity(4i32), parity(5i32));
// CHECK-NEXT:     println!("{}", aggregates());
// CHECK-NEXT:     println!("{}", sum_vla(5i32));
// CHECK-NEXT:     println!("{}", with_goto(4i32));
// CHECK-NEXT:     sort_with_nested();
// CHECK-NEXT:     println!("{}", shadowed(4i32));
// CHECK-NEXT:     println!("{}", loop_bound());
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
