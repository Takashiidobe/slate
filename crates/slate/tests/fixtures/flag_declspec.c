/* { dg-options "-fdeclspec" } */
int printf(const char *, ...);

__declspec(noinline) static int triple(int x) { return 3 * x; }

__declspec(align(16)) static int aligned_value = 5;

int main(void) {
  printf("%d %zu %d\n", triple(aligned_value), _Alignof(aligned_value), (int)((unsigned long)&aligned_value % 16));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fdeclspec" }
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn triple(x: i32) -> i32 {
// CHECK-NEXT:     3 * x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut aligned_value: __SlateAlign16<i32> = __SlateAlign16(5i32);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %zu %d\n".as_ptr(),
// CHECK-NEXT:             triple(aligned_value.0),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(aligned_value.0) as u64) % 16) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
