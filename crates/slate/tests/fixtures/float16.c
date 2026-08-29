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
  printf("%d\n",
         (int)sum_variadic(3, (_Float16)1.0f16, (_Float16)2.0f16,
                            (_Float16)3.0f16));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(f16)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: struct __SlateVaArg {
// LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-NEXT:     size: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArg {
// LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-NEXT:         Self { value: Box::new(value), size: std::mem::size_of::<T>() }
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
// LOWERING-NEXT:         Self { args: Some(std::rc::Rc::new(args)), index: 0 }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     const fn empty() -> Self {
// LOWERING-NEXT:         Self { args: None, index: 0 }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-NEXT:         let value = args[self.index].read::<T>();
// LOWERING-NEXT:         self.index += 1;
// LOWERING-NEXT:         value
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add16(arg3: f16, arg4: f16) -> f16 {
// LOWERING-NEXT:     let mut a: f16 = 0.0f16;
// LOWERING-NEXT:     let mut b: f16 = 0.0f16;
// LOWERING-NEXT:     let mut __retval: f16 = 0.0f16;
// LOWERING-NEXT:     a = arg3;
// LOWERING-NEXT:     b = arg4;
// LOWERING-NEXT:     let _v0: f16 = a;
// LOWERING-NEXT:     let _v1: f32 = _v0 as f32;
// LOWERING-NEXT:     let _v2: f16 = b;
// LOWERING-NEXT:     let _v3: f32 = _v2 as f32;
// LOWERING-NEXT:     let _v4: f32 = _v1 + _v3;
// LOWERING-NEXT:     let _v5: f16 = _v4 as f16;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: f16 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mul16(arg1: f16, arg2: f16) -> f16 {
// LOWERING-NEXT:     let mut a: f16 = 0.0f16;
// LOWERING-NEXT:     let mut b: f16 = 0.0f16;
// LOWERING-NEXT:     let mut __retval: f16 = 0.0f16;
// LOWERING-NEXT:     a = arg1;
// LOWERING-NEXT:     b = arg2;
// LOWERING-NEXT:     let _v0: f16 = a;
// LOWERING-NEXT:     let _v1: f32 = _v0 as f32;
// LOWERING-NEXT:     let _v2: f16 = b;
// LOWERING-NEXT:     let _v3: f32 = _v2 as f32;
// LOWERING-NEXT:     let _v4: f32 = _v1 * _v3;
// LOWERING-NEXT:     let _v5: f16 = _v4 as f16;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: f16 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn sum_variadic(arg0: i32, mut __slate_va_args: __SlateVaArgs) -> f16 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut __retval: f16 = 0.0f16;
// LOWERING-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-NEXT:     let mut total: f16 = 0.0f16;
// LOWERING-NEXT:     n = arg0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         ap = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     let _v1: f16 = _v0 as f16;
// LOWERING-NEXT:     total = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v2: i32 = 0;
// LOWERING-NEXT:         i = _v2;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: i32 = i;
// LOWERING-NEXT:             let _v4: i32 = n;
// LOWERING-NEXT:             let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:             if !_v5 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v6: f16 = total;
// LOWERING-NEXT:                 let _v7: f32 = _v6 as f32;
// LOWERING-NEXT:                 let _v8: f16 = unsafe { ap.next_arg::<f16>() };
// LOWERING-NEXT:                 let _v9: f32 = _v8 as f32;
// LOWERING-NEXT:                 let _v10: f32 = _v7 + _v9;
// LOWERING-NEXT:                 let _v11: f16 = _v10 as f16;
// LOWERING-NEXT:                 total = _v11;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v12: i32 = i;
// LOWERING-NEXT:             let _v13: i32 = _v12 + 1;
// LOWERING-NEXT:             i = _v13;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: f16 = total;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: f16 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: f16 = 0.0f16;
// LOWERING-NEXT:     let mut b: f16 = 0.0f16;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f16 = 3.0;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: f16 = 4.0;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: f16 = a;
// LOWERING-NEXT:     let _v5: f16 = b;
// LOWERING-NEXT:     let _v6: f16 = add16(_v4, _v5);
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v3 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: f16 = a;
// LOWERING-NEXT:     let _v11: f16 = b;
// LOWERING-NEXT:     let _v12: f16 = mul16(_v10, _v11);
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v9 as *const i8, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i32 = 3;
// LOWERING-NEXT:     let _v17: f16 = 1.0;
// LOWERING-NEXT:     let _v18: f16 = 2.0;
// LOWERING-NEXT:     let _v19: f16 = 3.0;
// LOWERING-NEXT:     let _v20: f16 = unsafe { sum_variadic(_v16, __SlateVaArgs::new(vec![__SlateVaArg::new(_v17), __SlateVaArg::new(_v18), __SlateVaArg::new(_v19)])) };
// LOWERING-NEXT:     let _v21: i32 = _v20 as i32;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v15 as *const i8, _v21) };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     __retval = _v23;
// LOWERING-NEXT:     let _v24: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v24 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(f16)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: struct __SlateVaArg {
// REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-NEXT:     size: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArg {
// REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-NEXT:         Self { value: Box::new(value), size: std::mem::size_of::<T>() }
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
// REWRITES-NEXT:         Self { args: Some(std::rc::Rc::new(args)), index: 0 }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     const fn empty() -> Self {
// REWRITES-NEXT:         Self { args: None, index: 0 }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// REWRITES-NEXT:         let value = args[self.index].read::<T>();
// REWRITES-NEXT:         self.index += 1;
// REWRITES-NEXT:         value
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add16(arg3: f16, arg4: f16) -> f16 {
// REWRITES-NEXT: let mut a: f16 = 0.0f16;
// REWRITES-NEXT: let mut b: f16 = 0.0f16;
// REWRITES-NEXT: let mut __retval: f16 = 0.0f16;
// REWRITES-NEXT: a = arg3;
// REWRITES-NEXT: b = arg4;
// REWRITES-NEXT: __retval = ((a as f32) + (b as f32)) as f16;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mul16(arg1: f16, arg2: f16) -> f16 {
// REWRITES-NEXT: let mut a: f16 = 0.0f16;
// REWRITES-NEXT: let mut b: f16 = 0.0f16;
// REWRITES-NEXT: let mut __retval: f16 = 0.0f16;
// REWRITES-NEXT: a = arg1;
// REWRITES-NEXT: b = arg2;
// REWRITES-NEXT: __retval = ((a as f32) * (b as f32)) as f16;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn sum_variadic(arg0: i32, mut __slate_va_args: __SlateVaArgs) -> f16 {
// REWRITES-NEXT: let mut n: i32 = arg0;
// REWRITES-NEXT: let mut __retval: f16 = 0.0f16;
// REWRITES-NEXT: let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-NEXT: let mut total: f16 = 0.0f16;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         ap = __slate_va_args.clone();
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v0: i32 = 0;
// REWRITES-NEXT: total = _v0 as f16;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v7: f32 = total as f32;
// REWRITES-NEXT:                                     let _v8: f16 = unsafe { ap.next_arg::<f16>() };
// REWRITES-NEXT:                                     total = (_v7 + (_v8 as f32)) as f16;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: f16 = 0.0f16;
// REWRITES-NEXT: let mut b: f16 = 0.0f16;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 3.0;
// REWRITES-NEXT: b = 4.0;
// REWRITES-NEXT: let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: f16 = add16(a, b);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v3 as *const i8, _v6 as i32) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: f16 = mul16(a, b);
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v9 as *const i8, _v12 as i32) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = 3;
// REWRITES-NEXT: let _v17: f16 = 1.0;
// REWRITES-NEXT: let _v18: f16 = 2.0;
// REWRITES-NEXT: let _v19: f16 = 3.0;
// REWRITES-NEXT: let _v20: f16 = unsafe { sum_variadic(_v16, __SlateVaArgs::new(vec![__SlateVaArg::new(_v17), __SlateVaArg::new(_v18), __SlateVaArg::new(_v19)])) };
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v15 as *const i8, _v20 as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
