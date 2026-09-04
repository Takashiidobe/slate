#include <langinfo.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#include <xlocale.h>

_Static_assert(sizeof(fpos_t) == 8, "fpos_t");
_Static_assert(sizeof(mbstate_t) == 128, "mbstate_t");
_Static_assert(_Alignof(mbstate_t) == 8, "mbstate_t alignment");
_Static_assert(sizeof(wint_t) == 4, "wint_t");
_Static_assert(sizeof(wctype_t) == 4, "wctype_t");
_Static_assert(sizeof(wctrans_t) == 4, "wctrans_t");
_Static_assert(sizeof(locale_t) == 8, "locale_t");
_Static_assert(sizeof(struct __sbuf) == 16, "struct __sbuf");
_Static_assert(sizeof(struct __sFILE) == 152, "struct __sFILE");
_Static_assert(offsetof(struct __sFILE, _p) == 0, "FILE _p");
_Static_assert(offsetof(struct __sFILE, _bf) == 24, "FILE _bf");
_Static_assert(offsetof(struct __sFILE, _cookie) == 48, "FILE _cookie");
_Static_assert(offsetof(struct __sFILE, _ub) == 88, "FILE _ub");
_Static_assert(offsetof(struct __sFILE, _lb) == 120, "FILE _lb");
_Static_assert(offsetof(struct __sFILE, _offset) == 144, "FILE _offset");
_Static_assert(sizeof(struct lconv) == 96, "struct lconv");
_Static_assert(offsetof(struct lconv, int_p_cs_precedes) == 88,
               "lconv int_p_cs_precedes");
_Static_assert(offsetof(struct lconv, int_n_cs_precedes) == 89,
               "lconv int_n_cs_precedes");
_Static_assert(LC_ALL == 0, "LC_ALL");
_Static_assert(LC_COLLATE == 1, "LC_COLLATE");
_Static_assert(LC_CTYPE == 2, "LC_CTYPE");
_Static_assert(LC_MONETARY == 3, "LC_MONETARY");
_Static_assert(LC_NUMERIC == 4, "LC_NUMERIC");
_Static_assert(LC_TIME == 5, "LC_TIME");
_Static_assert(LC_MESSAGES == 6, "LC_MESSAGES");
_Static_assert(CODESET == 0, "CODESET");
_Static_assert(CRNCYSTR == 56, "CRNCYSTR");
_Static_assert(L_ctermid == 1024, "L_ctermid");
_Static_assert(sizeof(P_tmpdir) == 10, "P_tmpdir");
_Static_assert(__builtin_types_compatible_p(__typeof__(&fgetpos),
                                            int (*)(FILE *, fpos_t *)),
               "fgetpos signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&mbrtowc),
                                            size_t (*)(wchar_t *, const char *,
                                                       size_t, mbstate_t *)),
               "mbrtowc signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&newlocale),
                                            locale_t (*)(int, const char *,
                                                         locale_t)),
               "newlocale signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&freelocale),
                                            int (*)(locale_t)),
               "freelocale signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&setbuffer),
                                            void (*)(FILE *, char *, int)),
               "setbuffer signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&setlinebuf),
                                            int (*)(FILE *)),
               "setlinebuf signature");

FILE *standard_input(void) { return stdin; }

int classify_wide(wint_t value, locale_t locale) {
  return iswctype_l(value, wctype("alpha"), locale);
}

// REWRITES-MACOS-DAG: struct __sFILE
// REWRITES-MACOS-DAG: fn standard_input() -> *mut __sFILE
// REWRITES-MACOS-DAG: fn classify_wide(

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![allow(
// LOWERING-MACOS-NEXT:     dead_code,
// LOWERING-MACOS-NEXT:     unused,
// LOWERING-MACOS-NEXT:     non_camel_case_types,
// LOWERING-MACOS-NEXT:     non_snake_case,
// LOWERING-MACOS-NEXT:     non_upper_case_globals,
// LOWERING-MACOS-NEXT:     arithmetic_overflow,
// LOWERING-MACOS-NEXT:     unconditional_panic,
// LOWERING-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MACOS-NEXT:     unused_comparisons
// LOWERING-MACOS-NEXT: )]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct __sFILE {
// LOWERING-MACOS-NEXT:     _p: *mut u8,
// LOWERING-MACOS-NEXT:     _r: i32,
// LOWERING-MACOS-NEXT:     _w: i32,
// LOWERING-MACOS-NEXT:     _flags: i16,
// LOWERING-MACOS-NEXT:     _file: i16,
// LOWERING-MACOS-NEXT:     _bf: __sbuf,
// LOWERING-MACOS-NEXT:     _lbfsize: i32,
// LOWERING-MACOS-NEXT:     _cookie: *mut core::ffi::c_void,
// LOWERING-MACOS-NEXT:     _close: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// LOWERING-MACOS-NEXT:     _read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, i32) -> i32>,
// LOWERING-MACOS-NEXT:     _seek: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, i64, i32) -> i64>,
// LOWERING-MACOS-NEXT:     _write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, i32) -> i32>,
// LOWERING-MACOS-NEXT:     _ub: __sbuf,
// LOWERING-MACOS-NEXT:     _extra: *mut __sFILEX,
// LOWERING-MACOS-NEXT:     _ur: i32,
// LOWERING-MACOS-NEXT:     _ubuf: [u8; 3],
// LOWERING-MACOS-NEXT:     _nbuf: [u8; 1],
// LOWERING-MACOS-NEXT:     _lb: __sbuf,
// LOWERING-MACOS-NEXT:     _blksize: i32,
// LOWERING-MACOS-NEXT:     _offset: i64,
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct __sFILEX {
// LOWERING-MACOS-NEXT:     __slate_empty: [u8; 0],
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct __sbuf {
// LOWERING-MACOS-NEXT:     _base: *mut u8,
// LOWERING-MACOS-NEXT:     _size: i32,
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct _xlocale {
// LOWERING-MACOS-NEXT:     __slate_empty: [u8; 0],
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: unsafe extern "C" {
// LOWERING-MACOS-NEXT:     static mut __stdinp: *mut __sFILE;
// LOWERING-MACOS-NEXT:     fn iswctype_l(_0: i32, _1: u32, _2: *mut _xlocale) -> i32;
// LOWERING-MACOS-NEXT:     fn wctype(_0: *const core::ffi::c_char) -> u32;
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn standard_input() -> *mut __sFILE {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut __sFILE = unsafe { __stdinp };
// LOWERING-MACOS-NEXT:     return {{_v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn classify_wide({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut _xlocale) -> i32 {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"alpha\0".as_ptr() as *mut i8;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { wctype({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { iswctype_l({{arg[0-9]+}} as i32, {{_v[0-9]+}} as u32, {{arg[0-9]+}} as *mut _xlocale) };
// LOWERING-MACOS-NEXT:     return {{_v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos
