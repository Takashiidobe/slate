#include <stdarg.h>
#include <stdio.h>

static _Float16 add16(_Float16 a, _Float16 b) { return a + b; }

static _Float16 mul16(_Float16 a, _Float16 b) { return a * b; }

static _Float16 sum_variadic(int n, ...) {
  va_list ap;
  va_start(ap, n);
  _Float16 total = (_Float16)0;
  for (int i = 0; i < n; i++) {
    total = total + va_arg(ap, _Float16);
  }
  va_end(ap);
  return total;
}

int main(void) {
  _Float16 a = 3.0f16;
  _Float16 b = 4.0f16;
  printf("%d\n", (int)add16(a, b));
  printf("%d\n", (int)mul16(a, b));
  printf("%d\n", (int)sum_variadic(3, (_Float16)1.0f16, (_Float16)2.0f16,
                                   (_Float16)3.0f16));
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f16)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: struct __SlateVaArg {
// COMMON-LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-LOWERING-NEXT:     size: usize,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArg {
// COMMON-LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             value: Box::new(value),
// COMMON-LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// COMMON-LOWERING-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// COMMON-LOWERING-NEXT:             return *value;
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             std::ptr::read_unaligned(
// COMMON-LOWERING-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[derive(Clone)]
// COMMON-LOWERING-NEXT: struct __SlateVaArgs {
// COMMON-LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-LOWERING-NEXT:     index: usize,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArgs {
// COMMON-LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     const fn empty() -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: None,
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-LOWERING-NEXT:         let index = self.index;
// COMMON-LOWERING-NEXT:         self.index += 1;
// COMMON-LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-LOWERING-NEXT:         args[index].read::<T>()
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 4.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = add16({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = mul16({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = unsafe {
// COMMON-LOWERING-NEXT:         sum_variadic(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             __SlateVaArgs::new(vec![
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:             ]),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn add16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn mul16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe fn sum_variadic({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> f16 {
// COMMON-LOWERING-NEXT:     let mut n: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-LOWERING-NEXT:     let mut total: f16 = 0.0f16;
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         ap = __slate_va_args.clone();
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: f16 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: f16 = unsafe { ap.next_arg::<f16>() };
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-AARCH64-GNU-EMPTY:
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(f16)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: struct __SlateVaArg {
// COMMON-REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-REWRITES-NEXT:     size: usize,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArg {
// COMMON-REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             value: Box::new(value),
// COMMON-REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// COMMON-REWRITES-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// COMMON-REWRITES-NEXT:             return *value;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::ptr::read_unaligned(
// COMMON-REWRITES-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[derive(Clone)]
// COMMON-REWRITES-NEXT: struct __SlateVaArgs {
// COMMON-REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-REWRITES-NEXT:     index: usize,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArgs {
// COMMON-REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     const fn empty() -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: None,
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-REWRITES-NEXT:         let index = self.index;
// COMMON-REWRITES-NEXT:         self.index += 1;
// COMMON-REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-REWRITES-NEXT:         args[index].read::<T>()
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f16 = 3.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f16 = 4.0;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add16({{__v[0-9]+}}, {{__v[0-9]+}}) as i32) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), mul16({{__v[0-9]+}}, {{__v[0-9]+}}) as i32) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (unsafe {
// COMMON-REWRITES-NEXT:                 sum_variadic(
// COMMON-REWRITES-NEXT:                     3,
// COMMON-REWRITES-NEXT:                     __SlateVaArgs::new(vec![
// COMMON-REWRITES-NEXT:                         __SlateVaArg::new(1.0 as f16),
// COMMON-REWRITES-NEXT:                         __SlateVaArg::new(2.0 as f16),
// COMMON-REWRITES-NEXT:                         __SlateVaArg::new(3.0 as f16),
// COMMON-REWRITES-NEXT:                     ]),
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             }) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn add16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn mul16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe fn sum_variadic(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> f16 {
// COMMON-REWRITES-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-REWRITES-NEXT:     let mut total: f16 = 0.0f16;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         ap = __slate_va_args.clone();
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total = (0 as i32) as f16;
// COMMON-REWRITES-NEXT:     for i in 0..n {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f16 = unsafe { ap.next_arg::<f16>() };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     (({{arg[0-9]+}} as f32) + ({{arg[0-9]+}} as f32)) as f16
// REWRITES-X86_64-GNU-NEXT:     (({{arg[0-9]+}} as f32) * ({{arg[0-9]+}} as f32)) as f16
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: f32 = total as f32;
// REWRITES-X86_64-GNU-NEXT:         total = ({{__v[0-9]+}} + ({{__v[0-9]+}} as f32)) as f16;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-AARCH64-GNU-EMPTY:
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f16 = total;
// REWRITES-AARCH64-GNU-NEXT:         total = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-AARCH64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
