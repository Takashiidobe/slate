#include <locale.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

int bionic_stream_position(FILE *stream, fpos_t *pos) {
  if (fgetpos(stream, pos) != 0)
    return -1;
  return fsetpos(stream, pos);
}

int bionic_multibyte_roundtrip(const char *input, char *output) {
  mbstate_t decode_state;
  mbstate_t encode_state;
  wchar_t   wide;
  __builtin_memset(&decode_state, 0, sizeof(decode_state));
  __builtin_memset(&encode_state, 0, sizeof(encode_state));
  if (mbrtowc(&wide, input, 1, &decode_state) == (size_t)-1)
    return -1;
  return (int)wcrtomb(output, wide, &encode_state);
}

int bionic_locale_scope(void) {
  locale_t updated = newlocale(LC_ALL_MASK, "C", (locale_t)0);
  locale_t previous = uselocale(updated);
  int      is_alpha = iswctype(L'a', wctype("alpha"));
  freelocale(previous);
  return is_alpha;
}

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64: #![allow(
// LOWERING-BIONIC-AARCH64-NEXT:     dead_code,
// LOWERING-BIONIC-AARCH64-NEXT:     unused,
// LOWERING-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-AARCH64-NEXT:     non_snake_case,
// LOWERING-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-AARCH64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-AARCH64-NEXT:     unused_comparisons
// LOWERING-BIONIC-AARCH64-NEXT: )]
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: #[repr(C)]
// LOWERING-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-AARCH64-NEXT: struct __locale_map {}
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: #[repr(C)]
// LOWERING-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-AARCH64-NEXT: struct __locale_struct {
// LOWERING-BIONIC-AARCH64-NEXT:     cat: [*mut __locale_map; 6],
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: #[repr(C)]
// LOWERING-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-AARCH64-NEXT: struct __mbstate_t {
// LOWERING-BIONIC-AARCH64-NEXT:     __seq: [u8; 4],
// LOWERING-BIONIC-AARCH64-NEXT:     __reserved: [u8; 4],
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-AARCH64-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// LOWERING-BIONIC-AARCH64-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// LOWERING-BIONIC-AARCH64-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// LOWERING-BIONIC-AARCH64-NEXT:     fn mbrtowc(
// LOWERING-BIONIC-AARCH64-NEXT:         _0: *mut u32,
// LOWERING-BIONIC-AARCH64-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:         _2: usize,
// LOWERING-BIONIC-AARCH64-NEXT:         _3: *mut __mbstate_t,
// LOWERING-BIONIC-AARCH64-NEXT:     ) -> usize;
// LOWERING-BIONIC-AARCH64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-BIONIC-AARCH64-NEXT:     fn newlocale(
// LOWERING-BIONIC-AARCH64-NEXT:         _0: i32,
// LOWERING-BIONIC-AARCH64-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:         _2: *mut __locale_struct,
// LOWERING-BIONIC-AARCH64-NEXT:     ) -> *mut __locale_struct;
// LOWERING-BIONIC-AARCH64-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// LOWERING-BIONIC-AARCH64-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// LOWERING-BIONIC-AARCH64-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// LOWERING-BIONIC-AARCH64-NEXT:     fn freelocale(_0: *mut __locale_struct);
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn bionic_stream_position({{arg[0-9]+}}: *mut libc::FILE, {{arg[0-9]+}}: *mut i64) -> i32 {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut stream: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-BIONIC-AARCH64-NEXT:     let mut pos: *mut i64 = std::ptr::null_mut();
// LOWERING-BIONIC-AARCH64-NEXT:     let mut __retval: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     pos = {{arg[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     {
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = stream;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: *mut i64 = pos;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { fgetpos({{_v[0-9]+}} as *mut libc::FILE, {{_v[0-9]+}} as *mut i64) };
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:         if {{_v[0-9]+}} {
// LOWERING-BIONIC-AARCH64-NEXT:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-BIONIC-AARCH64-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-AARCH64-NEXT:             return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:         }
// LOWERING-BIONIC-AARCH64-NEXT:     }
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = stream;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut i64 = pos;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fsetpos({{_v[0-9]+}} as *mut libc::FILE, {{_v[0-9]+}} as *const i64) };
// LOWERING-BIONIC-AARCH64-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut input: *mut u8 = std::ptr::null_mut();
// LOWERING-BIONIC-AARCH64-NEXT:     let mut __retval: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// LOWERING-BIONIC-AARCH64-NEXT:         __seq: [0; 4],
// LOWERING-BIONIC-AARCH64-NEXT:         __reserved: [0; 4],
// LOWERING-BIONIC-AARCH64-NEXT:     };
// LOWERING-BIONIC-AARCH64-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// LOWERING-BIONIC-AARCH64-NEXT:         __seq: [0; 4],
// LOWERING-BIONIC-AARCH64-NEXT:         __reserved: [0; 4],
// LOWERING-BIONIC-AARCH64-NEXT:     };
// LOWERING-BIONIC-AARCH64-NEXT:     let mut wide: u32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     input = {{arg[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-AARCH64-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-AARCH64-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as u64) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-AARCH64-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-AARCH64-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as u64) };
// LOWERING-BIONIC-AARCH64-NEXT:     {
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: *mut u8 = input;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: u64 = 1;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-BIONIC-AARCH64-NEXT:             mbrtowc(
// LOWERING-BIONIC-AARCH64-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut u32,
// LOWERING-BIONIC-AARCH64-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:                 {{_v[0-9]+}} as usize,
// LOWERING-BIONIC-AARCH64-NEXT:                 std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// LOWERING-BIONIC-AARCH64-NEXT:             )
// LOWERING-BIONIC-AARCH64-NEXT:         }) as u64;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: u64 = 18446744073709551615u64;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:         if {{_v[0-9]+}} {
// LOWERING-BIONIC-AARCH64-NEXT:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-BIONIC-AARCH64-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-AARCH64-NEXT:             return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:         }
// LOWERING-BIONIC-AARCH64-NEXT:     }
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u32 = wide;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-BIONIC-AARCH64-NEXT:         wcrtomb(
// LOWERING-BIONIC-AARCH64-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as u32,
// LOWERING-BIONIC-AARCH64-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// LOWERING-BIONIC-AARCH64-NEXT:         )
// LOWERING-BIONIC-AARCH64-NEXT:     }) as u64;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-BIONIC-AARCH64-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn bionic_locale_scope() -> i32 {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 2147483647;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = b"C\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe {
// LOWERING-BIONIC-AARCH64-NEXT:         newlocale(
// LOWERING-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as *mut __locale_struct,
// LOWERING-BIONIC-AARCH64-NEXT:         )
// LOWERING-BIONIC-AARCH64-NEXT:     };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{_v[0-9]+}} as *mut __locale_struct) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u32 = 97;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut u8 = b"alpha\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { wctype({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { iswctype({{_v[0-9]+}} as u32, {{_v[0-9]+}} as i64) };
// LOWERING-BIONIC-AARCH64-NEXT:     unsafe { freelocale({{_v[0-9]+}} as *mut __locale_struct) };
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64: #![allow(
// LOWERING-BIONIC-X86_64-NEXT:     dead_code,
// LOWERING-BIONIC-X86_64-NEXT:     unused,
// LOWERING-BIONIC-X86_64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-X86_64-NEXT:     non_snake_case,
// LOWERING-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-X86_64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-X86_64-NEXT:     unused_comparisons
// LOWERING-BIONIC-X86_64-NEXT: )]
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: #[repr(C)]
// LOWERING-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-X86_64-NEXT: struct __locale_map {}
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: #[repr(C)]
// LOWERING-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-X86_64-NEXT: struct __locale_struct {
// LOWERING-BIONIC-X86_64-NEXT:     cat: [*mut __locale_map; 6],
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: #[repr(C)]
// LOWERING-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// LOWERING-BIONIC-X86_64-NEXT: struct __mbstate_t {
// LOWERING-BIONIC-X86_64-NEXT:     __seq: [u8; 4],
// LOWERING-BIONIC-X86_64-NEXT:     __reserved: [u8; 4],
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-X86_64-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// LOWERING-BIONIC-X86_64-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// LOWERING-BIONIC-X86_64-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// LOWERING-BIONIC-X86_64-NEXT:     fn mbrtowc(
// LOWERING-BIONIC-X86_64-NEXT:         _0: *mut i32,
// LOWERING-BIONIC-X86_64-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:         _2: usize,
// LOWERING-BIONIC-X86_64-NEXT:         _3: *mut __mbstate_t,
// LOWERING-BIONIC-X86_64-NEXT:     ) -> usize;
// LOWERING-BIONIC-X86_64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: i32, _2: *mut __mbstate_t) -> usize;
// LOWERING-BIONIC-X86_64-NEXT:     fn newlocale(
// LOWERING-BIONIC-X86_64-NEXT:         _0: i32,
// LOWERING-BIONIC-X86_64-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:         _2: *mut __locale_struct,
// LOWERING-BIONIC-X86_64-NEXT:     ) -> *mut __locale_struct;
// LOWERING-BIONIC-X86_64-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// LOWERING-BIONIC-X86_64-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// LOWERING-BIONIC-X86_64-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// LOWERING-BIONIC-X86_64-NEXT:     fn freelocale(_0: *mut __locale_struct);
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn bionic_stream_position({{arg[0-9]+}}: *mut libc::FILE, {{arg[0-9]+}}: *mut i64) -> i32 {
// LOWERING-BIONIC-X86_64-NEXT:     let mut stream: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-BIONIC-X86_64-NEXT:     let mut pos: *mut i64 = std::ptr::null_mut();
// LOWERING-BIONIC-X86_64-NEXT:     let mut __retval: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     pos = {{arg[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     {
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = stream;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: *mut i64 = pos;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { fgetpos({{_v[0-9]+}} as *mut libc::FILE, {{_v[0-9]+}} as *mut i64) };
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:         if {{_v[0-9]+}} {
// LOWERING-BIONIC-X86_64-NEXT:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-BIONIC-X86_64-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-X86_64-NEXT:             return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:         }
// LOWERING-BIONIC-X86_64-NEXT:     }
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = stream;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i64 = pos;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fsetpos({{_v[0-9]+}} as *mut libc::FILE, {{_v[0-9]+}} as *const i64) };
// LOWERING-BIONIC-X86_64-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-BIONIC-X86_64-NEXT:     let mut input: *mut i8 = std::ptr::null_mut();
// LOWERING-BIONIC-X86_64-NEXT:     let mut __retval: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// LOWERING-BIONIC-X86_64-NEXT:         __seq: [0; 4],
// LOWERING-BIONIC-X86_64-NEXT:         __reserved: [0; 4],
// LOWERING-BIONIC-X86_64-NEXT:     };
// LOWERING-BIONIC-X86_64-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// LOWERING-BIONIC-X86_64-NEXT:         __seq: [0; 4],
// LOWERING-BIONIC-X86_64-NEXT:         __reserved: [0; 4],
// LOWERING-BIONIC-X86_64-NEXT:     };
// LOWERING-BIONIC-X86_64-NEXT:     let mut wide: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     input = {{arg[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-X86_64-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-X86_64-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as u64) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-X86_64-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-BIONIC-X86_64-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as u64) };
// LOWERING-BIONIC-X86_64-NEXT:     {
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: *mut i8 = input;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: u64 = 1;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-BIONIC-X86_64-NEXT:             mbrtowc(
// LOWERING-BIONIC-X86_64-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut i32,
// LOWERING-BIONIC-X86_64-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:                 {{_v[0-9]+}} as usize,
// LOWERING-BIONIC-X86_64-NEXT:                 std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// LOWERING-BIONIC-X86_64-NEXT:             )
// LOWERING-BIONIC-X86_64-NEXT:         }) as u64;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: u64 = 18446744073709551615u64;
// LOWERING-BIONIC-X86_64-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:         if {{_v[0-9]+}} {
// LOWERING-BIONIC-X86_64-NEXT:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-BIONIC-X86_64-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-X86_64-NEXT:             return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:         }
// LOWERING-BIONIC-X86_64-NEXT:     }
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = wide;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-BIONIC-X86_64-NEXT:         wcrtomb(
// LOWERING-BIONIC-X86_64-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-BIONIC-X86_64-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// LOWERING-BIONIC-X86_64-NEXT:         )
// LOWERING-BIONIC-X86_64-NEXT:     }) as u64;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-BIONIC-X86_64-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn bionic_locale_scope() -> i32 {
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 2147483647;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"C\0".as_ptr() as *mut i8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe {
// LOWERING-BIONIC-X86_64-NEXT:         newlocale(
// LOWERING-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as *mut __locale_struct,
// LOWERING-BIONIC-X86_64-NEXT:         )
// LOWERING-BIONIC-X86_64-NEXT:     };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{_v[0-9]+}} as *mut __locale_struct) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: u32 = 97;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"alpha\0".as_ptr() as *mut i8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { wctype({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { iswctype({{_v[0-9]+}} as u32, {{_v[0-9]+}} as i64) };
// LOWERING-BIONIC-X86_64-NEXT:     unsafe { freelocale({{_v[0-9]+}} as *mut __locale_struct) };
// LOWERING-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn main() {
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64: #![allow(
// REWRITES-BIONIC-AARCH64-NEXT:     dead_code,
// REWRITES-BIONIC-AARCH64-NEXT:     unused,
// REWRITES-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-AARCH64-NEXT:     non_snake_case,
// REWRITES-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-AARCH64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-AARCH64-NEXT:     unused_comparisons
// REWRITES-BIONIC-AARCH64-NEXT: )]
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: #[repr(C)]
// REWRITES-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-AARCH64-NEXT: struct __locale_map {}
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: #[repr(C)]
// REWRITES-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-AARCH64-NEXT: struct __locale_struct {
// REWRITES-BIONIC-AARCH64-NEXT:     cat: [*mut __locale_map; 6],
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: #[repr(C)]
// REWRITES-BIONIC-AARCH64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-AARCH64-NEXT: struct __mbstate_t {
// REWRITES-BIONIC-AARCH64-NEXT:     __seq: [u8; 4],
// REWRITES-BIONIC-AARCH64-NEXT:     __reserved: [u8; 4],
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-AARCH64-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// REWRITES-BIONIC-AARCH64-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// REWRITES-BIONIC-AARCH64-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// REWRITES-BIONIC-AARCH64-NEXT:     fn mbrtowc(
// REWRITES-BIONIC-AARCH64-NEXT:         _0: *mut u32,
// REWRITES-BIONIC-AARCH64-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-BIONIC-AARCH64-NEXT:         _2: usize,
// REWRITES-BIONIC-AARCH64-NEXT:         _3: *mut __mbstate_t,
// REWRITES-BIONIC-AARCH64-NEXT:     ) -> usize;
// REWRITES-BIONIC-AARCH64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-BIONIC-AARCH64-NEXT:     fn newlocale(
// REWRITES-BIONIC-AARCH64-NEXT:         _0: i32,
// REWRITES-BIONIC-AARCH64-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-BIONIC-AARCH64-NEXT:         _2: *mut __locale_struct,
// REWRITES-BIONIC-AARCH64-NEXT:     ) -> *mut __locale_struct;
// REWRITES-BIONIC-AARCH64-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// REWRITES-BIONIC-AARCH64-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// REWRITES-BIONIC-AARCH64-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// REWRITES-BIONIC-AARCH64-NEXT:     fn freelocale(_0: *mut __locale_struct);
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn bionic_stream_position({{arg[0-9]+}}: *mut libc::FILE, {{arg[0-9]+}}: *mut i64) -> i32 {
// REWRITES-BIONIC-AARCH64-NEXT:     let mut stream: *mut libc::FILE = {{arg[0-9]+}};
// REWRITES-BIONIC-AARCH64-NEXT:     let mut pos: *mut i64 = {{arg[0-9]+}};
// REWRITES-BIONIC-AARCH64-NEXT:     let mut __retval: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fgetpos(stream as *mut libc::FILE, pos as *mut i64) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     if {{_v[0-9]+}} != {{_v[0-9]+}} {
// REWRITES-BIONIC-AARCH64-NEXT:         __retval = -1;
// REWRITES-BIONIC-AARCH64-NEXT:         return __retval;
// REWRITES-BIONIC-AARCH64-NEXT:     }
// REWRITES-BIONIC-AARCH64-NEXT:     __retval = unsafe { fsetpos(stream as *mut libc::FILE, pos as *const i64) };
// REWRITES-BIONIC-AARCH64-NEXT:     return __retval;
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// REWRITES-BIONIC-AARCH64-NEXT:     let mut input: *mut u8 = {{arg[0-9]+}};
// REWRITES-BIONIC-AARCH64-NEXT:     let mut __retval: i32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// REWRITES-BIONIC-AARCH64-NEXT:         __seq: [0; 4],
// REWRITES-BIONIC-AARCH64-NEXT:         __reserved: [0; 4],
// REWRITES-BIONIC-AARCH64-NEXT:     };
// REWRITES-BIONIC-AARCH64-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// REWRITES-BIONIC-AARCH64-NEXT:         __seq: [0; 4],
// REWRITES-BIONIC-AARCH64-NEXT:         __reserved: [0; 4],
// REWRITES-BIONIC-AARCH64-NEXT:     };
// REWRITES-BIONIC-AARCH64-NEXT:     let mut wide: u32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-BIONIC-AARCH64-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-BIONIC-AARCH64-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-BIONIC-AARCH64-NEXT:         mbrtowc(
// REWRITES-BIONIC-AARCH64-NEXT:             std::ptr::addr_of_mut!(wide) as *mut u32,
// REWRITES-BIONIC-AARCH64-NEXT:             input as *const core::ffi::c_char,
// REWRITES-BIONIC-AARCH64-NEXT:             (1 as u64) as usize,
// REWRITES-BIONIC-AARCH64-NEXT:             std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// REWRITES-BIONIC-AARCH64-NEXT:         )
// REWRITES-BIONIC-AARCH64-NEXT:     }) as u64;
// REWRITES-BIONIC-AARCH64-NEXT:     if {{_v[0-9]+}} == 18446744073709551615u64 {
// REWRITES-BIONIC-AARCH64-NEXT:         __retval = -1;
// REWRITES-BIONIC-AARCH64-NEXT:         return __retval;
// REWRITES-BIONIC-AARCH64-NEXT:     }
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-BIONIC-AARCH64-NEXT:         wcrtomb(
// REWRITES-BIONIC-AARCH64-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-BIONIC-AARCH64-NEXT:             wide as u32,
// REWRITES-BIONIC-AARCH64-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// REWRITES-BIONIC-AARCH64-NEXT:         )
// REWRITES-BIONIC-AARCH64-NEXT:     }) as u64;
// REWRITES-BIONIC-AARCH64-NEXT:     __retval = {{_v[0-9]+}} as i32;
// REWRITES-BIONIC-AARCH64-NEXT:     return __retval;
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn bionic_locale_scope() -> i32 {
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe {
// REWRITES-BIONIC-AARCH64-NEXT:         newlocale(
// REWRITES-BIONIC-AARCH64-NEXT:             2147483647 as i32,
// REWRITES-BIONIC-AARCH64-NEXT:             c"C".as_ptr(),
// REWRITES-BIONIC-AARCH64-NEXT:             {{_v[0-9]+}} as *mut __locale_struct,
// REWRITES-BIONIC-AARCH64-NEXT:         )
// REWRITES-BIONIC-AARCH64-NEXT:     };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{_v[0-9]+}} as *mut __locale_struct) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { wctype(c"alpha".as_ptr()) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { iswctype(97 as u32, {{_v[0-9]+}} as i64) };
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { freelocale({{_v[0-9]+}} as *mut __locale_struct) };
// REWRITES-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit(0 as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64: #![allow(
// REWRITES-BIONIC-X86_64-NEXT:     dead_code,
// REWRITES-BIONIC-X86_64-NEXT:     unused,
// REWRITES-BIONIC-X86_64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-X86_64-NEXT:     non_snake_case,
// REWRITES-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-X86_64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-X86_64-NEXT:     unused_comparisons
// REWRITES-BIONIC-X86_64-NEXT: )]
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: #[repr(C)]
// REWRITES-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-X86_64-NEXT: struct __locale_map {}
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: #[repr(C)]
// REWRITES-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-X86_64-NEXT: struct __locale_struct {
// REWRITES-BIONIC-X86_64-NEXT:     cat: [*mut __locale_map; 6],
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: #[repr(C)]
// REWRITES-BIONIC-X86_64-NEXT: #[derive(Clone, Copy)]
// REWRITES-BIONIC-X86_64-NEXT: struct __mbstate_t {
// REWRITES-BIONIC-X86_64-NEXT:     __seq: [u8; 4],
// REWRITES-BIONIC-X86_64-NEXT:     __reserved: [u8; 4],
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-X86_64-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// REWRITES-BIONIC-X86_64-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// REWRITES-BIONIC-X86_64-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// REWRITES-BIONIC-X86_64-NEXT:     fn mbrtowc(
// REWRITES-BIONIC-X86_64-NEXT:         _0: *mut i32,
// REWRITES-BIONIC-X86_64-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-BIONIC-X86_64-NEXT:         _2: usize,
// REWRITES-BIONIC-X86_64-NEXT:         _3: *mut __mbstate_t,
// REWRITES-BIONIC-X86_64-NEXT:     ) -> usize;
// REWRITES-BIONIC-X86_64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: i32, _2: *mut __mbstate_t) -> usize;
// REWRITES-BIONIC-X86_64-NEXT:     fn newlocale(
// REWRITES-BIONIC-X86_64-NEXT:         _0: i32,
// REWRITES-BIONIC-X86_64-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-BIONIC-X86_64-NEXT:         _2: *mut __locale_struct,
// REWRITES-BIONIC-X86_64-NEXT:     ) -> *mut __locale_struct;
// REWRITES-BIONIC-X86_64-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// REWRITES-BIONIC-X86_64-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// REWRITES-BIONIC-X86_64-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// REWRITES-BIONIC-X86_64-NEXT:     fn freelocale(_0: *mut __locale_struct);
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn bionic_stream_position({{arg[0-9]+}}: *mut libc::FILE, {{arg[0-9]+}}: *mut i64) -> i32 {
// REWRITES-BIONIC-X86_64-NEXT:     let mut stream: *mut libc::FILE = {{arg[0-9]+}};
// REWRITES-BIONIC-X86_64-NEXT:     let mut pos: *mut i64 = {{arg[0-9]+}};
// REWRITES-BIONIC-X86_64-NEXT:     let mut __retval: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fgetpos(stream as *mut libc::FILE, pos as *mut i64) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     if {{_v[0-9]+}} != {{_v[0-9]+}} {
// REWRITES-BIONIC-X86_64-NEXT:         __retval = -1;
// REWRITES-BIONIC-X86_64-NEXT:         return __retval;
// REWRITES-BIONIC-X86_64-NEXT:     }
// REWRITES-BIONIC-X86_64-NEXT:     __retval = unsafe { fsetpos(stream as *mut libc::FILE, pos as *const i64) };
// REWRITES-BIONIC-X86_64-NEXT:     return __retval;
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-BIONIC-X86_64-NEXT:     let mut input: *mut i8 = {{arg[0-9]+}};
// REWRITES-BIONIC-X86_64-NEXT:     let mut __retval: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// REWRITES-BIONIC-X86_64-NEXT:         __seq: [0; 4],
// REWRITES-BIONIC-X86_64-NEXT:         __reserved: [0; 4],
// REWRITES-BIONIC-X86_64-NEXT:     };
// REWRITES-BIONIC-X86_64-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// REWRITES-BIONIC-X86_64-NEXT:         __seq: [0; 4],
// REWRITES-BIONIC-X86_64-NEXT:         __reserved: [0; 4],
// REWRITES-BIONIC-X86_64-NEXT:     };
// REWRITES-BIONIC-X86_64-NEXT:     let mut wide: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-BIONIC-X86_64-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// REWRITES-BIONIC-X86_64-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-BIONIC-X86_64-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// REWRITES-BIONIC-X86_64-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-BIONIC-X86_64-NEXT:         mbrtowc(
// REWRITES-BIONIC-X86_64-NEXT:             std::ptr::addr_of_mut!(wide) as *mut i32,
// REWRITES-BIONIC-X86_64-NEXT:             input as *const core::ffi::c_char,
// REWRITES-BIONIC-X86_64-NEXT:             (1 as u64) as usize,
// REWRITES-BIONIC-X86_64-NEXT:             std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// REWRITES-BIONIC-X86_64-NEXT:         )
// REWRITES-BIONIC-X86_64-NEXT:     }) as u64;
// REWRITES-BIONIC-X86_64-NEXT:     if {{_v[0-9]+}} == 18446744073709551615u64 {
// REWRITES-BIONIC-X86_64-NEXT:         __retval = -1;
// REWRITES-BIONIC-X86_64-NEXT:         return __retval;
// REWRITES-BIONIC-X86_64-NEXT:     }
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-BIONIC-X86_64-NEXT:         wcrtomb(
// REWRITES-BIONIC-X86_64-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-BIONIC-X86_64-NEXT:             wide as i32,
// REWRITES-BIONIC-X86_64-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// REWRITES-BIONIC-X86_64-NEXT:         )
// REWRITES-BIONIC-X86_64-NEXT:     }) as u64;
// REWRITES-BIONIC-X86_64-NEXT:     __retval = {{_v[0-9]+}} as i32;
// REWRITES-BIONIC-X86_64-NEXT:     return __retval;
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn bionic_locale_scope() -> i32 {
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe {
// REWRITES-BIONIC-X86_64-NEXT:         newlocale(
// REWRITES-BIONIC-X86_64-NEXT:             2147483647 as i32,
// REWRITES-BIONIC-X86_64-NEXT:             c"C".as_ptr(),
// REWRITES-BIONIC-X86_64-NEXT:             {{_v[0-9]+}} as *mut __locale_struct,
// REWRITES-BIONIC-X86_64-NEXT:         )
// REWRITES-BIONIC-X86_64-NEXT:     };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{_v[0-9]+}} as *mut __locale_struct) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { wctype(c"alpha".as_ptr()) };
// REWRITES-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { iswctype(97 as u32, {{_v[0-9]+}} as i64) };
// REWRITES-BIONIC-X86_64-NEXT:     unsafe { freelocale({{_v[0-9]+}} as *mut __locale_struct) };
// REWRITES-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// REWRITES-BIONIC-X86_64-NEXT: }
// REWRITES-BIONIC-X86_64-EMPTY:
// REWRITES-BIONIC-X86_64-NEXT: fn main() {
// REWRITES-BIONIC-X86_64-NEXT:     std::process::exit(0 as i32);
// REWRITES-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-x86_64
