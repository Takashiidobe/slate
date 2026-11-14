#include <stdarg.h>
#undef va_copy
#define va_copy(d, s) __builtin_va_copy(d, s)
#include <stdio.h>

struct va_format {
  const char *fmt;
  va_list *va;
};

static void emit(struct va_format *vaf) {
  va_list va;
  va_copy(va, *vaf->va);
  vprintf(vaf->fmt, va);
  va_end(va);
}

static void outer(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  struct va_format vaf = {fmt, &args};
  emit(&vaf);
  emit(&vaf);
  va_end(args);
}

int main(void) {
  outer("%d-%s\n", 7, "x");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! va_copy {
// CHECK-NEXT:     ($d:expr, $s:expr) => {
// CHECK-NEXT:         $d = unsafe { (* std::ptr::addr_of_mut!($s)).clone() };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vprintf([[ARG_0:arg[0-9]+]]: *const i8, [[ARG_1:arg[0-9]+]]: core::ffi::VaList<'static>) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct va_format {
// CHECK-NEXT:     fmt: *const i8,
// CHECK-NEXT:     va: *mut core::ffi::VaList<'static>,
// CHECK-NEXT: }
// CHECK-NEXT: fn emit(vaf: *mut va_format) {
// CHECK-NEXT:     let mut va: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     va_copy!(va, * unsafe { (* vaf).va });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { vprintf((*vaf).fmt, va.clone()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn outer(fmt: *const i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut args: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     args = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut vaf: va_format = va_format {
// CHECK-NEXT:         fmt: fmt,
// CHECK-NEXT:         va: std::ptr::addr_of_mut!(args),
// CHECK-NEXT:     };
// CHECK-NEXT:     emit(std::ptr::addr_of_mut!(vaf));
// CHECK-NEXT:     emit(std::ptr::addr_of_mut!(vaf));
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { outer(c"%d-%s\n".as_ptr(), 7i32, c"x".as_ptr() as *mut i8) };
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
