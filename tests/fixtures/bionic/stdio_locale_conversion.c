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
  locale_t updated  = newlocale(LC_ALL_MASK, "C", (locale_t)0);
  locale_t previous = uselocale(updated);
  int      is_alpha = iswctype(L'a', wctype("alpha"));
  freelocale(previous);
  return is_alpha;
}

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __locale_map {
// COMMON-LOWERING-NEXT:     __slate_empty: [u8; 0],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __locale_struct {
// COMMON-LOWERING-NEXT:     cat: [*mut __locale_map; 6],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __mbstate_t {
// COMMON-LOWERING-NEXT:     __seq: [u8; 4],
// COMMON-LOWERING-NEXT:     __reserved: [u8; 4],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// COMMON-LOWERING-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// COMMON-LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn mbrtowc(
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: *mut __mbstate_t,
// COMMON-LOWERING-NEXT:     ) -> usize;
// COMMON-LOWERING-NEXT:     fn newlocale(
// COMMON-LOWERING-NEXT:         _0: i32,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: *mut __locale_struct,
// COMMON-LOWERING-NEXT:     ) -> *mut __locale_struct;
// COMMON-LOWERING-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// COMMON-LOWERING-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// COMMON-LOWERING-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// COMMON-LOWERING-NEXT:     fn freelocale(_0: *mut __locale_struct);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn bionic_stream_position({{arg[0-9]+}}: *mut libc::FILE, {{arg[0-9]+}}: *mut i64) -> i32 {
// COMMON-LOWERING-NEXT:     let mut stream: *mut libc::FILE = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut pos: *mut i64 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     stream = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     pos = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut libc::FILE = stream;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i64 = pos;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { fgetpos({{__v[0-9]+}} as *mut libc::FILE, {{__v[0-9]+}} as *mut i64) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = stream;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i64 = pos;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fsetpos({{__v[0-9]+}} as *mut libc::FILE, {{__v[0-9]+}} as *const i64) };
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __seq: [0; 4],
// COMMON-LOWERING-NEXT:         __reserved: [0; 4],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __seq: [0; 4],
// COMMON-LOWERING-NEXT:         __reserved: [0; 4],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     input = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as u64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as u64) };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:             mbrtowc(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         }) as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 18446744073709551615u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         wcrtomb(
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn bionic_locale_scope() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2147483647;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe {
// COMMON-LOWERING-NEXT:         newlocale(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut __locale_struct,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{__v[0-9]+}} as *mut __locale_struct) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 97;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { wctype({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { iswctype({{__v[0-9]+}} as u32, {{__v[0-9]+}} as i64) };
// COMMON-LOWERING-NEXT:     unsafe { freelocale({{__v[0-9]+}} as *mut __locale_struct) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64-NEXT:         _0: *mut u32,
// LOWERING-BIONIC-AARCH64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-BIONIC-AARCH64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut input: *mut u8 = std::ptr::null_mut();
// LOWERING-BIONIC-AARCH64-NEXT:     let mut wide: u32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:         let {{__v[0-9]+}}: *mut u8 = input;
// LOWERING-BIONIC-AARCH64-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut u32,
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: u32 = wide;
// LOWERING-BIONIC-AARCH64-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"C\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"alpha\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64-NEXT:         _0: *mut i32,
// LOWERING-BIONIC-X86_64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: i32, _2: *mut __mbstate_t) -> usize;
// LOWERING-BIONIC-X86_64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-BIONIC-X86_64-NEXT:     let mut input: *mut i8 = std::ptr::null_mut();
// LOWERING-BIONIC-X86_64-NEXT:     let mut wide: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:         let {{__v[0-9]+}}: *mut i8 = input;
// LOWERING-BIONIC-X86_64-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut i32,
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: i32 = wide;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"C\0".as_ptr() as *mut i8;
// LOWERING-BIONIC-X86_64-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"alpha\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __locale_map {
// COMMON-REWRITES-NEXT:     __slate_empty: [u8; 0],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __locale_struct {
// COMMON-REWRITES-NEXT:     cat: [*mut __locale_map; 6],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __mbstate_t {
// COMMON-REWRITES-NEXT:     __seq: [u8; 4],
// COMMON-REWRITES-NEXT:     __reserved: [u8; 4],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// COMMON-REWRITES-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// COMMON-REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn mbrtowc(
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: *mut __mbstate_t,
// COMMON-REWRITES-NEXT:     ) -> usize;
// COMMON-REWRITES-NEXT:     fn newlocale(
// COMMON-REWRITES-NEXT:         _0: i32,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: *mut __locale_struct,
// COMMON-REWRITES-NEXT:     ) -> *mut __locale_struct;
// COMMON-REWRITES-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// COMMON-REWRITES-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// COMMON-REWRITES-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// COMMON-REWRITES-NEXT:     fn freelocale(_0: *mut __locale_struct);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bionic_stream_position(mut stream: *mut libc::FILE, mut pos: *mut i64) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fgetpos(stream as *mut libc::FILE, pos as *mut i64) };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         return -1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { fsetpos(stream as *mut libc::FILE, pos as *const i64) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// COMMON-REWRITES-NEXT:         __seq: [0; 4],
// COMMON-REWRITES-NEXT:         __reserved: [0; 4],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// COMMON-REWRITES-NEXT:         __seq: [0; 4],
// COMMON-REWRITES-NEXT:         __reserved: [0; 4],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         mbrtowc(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (1 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} == 18446744073709551615u64 {
// COMMON-REWRITES-NEXT:         return -1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         wcrtomb(
// COMMON-REWRITES-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bionic_locale_scope() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe {
// COMMON-REWRITES-NEXT:         newlocale(
// COMMON-REWRITES-NEXT:             2147483647 as i32,
// COMMON-REWRITES-NEXT:             c"C".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut __locale_struct,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{__v[0-9]+}} as *mut __locale_struct) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { wctype(c"alpha".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { iswctype(97 as u32, {{__v[0-9]+}} as i64) };
// COMMON-REWRITES-NEXT:     unsafe { freelocale({{__v[0-9]+}} as *mut __locale_struct) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64-NEXT:         _0: *mut u32,
// REWRITES-BIONIC-AARCH64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-BIONIC-AARCH64-NEXT: fn bionic_multibyte_roundtrip(mut {{__v[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// REWRITES-BIONIC-AARCH64-NEXT:     let mut wide: u32 = 0;
// REWRITES-BIONIC-AARCH64-NEXT:             std::ptr::addr_of_mut!(wide) as *mut u32,
// REWRITES-BIONIC-AARCH64-NEXT:             wide as u32,
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64-NEXT:         _0: *mut i32,
// REWRITES-BIONIC-X86_64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: i32, _2: *mut __mbstate_t) -> usize;
// REWRITES-BIONIC-X86_64-NEXT: fn bionic_multibyte_roundtrip(mut {{__v[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-BIONIC-X86_64-NEXT:     let mut wide: i32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:             std::ptr::addr_of_mut!(wide) as *mut i32,
// REWRITES-BIONIC-X86_64-NEXT:             wide as i32,
// SLATE-FILECHECK-END rewrites-bionic-x86_64
