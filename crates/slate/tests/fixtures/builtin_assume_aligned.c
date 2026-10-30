#include <stddef.h>
#include <stdio.h>

static void *first_word(void *p) {
  void *q = __builtin_assume_aligned(p, 32);
  return q;
}

static void *first_word_offset(void *p, size_t off) {
  void *q = __builtin_assume_aligned(p, 32, off);
  return q;
}

int main(void) {
  _Alignas(32) static unsigned char buf[64];
  void                             *a = first_word(buf);
  void                             *b = first_word_offset(buf + 8, 8);
  printf("%d %d\n", a == buf, b == buf + 8);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_assume_aligned([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: u64, ...) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: fn first_word(p: *mut ()) -> *mut () {
// CHECK-NEXT:     let q: *mut () = p as *mut ();
// CHECK-NEXT:     q
// CHECK-NEXT: }
// CHECK-NEXT: fn first_word_offset(p: *mut (), off: u64) -> *mut () {
// CHECK-NEXT:     let q: *mut () = p as *mut ();
// CHECK-NEXT:     q
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: *mut () = first_word(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut u8 }) as *mut (),
// CHECK-NEXT:     );
// CHECK-NEXT:     let b: *mut () = first_word_offset(
// CHECK-NEXT:         (unsafe { (std::ptr::addr_of_mut!(buf.0) as *mut u8).add(8) }) as *mut (),
// CHECK-NEXT:         8u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", (a == ((unsafe { std::ptr::addr_of_mut!(buf.0) as * mut u8 }) as * mut
// CHECK-NEXT:         ())) as i32, (b == ((unsafe { (std::ptr::addr_of_mut!(buf.0) as * mut u8).add(8)
// CHECK-NEXT:         }) as * mut ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut buf: __SlateAlign32<[u8; 64]> = __SlateAlign32(unsafe { std::mem::zeroed() });
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
