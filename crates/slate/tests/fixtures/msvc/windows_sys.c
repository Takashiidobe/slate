#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <stdlib.h>

static DWORD current_process_id(void) { return GetCurrentProcessId(); }

int main(void) {
    DWORD (WINAPI *process_id)(void) = GetCurrentProcessId;
    if (current_process_id() != (DWORD)_getpid() || process_id() != current_process_id())
        abort();
    SYSTEMTIME time;
    GetSystemTime(&time);
    if (time.wYear < 2020 || time.wMonth < 1 || time.wMonth > 12)
        abort();
    HANDLE event = CreateEventW(NULL, FALSE, FALSE, NULL);
    if (event == NULL || !SetEvent(event) || WaitForSingleObject(event, 0) != WAIT_OBJECT_0)
        abort();
    if (!CloseHandle(event))
        abort();
    return 0;
}

#else
int main(void) { return 0; }
#endif

/* { dg-options "/Zc:preprocessor" } */

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// I686-PC-WINDOWS-MSVC-NEXT:     fn _getpid() -> i32;
// I686-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: unsafe extern "stdcall" {
// CHECK-NEXT:     fn CloseHandle([[ARG_0:arg[0-9]+]]: *mut ()) -> i32;
// CHECK-NEXT:     fn SetEvent([[ARG_0]]: *mut ()) -> i32;
// CHECK-NEXT:     fn WaitForSingleObject([[ARG_0]]: *mut (), [[ARG_1:arg[0-9]+]]: u32) -> u32;
// CHECK-NEXT:     fn CreateEventW(
// CHECK-NEXT:         [[ARG_0]]: *mut _SECURITY_ATTRIBUTES,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: i32,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *const u16,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     fn GetCurrentProcessId() -> u32;
// CHECK-NEXT:     fn GetSystemTime([[ARG_0]]: *mut _SYSTEMTIME);
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _getpid() -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _getpid() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _SECURITY_ATTRIBUTES {
// CHECK-NEXT:     nLength: u32,
// CHECK-NEXT:     lpSecurityDescriptor: *mut (),
// CHECK-NEXT:     bInheritHandle: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _SYSTEMTIME {
// CHECK-NEXT:     wYear: u16,
// CHECK-NEXT:     wMonth: u16,
// CHECK-NEXT:     wDayOfWeek: u16,
// CHECK-NEXT:     wDay: u16,
// CHECK-NEXT:     wHour: u16,
// CHECK-NEXT:     wMinute: u16,
// CHECK-NEXT:     wSecond: u16,
// CHECK-NEXT:     wMilliseconds: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-EMPTY:
// CHECK-NEXT: fn current_process_id() -> u32 {
// CHECK-NEXT:     unsafe { GetCurrentProcessId() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let process_id: Option<unsafe extern "C-unwind" fn() -> u32> = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> u32>>(
// I686-PC-WINDOWS-MSVC-NEXT:     let process_id: Option<unsafe extern "stdcall-unwind" fn() -> u32> = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "stdcall-unwind" fn() -> u32>>(
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let process_id: Option<unsafe extern "C-unwind" fn() -> u32> = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> u32>>(
// CHECK-NEXT:             GetCurrentProcessId as *const (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = current_process_id() != ((unsafe { _getpid() }) as u32)
// CHECK-NEXT:         || (unsafe { process_id.unwrap()() }) != current_process_id();
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut time: _SYSTEMTIME = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { GetSystemTime(std::ptr::addr_of_mut!(time)) };
// CHECK-NEXT:     if (time.wYear as i32) < 2020 || (time.wMonth as i32) < 1 || (time.wMonth as i32) > 12 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let event: *mut () = unsafe {
// CHECK-NEXT:         CreateEventW(
// CHECK-NEXT:             std::ptr::null_mut::<_SECURITY_ATTRIBUTES>(),
// CHECK-NEXT:             0i32,
// CHECK-NEXT:             0i32,
// CHECK-NEXT:             std::ptr::null::<u16>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = event.is_null() || (unsafe { SetEvent(event) }) == 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = [[V_1]] || (unsafe { WaitForSingleObject(event, 0u32) }) != 0u32;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { CloseHandle(event) }) == 0 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
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
