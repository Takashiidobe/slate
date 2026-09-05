#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

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
_Static_assert(sizeof(wint_t) == 4, "wint_t");
_Static_assert(sizeof(long double) == 16, "long double");
_Static_assert(_Alignof(long double) == 16, "long double alignment");
_Static_assert(sizeof(max_align_t) == 32, "max_align_t");
_Static_assert(_Alignof(max_align_t) == 16, "max_align_t alignment");
_Static_assert(LDBL_MANT_DIG == 113, "long double mantissa");
_Static_assert(LDBL_MAX_EXP == 16384, "long double exponent");
_Static_assert(INTMAX_MAX == 9223372036854775807L, "INTMAX_MAX");
_Static_assert(UINTMAX_MAX == 18446744073709551615UL, "UINTMAX_MAX");
_Static_assert(WINT_MAX == 4294967295U, "WINT_MAX");

#if defined(__aarch64__)
_Static_assert(WCHAR_MAX == 4294967295U, "WCHAR_MAX");
_Static_assert(WCHAR_MIN == 0, "WCHAR_MIN");
_Static_assert(sizeof(va_list) == 32, "va_list");
#elif defined(__x86_64__)
_Static_assert(WCHAR_MAX == 2147483647, "WCHAR_MAX");
_Static_assert(WCHAR_MIN == (-2147483647 - 1), "WCHAR_MIN");
_Static_assert(sizeof(va_list) == 24, "va_list");
#else
#error "unsupported Bionic architecture"
#endif

extern long bionic_import(size_t, ptrdiff_t, intptr_t, uintptr_t, wchar_t,
                          wint_t, long double);

long call_bionic_import(size_t size, ptrdiff_t difference, intptr_t signed_ptr,
                        uintptr_t unsigned_ptr, wchar_t wide, wint_t wide_int,
                        long double real) {
  return bionic_import(size, difference, signed_ptr, unsigned_ptr, wide,
                       wide_int, real);
}

int bionic_variadic_count(int count, ...) {
  va_list values;
  int     total = 0;
  va_start(values, count);
  for (int i = 0; i < count; ++i)
    total += va_arg(values, int);
  va_end(values);
  return total;
}

int main(void) { return 0; }
// REWRITES-DAG: fn bionic_import(
// REWRITES-DAG: _0: usize
// REWRITES-DAG: _1: isize
// REWRITES-DAG: _2: isize
// REWRITES-DAG: _3: usize
// REWRITES-DAG: _5: u32
// REWRITES-DAG: #![feature(f128)]
// REWRITES-DAG: _6: f128
// REWRITES-DAG: next_arg::<i32>()
// REWRITES-NOT: struct LongDouble
// REWRITES-BIONIC-AARCH64-DAG: _4: u32
// REWRITES-BIONIC-X86_64-DAG: _4: i32

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(f128)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: #[repr(C)]
// LOWERING-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-AARCH64-NEXT: struct __va_list {
// LOWERING-BIONIC-AARCH64-NEXT:     __slate_empty: [u8; 0],
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-AARCH64-NEXT:     fn bionic_import(_0: usize, _1: isize, _2: isize, _3: usize, _4: u32, _5: u32, _6: f128)
// LOWERING-BIONIC-X86_64-NEXT:     fn bionic_import(_0: usize, _1: isize, _2: isize, _3: usize, _4: i32, _5: u32, _6: f128)
// LOWERING-NEXT:     -> i64;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: struct __SlateVaArg {
// LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-NEXT:     size: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArg {
// LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             value: Box::new(value),
// LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// LOWERING-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// LOWERING-NEXT:             return *value;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             std::ptr::read_unaligned(
// LOWERING-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[derive(Clone)]
// LOWERING-NEXT: struct __SlateVaArgs {
// LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// LOWERING-NEXT:     index: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArgs {
// LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// LOWERING-NEXT:             index: 0,
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     const fn empty() -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             args: None,
// LOWERING-NEXT:             index: 0,
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-NEXT:         let index = self.index;
// LOWERING-NEXT:         self.index += 1;
// LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-NEXT:         args[index].read::<T>()
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn call_bionic_import(
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-NEXT:     {{arg[0-9]+}}: i64,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-BIONIC-AARCH64-NEXT:     {{arg[0-9]+}}: u32,
// LOWERING-BIONIC-X86_64-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: u32,
// LOWERING-NEXT:     {{arg[0-9]+}}: f128,
// LOWERING-NEXT: ) -> i64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         bionic_import(
// LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-NEXT:             {{arg[0-9]+}} as isize,
// LOWERING-NEXT:             {{arg[0-9]+}} as isize,
// LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-BIONIC-AARCH64-NEXT:             {{arg[0-9]+}} as u32,
// LOWERING-BIONIC-X86_64-NEXT:             {{arg[0-9]+}} as i32,
// LOWERING-NEXT:             {{arg[0-9]+}} as u32,
// LOWERING-NEXT:             {{arg[0-9]+}} as f128,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn bionic_variadic_count({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-NEXT:     let mut count: i32 = 0;
// LOWERING-NEXT:     let mut values: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         values = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = count;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { values.next_arg::<i32>() };
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:             total = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
