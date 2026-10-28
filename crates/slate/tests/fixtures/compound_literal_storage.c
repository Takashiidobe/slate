#include <stdio.h>

struct Point {
  int x;
  int y;
};

struct Pair {
  struct Point *first;
  int          *values;
};

static struct Point *origin = &(struct Point){3, 4};
static int          *primes = (int[]){2, 3, 5, 7};
static struct Pair   pair   = {&(struct Point){5, 6}, (int[]){8, 9}};

static int sum(const int *values, int count) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += values[i];
  return total;
}

int main(void) {
  origin->x += 10;
  primes[3] = 11;
  printf("%d %d %d %d\n", origin->x, origin->y, primes[3], sum(primes, 4));
  printf("%d %d %d\n", pair.first->x, pair.first->y, pair.values[1]);

  for (int i = 0; i < 3; i++) {
    int *counter = &(int){i * 10};
    *counter += 1;
    printf("%d ", *counter);
  }
  printf("\n");

  struct Point copied = (struct Point){.y = 7};
  int          total  = sum((int[]){1, 2, 3, copied.y}, 4);
  printf("%d %d %d\n", copied.x, copied.y, total);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     first: *mut Point,
// CHECK-NEXT:     values: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut origin: *mut Point = unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]) };
// CHECK-NEXT: static mut primes: *mut i32 = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!([[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]) as *mut i32
// CHECK-NEXT: };
// CHECK-NEXT: static mut pair: Pair = Pair {
// CHECK-NEXT:     first: unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]) },
// CHECK-NEXT:     values: unsafe { std::ptr::addr_of_mut!([[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]) as *mut i32 },
// CHECK-NEXT: };
// CHECK-NEXT: fn sum(values: *const i32, count: i32) -> i32 {
// CHECK-NEXT:     let __values_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts(values as *const i32, count as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         total += unsafe { __values_view[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: Point = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_6:__slate_compound_[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut Point = unsafe { origin };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_0]]).x = (*[[V_0]]).x + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { primes }.add(3) = 11;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { (* origin).x }, unsafe { (* origin).y }, unsafe { *
// CHECK-NEXT:         unsafe { primes } .add(3) }, sum((unsafe { primes }) as * const i32, 4i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe { (* pair.first).x }, unsafe { (* pair.first).y }, unsafe { *
// CHECK-NEXT:         unsafe { pair.values } .add(1) }
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..3i32 {
// CHECK-NEXT:         let counter: *mut i32 = unsafe {
// CHECK-NEXT:             std::ptr::addr_of_mut!(
// CHECK-NEXT:                 * { [[SLATE_COMPOUND_4]] = i * 10;
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]]) }
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *counter = *counter + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         print!("{} ", unsafe { * counter });
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     let copied: Point = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_5]] = {
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: Point = std::mem::zeroed();
// CHECK-NEXT:                 [[T_0]].y = 7i32;
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_5]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let total: i32 = sum(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             (*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_6]] = [1i32, 2i32, 3i32, copied.y];
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_6]])
// CHECK-NEXT:             })
// CHECK-NEXT:                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:         }) as *const i32,
// CHECK-NEXT:         4i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {} {}", copied.x, copied.y, total);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[SLATE_COMPOUND_0]]: Point = Point { x: 3i32, y: 4i32 };
// CHECK-NEXT: static mut [[SLATE_COMPOUND_1]]: [i32; 4] = [2, 3, 5, 7];
// CHECK-NEXT: static mut [[SLATE_COMPOUND_2]]: Point = Point { x: 5i32, y: 6i32 };
// CHECK-NEXT: static mut [[SLATE_COMPOUND_3]]: [i32; 2] = [8, 9];
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
