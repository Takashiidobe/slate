/* { dg-options "/Zc:preprocessor /O2 /MT /TC" } */
#include <windows.h>
void abort(void);

int exp_fn(void) { return 7; }
int inner_fn(void) { return 9; }
int exp_data = 5;
int hidden_fn(void) { return 1; }

#pragma comment(linker, "/export:exp_fn")
#pragma comment(linker, "/export:alias_fn=inner_fn")
#pragma comment(linker, "/export:exp_data,DATA")

int main(void) {
  HMODULE self = GetModuleHandleA(NULL);
  int (*exported)(void) = (int (*)(void))GetProcAddress(self, "exp_fn");
  int (*aliased)(void) = (int (*)(void))GetProcAddress(self, "alias_fn");
  int *data = (int *)GetProcAddress(self, "exp_data");
  if (exported == NULL || exported() != 7)
    abort();
  if (aliased == NULL || aliased() != 9)
    abort();
  if (data == NULL || *data != 5)
    abort();
  if (GetProcAddress(self, "hidden_fn") != NULL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /O2 /MT /TC" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn GetModuleHandleA([[ARG_0:arg[0-9]+]]: *const i8) -> *mut HINSTANCE__;
// CHECK-NEXT:     fn GetProcAddress(
// CHECK-NEXT:         [[ARG_0]]: *mut HINSTANCE__,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const i8,
// CHECK-NEXT:     ) -> Option<unsafe extern "C-unwind" fn() -> i64>;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct HINSTANCE__ {
// CHECK-NEXT:     unused: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn exp_fn() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".section .drectve,\"yni\"\n.ascii \" /export:exp_fn /export:alias_fn=inner_fn /export:exp_data,DATA\""
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn inner_fn() -> i32 {
// CHECK-NEXT:     9
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut exp_data: i32 = 5;
// CHECK-EMPTY:
// CHECK-NEXT: fn hidden_fn() -> i32 {
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let self_: *mut HINSTANCE__ = unsafe { GetModuleHandleA(std::ptr::null::<i8>()) };
// CHECK-NEXT:     let exported: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:         >(unsafe { GetProcAddress(self_, c"exp_fn".as_ptr()) })
// CHECK-NEXT:     };
// CHECK-NEXT:     let aliased: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:         >(unsafe { GetProcAddress(self_, c"alias_fn".as_ptr()) })
// CHECK-NEXT:     };
// CHECK-NEXT:     let data: *mut i32 = unsafe {
// CHECK-NEXT:         std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i64>, *mut i32>(unsafe {
// CHECK-NEXT:             GetProcAddress(self_, c"exp_data".as_ptr())
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = exported == None || (unsafe { exported.unwrap()() }) != 7;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = aliased == None || (unsafe { aliased.unwrap()() }) != 9;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if data.is_null() || (unsafe { *data }) != 5 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { GetProcAddress(self_, c"hidden_fn".as_ptr()) }) != None {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
