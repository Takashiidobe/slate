// merge-sources: units/macro_calls_helper.c
#include <stdio.h>
#include <stdlib.h>

extern int exported_bump(int *value);
extern int exported_length(const char *text);
extern int helper_run(void);

static int local_counter;

static void bump(int *value) {
    *value += 1;
}

static int length_of(const char *text) {
    int count = 0;
    while (text[count]) {
        count++;
    }
    return count;
}

static int sum(const int *values, int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += values[i];
    }
    return total;
}

static int *make_buffer(int count) {
    int *buffer = malloc((size_t)count * sizeof(int));
    for (int i = 0; i < count; i++) {
        buffer[i] = i + 1;
    }
    return buffer;
}

static int same_name(int value) {
    return value + 1;
}

static int only_in_macro(int value) {
    return value * 3;
}

static int twice(int value);

#define CALL_BUMP(p) { bump(p); }
#define CALL_LENGTH(s) { printf("%d\n", length_of(s)); }
#define CALL_SUM(v, n) { printf("%d\n", sum(v, n)); }
#define CALL_BUFFER(n) { int *made = make_buffer(n); printf("%d\n", made[(n) - 1]); free(made); }
#define CALL_SAME(a) { printf("%d\n", same_name(a)); }
#define CALL_ONLY(a) { printf("%d\n", only_in_macro(a)); }
#define CALL_EXPORTED(p) { exported_bump(p); }
#define CALL_EXPORTED_LENGTH(s) { printf("%d\n", exported_length(s)); }
#define THROUGH_POINTER(a) { int (*fn)(int) = twice; printf("%d\n", fn(a)); }
#define CALL_AND_WRITE(x) { (x) = same_name(x); }
#define CALL_INNER(a) { CALL_SAME(a); CALL_ONLY(a); }
#define COUNT_CALL() { local_counter = same_name(local_counter); }

static int twice(int value) {
    return value * 2;
}

int main(void) {
    int value = 1;
    int direct = 1;
    int numbers[3] = {1, 2, 3};

    CALL_BUMP(&value);
    bump(&direct);
    CALL_LENGTH("macro");
    printf("%d\n", length_of("direct"));
    CALL_SUM(numbers, 3);
    printf("%d\n", sum(numbers, 2));
    CALL_BUFFER(4);
    int *direct_buffer = make_buffer(2);
    printf("%d\n", direct_buffer[1]);
    free(direct_buffer);
    CALL_SAME(value);
    CALL_ONLY(value);
    CALL_EXPORTED(&value);
    CALL_EXPORTED_LENGTH("exported");
    THROUGH_POINTER(value);
    CALL_AND_WRITE(value);
    CALL_INNER(value);
    COUNT_CALL();
    COUNT_CALL();

    printf("%d %d %d %d\n", value, direct, local_counter, helper_run());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   macro_calls.c: [] 1 compiled
// CHECK-NEXT:   units/macro_calls_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   macro_calls.c: [] 1 compiled
// CHECK-NEXT:   units/macro_calls_helper.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit macro_calls: 41 items, 18 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:make_buffer: 2 variants
// CHECK-NEXT:   spliced fn:twice: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit macro_calls_helper: 8 items, 3 gated, 0 duplicated functions
// CHECK-NEXT: macros: 12 function-like, 12 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro CALL_AND_WRITE: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_BUFFER: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_BUMP: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_EXPORTED: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_EXPORTED_LENGTH: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_INNER: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_LENGTH: 2 lifted, inline:
// CHECK-NEXT:   macro CALL_ONLY: 4 lifted, inline:
// CHECK-NEXT:   macro CALL_SAME: 6 lifted, inline:
// CHECK-NEXT:   macro CALL_SUM: 2 lifted, inline:
// CHECK-NEXT:   macro COUNT_CALL: 4 lifted, inline:
// CHECK-NEXT:   macro THROUGH_POINTER: 2 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 7 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file __slate_macros.rs
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALL_SAME {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", same_name($a));
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALL_SAME {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), same_name($a)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: file macro_calls_helper.rs
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn exported_bump(value: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *value = *value + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *value }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn exported_length(text: *const c_char) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     while (unsafe { *text.offset(count as isize) }) != 0 {
// CHECK-NEXT:         count += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper_run() -> i32 {
// CHECK-NEXT:     let local: i32 = 5;
// CHECK-NEXT:     CALL_SAME!(local);
// CHECK-NEXT:     local
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn same_name(value: i32) -> i32 {
// CHECK-NEXT:     value + 100
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALL_BUFFER {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         let made: *mut i32 = make_buffer($n);
// CHECK-NEXT:         println!("{}", unsafe { *made.offset((($n) - 1) as isize) });
// CHECK-NEXT:         unsafe { free((made as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALL_EXPORTED_LENGTH {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         println!("{}", unsafe { exported_length(($s) as *const c_char) });
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALL_LENGTH {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         println!("{}", length_of(($s) as *const c_char));
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALL_ONLY {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", only_in_macro($a));
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALL_SUM {
// CHECK-NEXT:     ($v:expr, $n:expr) => {
// CHECK-NEXT:         println!("{}", sum(($v) as *const i32, $n));
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! THROUGH_POINTER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let r#fn: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(twice);
// CHECK-NEXT:         println!("{}", unsafe { r#fn.unwrap()($a) });
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: type __SlateHoist_make_buffer = c_ulong;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALL_BUFFER {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         let made: *mut i32 = make_buffer($n);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), *made.offset((($n) - 1) as isize)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         unsafe { free((made as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALL_EXPORTED_LENGTH {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), exported_length(($s) as *const c_char)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALL_LENGTH {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), length_of(($s) as *const c_char)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALL_ONLY {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), only_in_macro($a)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALL_SUM {
// CHECK-NEXT:     ($v:expr, $n:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), sum(($v) as *const i32, $n)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! THROUGH_POINTER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let r#fn: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(twice);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), r#fn.unwrap()($a)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: type __SlateHoist_make_buffer = u64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg_attr(unix, unsafe(link_section = ".text.slate_distinct.macro_calls.twice"))]
// CHECK-NEXT: extern "C-unwind" fn twice(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[macro_use]
// CHECK-NEXT: mod __slate_macros;
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "macro_calls_helper.rs"]
// CHECK-NEXT: mod __slate_unit_macro_calls_helper;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/macro_calls_helper.c
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 1;
// CHECK-NEXT:     let mut direct: i32 = 1;
// CHECK-NEXT:     let mut numbers: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     CALL_BUMP!(std::ptr::addr_of_mut!(value));
// CHECK-NEXT:     bump(std::ptr::addr_of_mut!(direct));
// CHECK-NEXT:     CALL_LENGTH!(c"macro".as_ptr() as *mut c_char);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), length_of(c"direct".as_ptr())) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", length_of(c"direct".as_ptr()));
// CHECK-NEXT:     }
// CHECK-NEXT:     CALL_SUM!(numbers.as_mut_ptr() as *mut i32, 3i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 sum(numbers.as_mut_ptr() as *const i32, 2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     let __slate_printf_arg0 = sum(numbers.as_mut_ptr() as *const i32, 2i32);
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     }
// CHECK-NEXT:     CALL_BUFFER!(4i32);
// CHECK-NEXT:     let direct_buffer: *mut i32 = make_buffer(2i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), *direct_buffer.add(1)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", unsafe { *direct_buffer.add(1) });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free((direct_buffer as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     CALL_SAME!(value);
// CHECK-NEXT:     CALL_ONLY!(value);
// CHECK-NEXT:     CALL_EXPORTED!(std::ptr::addr_of_mut!(value));
// CHECK-NEXT:     CALL_EXPORTED_LENGTH!(c"exported".as_ptr() as *mut c_char);
// CHECK-NEXT:     THROUGH_POINTER!(value);
// CHECK-NEXT:     CALL_AND_WRITE!(value);
// CHECK-NEXT:     CALL_INNER!(value);
// CHECK-NEXT:     COUNT_CALL!();
// CHECK-NEXT:     COUNT_CALL!();
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 value,
// CHECK-NEXT:                 direct,
// CHECK-NEXT:                 local_counter,
// CHECK-NEXT:                 helper_run(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {}",
// CHECK-NEXT:             value,
// CHECK-NEXT:             direct,
// CHECK-NEXT:             unsafe { local_counter },
// CHECK-NEXT:             unsafe { helper_run() }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bump([[V_0:__v[0-9]+]]: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = *[[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn length_of(text: *const c_char) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     while (unsafe { *text.offset(count as isize) }) != 0 {
// CHECK-NEXT:         count += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn make_buffer(count: i32) -> *mut i32 {
// CHECK-NEXT:     let buffer: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc(
// CHECK-NEXT:             ((count as size_t).wrapping_mul(std::mem::size_of::<i32>() as size_t)
// CHECK-NEXT:                 as __SlateHoist_make_buffer) as usize,
// CHECK-NEXT:         ) as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *buffer.offset(i as isize) = i + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     buffer
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn only_in_macro(value: i32) -> i32 {
// CHECK-NEXT:     value * 3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn same_name(value: i32) -> i32 {
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: *const i32, count: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         total += unsafe { *values.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALL_AND_WRITE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         $x = same_name($x);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALL_BUMP {
// CHECK-NEXT:     ($p:expr) => {
// CHECK-NEXT:         bump($p);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALL_EXPORTED {
// CHECK-NEXT:     ($p:expr) => {
// CHECK-NEXT:         unsafe { exported_bump($p) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALL_INNER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         CALL_SAME!($a);
// CHECK-NEXT:         CALL_ONLY!($a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! COUNT_CALL {
// CHECK-NEXT:     () => {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             local_counter = same_name(local_counter);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut local_counter: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exported_bump([[ARG_0:arg[0-9]+]]: *mut i32) -> i32;
// CHECK-NEXT:     fn exported_length([[ARG_0]]: *const c_char) -> i32;
// CHECK-NEXT:     fn helper_run() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
