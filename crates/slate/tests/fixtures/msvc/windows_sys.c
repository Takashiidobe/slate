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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn current_process_id() -> u32 {
// CHECK-NEXT:     unsafe { GetCurrentProcessId() as u32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
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
// CHECK-NEXT:     let mut time: slate_support::windows::Win32::Foundation::SYSTEMTIME =
// CHECK-NEXT:         unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         GetSystemTime(std::ptr::addr_of_mut!(time)
// CHECK-NEXT:             as *mut slate_support::windows::Win32::Foundation::SYSTEMTIME)
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((time.wYear as u32) as i32) < 2020
// CHECK-NEXT:         || ((time.wMonth as u32) as i32) < 1
// CHECK-NEXT:         || ((time.wMonth as u32) as i32) > 12
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let event: *mut () = unsafe {
// CHECK-NEXT:         CreateEventW(
// CHECK-NEXT:             std::ptr::null_mut::<slate_support::windows::Win32::Security::SECURITY_ATTRIBUTES>(),
// CHECK-NEXT:             slate_support::windows::Win32::Foundation::FALSE as i32,
// CHECK-NEXT:             slate_support::windows::Win32::Foundation::FALSE as i32,
// CHECK-NEXT:             std::ptr::null::<u16>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool =
// CHECK-NEXT:         event.is_null() || (unsafe { SetEvent(event as *mut core::ffi::c_void) as i32 }) == 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = [[V_1]]
// CHECK-NEXT:         || (unsafe { WaitForSingleObject(event as *mut core::ffi::c_void, 0u32 as u32) as u32 })
// CHECK-NEXT:             != 0u32;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { CloseHandle(event as *mut core::ffi::c_void) as i32 }) == 0 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(windows))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(windows))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// I686-PC-WINDOWS-MSVC-NEXT:     fn _getpid() -> i32;
// I686-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-NEXT: unsafe extern "stdcall" {
// CHECK-NEXT:     fn CreateEventW(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *mut slate_support::windows::Win32::Security::SECURITY_ATTRIBUTES,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: i32,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: i32,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *const u16,
// CHECK-NEXT:     ) -> *mut ();
// X86_64-PC-WINDOWS-MSVC-NEXT:     fn _getpid() -> i32;
// AARCH64-PC-WINDOWS-MSVC-NEXT:     fn _getpid() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::abort;
// CHECK-NEXT: use slate_support::windows::Win32::Foundation::CloseHandle;
// CHECK-NEXT: use slate_support::windows::Win32::System::SystemInformation::GetSystemTime;
// CHECK-NEXT: use slate_support::windows::Win32::System::Threading::GetCurrentProcessId;
// CHECK-NEXT: use slate_support::windows::Win32::System::Threading::SetEvent;
// CHECK-NEXT: use slate_support::windows::Win32::System::Threading::WaitForSingleObject;
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
