#include <stdarg.h>
#include <stdio.h>

static __float128 sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  __float128 total = 0;
  for (int i = 0; i < n; i++) {
    int scale = va_arg(ap, int);
    total += scale * va_arg(ap, __float128);
  }
  va_end(ap);
  return total;
}

int main(void) {
  __float128 big = 1.0Q + 0x1p-100Q;
  __float128 total = sum(3, 2, big, 3, -1.5Q, 1, 0x1p-90Q);
  printf("%a %d\n", (double)total, total - (2 * big - 4.5Q) == 0x1p-90Q);
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_i386_va {
// CHECK-NEXT:     pub unsafe fn arg<L, T>(list: *mut L) -> T {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let cursor = &mut *(list as *mut *const u8);
// CHECK-NEXT:             if core::mem::align_of::<T>() >= 16 {
// CHECK-NEXT:                 *cursor = ((*cursor as usize + 15) & !15) as *const u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             let size = core::mem::size_of::<T>();
// CHECK-NEXT:             let value = core::ptr::read_unaligned(*cursor as *const T);
// CHECK-NEXT:             *cursor = cursor.add((size + 3) & !3);
// CHECK-NEXT:             value
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn sum(n: i32, mut __va_args: ...) -> f128 {
// CHECK-NEXT:     let mut ap: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: f128 = 0i32 as f128;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let scale: i32 = unsafe {
// CHECK-NEXT:             __slate_i386_va::arg::<_, i32>(std::ptr::addr_of_mut!(ap))
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: f128 = (scale as f128)
// CHECK-NEXT:             * unsafe { __slate_i386_va::arg::<_, f128>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let big: f128 = 1f128 + 7.88860905221011805411728565282786229E-31f128;
// CHECK-NEXT:     let total: f128 = unsafe {
// CHECK-NEXT:         sum(
// CHECK-NEXT:             3i32,
// CHECK-NEXT:             2i32,
// CHECK-NEXT:             big,
// CHECK-NEXT:             3i32,
// CHECK-NEXT:             -1.5f128,
// CHECK-NEXT:             1i32,
// CHECK-NEXT:             8.07793566946316088741610050849573099E-28f128,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%a %d\n".as_ptr(),
// CHECK-NEXT:             total as f64,
// CHECK-NEXT:             (total - ((2i32 as f128) * big - 4.5f128)
// CHECK-NEXT:                 == 8.07793566946316088741610050849573099E-28f128) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
