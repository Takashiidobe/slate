// merge-sources: units/libc_spellings_address.c
// merge-extra-targets: musl-x86_64 x86_64-unknown-linux-musl clang android-x86_64 x86_64-linux-android clang darwin-x86_64 x86_64-apple-darwin clang freebsd-x86_64 x86_64-unknown-freebsd clang
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

extern int count_digits(void);

static int graph_at(const char *text, int *position) {
    return isgraph(text[(*position)++]) ? 1 : 0;
}

static int classify(int c) {
    int bits = 0;
    bits |= isalnum(c) ? 1 : 0;
    bits |= isalpha(c) ? 2 : 0;
    bits |= isblank(c) ? 4 : 0;
    bits |= iscntrl(c) ? 8 : 0;
    bits |= isdigit(c) ? 16 : 0;
    bits |= isgraph(c) ? 32 : 0;
    bits |= islower(c) ? 64 : 0;
    bits |= isprint(c) ? 128 : 0;
    bits |= ispunct(c) ? 256 : 0;
    bits |= isspace(c) ? 512 : 0;
    bits |= isupper(c) ? 1024 : 0;
    bits |= isxdigit(c) ? 2048 : 0;
    return bits;
}

static int hand_written(int c) {
    return 0 ? isgraph(c) : 7;
}

int main(void) {
    int sum = 0;
    for (int c = -1; c < 256; c++) {
        sum = (sum * 31 + classify(c)) % 1000003;
        sum = (sum * 31 + tolower(c) + toupper(c)) % 1000003;
    }
    int position = 0;
    int graphs = graph_at(" a", &position);
    graphs += graph_at(" a", &position);
    errno = 0;
    int *location = &errno;
    *location = 7;
    int before = errno;
    errno += 1;
    FILE *missing = fopen("/nonexistent/slate", "r");
    printf("%d %d %d %d %d %d %d\n", sum, graphs, position, before,
           missing == NULL, errno != 8, hand_written('a'));
    printf("%d\n", count_digits());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   libc_spellings.c: [] 1 compiled
// CHECK-NEXT:   units/libc_spellings_address.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   libc_spellings.c: [] 1 compiled
// CHECK-NEXT:   units/libc_spellings_address.c: [] 1 compiled
// CHECK-NEXT: target musl-x86_64: 2 compiles
// CHECK-NEXT:   libc_spellings.c: [] 1 compiled
// CHECK-NEXT:   units/libc_spellings_address.c: [] 1 compiled
// CHECK-NEXT: target android-x86_64: 2 compiles
// CHECK-NEXT:   libc_spellings.c: [] 1 compiled
// CHECK-NEXT:   units/libc_spellings_address.c: [] 1 compiled
// CHECK-NEXT: target darwin-x86_64: 2 compiles
// CHECK-NEXT:   libc_spellings.c: [] 1 compiled
// CHECK-NEXT:   units/libc_spellings_address.c: [] 1 compiled
// CHECK-NEXT: target freebsd-x86_64: 2 compiles
// CHECK-NEXT:   libc_spellings.c: [] 1 compiled
// CHECK-NEXT:   units/libc_spellings_address.c: [] 1 compiled
// CHECK-NEXT: total: 12 compiles across 6 targets
// CHECK-NEXT: unit libc_spellings: 11 items, 4 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:graph_at: 2 variants
// CHECK-NEXT:   spliced fn:classify: 3 variants
// CHECK-NEXT:   spliced fn:hand_written: 3 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit libc_spellings_address: 35 items, 33 gated, 2 duplicated functions
// CHECK-NEXT:   duplicated fn:isdigit: 2 variants
// CHECK-NEXT:   duplicated fn:isgraph: 2 variants
// CHECK-NEXT:   spliced fn:count_digits: 6 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 6 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file libc_spellings_address.rs
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __mb_sb_limit: i32;
// CHECK-NEXT:     static mut _CurrentRuneLocale: *const __SlateRecord_runetype_h_82;
// CHECK-NEXT:     #[thread_local]
// CHECK-NEXT:     static mut _ThreadRuneLocale: *const __SlateRecord_runetype_h_82;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut _DefaultRuneLocale: __SlateRecord_runetype_h_109;
// CHECK-NEXT:     fn __maskrune([[ARG_0:arg[0-9]+]]: i32, [[ARG_1:arg[0-9]+]]: c_ulong) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(target_os = "macos", target_os = "freebsd"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __error() -> *mut i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(target_os = "macos", target_os = "freebsd"))]
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(any(target_os = "macos", target_os = "freebsd"))]
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: #[cfg(any(target_os = "macos", target_os = "freebsd"))]
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_52 {
// CHECK-NEXT:     __min: i32,
// CHECK-NEXT:     __max: i32,
// CHECK-NEXT:     __map: i32,
// CHECK-NEXT:     __types: *mut c_ulong,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_57 {
// CHECK-NEXT:     __nranges: i32,
// CHECK-NEXT:     __ranges: *mut __SlateRecord_runetype_h_52,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_82 {
// CHECK-NEXT:     __magic: [c_char; 8],
// CHECK-NEXT:     __encoding: [c_char; 32],
// CHECK-NEXT:     __sgetrune: Option<unsafe extern "C-unwind" fn(*const c_char, u64, *mut *const c_char) -> i32>,
// CHECK-NEXT:     __sputrune: Option<unsafe extern "C-unwind" fn(i32, *mut c_char, u64, *mut *mut c_char) -> i32>,
// CHECK-NEXT:     __invalid_rune: i32,
// CHECK-NEXT:     __runetype: [c_ulong; 256],
// CHECK-NEXT:     __maplower: [i32; 256],
// CHECK-NEXT:     __mapupper: [i32; 256],
// CHECK-NEXT:     __runetype_ext: __SlateRecord_runetype_h_57,
// CHECK-NEXT:     __maplower_ext: __SlateRecord_runetype_h_57,
// CHECK-NEXT:     __mapupper_ext: __SlateRecord_runetype_h_57,
// CHECK-NEXT:     __variable: *mut (),
// CHECK-NEXT:     __variable_len: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: fn __getCurrentRuneLocale() -> *const __SlateRecord_runetype_h_82 {
// CHECK-NEXT:     if !unsafe { _ThreadRuneLocale }.is_null() {
// CHECK-NEXT:         return unsafe { _ThreadRuneLocale };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { _CurrentRuneLocale }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: fn __sbmaskrune(_c: i32, _f: c_ulong) -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = if _c < 0 || _c >= unsafe { __mb_sb_limit } {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *((*__getCurrentRuneLocale()).__runetype.as_ptr() as *const c_ulong).offset(_c as isize)
// CHECK-NEXT:         }) & _f
// CHECK-NEXT:     };
// CHECK-NEXT:     ([[V_0]] as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "freebsd")]
// CHECK-NEXT: fn __sbistype(_c: i32, _f: c_ulong) -> i32 {
// CHECK-NEXT:     (__sbmaskrune(_c, _f) != 0) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_68 {
// CHECK-NEXT:     __min: i32,
// CHECK-NEXT:     __max: i32,
// CHECK-NEXT:     __map: i32,
// CHECK-NEXT:     __types: *mut u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_73 {
// CHECK-NEXT:     __nranges: i32,
// CHECK-NEXT:     __ranges: *mut __SlateRecord_runetype_h_68,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_78 {
// CHECK-NEXT:     __name: [c_char; 14],
// CHECK-NEXT:     __mask: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_runetype_h_109 {
// CHECK-NEXT:     __magic: [c_char; 8],
// CHECK-NEXT:     __encoding: [c_char; 32],
// CHECK-NEXT:     __sgetrune:
// CHECK-NEXT:         Option<unsafe extern "C-unwind" fn(*const c_char, c_ulong, *mut *const c_char) -> i32>,
// CHECK-NEXT:     __sputrune:
// CHECK-NEXT:         Option<unsafe extern "C-unwind" fn(i32, *mut c_char, c_ulong, *mut *mut c_char) -> i32>,
// CHECK-NEXT:     __invalid_rune: i32,
// CHECK-NEXT:     __runetype: [u32; 256],
// CHECK-NEXT:     __maplower: [i32; 256],
// CHECK-NEXT:     __mapupper: [i32; 256],
// CHECK-NEXT:     __runetype_ext: __SlateRecord_runetype_h_73,
// CHECK-NEXT:     __maplower_ext: __SlateRecord_runetype_h_73,
// CHECK-NEXT:     __mapupper_ext: __SlateRecord_runetype_h_73,
// CHECK-NEXT:     __variable: *mut (),
// CHECK-NEXT:     __variable_len: i32,
// CHECK-NEXT:     __ncharclasses: i32,
// CHECK-NEXT:     __charclasses: *mut __SlateRecord_runetype_h_78,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: fn isascii(_c: i32) -> i32 {
// CHECK-NEXT:     (_c & !127 == 0) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: fn __istype(_c: i32, _f: c_ulong) -> i32 {
// CHECK-NEXT:     let [[V_0]]: i32 = if isascii(_c) != 0 {
// CHECK-NEXT:         (((unsafe { _DefaultRuneLocale.__runetype[_c as usize] }) as c_ulong) & _f != 0) as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ((unsafe { __maskrune(_c, _f) }) != 0) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: fn __isctype(_c: i32, _f: c_ulong) -> i32 {
// CHECK-NEXT:     if _c < 0 || _c >= 256i32 {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (((unsafe { _DefaultRuneLocale.__runetype[_c as usize] }) as c_ulong) & _f != 0) as i32
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __errno() -> *mut i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: fn __bionic_ctype_in_range(__lo: u32, __ch: i32, __hi: u32) -> i32 {
// CHECK-NEXT:     ((__ch as u32).wrapping_sub(__lo) < __hi.wrapping_sub(__lo).wrapping_add(1)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.libc_spellings_address.isdigit")]
// CHECK-NEXT: extern "C-unwind" fn isdigit(__ch: i32) -> i32 {
// CHECK-NEXT:     __bionic_ctype_in_range(48u32, __ch, 57u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: extern "C-unwind" fn isdigit(_c: i32) -> i32 {
// CHECK-NEXT:     __isctype(_c, (1024 as c_ulong))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: fn isgraph(__ch: i32) -> i32 {
// CHECK-NEXT:     __bionic_ctype_in_range(33u32, __ch, 126u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "macos")]
// CHECK-NEXT: fn isgraph(_c: i32) -> i32 {
// CHECK-NEXT:     __istype(_c, (2048 as c_ulong))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __ctype_b_loc() -> *mut *const u16;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(target_os = "android", target_os = "macos")))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn isdigit([[ARG_0]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(windows, target_env = "musl"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn isgraph([[ARG_0]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _errno() -> *mut i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "linux")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __errno_location() -> *mut i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn count(test: Option<unsafe extern "C-unwind" fn(i32) -> i32>) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for c in -1i32..256i32 {
// CHECK-NEXT:         if (unsafe { test.unwrap()(c) }) != 0 {
// CHECK-NEXT:             total += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn count_digits() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         #[cfg(target_os = "linux")]
// CHECK-NEXT:         {
// CHECK-NEXT:             *__errno_location() = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             *_errno() = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(target_os = "android")]
// CHECK-NEXT:         {
// CHECK-NEXT:             *__errno() = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(any(target_os = "macos", target_os = "freebsd"))]
// CHECK-NEXT:         {
// CHECK-NEXT:             *__error() = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         count(unsafe {
// CHECK-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// CHECK-NEXT:                 isdigit as *const (),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) + unsafe { *__errno_location() }
// CHECK-NEXT:             + if (((unsafe { *unsafe { *__ctype_b_loc() }.add(97) }) as u32) as i32) & 32768i32 != 0
// CHECK-NEXT:             {
// CHECK-NEXT:                 1
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         count(unsafe {
// CHECK-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// CHECK-NEXT:                 isdigit as *const (),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) + unsafe { *_errno() }
// CHECK-NEXT:             + if (unsafe { isgraph(97i32) }) != 0 {
// CHECK-NEXT:                 1
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "musl")]
// CHECK-NEXT:     {
// CHECK-NEXT:         count(unsafe {
// CHECK-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// CHECK-NEXT:                 isdigit as *const (),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) + unsafe { *__errno_location() }
// CHECK-NEXT:             + if (if false {
// CHECK-NEXT:                 unsafe { isgraph(97i32) }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 (64u32 < 94) as i32
// CHECK-NEXT:             }) != 0
// CHECK-NEXT:             {
// CHECK-NEXT:                 1
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         count(Some(isdigit)) + unsafe { *__errno() } + if isgraph(97i32) != 0 { 1 } else { 0 }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "macos")]
// CHECK-NEXT:     {
// CHECK-NEXT:         count(Some(isdigit)) + unsafe { *__error() } + if isgraph(97i32) != 0 { 1 } else { 0 }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "freebsd")]
// CHECK-NEXT:     {
// CHECK-NEXT:         count(unsafe {
// CHECK-NEXT:             std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// CHECK-NEXT:                 isdigit as *const (),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) + unsafe { *__error() }
// CHECK-NEXT:             + if __sbistype(97i32, (2048 as c_ulong)) != 0 {
// CHECK-NEXT:                 1
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![feature(thread_local)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/libc_spellings_address.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-extra-targets: musl-x86_64 x86_64-unknown-linux-musl clang android-x86_64 x86_64-linux-android clang darwin-x86_64 x86_64-apple-darwin clang freebsd-x86_64 x86_64-unknown-freebsd clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "musl"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "android"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "macos"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "freebsd")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "libc_spellings_address.rs"]
// CHECK-NEXT: mod __slate_unit_libc_spellings_address;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn count_digits() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "musl")]
// CHECK-NEXT: fn __isspace(_c: i32) -> i32 {
// CHECK-NEXT:     (_c == 32 || (_c as u32).wrapping_sub(9) < 5) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(target_env = "musl"))]
// CHECK-NEXT: fn graph_at(text: *const c_char, position: &mut i32) -> i32 {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe { *position };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *position = [[V_1]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { slate_support::isgraph(*text.offset([[V_1]] as isize) as i32) }) != 0 {
// CHECK-NEXT:         1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "musl")]
// CHECK-NEXT: fn graph_at(text: *const c_char, position: &mut i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: i32;
// CHECK-NEXT:     if false {
// CHECK-NEXT:         let [[V_1]]: *mut i32 = position as *mut i32;
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = unsafe { *[[V_1]] };
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: i32 = [[V_2]] + 1;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_1]] = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = unsafe { slate_support::isgraph(*text.offset([[V_2]] as isize) as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: *mut i32 = position as *mut i32;
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = unsafe { *[[V_4]] };
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: i32 = [[V_5]] + 1;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_4]] = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = ((((unsafe { *text.offset([[V_5]] as isize) }) as i32) as u32).wrapping_sub(33) < 94)
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] != 0 { 1 } else { 0 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn classify(c: i32) -> i32 {
// CHECK-NEXT:     let mut bits: i32 = 0;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_0]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_0]]
// CHECK-NEXT:             | if (unsafe { slate_support::isalnum(c) }) != 0 {
// CHECK-NEXT:                 1
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_2]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_2]]
// CHECK-NEXT:             | if (unsafe { slate_support::isalpha(c) }) != 0 {
// CHECK-NEXT:                 2
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_4]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_4]]
// CHECK-NEXT:             | if (unsafe { slate_support::isblank(c) }) != 0 {
// CHECK-NEXT:                 4
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_6]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_6]]
// CHECK-NEXT:             | if (unsafe { slate_support::iscntrl(c) }) != 0 {
// CHECK-NEXT:                 8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_7]]
// CHECK-NEXT:             | if (unsafe { slate_support::isdigit(c) }) != 0 {
// CHECK-NEXT:                 16
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_8]]
// CHECK-NEXT:             | if (unsafe { slate_support::isgraph(c) }) != 0 {
// CHECK-NEXT:                 32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_9]]
// CHECK-NEXT:             | if (unsafe { slate_support::islower(c) }) != 0 {
// CHECK-NEXT:                 64
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_10]]
// CHECK-NEXT:             | if (unsafe { slate_support::isprint(c) }) != 0 {
// CHECK-NEXT:                 128
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_11]]
// CHECK-NEXT:             | if (unsafe { slate_support::ispunct(c) }) != 0 {
// CHECK-NEXT:                 256
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_env = "gnu", target_os = "macos"))]
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_env = "gnu", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_12]]
// CHECK-NEXT:             | if (unsafe { slate_support::isspace(c) }) != 0 {
// CHECK-NEXT:                 512
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "musl")]
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = if __isspace(c) != 0 { 512 } else { 0 };
// CHECK-NEXT:     #[cfg(target_env = "musl")]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_13]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_14]]
// CHECK-NEXT:             | if (unsafe { slate_support::isupper(c) }) != 0 {
// CHECK-NEXT:                 1024
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: i32 = bits;
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits = [[V_15]]
// CHECK-NEXT:             | if (unsafe { slate_support::isxdigit(c) }) != 0 {
// CHECK-NEXT:                 2048
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 0
// CHECK-NEXT:             };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_1]]: i32 = if (unsafe { slate_support::isalnum(c) }) != 0 {
// CHECK-NEXT:         1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_3]]: i32 = if (unsafe { slate_support::isalpha(c) }) != 0 {
// CHECK-NEXT:         2
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_5]]: i32 = if (unsafe { slate_support::isblank(c) }) != 0 {
// CHECK-NEXT:         4
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: i32 = if (unsafe { slate_support::iscntrl(c) }) != 0 {
// CHECK-NEXT:         8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_16]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: i32 = if (unsafe { slate_support::isdigit(c) }) != 0 {
// CHECK-NEXT:         16
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = if (unsafe { slate_support::isgraph(c) }) != 0 {
// CHECK-NEXT:         32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_18]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: i32 = if (unsafe { slate_support::islower(c) }) != 0 {
// CHECK-NEXT:         64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_19]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = if (unsafe { slate_support::isprint(c) }) != 0 {
// CHECK-NEXT:         128
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_20]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: i32 = if (unsafe { slate_support::ispunct(c) }) != 0 {
// CHECK-NEXT:         256
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_21]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_22:__v[0-9]+]]: i32 = if (unsafe { slate_support::isspace(c) }) != 0 {
// CHECK-NEXT:         512
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_22]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: i32 = if (unsafe { slate_support::isupper(c) }) != 0 {
// CHECK-NEXT:         1024
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_23]];
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: i32 = if (unsafe { slate_support::isxdigit(c) }) != 0 {
// CHECK-NEXT:         2048
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         bits |= [[V_24]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bits
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn hand_written(c: i32) -> i32 {
// CHECK-NEXT:     #[cfg(target_os = "freebsd")]
// CHECK-NEXT:     let mut [[V_0]]: i32;
// CHECK-NEXT:     #[cfg(target_os = "freebsd")]
// CHECK-NEXT:     {
// CHECK-NEXT:         if false {
// CHECK-NEXT:             [[V_0]] = unsafe { slate_support::isgraph(c) };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = 7;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(windows, target_os = "android"))]
// CHECK-NEXT:     let [[V_0]]: i32 = if false {
// CHECK-NEXT:         unsafe { slate_support::isgraph(c) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         7
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(not(any(target_os = "linux", target_os = "macos")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_0]]
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(target_os = "linux", target_os = "macos"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if false {
// CHECK-NEXT:             unsafe { slate_support::isgraph(c) }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             7
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for c in -1i32..256i32 {
// CHECK-NEXT:         sum = (sum * 31 + classify(c)) % 1000003;
// CHECK-NEXT:         sum = (sum * 31
// CHECK-NEXT:             + unsafe { slate_support::tolower(c) }
// CHECK-NEXT:             + unsafe { slate_support::toupper(c) })
// CHECK-NEXT:             % 1000003;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut position: i32 = 0;
// CHECK-NEXT:     let mut graphs: i32 = graph_at(c" a".as_ptr(), unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(position))
// CHECK-NEXT:     });
// CHECK-NEXT:     let [[V_1]]: i32 = graph_at(c" a".as_ptr(), unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(position))
// CHECK-NEXT:     });
// CHECK-NEXT:     graphs += [[V_1]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *slate_support::errno_location() = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let location: *mut i32 = unsafe { slate_support::errno_location() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *location = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     let before: i32 = unsafe { *slate_support::errno_location() };
// CHECK-NEXT:     let [[V_4]]: *mut i32 = unsafe { slate_support::errno_location() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_4]] = *[[V_4]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let missing: *mut slate_support::FILE =
// CHECK-NEXT:         unsafe { slate_support::fopen(c"/nonexistent/slate".as_ptr(), c"r".as_ptr()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 sum,
// CHECK-NEXT:                 graphs,
// CHECK-NEXT:                 position,
// CHECK-NEXT:                 before,
// CHECK-NEXT:                 missing.is_null() as i32,
// CHECK-NEXT:                 (*slate_support::errno_location() != 8) as i32,
// CHECK-NEXT:                 hand_written(97i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), count_digits()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {} {} {}",
// CHECK-NEXT:             sum,
// CHECK-NEXT:             graphs,
// CHECK-NEXT:             position,
// CHECK-NEXT:             before,
// CHECK-NEXT:             missing.is_null() as i32,
// CHECK-NEXT:             ((unsafe { *slate_support::errno_location() }) != 8) as i32,
// CHECK-NEXT:             hand_written(97i32)
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{}", unsafe { count_digits() });
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
