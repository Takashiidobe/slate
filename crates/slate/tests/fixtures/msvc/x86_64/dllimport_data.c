/* { dg-options "/Zc:preprocessor /O2 /MT /TC" } */
#include <windows.h>
void abort(void);

#pragma comment(linker, "/defaultlib:ntdll.lib")

__declspec(dllimport) extern USHORT NlsAnsiCodePage;
__declspec(dllimport) BOOLEAN NlsMbCodePageTag;
__declspec(dllimport) extern int NeverDefinedAnywhere;

static USHORT *code_page(void) { return &NlsAnsiCodePage; }

int main(void) {
  USHORT *page = code_page();
  if (NlsAnsiCodePage != GetACP())
    abort();
  if (*page != NlsAnsiCodePage || page != &NlsAnsiCodePage)
    abort();
  if (NlsMbCodePageTag > 1)
    abort();
  if (sizeof NlsAnsiCodePage != 2)
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
// CHECK-NEXT:     #[link_name = "__imp_NlsAnsiCodePage"]
// CHECK-NEXT:     static NlsAnsiCodePage: *mut u16;
// CHECK-NEXT:     #[link_name = "__imp_NlsMbCodePageTag"]
// CHECK-NEXT:     static NlsMbCodePageTag: *mut u8;
// CHECK-NEXT:     #[link_name = "__imp_NeverDefinedAnywhere"]
// CHECK-NEXT:     static NeverDefinedAnywhere: *mut i32;
// CHECK-NEXT:     fn GetACP() -> u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(".section .drectve,\"yni\"\n.ascii \" /defaultlib:ntdll.lib\"");
// CHECK-EMPTY:
// CHECK-NEXT: fn code_page() -> *mut u16 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(*NlsAnsiCodePage) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let page: *mut u16 = code_page();
// CHECK-NEXT:     if (((unsafe { *NlsAnsiCodePage }) as i32) as u32) != unsafe { GetACP() } {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *page }) as i32) != ((unsafe { *NlsAnsiCodePage }) as i32)
// CHECK-NEXT:         || page != unsafe { std::ptr::addr_of_mut!(*NlsAnsiCodePage) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *NlsMbCodePageTag }) as i32) > 1 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (std::mem::size_of::<u16>() as u64) != 2 {
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
