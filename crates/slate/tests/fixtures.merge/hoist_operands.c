#include <stdio.h>

#ifdef _WIN32
#define NUMBER 387
#define NAME "windows"
#else
#define NUMBER 295
#define NAME "unix"
#endif

int number(void) { return NUMBER; }

int in_cast(void) { return (int)(unsigned long long)NUMBER; }

int repeated(int value) { return value == NUMBER ? NUMBER : 0; }

const char *name(void) { return NAME; }

long same_type_everywhere(long value) {
    long doubled = value + value;
    return doubled;
}

#ifdef ALPHA
int gated(void) { return NUMBER + 1; }
#endif

int main(void) {
    puts(name());
#ifdef ALPHA
    if (gated() != NUMBER + 1)
        return 1;
#endif
    return number() + in_cast() + repeated(NUMBER) - 3 * NUMBER +
           (int)same_type_everywhere(2) - 4;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   hoist_operands.c: [alpha] 2 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   hoist_operands.c: [alpha] 2 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit hoist_operands: 21 items, 13 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:repeated: 2 variants
// CHECK-NEXT:   hoisted fn:number: 2 variants
// CHECK-NEXT:   hoisted fn:in_cast: 2 variants
// CHECK-NEXT:   hoisted fn:name: 2 variants
// CHECK-NEXT:   hoisted fn:same_type_everywhere: 2 variants
// CHECK-NEXT:   hoisted fn:gated: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 4 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn puts([[ARG_0:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: const __SLATE_HOIST_number: i32 = 295;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: const __SLATE_HOIST_number: i32 = 387;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn number() -> i32 {
// CHECK-NEXT:     __SLATE_HOIST_number
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: const __SLATE_HOIST_in_cast: i32 = 295;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: const __SLATE_HOIST_in_cast: i32 = 387;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn in_cast() -> i32 {
// CHECK-NEXT:     __SLATE_HOIST_in_cast
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn repeated(value: i32) -> i32 {
// CHECK-NEXT:     if value == 295 { 295 } else { 0 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn repeated(value: i32) -> i32 {
// CHECK-NEXT:     if value == 387 { 387 } else { 0 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: const __SLATE_HOIST_name: &core::ffi::CStr = c"unix";
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: const __SLATE_HOIST_name: &core::ffi::CStr = c"windows";
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn name() -> *const i8 {
// CHECK-NEXT:     __SLATE_HOIST_name.as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: type __SlateHoist_same_type_everywhere = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: type __SlateHoist_same_type_everywhere = i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn same_type_everywhere(
// CHECK-NEXT:     value: __SlateHoist_same_type_everywhere,
// CHECK-NEXT: ) -> __SlateHoist_same_type_everywhere {
// CHECK-NEXT:     let doubled: __SlateHoist_same_type_everywhere = value + value;
// CHECK-NEXT:     doubled
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: const __SLATE_HOIST_gated: i32 = 295;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     windows,
// CHECK-NEXT:     target_env = "msvc",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: const __SLATE_HOIST_gated: i32 = 387;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "alpha")]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn gated() -> i32 {
// CHECK-NEXT:     __SLATE_HOIST_gated + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { puts(name()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         target_os = "linux",
// CHECK-NEXT:         target_env = "gnu",
// CHECK-NEXT:         feature = "alpha"
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if gated() != 295 + 1 {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         number() + in_cast() + repeated(295i32) - 3 * 295 + (same_type_everywhere(2i64) as i32) - 4
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         windows,
// CHECK-NEXT:         target_env = "msvc",
// CHECK-NEXT:         feature = "alpha"
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if gated() != 387 + 1 {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         number() + in_cast() + repeated(387i32) - 3 * 387 + same_type_everywhere(2i32) - 4
// CHECK-NEXT:     }
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
