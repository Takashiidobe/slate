#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_copy(d, s) __builtin_va_copy(d, s)
typedef __builtin_va_list va_list;
int vsnprintf(char *, unsigned long, const char *, va_list);
int printf(const char *, ...);
int sum(int n, ...) {
  va_list ap, aq;
  va_start(ap, n);
  va_copy(aq, ap);
  int t = 0;
  for (int i = 0; i < n; i++) t += __builtin_va_arg(ap, int);
  for (int i = 0; i < n; i++) t += __builtin_va_arg(aq, int);
  va_end(aq);
  va_end(ap);
  return t;
}
int fmt(char *buf, const char *f, ...) {
  va_list args;
  va_start(args, f);
  int r = vsnprintf(buf, 64, f, args);
  va_end(args);
  return r;
}
int main(void) { char b[64]; fmt(b, "%d-%s", 7, "x"); printf("%d %s\n", sum(3, 1, 2, 3), b); return 0; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! va_copy {
// CHECK-NEXT:     ($d:expr, $s:expr) => {
// CHECK-NEXT:         $d = $s .clone();
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! va_end {
// CHECK-NEXT:     () => {};
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vsnprintf(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *mut i8,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: u64,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: *const i8,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: core::ffi::VaList<'static>,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C-unwind" fn sum(n: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut aq: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     va_copy!(aq, ap);
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         t += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = unsafe { aq.next_arg::<i32>() };
// CHECK-NEXT:         t += [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     va_end!();
// CHECK-NEXT:     va_end!();
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn fmt(buf: *mut i8, f: *const i8, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut args: core::ffi::VaList<'static> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     args = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let r: i32 = unsafe { vsnprintf(buf, 64u64, f, args.clone()) };
// CHECK-NEXT:     va_end!();
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fmt(
// CHECK-NEXT:             b.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             c"%d-%s".as_ptr(),
// CHECK-NEXT:             7i32,
// CHECK-NEXT:             c"x".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %s\n".as_ptr(),
// CHECK-NEXT:             sum(3i32, 1i32, 2i32, 3i32),
// CHECK-NEXT:             b.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
