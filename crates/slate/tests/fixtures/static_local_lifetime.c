#include <stdio.h>

struct Point {
  int x;
  int y;
};

static int global_base = 100;

static int counter(void) {
  static int count;
  return ++count;
}

static int other_counter(void) {
  static int count = 50;
  return count++;
}

static int *history(int value) {
  static int slots[4];
  static int next;
  slots[next++ % 4] = value;
  return slots;
}

static struct Point *moved(int dx) {
  static struct Point point = {1, 2};
  point.x += dx;
  point.y *= 2;
  return &point;
}

static int *base_pointer(void) {
  static int *pointer = &global_base;
  return pointer;
}

static int depth(int n) {
  static int deepest;
  if (n > deepest)
    deepest = n;
  return n == 0 ? deepest : depth(n - 1);
}

static double running_mean(double sample) {
  static double sum;
  static int samples;
  sum += sample;
  return sum / ++samples;
}

static const char *name_of(int index) {
  static const char *const names[] = {"zero", "one", "two"};
  return names[index];
}

static int loop_scoped(void) {
  int total = 0;
  for (int i = 0; i < 3; i++) {
    static int seen = 7;
    total += seen++;
  }
  return total;
}

static int tally(void) {
  static int counter = 100;
  return counter++;
}

int main(void) {
  int c1 = counter();
  int c2 = counter();
  int o1 = other_counter();
  int o2 = other_counter();
  int c3 = counter();
  printf("%d %d %d %d %d\n", c1, c2, o1, o2, c3);
  int t1 = tally();
  int t2 = tally();
  printf("%d %d %d\n", t1, t2, counter());

  for (int i = 1; i <= 6; i++)
    history(i * 10);
  int *slots = history(70);
  printf("%d %d %d %d\n", slots[0], slots[1], slots[2], slots[3]);

  moved(3);
  struct Point *point = moved(4);
  printf("%d %d\n", point->x, point->y);
  point->x = 0;
  printf("%d\n", moved(1)->x);

  *base_pointer() += 5;
  int through = *base_pointer();
  printf("%d %d\n", global_base, through);

  int deep1 = depth(4);
  int deep2 = depth(2);
  printf("%d %d\n", deep1, deep2);
  running_mean(1.0);
  running_mean(2.0);
  printf("%g\n", running_mean(6.0));
  printf("%s %s\n", name_of(2), name_of(0));
  int first  = loop_scoped();
  int second = loop_scoped();
  printf("%d %d\n", first, second);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut global_base: i32 = 100 as i32;
// CHECK-NEXT: fn counter() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (unsafe { [[COUNT_0:count_[0-9]+]] }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[COUNT_0]] = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[COUNT_0]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn other_counter() -> i32 {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe { [[COUNT_1:count_[0-9]+]] };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[COUNT_1]] = [[V_1]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[COUNT_1]]: i32 = 50 as i32;
// CHECK-NEXT: fn history(value: i32) -> *mut i32 {
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = unsafe { next };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         next = [[V_2]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slots.0[([[V_2]] % 4) as usize] = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(slots.0) as *mut i32 }
// CHECK-NEXT: }
// CHECK-NEXT: static mut slots: __SlateAlign16<[i32; 4]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut next: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn moved(dx: i32) -> *mut Point {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         point.x = point.x + dx;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         point.y = point.y * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(point) }
// CHECK-NEXT: }
// CHECK-NEXT: static mut point: Point = Point { x: 1 as i32, y: 2 as i32 };
// CHECK-NEXT: fn base_pointer() -> *mut i32 {
// CHECK-NEXT:     unsafe { pointer }
// CHECK-NEXT: }
// CHECK-NEXT: static mut pointer: *mut i32 = unsafe { std::ptr::addr_of_mut!(global_base) };
// CHECK-NEXT: fn depth(n: i32) -> i32 {
// CHECK-NEXT:     if n > unsafe { deepest } {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             deepest = n;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32;
// CHECK-NEXT:     if n == 0 {
// CHECK-NEXT:         [[V_3]] = unsafe { deepest };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = depth(n - 1);
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_3]]
// CHECK-NEXT: }
// CHECK-NEXT: static mut deepest: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn running_mean(sample: f64) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sum = sum + sample;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = (unsafe { samples }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         samples = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { sum }) / ([[V_4]] as f64)
// CHECK-NEXT: }
// CHECK-NEXT: static mut sum: f64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut samples: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn name_of(index: i32) -> *const i8 {
// CHECK-NEXT:     unsafe { names.0[index as usize] }
// CHECK-NEXT: }
// CHECK-NEXT: static mut names: __SlateAlign16<[*const i8; 3]> = __SlateAlign16([
// CHECK-NEXT:     (b"zero\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"one\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"two\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn loop_scoped() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = unsafe { seen };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             seen = [[V_5]] + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: i32 = [[V_5]];
// CHECK-NEXT:         total += [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: static mut seen: i32 = 7 as i32;
// CHECK-NEXT: fn tally() -> i32 {
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = unsafe { [[COUNTER_0:counter_[0-9]+]] };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[COUNTER_0]] = [[V_7]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_7]]
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[COUNTER_0]]: i32 = 100 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let c1: i32 = counter();
// CHECK-NEXT:     let c2: i32 = counter();
// CHECK-NEXT:     let o1: i32 = other_counter();
// CHECK-NEXT:     let o2: i32 = other_counter();
// CHECK-NEXT:     let c3: i32 = counter();
// CHECK-NEXT:     println!("{} {} {} {} {}", c1, c2, o1, o2, c3);
// CHECK-NEXT:     let t1: i32 = tally();
// CHECK-NEXT:     let t2: i32 = tally();
// CHECK-NEXT:     println!("{} {} {}", t1, t2, counter());
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     while i <= 6 {
// CHECK-NEXT:         history(i * 10);
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[SLOTS_0:slots_[0-9]+]]: *mut i32 = history(70 as i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { * [[SLOTS_0]].offset(0 as isize) }, unsafe { * [[SLOTS_0]]
// CHECK-NEXT:         .offset(1 as isize) }, unsafe { * [[SLOTS_0]].offset(2 as isize) }, unsafe { *
// CHECK-NEXT:         [[SLOTS_0]].offset(3 as isize) }
// CHECK-NEXT:     );
// CHECK-NEXT:     moved(3 as i32);
// CHECK-NEXT:     let [[POINT_0:point_[0-9]+]]: *mut Point = moved(4 as i32);
// CHECK-NEXT:     println!("{} {}", unsafe { (* [[POINT_0]]).x }, unsafe { (* [[POINT_0]]).y });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[POINT_0]]).x = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { (* moved(1 as i32)).x });
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: *mut i32 = base_pointer();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_8]] = *[[V_8]] + 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     let through: i32 = unsafe { *base_pointer() };
// CHECK-NEXT:     println!("{} {}", unsafe { global_base }, through);
// CHECK-NEXT:     let deep1: i32 = depth(4 as i32);
// CHECK-NEXT:     let deep2: i32 = depth(2 as i32);
// CHECK-NEXT:     println!("{} {}", deep1, deep2);
// CHECK-NEXT:     running_mean(1.0f64);
// CHECK-NEXT:     running_mean(2.0f64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g\n".as_ptr(), running_mean(6.0f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %s\n".as_ptr(), name_of(2 as i32), name_of(0 as i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let first: i32 = loop_scoped();
// CHECK-NEXT:     let second: i32 = loop_scoped();
// CHECK-NEXT:     println!("{} {}", first, second);
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
