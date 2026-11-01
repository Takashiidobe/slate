// merge-sources: units/macro_templates_helper.c
#include <stdio.h>

extern int helper(int);

static int calls;
static long total;

struct state {
    int count;
    int bits[4];
};

static int next(void) {
    calls += 1;
    return calls;
}

static int callee(int a) {
    return a + 100;
}

#define SHOW(a) { printf("%d\n", (int)(a)); }
#define WIDEN(a) { total += (a); }
#define TWICE(a) { printf("%d %d\n", (int)(a), (int)(a)); }
#define AFTER(a) { calls += 10; printf("%d\n", (int)(a)); }
#define UNUSED(a, b) { printf("%d\n", (int)(a)); }
#define TEMP(a) { int temp = (a); printf("%d\n", temp + 1); }
#define INNER(a) { printf("inner %d\n", (int)(a)); }
#define OUTER(a, b) { INNER(a); printf("outer %d\n", (int)(b)); }
#define BUMP(s) { (s)->count += 1; }
#define FILL(p, n) { for (int i = 0; i < (n); i++) (p)[i] = i; }
#define ONCE(a) do { if ((a) > 2) break; printf("small %d\n", (int)(a)); } while (0)
#define CALLS(a) { printf("%d\n", callee(a)); }
#define WRITES(p) { (p) = 7; }
#define READS_LOCAL(a) { printf("%d\n", (a) + local); }

int main(void) {
    int local = 1;
    unsigned char byte = 200;
    long wide = 1L << 33;
    int value = next();
    struct state s = {0, {0, 0, 0, 0}};
    int out = 0;

    SHOW(local);
    SHOW(local + 1);
    SHOW(byte);
    SHOW(wide >> 30);
    WIDEN(local);
    WIDEN(wide);
    WIDEN(byte);
    TWICE(next());
    AFTER(calls);
    int moved = calls;
    AFTER(moved);
    UNUSED(local, next());
    TEMP(local);
    TEMP(byte);
    OUTER(local, wide > 0);
    OUTER(local + 2, 3);
    BUMP(&s);
    BUMP(&s);
    FILL(s.bits, 4);
    ONCE(local);
    ONCE(local + 5);
    CALLS(local);
    WRITES(out);
    READS_LOCAL(value);

    printf("%d %d %d %d %ld %d %d\n", s.count, s.bits[3], out, calls, total, value, helper(local));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   macro_templates.c: [] 1 compiled
// CHECK-NEXT:   units/macro_templates_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   macro_templates.c: [] 1 compiled
// CHECK-NEXT:   units/macro_templates_helper.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit macro_templates: 45 items, 28 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit macro_templates_helper: 10 items, 7 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:helper: 2 variants
// CHECK-NEXT: macros: 15 function-like, 15 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro AFTER: 4 lifted, inline:
// CHECK-NEXT:   macro BUMP: 4 lifted, inline:
// CHECK-NEXT:   macro CALLS: 2 lifted, inline:
// CHECK-NEXT:   macro FILL: 2 lifted, inline:
// CHECK-NEXT:   macro INNER: 4 lifted, inline:
// CHECK-NEXT:   macro ONCE: 4 lifted, inline:
// CHECK-NEXT:   macro ONLY_HERE: 4 lifted, inline:
// CHECK-NEXT:   macro OUTER: 4 lifted, inline:
// CHECK-NEXT:   macro READS_LOCAL: 2 lifted, inline:
// CHECK-NEXT:   macro SHOW: 10 lifted, inline:
// CHECK-NEXT:   macro TEMP: 4 lifted, inline:
// CHECK-NEXT:   macro TWICE: 2 lifted, inline:
// CHECK-NEXT:   macro UNUSED: 2 lifted, inline:
// CHECK-NEXT:   macro WIDEN: 8 lifted, inline:
// CHECK-NEXT:   macro WRITES: 2 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 4 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file __slate_macros.rs
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: file macro_templates_helper.rs
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! ONLY_HERE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("helper {}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! ONLY_HERE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"helper %d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! WIDEN__4 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%ld\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! WIDEN__3 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%ld\n".as_ptr(), ($a) as c_long) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper(a: i32) -> i32 {
// CHECK-NEXT:     SHOW!(a);
// CHECK-NEXT:     ONLY_HERE!(a);
// CHECK-NEXT:     ONLY_HERE!(a + 1);
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         WIDEN__3!(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         WIDEN__4!(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     a * 2
// CHECK-NEXT: }
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/macro_templates_helper.c
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[macro_use]
// CHECK-NEXT: mod __slate_macros;
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "macro_templates_helper.rs"]
// CHECK-NEXT: mod __slate_unit_macro_templates_helper;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! AFTER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { calls };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             calls = __v + 10;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! AFTER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { calls };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             calls = __v + 10;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BUMP {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         let __v: *mut state = $s;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*__v).count = (*__v).count + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! CALLS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", callee($a));
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CALLS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), callee($a)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FILL {
// CHECK-NEXT:     ($p:expr, $n:expr) => {
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         while i < $n {
// CHECK-NEXT:             $p[i as usize] = i;
// CHECK-NEXT:             let __v: i32 = i;
// CHECK-NEXT:             i = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! INNER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("inner {}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! INNER {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"inner %d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! ONCE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         while ($a) <= 2 {
// CHECK-NEXT:             println!("small {}", $a);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! ONCE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         while ($a) <= 2 {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(c"small %d\n".as_ptr(), $a) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! OUTER {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         INNER!($a);
// CHECK-NEXT:         println!("outer {}", ($b) as i32);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! OUTER {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         INNER!($a);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"outer %d\n".as_ptr(), ($b) as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! OUTER__2 {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         INNER!($a);
// CHECK-NEXT:         println!("outer {}", $b);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! OUTER__2 {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         INNER!($a);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"outer %d\n".as_ptr(), $b) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! READS_LOCAL {
// CHECK-NEXT:     ($a:expr, $local:expr) => {
// CHECK-NEXT:         println!("{}", ($a) + $local);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! READS_LOCAL {
// CHECK-NEXT:     ($a:expr, $local:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), ($a) + $local) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", (($a) as u32) as i32);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), (($a) as u32) as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", ($a) as i32);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! TEMP {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let temp: i32 = (($a) as u32) as i32;
// CHECK-NEXT:         println!("{}", temp + 1);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! TEMP {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let temp: i32 = (($a) as u32) as i32;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), temp + 1) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! TEMP__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let temp: i32 = $a;
// CHECK-NEXT:         println!("{}", temp + 1);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! TEMP__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let temp: i32 = $a;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), temp + 1) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! TWICE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{} {}", $a, $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! TWICE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), $a, $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! UNUSED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! UNUSED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WIDEN {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: c_long = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + $a;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! WIDEN__5 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: c_long = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + (((($a) as u32) as i32) as c_long);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WIDEN__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: c_long = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + (($a) as c_long);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WRITES {
// CHECK-NEXT:     ($p:expr) => {
// CHECK-NEXT:         $p = 7;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut total: c_long = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct state {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     bits: [i32; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn next() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { calls }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn callee(a: i32) -> i32 {
// CHECK-NEXT:     a + 100
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let local: i32 = 1;
// CHECK-NEXT:     let byte: u8 = 200u8;
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     let wide: c_long = (8589934592 as c_long);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let wide: c_long = (1 as c_long) << 33i32;
// CHECK-NEXT:     let value: i32 = next();
// CHECK-NEXT:     let mut s: state = state {
// CHECK-NEXT:         count: 0,
// CHECK-NEXT:         bits: [0, 0, 0, 0],
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     SHOW!(local);
// CHECK-NEXT:     SHOW!(local + 1);
// CHECK-NEXT:     SHOW__2!(byte);
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         SHOW__3!(wide >> 30i32);
// CHECK-NEXT:         WIDEN__2!(local);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         SHOW!(wide >> 30i32);
// CHECK-NEXT:         WIDEN!(local);
// CHECK-NEXT:     }
// CHECK-NEXT:     WIDEN!(wide);
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         WIDEN__5!(byte);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         WIDEN__2!(byte);
// CHECK-NEXT:     }
// CHECK-NEXT:     TWICE!(next());
// CHECK-NEXT:     AFTER!(unsafe { calls });
// CHECK-NEXT:     let moved: i32 = unsafe { calls };
// CHECK-NEXT:     AFTER!(moved);
// CHECK-NEXT:     UNUSED!(local);
// CHECK-NEXT:     TEMP__2!(local);
// CHECK-NEXT:     TEMP!(byte);
// CHECK-NEXT:     OUTER!(local, wide > 0);
// CHECK-NEXT:     OUTER__2!(local + 2, 3i32);
// CHECK-NEXT:     BUMP!(std::ptr::addr_of_mut!(s));
// CHECK-NEXT:     BUMP!(std::ptr::addr_of_mut!(s));
// CHECK-NEXT:     FILL!(s.bits, 4i32);
// CHECK-NEXT:     ONCE!(local);
// CHECK-NEXT:     ONCE!(local + 5);
// CHECK-NEXT:     CALLS!(local);
// CHECK-NEXT:     WRITES!(out);
// CHECK-NEXT:     READS_LOCAL!(value, local);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d %ld %d %d\n".as_ptr(),
// CHECK-NEXT:                 s.count,
// CHECK-NEXT:                 s.bits[3],
// CHECK-NEXT:                 out,
// CHECK-NEXT:                 calls,
// CHECK-NEXT:                 total,
// CHECK-NEXT:                 value,
// CHECK-NEXT:                 helper(local),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d %ld %d %d\n".as_ptr(),
// CHECK-NEXT:                 s.count,
// CHECK-NEXT:                 s.bits[3],
// CHECK-NEXT:                 out,
// CHECK-NEXT:                 calls,
// CHECK-NEXT:                 total,
// CHECK-NEXT:                 value,
// CHECK-NEXT:                 helper(local),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
