// merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang linux-aarch64 aarch64-unknown-linux-gnu clang
#include <stdint.h>
#include <stdio.h>

struct code {
    unsigned short freq;
    unsigned short len;
};

static const struct code table[8] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}};
static const struct code *const descriptor = table;
const unsigned char exported[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
static const unsigned char small[15] = {1, 2, 3};
static const long crossing[3] = {1, 2, 3};
_Alignas(16) static const char explicit_large[32] = "explicit";
_Alignas(32) static const char larger[32] = "larger";
_Alignas(1) static const char opted_out[32] = "opted out";

int read_table(int index) {
    return table[index].len + descriptor[index].freq;
}

int read_local(int seed) {
    int local[8];
    for (int i = 0; i < 8; i++) {
        local[i] = seed + i;
    }
    int *escaped = local;
    return escaped[3] + local[7];
}

int read_crossing(void) {
    return (int)crossing[1];
}

int main(void) {
    printf("%d %d\n", read_table(3), read_local(10));
    printf("%d %d %d\n", exported[15], small[1], read_crossing());
    printf("%s %s %s\n", explicit_large, larger, opted_out);
    if ((uintptr_t)explicit_large % 16 != 0 || (uintptr_t)larger % 32 != 0) {
        return 1;
    }
#if defined(__x86_64__)
    if ((uintptr_t)table % 16 != 0 || (uintptr_t)exported % 16 != 0) {
        return 2;
    }
#endif
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   large_arrays.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   large_arrays.c: [] 1 compiled
// CHECK-NEXT: target linux-i686: 1 compiles
// CHECK-NEXT:   large_arrays.c: [] 1 compiled
// CHECK-NEXT: target linux-aarch64: 1 compiles
// CHECK-NEXT:   large_arrays.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 4 targets
// CHECK-NEXT: unit large_arrays: 42 items, 28 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:read_crossing: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 4 variants
// CHECK-NEXT: projection: 24 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang linux-aarch64 aarch64-unknown-linux-gnu clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "aarch64", target_os = "linux", target_env = "gnu")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __va_start([[ARG_0:arg[0-9]+]]: *mut *mut c_char, ...);
// CHECK-NEXT:     fn __acrt_iob_func([[ARG_0]]: u32) -> *mut _iobuf;
// CHECK-NEXT:     fn __stdio_common_vfprintf(
// CHECK-NEXT:         [[ARG_0]]: u64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *mut _iobuf,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: *const c_char,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut __crt_locale_pointers,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: *mut c_char,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0]]: *const c_char, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: #[cfg(not(target_arch = "x86"))]
// CHECK-NEXT: use slate_support::uintptr_t;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_pointers {
// CHECK-NEXT:     locinfo: *mut __crt_locale_data,
// CHECK-NEXT:     mbcinfo: *mut __crt_multibyte_data,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_multibyte_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn __local_stdio_printf_options() -> *mut u64 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(_OptionsStorage) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: static mut _OptionsStorage: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct code {
// CHECK-NEXT:     freq: u16,
// CHECK-NEXT:     len: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(target_arch = "x86_64"))]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "x86_64")]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(target_arch = "x86_64"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _iobuf {
// CHECK-NEXT:     _Placeholder: *mut (),
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn _vfprintf_l(
// CHECK-NEXT:     _Stream: *mut _iobuf,
// CHECK-NEXT:     _Format: *const c_char,
// CHECK-NEXT:     _Locale: *mut __crt_locale_pointers,
// CHECK-NEXT:     _ArgList: *mut c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __stdio_common_vfprintf(
// CHECK-NEXT:             *__local_stdio_printf_options(),
// CHECK-NEXT:             _Stream,
// CHECK-NEXT:             _Format,
// CHECK-NEXT:             _Locale,
// CHECK-NEXT:             _ArgList,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C-unwind" fn printf(_Format: *const c_char, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut _Result: i32 = 0;
// CHECK-NEXT:     let mut _ArgList: *mut c_char = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(_ArgList) =
// CHECK-NEXT:             core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone());
// CHECK-NEXT:     }
// CHECK-NEXT:     _Result = _vfprintf_l(
// CHECK-NEXT:         unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:         _Format,
// CHECK-NEXT:         std::ptr::null_mut::<__crt_locale_pointers>(),
// CHECK-NEXT:         _ArgList,
// CHECK-NEXT:     );
// CHECK-NEXT:     _ArgList = std::ptr::null_mut::<c_char>();
// CHECK-NEXT:     _Result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut table: __SlateAlignLargeArray<[code; 8]> = __SlateAlignLargeArray([
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 1u16,
// CHECK-NEXT:         len: 2u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 3u16,
// CHECK-NEXT:         len: 4u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 5u16,
// CHECK-NEXT:         len: 6u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 7u16,
// CHECK-NEXT:         len: 8u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 9u16,
// CHECK-NEXT:         len: 10u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 11u16,
// CHECK-NEXT:         len: 12u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 13u16,
// CHECK-NEXT:         len: 14u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         freq: 15u16,
// CHECK-NEXT:         len: 16u16,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut descriptor: *const code = unsafe { std::ptr::addr_of!(table.0) as *const code };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut exported: __SlateAlignLargeArray<[u8; 16]> = __SlateAlignLargeArray([
// CHECK-NEXT:     1u8, 2u8, 3u8, 4u8, 5u8, 6u8, 7u8, 8u8, 9u8, 10u8, 11u8, 12u8, 13u8, 14u8, 15u8, 16u8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut small: [u8; 15] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [u8; 15] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = 1u8;
// CHECK-NEXT:     [[T_0]][1] = 2u8;
// CHECK-NEXT:     [[T_0]][2] = 3u8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_pointer_width = "64", unix))]
// CHECK-NEXT: static mut crossing: __SlateAlignLargeArray<[c_long; 3]> =
// CHECK-NEXT:     __SlateAlignLargeArray([(1 as c_long), (2 as c_long), (3 as c_long)]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(windows, target_arch = "x86"))]
// CHECK-NEXT: static mut crossing: [c_long; 3] = [1, 2, 3];
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "x86_64")]
// CHECK-NEXT: static mut explicit_large: __SlateAlignLargeArray<[c_char; 32]> = __SlateAlignLargeArray([
// CHECK-NEXT:     101i8, 120i8, 112i8, 108i8, 105i8, 99i8, 105i8, 116i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:     0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "x86")]
// CHECK-NEXT: static mut explicit_large: __SlateAlign16<[c_char; 32]> = __SlateAlign16([
// CHECK-NEXT:     101i8, 120i8, 112i8, 108i8, 105i8, 99i8, 105i8, 116i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:     0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "aarch64")]
// CHECK-NEXT: static mut explicit_large: __SlateAlign16<[c_char; 32]> = __SlateAlign16([
// CHECK-NEXT:     101u8, 120u8, 112u8, 108u8, 105u8, 99u8, 105u8, 116u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8,
// CHECK-NEXT:     0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(target_arch = "aarch64"))]
// CHECK-NEXT: static mut larger: __SlateAlign32<[c_char; 32]> = __SlateAlign32([
// CHECK-NEXT:     108i8, 97i8, 114i8, 103i8, 101i8, 114i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:     0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "aarch64")]
// CHECK-NEXT: static mut larger: __SlateAlign32<[c_char; 32]> = __SlateAlign32([
// CHECK-NEXT:     108u8, 97u8, 114u8, 103u8, 101u8, 114u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8,
// CHECK-NEXT:     0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "x86_64")]
// CHECK-NEXT: static mut opted_out: [c_char; 32] = [
// CHECK-NEXT:     111i8, 112i8, 116i8, 101i8, 100i8, 32i8, 111i8, 117i8, 116i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:     0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT: ];
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "x86")]
// CHECK-NEXT: static mut opted_out: __SlateAlignLargeArray<[c_char; 32]> = __SlateAlignLargeArray([
// CHECK-NEXT:     111i8, 112i8, 116i8, 101i8, 100i8, 32i8, 111i8, 117i8, 116i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:     0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "aarch64")]
// CHECK-NEXT: static mut opted_out: __SlateAlignLargeArray<[c_char; 32]> = __SlateAlignLargeArray([
// CHECK-NEXT:     111u8, 112u8, 116u8, 101u8, 100u8, 32u8, 111u8, 117u8, 116u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8,
// CHECK-NEXT:     0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8, 0u8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn read_table(index: i32) -> i32 {
// CHECK-NEXT:     (((unsafe { table.0[index as usize].len }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { (*unsafe { descriptor }.offset(index as isize)).freq }) as u32) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn read_local(seed: i32) -> i32 {
// CHECK-NEXT:     let mut local: __SlateAlignLargeArray<[i32; 8]> = __SlateAlignLargeArray([0i32; 8]);
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         local.0[i as usize] = seed + i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let escaped: *mut i32 = local.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     (unsafe { *escaped.add(3) }) + local.0[7]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_pointer_width = "64", unix))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn read_crossing() -> i32 {
// CHECK-NEXT:     (unsafe { crossing.0[1] }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(windows, target_arch = "x86"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn read_crossing() -> i32 {
// CHECK-NEXT:     unsafe { crossing[1] }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d %d\n".as_ptr(), read_table(3i32), read_local(10i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 (exported.0[15] as u32) as i32,
// CHECK-NEXT:                 (small[1] as u32) as i32,
// CHECK-NEXT:                 read_crossing(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %s %s\n".as_ptr(),
// CHECK-NEXT:                 std::ptr::addr_of!(explicit_large.0) as *const c_char,
// CHECK-NEXT:                 std::ptr::addr_of!(larger.0) as *const c_char,
// CHECK-NEXT:                 std::ptr::addr_of!(opted_out) as *const c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", read_table(3i32), read_local(10i32));
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             ((unsafe { exported.0[15] }) as u32) as i32,
// CHECK-NEXT:             ((unsafe { small[1] }) as u32) as i32,
// CHECK-NEXT:             read_crossing()
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", unix))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %s %s\n".as_ptr(),
// CHECK-NEXT:                 std::ptr::addr_of!(explicit_large.0) as *const c_char,
// CHECK-NEXT:                 std::ptr::addr_of!(larger.0) as *const c_char,
// CHECK-NEXT:                 std::ptr::addr_of!(opted_out) as *const c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(target_arch = "x86_64"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %s %s\n".as_ptr(),
// CHECK-NEXT:                 std::ptr::addr_of!(explicit_large.0) as *const c_char,
// CHECK-NEXT:                 std::ptr::addr_of!(larger.0) as *const c_char,
// CHECK-NEXT:                 std::ptr::addr_of!(opted_out.0) as *const c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(not(target_arch = "x86"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if ((unsafe { std::ptr::addr_of!(explicit_large.0) as *const c_char }) as uintptr_t) % 16
// CHECK-NEXT:             != 0
// CHECK-NEXT:             || ((unsafe { std::ptr::addr_of!(larger.0) as *const c_char }) as uintptr_t) % 32 != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", unix))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if ((unsafe { std::ptr::addr_of!(table.0) as *const code }) as uintptr_t) % 16 != 0
// CHECK-NEXT:             || ((unsafe { std::ptr::addr_of!(exported.0) as *const u8 }) as uintptr_t) % 16 != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_arch = "x86")]
// CHECK-NEXT:     {
// CHECK-NEXT:         if ((unsafe { std::ptr::addr_of!(explicit_large.0) as *const c_char }) as u32) % 16 != 0
// CHECK-NEXT:             || ((unsafe { std::ptr::addr_of!(larger.0) as *const c_char }) as u32) % 32 != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
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
