void abort(void);
typedef void *HWND;
typedef unsigned int UINT;
#ifdef _WIN64
typedef unsigned __int64 WPARAM; typedef __int64 LPARAM; typedef __int64 LRESULT;
#else
typedef unsigned int WPARAM; typedef long LPARAM; typedef long LRESULT;
#endif
#define CALLBACK __stdcall
typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef struct { UINT style; WNDPROC lpfnWndProc; int cbClsExtra; } WNDCLASSA;
typedef void (*FARPROC)(void);

static LRESULT CALLBACK DefDlgProcA(HWND h, UINT m, WPARAM w, LPARAM l) { return (LRESULT)m + (LRESULT)w + l; }

int main(void) {
    WNDCLASSA wc;
    wc.style = 0;
    wc.lpfnWndProc = DefDlgProcA;
    FARPROC raw = (FARPROC)DefDlgProcA;
    WNDPROC back = (WNDPROC)raw;
    if (wc.lpfnWndProc(0, 1, 2, 3) != 6 || back(0, 1, 2, 3) != 6)
        abort();
    return 0;
}

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
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_callback_struct_field_c_11 {
// CHECK-NEXT:     style: u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:     lpfnWndProc: Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64>,
// I686-PC-WINDOWS-MSVC-NEXT:     lpfnWndProc: Option<unsafe extern "stdcall-unwind" fn(*mut (), u32, u32, i32) -> i32>,
// AARCH64-PC-WINDOWS-MSVC-NEXT:     lpfnWndProc: Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64>,
// CHECK-NEXT:     cbClsExtra: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: extern "C-unwind" fn DefDlgProcA(h: *mut (), m: u32, w: u64, l: i64) -> i64 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     (m as i64) + (w as i64) + l
// I686-PC-WINDOWS-MSVC-NEXT: extern "stdcall-unwind" fn DefDlgProcA(h: *mut (), m: u32, w: u32, l: i32) -> i32 {
// I686-PC-WINDOWS-MSVC-NEXT:     (m as i32) + (w as i32) + l
// AARCH64-PC-WINDOWS-MSVC-NEXT: extern "C-unwind" fn DefDlgProcA(h: *mut (), m: u32, w: u64, l: i64) -> i64 {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     (m as i64) + (w as i64) + l
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut wc: __SlateRecord_callback_struct_field_c_11 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     wc.style = 0;
// CHECK-NEXT:     wc.lpfnWndProc = Some(DefDlgProcA);
// CHECK-NEXT:     let raw: Option<unsafe extern "C-unwind" fn()> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64>,
// I686-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "stdcall-unwind" fn(*mut (), u32, u32, i32) -> i32>,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64>,
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn()>,
// CHECK-NEXT:         >(Some(DefDlgProcA))
// CHECK-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let back: Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64> = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let back: Option<unsafe extern "stdcall-unwind" fn(*mut (), u32, u32, i32) -> i32> = unsafe {
// AARCH64-PC-WINDOWS-MSVC-NEXT:     let back: Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn()>,
// X86_64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64>,
// I686-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "stdcall-unwind" fn(*mut (), u32, u32, i32) -> i32>,
// AARCH64-PC-WINDOWS-MSVC-NEXT:             Option<unsafe extern "C-unwind" fn(*mut (), u32, u64, i64) -> i64>,
// CHECK-NEXT:         >(raw)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool =
// X86_64-PC-WINDOWS-MSVC-NEXT:         (unsafe { wc.lpfnWndProc.unwrap()(std::ptr::null_mut::<()>(), 1u32, 2u64, 3i64) }) != 6
// X86_64-PC-WINDOWS-MSVC-NEXT:             || (unsafe { back.unwrap()(std::ptr::null_mut::<()>(), 1u32, 2u64, 3i64) }) != 6;
// I686-PC-WINDOWS-MSVC-NEXT:         (unsafe { wc.lpfnWndProc.unwrap()(std::ptr::null_mut::<()>(), 1u32, 2u32, 3i32) }) != 6
// I686-PC-WINDOWS-MSVC-NEXT:             || (unsafe { back.unwrap()(std::ptr::null_mut::<()>(), 1u32, 2u32, 3i32) }) != 6;
// AARCH64-PC-WINDOWS-MSVC-NEXT:         (unsafe { wc.lpfnWndProc.unwrap()(std::ptr::null_mut::<()>(), 1u32, 2u64, 3i64) }) != 6
// AARCH64-PC-WINDOWS-MSVC-NEXT:             || (unsafe { back.unwrap()(std::ptr::null_mut::<()>(), 1u32, 2u64, 3i64) }) != 6;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         unsafe { abort() };
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
