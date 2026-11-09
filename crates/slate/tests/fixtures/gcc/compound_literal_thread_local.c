// { dg-options "-std=c23" }
#include <stdio.h>
#include <threads.h>

struct Point {
  int x;
  int y;
};

static int *counter(void) { return &(static thread_local int){100}; }

static struct Point *point(void) {
  return &(static thread_local struct Point){1, 2};
}

static int *pair_a(void) { return (static thread_local int[2]){10, 20}; }
static int *pair_b(void) { return (static thread_local int[2]){10, 20}; }

static int step(int bump) {
  int *c = counter();
  *c += bump;
  struct Point *p = point();
  p->x += bump;
  p->y *= 2;
  pair_a()[0] += bump;
  pair_b()[1] -= bump;
  return *c * 10000 + p->x * 100 + p->y + pair_a()[0] + pair_b()[1];
}

static int worker(void *argument) {
  int bump = *(int *)argument;
  int first = step(bump);
  int second = step(bump);
  return (second - first) + (counter() != counter()) +
         (pair_a() == pair_b());
}

int main(void) {
  int a = step(1);
  int b = step(1);
  thrd_t thread;
  int    bump          = 3;
  int    worker_result = -1;
  int    created       = thrd_create(&thread, worker, &bump);
  int joined = created == thrd_success ? thrd_join(thread, &worker_result) : -1;
  int c = step(1);
  printf("%d %d %d %d %d %d\n", a, b, c, worker_result, created, joined);
  printf("%d %d %d %d\n", *counter(), point()->x, point()->y,
         pair_a()[0] + pair_b()[1]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(thread_local)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=c23" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn thrd_create(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *mut u64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: Option<unsafe extern "C-unwind" fn(*mut ()) -> i32>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn thrd_join([[ARG_0]]: u64, [[ARG_1]]: *mut i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn counter() -> *mut i32 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(__slate_compound_counter_0) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn point() -> *mut Point {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(__slate_compound_point_0) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pair_a() -> *mut i32 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(__slate_compound_pair_a_0) as *mut i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pair_b() -> *mut i32 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(__slate_compound_pair_b_0) as *mut i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn step(bump: i32) -> i32 {
// CHECK-NEXT:     let c: *mut i32 = counter();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *c = *c + bump;
// CHECK-NEXT:     }
// CHECK-NEXT:     let p: *mut Point = point();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).x = (*p).x + bump;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).y = (*p).y * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { pair_a().add(0) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = *[[V_0]] + bump;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = unsafe { pair_b().add(1) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = *[[V_1]] - bump;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *c }) * 10000
// CHECK-NEXT:         + (unsafe { (*p).x }) * 100
// CHECK-NEXT:         + unsafe { (*p).y }
// CHECK-NEXT:         + unsafe { *pair_a().add(0) }
// CHECK-NEXT:         + unsafe { *pair_b().add(1) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn worker(argument: *mut ()) -> i32 {
// CHECK-NEXT:     let bump: i32 = unsafe { *(argument as *mut i32) };
// CHECK-NEXT:     let first: i32 = step(bump);
// CHECK-NEXT:     let second: i32 = step(bump);
// CHECK-NEXT:     second - first + ((counter() != counter()) as i32) + ((pair_a() == pair_b()) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = step(1i32);
// CHECK-NEXT:     let b: i32 = step(1i32);
// CHECK-NEXT:     let mut thread: u64 = 0;
// CHECK-NEXT:     let mut bump: i32 = 3;
// CHECK-NEXT:     let mut worker_result: i32 = -1;
// CHECK-NEXT:     let created: i32 = unsafe {
// CHECK-NEXT:         thrd_create(
// CHECK-NEXT:             std::ptr::addr_of_mut!(thread),
// CHECK-NEXT:             Some(worker),
// CHECK-NEXT:             std::ptr::addr_of_mut!(bump) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut joined: i32 = 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = if created == 0 {
// CHECK-NEXT:         unsafe { thrd_join(thread, std::ptr::addr_of_mut!(worker_result)) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         -1
// CHECK-NEXT:     };
// CHECK-NEXT:     joined = [[V_2]];
// CHECK-NEXT:     let c: i32 = step(1i32);
// CHECK-NEXT:     println!("{} {} {} {} {} {}", a, b, c, worker_result, created, joined);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}",
// CHECK-NEXT:         unsafe { *counter() },
// CHECK-NEXT:         unsafe { (*point()).x },
// CHECK-NEXT:         unsafe { (*point()).y },
// CHECK-NEXT:         (unsafe { *pair_a().add(0) }) + unsafe { *pair_b().add(1) }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut __slate_compound_counter_0: i32 = 100;
// CHECK-EMPTY:
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut __slate_compound_point_0: Point = Point { x: 1i32, y: 2i32 };
// CHECK-EMPTY:
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut __slate_compound_pair_a_0: [i32; 2] = [10, 20];
// CHECK-EMPTY:
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut __slate_compound_pair_b_0: [i32; 2] = [10, 20];
// CHECK-EMPTY:
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
