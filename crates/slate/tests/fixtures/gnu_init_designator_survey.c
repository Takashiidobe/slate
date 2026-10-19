#include <stdio.h>

union FlexUnion {
  int  value;
  char data[];
};

struct OnlyFlex {
  char data[];
};

union Castable {
  int   i;
  float f;
};

enum Forward;
enum Forward { FORWARD_A, FORWARD_B };

struct Point {
  int x;
  int y;
};

struct Sized {
  int n;
  int data[];
};

static struct Sized sized = {3, {10, 20, 30}};

struct NestedOuter {
  struct Sized inner;
};

int main(void) {
  int            range_values[10] = {[2 ... 5] = 9};
  int            old_index[3]     = {[1] 11};
  struct Point   p                = {x : 1, y : 2};
  int            five             = 5;
  union Castable c                = (union Castable)five;
  enum Forward   f                = FORWARD_B;

  printf("%d %d %d %d\n", range_values[3], old_index[1], p.x + p.y, c.i);
  printf("%d\n", (int)f);
  printf("%zu %zu\n", sizeof(union FlexUnion), sizeof(struct OnlyFlex));
  printf("%d %d %d\n", sized.data[0], sized.data[1], sized.data[2]);
  printf("%zu\n", sizeof(struct NestedOuter));
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
// CHECK-NEXT: union Castable {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Sized {
// CHECK-NEXT:     n: i32,
// CHECK-NEXT:     data: [i32; 0],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateStorage_sized {
// CHECK-NEXT:     n: i32,
// CHECK-NEXT:     data: [i32; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut sized: __SlateStorage_sized = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: __SlateStorage_sized = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].n = 3i32;
// CHECK-NEXT:     [[T_0]].data = [10i32, 20i32, 30i32];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let range_values: __SlateAlign16<[i32; 10]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [i32; 10] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = 9i32;
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: usize = 2;
// CHECK-NEXT:         while [[T_2]] <= 5 {
// CHECK-NEXT:             [[T_0]][[[T_2]]] = [[T_1]];
// CHECK-NEXT:             [[T_2]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let old_index: [i32; 3] = {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [i32; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]][1] = 11i32;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let p: Point = Point { x: 1, y: 2 };
// CHECK-NEXT:     let five: i32 = 5;
// CHECK-NEXT:     let c: Castable = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: Castable = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]].i = five;
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let f: u32 = 1;
// CHECK-NEXT:     println!("{} {} {} {}", range_values.0[3], old_index[1], p.x + p.y, unsafe { c.i });
// CHECK-NEXT:     println!("{}", (f as u32) as i32);
// CHECK-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 4u64, 0u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe { * (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(sized)
// CHECK-NEXT:         as * mut Sized)).data) as * mut i32).offset(0 as isize) }, unsafe { *
// CHECK-NEXT:         (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(sized) as * mut Sized)).data)
// CHECK-NEXT:         as * mut i32).offset(1 as isize) }, unsafe { * (std::ptr::addr_of_mut!((*
// CHECK-NEXT:         (std::ptr::addr_of_mut!(sized) as * mut Sized)).data) as * mut i32).offset(2 as
// CHECK-NEXT:         isize) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), 4u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
