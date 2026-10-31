// merge-sources: units/named_types_helper.c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned long uLong;

struct counters {
    unsigned long total;
    long delta;
    size_t bytes;
    uint64_t wide;
    char tag;
    const char *name;
    char label[4];
};

struct tally {
    long count;
    size_t bytes;
    char tag;
};

extern long tally_sum(const struct tally *tally);

static struct counters shared = {1UL, -2L, 3, 4, 'x', "shared", "abc"};

uLong add_total(uLong left, unsigned long right) { return left + right; }

long negate(long value) { return -value; }

size_t measure(const char *text, size_t limit) {
    size_t length = 0;
    while (length < limit && text[length]) {
        length += 1;
    }
    return length;
}

uint64_t widen(uint32_t value) { return (uint64_t)value << 32; }

uLong apply(uLong (*operation)(uLong, unsigned long), uLong value) {
    return operation(value, 1UL);
}

uLong clamp_base(uLong value, unsigned char byte, size_t length) {
    if (length < 16) {
        value += byte;
    }
    if (value >= 65521U) {
        value -= 65521U;
    }
    return value;
}

size_t width_of_long(void) { return sizeof(long); }

int main(void) {
    struct tally tally = {5, 6, 'a'};
    uLong total = add_total(shared.total, 41UL);
    long delta = negate(shared.delta);
    size_t bytes = measure(shared.name, 16) + shared.bytes;
    printf("%lu %ld %zu %llu %c %s\n", total, delta, bytes,
           (unsigned long long)widen(1), shared.tag, shared.label);
    printf("%lu %llu\n", apply(add_total, total), (unsigned long long)shared.wide);
    printf("%lu %ld\n", clamp_base(65530UL, 200, 3), tally_sum(&tally));
    printf("%d\n", width_of_long() == sizeof(long));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   named_types.c: [] 1 compiled
// CHECK-NEXT:   units/named_types_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   named_types.c: [] 1 compiled
// CHECK-NEXT:   units/named_types_helper.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit named_types: 35 items, 18 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:width_of_long: 2 variants
// CHECK-NEXT:   spliced fn:add_total: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit named_types_helper: 7 items, 4 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:tally_sum: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 2 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/named_types_helper.c
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "named_types_helper.rs"]
// CHECK-NEXT: mod __slate_unit_named_types_helper;
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
// CHECK-NEXT:     fn tally_sum([[ARG_0]]: *const tally) -> c_long;
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: type c_char = i8;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: type c_long = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: type c_long = i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: type c_ulong = u64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: type c_ulong = u32;
// CHECK-EMPTY:
// CHECK-NEXT: type size_t = u64;
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct counters {
// CHECK-NEXT:     total: c_ulong,
// CHECK-NEXT:     delta: c_long,
// CHECK-NEXT:     bytes: size_t,
// CHECK-NEXT:     wide: u64,
// CHECK-NEXT:     tag: c_char,
// CHECK-NEXT:     name: *const c_char,
// CHECK-NEXT:     label: [c_char; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tally {
// CHECK-NEXT:     count: c_long,
// CHECK-NEXT:     bytes: size_t,
// CHECK-NEXT:     tag: c_char,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut shared: counters = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: counters = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].total = (1 as c_ulong);
// CHECK-NEXT:     [[T_0]].delta = -(2 as c_long);
// CHECK-NEXT:     [[T_0]].bytes = (3 as size_t);
// CHECK-NEXT:     [[T_0]].wide = 4u64;
// CHECK-NEXT:     [[T_0]].tag = (120 as c_char);
// CHECK-NEXT:     [[T_0]].name = (b"shared\0".as_ptr() as *mut c_char) as *const c_char;
// CHECK-NEXT:     [[T_0]].label = [97i8, 98i8, 99i8, 0i8];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: #[cfg_attr(
// CHECK-NEXT:     unix,
// CHECK-NEXT:     unsafe(link_section = ".text.slate_distinct.named_types.add_total")
// CHECK-NEXT: )]
// CHECK-NEXT: extern "C-unwind" fn add_total(left: c_ulong, right: c_ulong) -> c_ulong {
// CHECK-NEXT:     left.wrapping_add(right)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn negate(value: c_long) -> c_long {
// CHECK-NEXT:     -value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn measure(text: *const c_char, limit: size_t) -> size_t {
// CHECK-NEXT:     let mut length: size_t = 0;
// CHECK-NEXT:     while length < limit && (unsafe { *text.add(length as usize) }) != 0 {
// CHECK-NEXT:         length = length.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     length
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn widen(value: u32) -> u64 {
// CHECK-NEXT:     (value as u64) << 32i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn apply(
// CHECK-NEXT:     operation: Option<unsafe extern "C-unwind" fn(c_ulong, c_ulong) -> c_ulong>,
// CHECK-NEXT:     value: c_ulong,
// CHECK-NEXT: ) -> c_ulong {
// CHECK-NEXT:     unsafe { operation.unwrap()(value, (1 as c_ulong)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn clamp_base(mut value: c_ulong, byte: u8, length: size_t) -> c_ulong {
// CHECK-NEXT:     if length < 16 {
// CHECK-NEXT:         value = value.wrapping_add(((byte as u32) as i32) as c_ulong);
// CHECK-NEXT:     }
// CHECK-NEXT:     if value >= 65521 {
// CHECK-NEXT:         value = value.wrapping_sub(65521);
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: const __SLATE_HOIST_width_of_long: size_t = 8;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: const __SLATE_HOIST_width_of_long: size_t = 4;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn width_of_long() -> size_t {
// CHECK-NEXT:     __SLATE_HOIST_width_of_long
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut tally: tally = {
// CHECK-NEXT:         let mut [[T_0]]: tally = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].count = (5 as c_long);
// CHECK-NEXT:         [[T_0]].bytes = (6 as size_t);
// CHECK-NEXT:         [[T_0]].tag = (97 as c_char);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let total: c_ulong = add_total(unsafe { shared.total }, (41 as c_ulong));
// CHECK-NEXT:     let delta: c_long = negate(unsafe { shared.delta });
// CHECK-NEXT:     let bytes: size_t =
// CHECK-NEXT:         measure(unsafe { shared.name }, (16 as size_t)).wrapping_add(unsafe { shared.bytes });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%lu %ld %zu %llu %c %s\n".as_ptr(),
// CHECK-NEXT:                 total,
// CHECK-NEXT:                 delta,
// CHECK-NEXT:                 bytes,
// CHECK-NEXT:                 widen(1u32),
// CHECK-NEXT:                 shared.tag as i32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(shared.label) as *mut c_char,
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
// CHECK-NEXT:                 c"%lu %llu\n".as_ptr(),
// CHECK-NEXT:                 apply(Some(add_total), total),
// CHECK-NEXT:                 shared.wide,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%lu %ld %zu %llu %c %s\n".as_ptr(),
// CHECK-NEXT:                 total,
// CHECK-NEXT:                 delta,
// CHECK-NEXT:                 bytes,
// CHECK-NEXT:                 widen(1u32),
// CHECK-NEXT:                 shared.tag as i32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(shared.label) as *mut c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%lu %ld\n".as_ptr(),
// CHECK-NEXT:                 clamp_base((65530 as c_ulong), 200u8, (3 as size_t)),
// CHECK-NEXT:                 tally_sum(std::ptr::addr_of_mut!(tally) as *const tally),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%lu %llu\n".as_ptr(),
// CHECK-NEXT:                 apply(Some(add_total), total),
// CHECK-NEXT:                 shared.wide,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), (width_of_long() == 4) as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%lu %ld\n".as_ptr(),
// CHECK-NEXT:                 clamp_base((65530 as c_ulong), 200u8, (3 as size_t)),
// CHECK-NEXT:                 tally_sum(std::ptr::addr_of_mut!(tally) as *const tally),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", (width_of_long() == 8) as i32);
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
// CHECK-NEXT: file named_types_helper.rs
// CHECK-NEXT: type c_char = i8;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: type c_long = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: type c_long = i32;
// CHECK-EMPTY:
// CHECK-NEXT: type size_t = u64;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tally {
// CHECK-NEXT:     count: c_long,
// CHECK-NEXT:     bytes: size_t,
// CHECK-NEXT:     tag: c_char,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn tally_sum(tally: *const tally) -> c_long {
// CHECK-NEXT:     (unsafe { (*tally).count })
// CHECK-NEXT:         + ((unsafe { (*tally).bytes }) as c_long)
// CHECK-NEXT:         + ((unsafe { (*tally).tag }) as c_long)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn tally_sum(tally: *const tally) -> c_long {
// CHECK-NEXT:     (unsafe { (*tally).count })
// CHECK-NEXT:         + (((unsafe { (*tally).bytes }) as u32) as c_long)
// CHECK-NEXT:         + ((unsafe { (*tally).tag }) as c_long)
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
