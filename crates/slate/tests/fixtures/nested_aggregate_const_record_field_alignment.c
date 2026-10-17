#include <stdio.h>

typedef int (*fnptr)(int);

int add1(int x) { return x + 1; }
int add2(int x) { return x + 2; }
int add3(int x) { return x + 3; }
int mul5(int x) { return x * 5; }
int mul7(int x) { return x * 7; }

struct Inner {
  fnptr scanners[3];
  int   tag;
};

struct Outer {
  struct Inner  inner;
  unsigned char bytes[8];
  fnptr         trailing1;
  fnptr         trailing2;
  int           trailing3;
};

struct Outer g = {
    {{add1, add2, add3}, 42}, {1, 2, 3, 4, 5, 6, 7, 8}, mul5, mul7, 99,
};

int main(void) {
  printf("%d %d %d %d\n", g.inner.scanners[0](10), g.inner.scanners[1](10),
         g.inner.scanners[2](10), g.inner.tag);
  for (int i = 0; i < 8; i++) {
    printf("%d ", g.bytes[i]);
  }
  printf("\n");
  printf("%d %d %d\n", g.trailing1(10), g.trailing2(10), g.trailing3);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add1")]
// CHECK-NEXT: extern "C-unwind" fn add1(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add2")]
// CHECK-NEXT: extern "C-unwind" fn add2(x: i32) -> i32 {
// CHECK-NEXT:     x + 2
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add3")]
// CHECK-NEXT: extern "C-unwind" fn add3(x: i32) -> i32 {
// CHECK-NEXT:     x + 3
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.mul5")]
// CHECK-NEXT: extern "C-unwind" fn mul5(x: i32) -> i32 {
// CHECK-NEXT:     x * 5
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.mul7")]
// CHECK-NEXT: extern "C-unwind" fn mul7(x: i32) -> i32 {
// CHECK-NEXT:     x * 7
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Inner {
// CHECK-NEXT:     scanners: [Option<unsafe extern "C-unwind" fn(i32) -> i32>; 3],
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Outer {
// CHECK-NEXT:     inner: Inner,
// CHECK-NEXT:     bytes: [u8; 8],
// CHECK-NEXT:     trailing1: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     trailing2: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     trailing3: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut g: Outer = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: Outer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].inner = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: Inner = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].scanners = [Some(add1), Some(add2), Some(add3)];
// CHECK-NEXT:         [[T_1]].tag = 42 as i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]].bytes = [
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]].trailing1 = Some(mul5);
// CHECK-NEXT:     [[T_0]].trailing2 = Some(mul7);
// CHECK-NEXT:     [[T_0]].trailing3 = 99 as i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { unsafe { g.inner.scanners[0] } .unwrap() (10 as i32) },
// CHECK-NEXT:         unsafe { unsafe { g.inner.scanners[1] } .unwrap() (10 as i32) }, unsafe { unsafe
// CHECK-NEXT:         { g.inner.scanners[2] } .unwrap() (10 as i32) }, unsafe { g.inner.tag }
// CHECK-NEXT:     );
// CHECK-NEXT:     for i in 0 as i32..8 as i32 {
// CHECK-NEXT:         print!("{} ", ((unsafe { g.bytes[i as usize] }) as u32) as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe { unsafe { g.trailing1 } .unwrap() (10 as i32) }, unsafe {
// CHECK-NEXT:         unsafe { g.trailing2 } .unwrap() (10 as i32) }, unsafe { g.trailing3 }
// CHECK-NEXT:     );
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
