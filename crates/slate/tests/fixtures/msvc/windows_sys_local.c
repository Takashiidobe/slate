#ifdef _WIN32
void abort(void);
typedef unsigned long DWORD;
typedef struct _SYSTEMTIME { unsigned value; } SYSTEMTIME;

static DWORD __stdcall GetCurrentProcessId(void) { return 42; }
static void GetSystemTime(SYSTEMTIME *time) { time->value = 123; }

int main(void) {
    SYSTEMTIME time;
    GetSystemTime(&time);
    if (GetCurrentProcessId() != 42 || time.value != 123)
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
// CHECK-NEXT: #[cfg(windows)]
// X86_64-PC-WINDOWS-MSVC-NEXT: fn GetCurrentProcessId() -> u32 {
// I686-PC-WINDOWS-MSVC-NEXT: extern "stdcall-unwind" fn GetCurrentProcessId() -> u32 {
// AARCH64-PC-WINDOWS-MSVC-NEXT: fn GetCurrentProcessId() -> u32 {
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn GetSystemTime(time: &mut _SYSTEMTIME) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(time as *mut _SYSTEMTIME)).value = 123;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut time: _SYSTEMTIME = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     GetSystemTime(unsafe { &mut (*std::ptr::addr_of_mut!(time)) });
// CHECK-NEXT:     if GetCurrentProcessId() != 42 || time.value != 123 {
// CHECK-NEXT:         unsafe { abort() };
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
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _SYSTEMTIME {
// CHECK-NEXT:     value: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor" }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
