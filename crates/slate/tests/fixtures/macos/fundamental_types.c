#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

_Static_assert(sizeof(char) == 1, "char");
_Static_assert(sizeof(short) == 2, "short");
_Static_assert(sizeof(int) == 4, "int");
_Static_assert(sizeof(long) == 8, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(void *) == 8, "pointer");
_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(intmax_t) == 8, "intmax_t");
_Static_assert(sizeof(uintmax_t) == 8, "uintmax_t");
_Static_assert(sizeof(wchar_t) == 4, "wchar_t");
_Static_assert(sizeof(long double) == 8, "long double");
_Static_assert(_Alignof(long double) == 8, "long double alignment");
_Static_assert(sizeof(max_align_t) == 8, "max_align_t");
_Static_assert(_Alignof(max_align_t) == 8, "max_align_t alignment");
_Static_assert(sizeof(time_t) == 8, "time_t");
_Static_assert(sizeof(clock_t) == 8, "clock_t");
_Static_assert((clock_t)-1 > 0, "clock_t signedness");
_Static_assert(sizeof(va_list) == 8, "va_list");
_Static_assert(__builtin_types_compatible_p(wchar_t, int), "wchar_t type");
_Static_assert(__builtin_types_compatible_p(time_t, long), "time_t type");
_Static_assert(__builtin_types_compatible_p(clock_t, unsigned long),
               "clock_t type");
_Static_assert(__builtin_types_compatible_p(va_list, char *), "va_list type");
_Static_assert(__builtin_types_compatible_p(fpos_t, long long), "fpos_t type");
_Static_assert(LDBL_MANT_DIG == 53, "long double mantissa");
_Static_assert(LDBL_MAX_EXP == 1024, "long double exponent");
_Static_assert(__builtin_types_compatible_p(int64_t, long long), "int64_t");
_Static_assert(__builtin_types_compatible_p(int_fast16_t, short),
               "int_fast16_t");
_Static_assert(__builtin_types_compatible_p(int_fast32_t, int), "int_fast32_t");
_Static_assert(__builtin_types_compatible_p(int_fast64_t, long long),
               "int_fast64_t");
_Static_assert(__builtin_types_compatible_p(intmax_t, long), "intmax_t");
_Static_assert(__builtin_types_compatible_p(uintmax_t, unsigned long),
               "uintmax_t");
_Static_assert(WCHAR_MIN == (-2147483647 - 1), "WCHAR_MIN");
_Static_assert(WCHAR_MAX == 2147483647, "WCHAR_MAX");
_Static_assert(EDOM == 33, "EDOM");
_Static_assert(ERANGE == 34, "ERANGE");
_Static_assert(EILSEQ == 92, "EILSEQ");
_Static_assert(FOPEN_MAX == 20, "FOPEN_MAX");
_Static_assert(FILENAME_MAX == 1024, "FILENAME_MAX");
_Static_assert(TMP_MAX == 308915776, "TMP_MAX");
_Static_assert(L_tmpnam == 1024, "L_tmpnam");
_Static_assert(CLOCKS_PER_SEC == 1000000L, "CLOCKS_PER_SEC");
_Static_assert(sizeof(struct tm) == 56, "struct tm");
_Static_assert(__builtin_types_compatible_p(__typeof__(&__error),
                                            int *(*)(void)),
               "__error signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&printf),
                                            int (*)(const char *, ...)),
               "printf signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&malloc),
                                            void *(*)(size_t)),
               "malloc signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&memcpy),
                                            void *(*)(void *, const void *,
                                                      size_t)),
               "memcpy signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&sin),
                                            double (*)(double)),
               "sin signature");

#if defined(__linux__) || defined(__GLIBC__) || defined(__MUSL__)
#error "Linux target state leaked into the Darwin profile"
#endif

extern long darwin_import(size_t, ptrdiff_t, intptr_t, uintptr_t, wchar_t,
                          time_t, clock_t, long double);

long call_darwin_import(size_t size, ptrdiff_t difference, intptr_t signed_ptr,
                        uintptr_t unsigned_ptr, wchar_t wide, time_t seconds,
                        clock_t ticks, long double real) {
  return darwin_import(size, difference, signed_ptr, unsigned_ptr, wide,
                       seconds, ticks, real);
}

int darwin_variadic_count(int count, ...) {
  va_list values;
  int     total = 0;
  va_start(values, count);
  for (int i = 0; i < count; ++i)
    total += va_arg(values, int);
  va_end(values);
  return total;
}

int main(void) { return printf("%d\n", errno == 0); }
// REWRITES-MACOS-DAG: fn darwin_import(
// REWRITES-MACOS-DAG: _0: usize
// REWRITES-MACOS-DAG: _1: isize
// REWRITES-MACOS-DAG: _2: isize
// REWRITES-MACOS-DAG: _3: usize
// REWRITES-MACOS-DAG: _4: i32
// REWRITES-MACOS-DAG: _5: i64
// REWRITES-MACOS-DAG: _6: u64
// REWRITES-MACOS-DAG: _7: f64
// REWRITES-MACOS-DAG: next_arg::<i32>()
// REWRITES-MACOS-NOT: f128

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![feature(c_variadic)]
// LOWERING-MACOS-NEXT: #![allow(
// LOWERING-MACOS-NEXT:     dead_code,
// LOWERING-MACOS-NEXT:     unused,
// LOWERING-MACOS-NEXT:     non_camel_case_types,
// LOWERING-MACOS-NEXT:     non_snake_case,
// LOWERING-MACOS-NEXT:     non_upper_case_globals,
// LOWERING-MACOS-NEXT:     arithmetic_overflow,
// LOWERING-MACOS-NEXT:     unconditional_panic,
// LOWERING-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MACOS-NEXT:     unused_comparisons
// LOWERING-MACOS-NEXT: )]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: unsafe extern "C" {
// LOWERING-MACOS-NEXT:     fn darwin_import(
// LOWERING-MACOS-NEXT:         _0: usize,
// LOWERING-MACOS-NEXT:         _1: isize,
// LOWERING-MACOS-NEXT:         _2: isize,
// LOWERING-MACOS-NEXT:         _3: usize,
// LOWERING-MACOS-NEXT:         _4: i32,
// LOWERING-MACOS-NEXT:         _5: i64,
// LOWERING-MACOS-NEXT:         _6: u64,
// LOWERING-MACOS-NEXT:         _7: f64,
// LOWERING-MACOS-NEXT:     ) -> i64;
// LOWERING-MACOS-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-MACOS-NEXT:     fn __error() -> *mut i32;
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: struct __SlateVaArg {
// LOWERING-MACOS-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-MACOS-NEXT:     size: usize,
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: impl __SlateVaArg {
// LOWERING-MACOS-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-MACOS-NEXT:         Self {
// LOWERING-MACOS-NEXT:             value: Box::new(value),
// LOWERING-MACOS-NEXT:             size: std::mem::size_of::<T>(),
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// LOWERING-MACOS-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// LOWERING-MACOS-NEXT:             return *value;
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// LOWERING-MACOS-NEXT:         unsafe {
// LOWERING-MACOS-NEXT:             std::ptr::read_unaligned(
// LOWERING-MACOS-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// LOWERING-MACOS-NEXT:             )
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[derive(Clone)]
// LOWERING-MACOS-NEXT: struct __SlateVaArgs {
// LOWERING-MACOS-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// LOWERING-MACOS-NEXT:     index: usize,
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: impl __SlateVaArgs {
// LOWERING-MACOS-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// LOWERING-MACOS-NEXT:         Self {
// LOWERING-MACOS-NEXT:             args: Some(std::rc::Rc::new(args)),
// LOWERING-MACOS-NEXT:             index: 0,
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT:     const fn empty() -> Self {
// LOWERING-MACOS-NEXT:         Self {
// LOWERING-MACOS-NEXT:             args: None,
// LOWERING-MACOS-NEXT:             index: 0,
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-MACOS-NEXT:         let index = self.index;
// LOWERING-MACOS-NEXT:         self.index += 1;
// LOWERING-MACOS-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-MACOS-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-MACOS-NEXT:         args[index].read::<T>()
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn call_darwin_import(
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-MACOS-NEXT:     {{arg[0-9]+}}: f64,
// LOWERING-MACOS-NEXT: ) -> i64 {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-MACOS-NEXT:         darwin_import(
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as isize,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as isize,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as i32,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as i64,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as u64,
// LOWERING-MACOS-NEXT:             {{arg[0-9]+}} as f64,
// LOWERING-MACOS-NEXT:         )
// LOWERING-MACOS-NEXT:     };
// LOWERING-MACOS-NEXT:     return {{_v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: unsafe fn darwin_variadic_count({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-MACOS-NEXT:     let mut count: i32 = 0;
// LOWERING-MACOS-NEXT:     let mut values: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-MACOS-NEXT:     let mut total: i32 = 0;
// LOWERING-MACOS-NEXT:     count = {{arg[0-9]+}};
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     total = {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:     unsafe {
// LOWERING-MACOS-NEXT:         values = __slate_va_args.clone();
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-NEXT:     {
// LOWERING-MACOS-NEXT:         let mut i: i32 = 0;
// LOWERING-MACOS-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:         i = {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:         loop {
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = count;
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-MACOS-NEXT:                 break;
// LOWERING-MACOS-NEXT:             }
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { values.next_arg::<i32>() };
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = total;
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:             total = {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-MACOS-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-MACOS-NEXT:             i = {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:         }
// LOWERING-MACOS-NEXT:     }
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-MACOS-NEXT:     return {{_v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn main() {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __error() };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-MACOS-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos
