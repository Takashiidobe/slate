#include <stdio.h>

static int bump(int *n, int x) {
  *n = *n + 1;
  return x;
}

static int safe_ratio(int a, int b) { return b != 0 && a / b > 1; }

static int either_zero(int a, int b) { return a == 0 || b == 0 || 100 / a / b > 3; }

static int mixed(int a, int b, int c) { return (a || b) && c; }

static int grouped(int a, int b, int c) { return a && (b || c); }

static int negated(int a, int b) { return !(a && b) || !a; }

int main(void) {
  int n = 0, b = 0, i = 0;
  int r1 = n > 5 && bump(&n, 1);
  int r2 = n < 5 || bump(&n, 1);
  int r3 = bump(&n, 0) && (b = bump(&n, 7));
  int r4 = bump(&n, 0) || (b = bump(&n, 9));
  int r5 = (i++ > 0) && (i++ > 0);
  printf("%d %d %d %d %d n=%d b=%d i=%d\n", r1, r2, r3, r4, r5, n, b, i);

  if (bump(&n, 1) && bump(&n, 2)) printf("both n=%d\n", n);
  if (bump(&n, 0) || bump(&n, 0)) printf("unreachable\n");
  printf("n=%d\n", n);

  while (i < 10 && bump(&n, 1)) i++;
  printf("i=%d n=%d\n", i, n);

  printf("%d %d %d\n", safe_ratio(9, 0), safe_ratio(9, 2), safe_ratio(3, 2));
  printf("%d %d %d\n", either_zero(0, 5), either_zero(5, 0), either_zero(2, 3));
  printf("%d %d %d %d\n", mixed(0, 0, 1), mixed(0, 1, 1), mixed(1, 0, 0), mixed(1, 1, 1));
  printf("%d %d %d\n", grouped(0, 1, 1), grouped(1, 0, 1), grouped(1, 0, 0));
  printf("%d %d %d\n", negated(1, 1), negated(0, 1), negated(1, 0));
  return safe_ratio(4, 1) + mixed(1, 0, 1);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn bump(n: &mut i32, x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(n as *mut i32) = *(n as *mut i32) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn safe_ratio(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     (b != 0 && a / b > 1) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn either_zero(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     (a == 0 || b == 0 || 100 / a / b > 3) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn mixed(a: i32, b: i32, c: i32) -> i32 {
// CHECK-NEXT:     ((a != 0 || b != 0) && c != 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn grouped(a: i32, b: i32, c: i32) -> i32 {
// CHECK-NEXT:     (a != 0 && (b != 0 || c != 0)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn negated(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     (!(a != 0 && b != 0) || !(a != 0)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut n: i32 = 0;
// CHECK-NEXT:     let mut b: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     let mut r1: i32 = 0;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if n > 5 {
// CHECK-NEXT:         [[V_0]] = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 1i32) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     r1 = [[V_0]] as i32;
// CHECK-NEXT:     let mut r2: i32 = 0;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if n < 5 {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 1i32) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     r2 = [[V_1]] as i32;
// CHECK-NEXT:     let mut r3: i32 = 0;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 0i32) != 0 {
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: i32 = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 7i32);
// CHECK-NEXT:         b = [[V_3]];
// CHECK-NEXT:         [[V_2]] = [[V_3]] != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     r3 = [[V_2]] as i32;
// CHECK-NEXT:     let mut r4: i32 = 0;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 0i32) != 0 {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 9i32);
// CHECK-NEXT:         b = [[V_5]];
// CHECK-NEXT:         [[V_4]] = [[V_5]] != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     r4 = [[V_4]] as i32;
// CHECK-NEXT:     let mut r5: i32 = 0;
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     i = [[V_6]] + 1;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] > 0 {
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         i = [[V_8]] + 1;
// CHECK-NEXT:         [[V_7]] = [[V_8]] > 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     r5 = [[V_7]] as i32;
// CHECK-NEXT:     println!("{} {} {} {} {} n={} b={} i={}", r1, r2, r3, r4, r5, n, b, i);
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 1i32) != 0 {
// CHECK-NEXT:         [[V_9]] = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 2i32) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_9]] {
// CHECK-NEXT:         println!("both n={}", n);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 0i32) != 0 {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 0i32) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         println!("unreachable");
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("n={}", n);
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: bool;
// CHECK-NEXT:         if i < 10 {
// CHECK-NEXT:             [[V_11]] = bump(unsafe { &mut (*std::ptr::addr_of_mut!(n)) }, 1i32) != 0;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_11]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         if ![[V_11]] {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("i={} n={}", i, n);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", safe_ratio(9i32, 0i32), safe_ratio(9i32, 2i32), safe_ratio(3i32,
// CHECK-NEXT:         2i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", either_zero(0i32, 5i32), either_zero(5i32, 0i32), either_zero(2i32,
// CHECK-NEXT:         3i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", mixed(0i32, 0i32, 1i32), mixed(0i32, 1i32, 1i32), mixed(1i32,
// CHECK-NEXT:         0i32, 0i32), mixed(1i32, 1i32, 1i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", grouped(0i32, 1i32, 1i32), grouped(1i32, 0i32, 1i32), grouped(1i32,
// CHECK-NEXT:         0i32, 0i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", negated(1i32, 1i32), negated(0i32, 1i32), negated(1i32, 0i32));
// CHECK-NEXT:     safe_ratio(4i32, 1i32) + mixed(1i32, 0i32, 1i32)
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
