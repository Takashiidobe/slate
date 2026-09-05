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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __locale_map {
// LOWERING-NEXT:     __slate_empty: [u8; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __locale_struct {
// LOWERING-NEXT:     cat: [*mut __locale_map; 6],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __mbstate_t {
// LOWERING-NEXT:     __seq: [u8; 4],
// LOWERING-NEXT:     __reserved: [u8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// LOWERING-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn mbrtowc(
// LOWERING-BIONIC-AARCH64-NEXT:         _0: *mut u32,
// LOWERING-BIONIC-X86_64-NEXT:         _0: *mut i32,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: *mut __mbstate_t,
// LOWERING-NEXT:     ) -> usize;
// LOWERING-BIONIC-AARCH64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-BIONIC-X86_64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: i32, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn newlocale(
// LOWERING-NEXT:         _0: i32,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: *mut __locale_struct,
// LOWERING-NEXT:     ) -> *mut __locale_struct;
// LOWERING-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// LOWERING-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// LOWERING-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// LOWERING-NEXT:     fn freelocale(_0: *mut __locale_struct);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bionic_stream_position({{arg[0-9]+}}: *mut libc::FILE, {{arg[0-9]+}}: *mut i64) -> i32 {
// LOWERING-NEXT:     let mut stream: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut pos: *mut i64 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-NEXT:     pos = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut libc::FILE = stream;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i64 = pos;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { fgetpos({{__v[0-9]+}} as *mut libc::FILE, {{__v[0-9]+}} as *mut i64) };
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = stream;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i64 = pos;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fsetpos({{__v[0-9]+}} as *mut libc::FILE, {{__v[0-9]+}} as *const i64) };
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut input: *mut u8 = std::ptr::null_mut();
// LOWERING-BIONIC-X86_64-NEXT: fn bionic_multibyte_roundtrip({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-BIONIC-X86_64-NEXT:     let mut input: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __seq: [0; 4],
// LOWERING-NEXT:         __reserved: [0; 4],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __seq: [0; 4],
// LOWERING-NEXT:         __reserved: [0; 4],
// LOWERING-NEXT:     };
// LOWERING-BIONIC-AARCH64-NEXT:     let mut wide: u32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let mut wide: i32 = 0;
// LOWERING-NEXT:     input = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as u64) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as u64) };
// LOWERING-NEXT:     {
// LOWERING-BIONIC-AARCH64-NEXT:         let {{__v[0-9]+}}: *mut u8 = input;
// LOWERING-BIONIC-X86_64-NEXT:         let {{__v[0-9]+}}: *mut i8 = input;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:             mbrtowc(
// LOWERING-BIONIC-AARCH64-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut u32,
// LOWERING-BIONIC-X86_64-NEXT:                 std::ptr::addr_of_mut!(wide) as *mut i32,
// LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 {{__v[0-9]+}} as usize,
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         }) as u64;
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 18446744073709551615u64;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: u32 = wide;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: i32 = wide;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         wcrtomb(
// LOWERING-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-BIONIC-AARCH64-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-BIONIC-X86_64-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bionic_locale_scope() -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2147483647;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"C\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"C\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe {
// LOWERING-NEXT:         newlocale(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut __locale_struct,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{__v[0-9]+}} as *mut __locale_struct) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 97;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"alpha\0".as_ptr() as *mut u8;
// LOWERING-BIONIC-X86_64-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"alpha\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { wctype({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { iswctype({{__v[0-9]+}} as u32, {{__v[0-9]+}} as i64) };
// LOWERING-NEXT:     unsafe { freelocale({{__v[0-9]+}} as *mut __locale_struct) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __locale_map {
// REWRITES-NEXT:     __slate_empty: [u8; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __locale_struct {
// REWRITES-NEXT:     cat: [*mut __locale_map; 6],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __mbstate_t {
// REWRITES-NEXT:     __seq: [u8; 4],
// REWRITES-NEXT:     __reserved: [u8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fgetpos(_0: *mut libc::FILE, _1: *mut i64) -> i32;
// REWRITES-NEXT:     fn fsetpos(_0: *mut libc::FILE, _1: *const i64) -> i32;
// REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: u64) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn mbrtowc(
// REWRITES-BIONIC-AARCH64-NEXT:         _0: *mut u32,
// REWRITES-BIONIC-X86_64-NEXT:         _0: *mut i32,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: *mut __mbstate_t,
// REWRITES-NEXT:     ) -> usize;
// REWRITES-BIONIC-AARCH64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-BIONIC-X86_64-NEXT:     fn wcrtomb(_0: *mut core::ffi::c_char, _1: i32, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn newlocale(
// REWRITES-NEXT:         _0: i32,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: *mut __locale_struct,
// REWRITES-NEXT:     ) -> *mut __locale_struct;
// REWRITES-NEXT:     fn uselocale(_0: *mut __locale_struct) -> *mut __locale_struct;
// REWRITES-NEXT:     fn iswctype(_0: u32, _1: i64) -> i32;
// REWRITES-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> i64;
// REWRITES-NEXT:     fn freelocale(_0: *mut __locale_struct);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bionic_stream_position(mut stream: *mut libc::FILE, mut pos: *mut i64) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fgetpos(stream as *mut libc::FILE, pos as *mut i64) };
// REWRITES-NEXT:     if {{__v[0-9]+}} != 0 {
// REWRITES-NEXT:         return -1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { fsetpos(stream as *mut libc::FILE, pos as *const i64) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn bionic_multibyte_roundtrip(mut {{__v[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// REWRITES-BIONIC-X86_64-NEXT: fn bionic_multibyte_roundtrip(mut {{__v[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-NEXT:     let mut decode_state: __mbstate_t = __mbstate_t {
// REWRITES-NEXT:         __seq: [0; 4],
// REWRITES-NEXT:         __reserved: [0; 4],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut encode_state: __mbstate_t = __mbstate_t {
// REWRITES-NEXT:         __seq: [0; 4],
// REWRITES-NEXT:         __reserved: [0; 4],
// REWRITES-NEXT:     };
// REWRITES-BIONIC-AARCH64-NEXT:     let mut wide: u32 = 0;
// REWRITES-BIONIC-X86_64-NEXT:     let mut wide: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(decode_state) as *mut core::ffi::c_void;
// REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(encode_state) as *mut core::ffi::c_void;
// REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (0 as i32) as u8, (8 as u64) as usize) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         mbrtowc(
// REWRITES-BIONIC-AARCH64-NEXT:             std::ptr::addr_of_mut!(wide) as *mut u32,
// REWRITES-BIONIC-X86_64-NEXT:             std::ptr::addr_of_mut!(wide) as *mut i32,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             (1 as u64) as usize,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(decode_state) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     if {{__v[0-9]+}} == 18446744073709551615u64 {
// REWRITES-NEXT:         return -1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         wcrtomb(
// REWRITES-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-BIONIC-AARCH64-NEXT:             wide as u32,
// REWRITES-BIONIC-X86_64-NEXT:             wide as i32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(encode_state) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     {{__v[0-9]+}} as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bionic_locale_scope() -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = std::ptr::null_mut();
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe {
// REWRITES-NEXT:         newlocale(
// REWRITES-NEXT:             2147483647 as i32,
// REWRITES-NEXT:             c"C".as_ptr(),
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut __locale_struct,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut __locale_struct = unsafe { uselocale({{__v[0-9]+}} as *mut __locale_struct) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { wctype(c"alpha".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { iswctype(97 as u32, {{__v[0-9]+}} as i64) };
// REWRITES-NEXT:     unsafe { freelocale({{__v[0-9]+}} as *mut __locale_struct) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
