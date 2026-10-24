#include <stdio.h>

struct node {
  int          value;
  struct node *next;
};

unsigned int flags[1] = {2};
int          calls    = 0;

unsigned int mark(void) {
  flags[0] |= 128;
  return 1;
}

int *advance(int *p) {
  calls++;
  return p + 1;
}

int main(void) {
  struct node self = {7, &self};
  struct node ring[2] = {{1, &ring[1]}, {2, &ring[0]}};
  printf("%d %d %d\n", self.next == &self, ring[0].next->value, ring[1].next->value);

  flags[0] |= mark();
  printf("%u\n", flags[0]);

  int  data[4] = {0, 1, 2, 3};
  int *p       = data;
  int  i       = 0;
  __builtin_prefetch(advance(p), 0, 0);
  __builtin_prefetch(&data[i++], 1, 3);
  __builtin_prefetch(++p);
  printf("%d %d %d\n", calls, i, *p);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_prefetch([[V_0:__v[0-9]+]]: *const (), ...);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct node {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     next: *mut node,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut flags: [u32; 1] = [2];
// CHECK-NEXT: static mut calls: i32 = 0;
// CHECK-NEXT: fn mark() -> u32 {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut u32 = unsafe { (std::ptr::addr_of_mut!(flags) as *mut u32).add(0) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = *[[V_1]] | 128;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn advance(p: *mut i32) -> *mut i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { p.add(1) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut self_: node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     self_ = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 7i32;
// CHECK-NEXT:         [[T_0]].next = std::ptr::addr_of_mut!(self_);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut ring: __SlateAlign16<[node; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     ring.0 = [
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].value = 1i32;
// CHECK-NEXT:             [[T_1]].next = unsafe { (ring.0.as_mut_ptr() as *mut node).add(1) };
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: node = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].value = 2i32;
// CHECK-NEXT:             [[T_2]].next = unsafe { (ring.0.as_mut_ptr() as *mut node).add(0) };
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", (self_.next == std::ptr::addr_of_mut!(self_)) as i32, unsafe { (*
// CHECK-NEXT:         ring.0[0].next).value }, unsafe { (* ring.0[1].next).value }
// CHECK-NEXT:     );
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u32 = mark();
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: *mut u32 = unsafe { (std::ptr::addr_of_mut!(flags) as *mut u32).add(0) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_3]] = *[[V_3]] | [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), flags[0]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut data: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32, 1i32, 2i32, 3i32]);
// CHECK-NEXT:     let mut p: *mut i32 = data.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         let _ = advance(p) as *const ();
// CHECK-NEXT:     };
// CHECK-NEXT:     i += 1;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     p = unsafe { [[V_4]].add(1) };
// CHECK-NEXT:     println!("{} {} {}", unsafe { calls }, i, unsafe { * p });
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
