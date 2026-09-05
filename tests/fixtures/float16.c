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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(f16)]
// LOWERING-NEXT: #![feature(c_variadic)]
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
// LOWERING-AARCH64-GNU-EMPTY:
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
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
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 3.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 4.0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = add16({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = mul16({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 1.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 2.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = 3.0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = unsafe {
// LOWERING-NEXT:         sum_variadic(
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             __SlateVaArgs::new(vec![
// LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// LOWERING-NEXT:             ]),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mul16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{arg[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f16 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn sum_variadic({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> f16 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-NEXT:     let mut total: f16 = 0.0f16;
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         ap = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = n;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: f16 = total;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: f16 = unsafe { ap.next_arg::<f16>() };
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 total = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: f16 = total;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(f16)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-AARCH64-GNU-EMPTY:
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: struct __SlateVaArg {
// REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-NEXT:     size: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArg {
// REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             value: Box::new(value),
// REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// REWRITES-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// REWRITES-NEXT:             return *value;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::ptr::read_unaligned(
// REWRITES-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[derive(Clone)]
// REWRITES-NEXT: struct __SlateVaArgs {
// REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// REWRITES-NEXT:     index: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArgs {
// REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// REWRITES-NEXT:             index: 0,
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     const fn empty() -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             args: None,
// REWRITES-NEXT:             index: 0,
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// REWRITES-NEXT:         let index = self.index;
// REWRITES-NEXT:         self.index += 1;
// REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// REWRITES-NEXT:         args[index].read::<T>()
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{__v[0-9]+}}: f16 = 3.0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: f16 = 4.0;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add16({{__v[0-9]+}}, {{__v[0-9]+}}) as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), mul16({{__v[0-9]+}}, {{__v[0-9]+}}) as i32) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             (unsafe {
// REWRITES-NEXT:                 sum_variadic(
// REWRITES-NEXT:                     3,
// REWRITES-NEXT:                     __SlateVaArgs::new(vec![
// REWRITES-NEXT:                         __SlateVaArg::new(1.0 as f16),
// REWRITES-NEXT:                         __SlateVaArg::new(2.0 as f16),
// REWRITES-NEXT:                         __SlateVaArg::new(3.0 as f16),
// REWRITES-NEXT:                     ]),
// REWRITES-NEXT:                 )
// REWRITES-NEXT:             }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// REWRITES-X86_64-GNU-NEXT:     (({{arg[0-9]+}} as f32) + ({{arg[0-9]+}} as f32)) as f16
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mul16({{arg[0-9]+}}: f16, {{arg[0-9]+}}: f16) -> f16 {
// REWRITES-X86_64-GNU-NEXT:     (({{arg[0-9]+}} as f32) * ({{arg[0-9]+}} as f32)) as f16
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn sum_variadic(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> f16 {
// REWRITES-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-NEXT:     let mut total: f16 = 0.0f16;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         ap = __slate_va_args.clone();
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total = (0 as i32) as f16;
// REWRITES-NEXT:     for i in 0..n {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: f32 = total as f32;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: f16 = total;
// REWRITES-NEXT:         let {{__v[0-9]+}}: f16 = unsafe { ap.next_arg::<f16>() };
// REWRITES-X86_64-GNU-NEXT:         total = ({{__v[0-9]+}} + ({{__v[0-9]+}} as f32)) as f16;
// REWRITES-AARCH64-GNU-NEXT:         total = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
