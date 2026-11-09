/* { dg-options "/Zc:preprocessor /O2 /MT /TC" } */
#include <wchar.h>
void abort(void);

extern int missing_flag;
extern int present_flag;
__declspec(selectany) int missing_flag_default = 5;
__declspec(selectany) int present_flag_default = 9;
__declspec(selectany) int kept_unreferenced = 3;
int present_flag = 1;

#pragma comment(linker, "/alternatename:_missing_flag=_missing_flag_default")
#pragma comment(linker, "/alternatename:_present_flag=_present_flag_default")
#pragma comment(linker, "/include:_kept_unreferenced")

int main(void) {
  wchar_t source[] = L"wide";
  wchar_t target[5] = {0};
  if (missing_flag != 5)
    abort();
  if (present_flag != 1)
    abort();
  if (wmemcpy(target, source, 5) != target || wcscmp(target, L"wide") != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-pc-windows-msvc
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
// CHECK-NEXT:     static mut _Avx2WmemEnabledWeakValue: i32;
// CHECK-NEXT:     static mut missing_flag: i32;
// CHECK-NEXT:     static mut missing_flag_default: i32;
// CHECK-NEXT:     static mut present_flag_default: i32;
// CHECK-NEXT:     static mut kept_unreferenced: i32;
// CHECK-NEXT:     fn wcscmp([[ARG_0:arg[0-9]+]]: *const u16, [[ARG_1:arg[0-9]+]]: *const u16) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .data$__Avx2WmemEnabledWeakValue,\"dw\",discard,__Avx2WmemEnabledWeakValue\n.globl __Avx2WmemEnabledWeakValue\n.p2align 2\n__Avx2WmemEnabledWeakValue:\n.long {}\n.section .drectve,\"yni\"\n.ascii \" /include:__Avx2WmemEnabledWeakValue\"\n.text", const 0 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: fn wmemcpy(_S1: *mut u16, _S2: *const u16, _N: u32) -> *mut u16 {
// CHECK-NEXT:     // Buffer overrun
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (_S1 as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             (_S2 as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             _N.wrapping_mul(std::mem::size_of::<u16>() as u32) as usize,
// CHECK-NEXT:         ) as *mut ()
// CHECK-NEXT:     }) as *mut u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .data$_missing_flag_default,\"dw\",discard,_missing_flag_default\n.globl _missing_flag_default\n.p2align 2\n_missing_flag_default:\n.long {}\n.section .drectve,\"yni\"\n.ascii \" /include:_missing_flag_default\"\n.text", const 5 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .data$_present_flag_default,\"dw\",discard,_present_flag_default\n.globl _present_flag_default\n.p2align 2\n_present_flag_default:\n.long {}\n.section .drectve,\"yni\"\n.ascii \" /include:_present_flag_default\"\n.text", const 9 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .data$_kept_unreferenced,\"dw\",discard,_kept_unreferenced\n.globl _kept_unreferenced\n.p2align 2\n_kept_unreferenced:\n.long {}\n.section .drectve,\"yni\"\n.ascii \" /include:_kept_unreferenced\"\n.text", const 3 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: static mut present_flag: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".section .drectve,\"yni\"\n.ascii \" /alternatename:__Avx2WmemEnabled=__Avx2WmemEnabledWeakValue /alternatename:_missing_flag=_missing_flag_default /alternatename:_present_flag=_present_flag_default\""
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut source: [u16; 5] = [119, 105, 100, 101, 0];
// CHECK-NEXT:     let mut target: [u16; 5] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u16; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0u16;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { missing_flag }) != 5 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { present_flag }) != 1 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = wmemcpy(
// CHECK-NEXT:         target.as_mut_ptr() as *mut u16,
// CHECK-NEXT:         source.as_mut_ptr() as *const u16,
// CHECK-NEXT:         5u32,
// CHECK-NEXT:     ) != (target.as_mut_ptr() as *mut u16)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             wcscmp(
// CHECK-NEXT:                 target.as_mut_ptr() as *const u16,
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__slate_str28) as *mut u16) as *const u16,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != 0;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut __slate_str28: [u16; 5] = [119, 105, 100, 101, 0];
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
