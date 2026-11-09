// merge-sources: units/macro_fold_helper.c
#include <stdio.h>
#include <stdlib.h>

extern int helper(int);

static int verbose = 1;
static int bumps;

static void report(const char *message) {
    printf("error: %s\n", message);
}

static int next_id(int *state) {
    *state += 1;
    return *state;
}

#define LOG(x) { fprintf x; }
#define DIFFERS(a) { printf("main %d\n", a); }
#define ASSERT(cond, msg) { if (!(cond)) report(msg); }
#define NOTE(c, x) { if (verbose > 0 && (c)) fprintf x; }
#define EARLY_LIST(x, c) { if (c) fprintf x; }
#define TWICE(a) { printf("%d %d\n", a, a); }
#define DROPS(a, b) { printf("%d\n", a); }
#define STORE(p, v) { *(p)++ = (v); }
#define LOCALS(a) { int t = (a); printf("%d\n", t + 1); }
#define SHADOWS(a) { int t = 5; printf("%d\n", (a) + t); }
#define READS_LOCAL(a) { printf("%d\n", (a) + count); }
#define WRITES_LOCAL(a) { count += (a); }
#define RETURNS(a) { if ((a) < 0) return -1; }
#define COMMA_RETURN(a) return (bumps = (a), (a))
#define NAME(a) { printf("%s %d\n", #a, a); }
#define PASTE(a) { printf("%d\n", a##_value); }
#define VARIADIC(...) { printf(__VA_ARGS__); }
#define OPEN(c) if (c) {
#define CLOSE }
#define DECLARE(a) int a = 7;
#define SPLIT(a) int split = (a); printf("%d\n", split); printf("%d\n", (a));
#define print(a) { printf("%d\n", a); }
#define type(a) { printf("%d\n", a); }
#define MOVED_CONST(a) { printf("%d\n", (a) + 1); }
#define BUMPED(a) { printf("%d\n", a); bumps += 1; }

static int comma_return(int a) {
    COMMA_RETURN(a);
}

static int early(int a) {
    RETURNS(a);
    return a * 2;
}

static int with_goto(int a, int b) {
    int i = 0;
    LOG((stdout, "goto entry\n"));
again:
    ASSERT(a == b, "goto mismatch");
    LOG((stdout, "goto %d\n", i));
    NOTE(i > 0, (stdout, "goto note %d\n", i));
    i++;
    if (i < 2) goto again;
    return i;
}

static int with_switch(int mode, int count) {
    switch (mode) {
    case 0:
        LOG((stdout, "case zero %d\n", count));
        count++;
    case 1:
        ASSERT(count != 0, "count was zero");
        LOG((stdout, "case one %d\n", count));
        break;
    default:
        LOG((stdout, "default\n"));
    }
    return count;
}

int main(void) {
    int count = 2;
    int state = 0;
    int t = 40;
    int a_value = 9;
    char buffer[4] = {0};
    char *cursor = buffer;

    LOG((stdout, "start %d\n", count));
    LOG((stdout, "no args\n"));
    DIFFERS(count);
    ASSERT(count == 3, "count is not three");
    ASSERT(next_id(&state) == 1, "side effect");
    NOTE(count > 1, (stdout, "note %d\n", count));
    EARLY_LIST((stdout, "early list\n"), count);
    TWICE(count + 1);
    DROPS(count, next_id(&state));
    STORE(cursor, 'x');
    STORE(cursor, 'y');
    LOCALS(count);
    SHADOWS(count);
    SHADOWS(t);
    READS_LOCAL(1);
    WRITES_LOCAL(1);
    NAME(count);
    PASTE(a);
    VARIADIC("%d\n", count);
    OPEN(count > 0)
        printf("opened\n");
    CLOSE
    DECLARE(declared);
    printf("%d\n", declared);
    SPLIT(count);
    print(count);
    type(count);
    MOVED_CONST(1);
    MOVED_CONST(count);
    BUMPED(count);
    BUMPED(count + 1);
    for (int i = 0; i < 2; i++) {
        LOG((stdout, "loop %d\n", i));
    }
    printf("%s %d %d %d %d\n", buffer, state, bumps, count, helper(count));
    printf("%d %d %d\n", early(-1), early(4), comma_return(6));
    printf("%d %d\n", with_goto(1, 1), with_switch(0, 0));
    printf("%d\n", with_switch(1, 5) + with_switch(9, 5));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   macro_fold.c: [] 1 compiled
// CHECK-NEXT:   units/macro_fold_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   macro_fold.c: [] 1 compiled
// CHECK-NEXT:   units/macro_fold_helper.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit macro_fold: 41 items, 19 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit macro_fold_helper: 9 items, 5 gated, 0 duplicated functions
// CHECK-NEXT: macros: 25 function-like, 18 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline no_statement_region: 4
// CHECK-NEXT:   inline reserved_macro_name: 4
// CHECK-NEXT:   inline stringify_or_paste: 4
// CHECK-NEXT:   inline local_used_after_macro: 2
// CHECK-NEXT:   inline variadic: 2
// CHECK-NEXT:   macro ASSERT: 8 lifted, inline:
// CHECK-NEXT:   macro BUMPED: 4 lifted, inline:
// CHECK-NEXT:   macro COMMA_RETURN: 2 lifted, inline:
// CHECK-NEXT:   macro DECLARE: 0 lifted, inline: local_used_after_macro 2
// CHECK-NEXT:   macro DIFFERS: 4 lifted, inline:
// CHECK-NEXT:   macro DROPS: 2 lifted, inline:
// CHECK-NEXT:   macro EARLY_LIST: 2 lifted, inline:
// CHECK-NEXT:   macro LOCALS: 2 lifted, inline:
// CHECK-NEXT:   macro LOG: 18 lifted, inline:
// CHECK-NEXT:   macro MOVED_CONST: 4 lifted, inline:
// CHECK-NEXT:   macro NAME: 0 lifted, inline: stringify_or_paste 2
// CHECK-NEXT:   macro NOTE: 4 lifted, inline:
// CHECK-NEXT:   macro ONLY_HERE: 2 lifted, inline:
// CHECK-NEXT:   macro OPEN: 0 lifted, inline: no_statement_region 2
// CHECK-NEXT:   macro PASTE: 0 lifted, inline: stringify_or_paste 2
// CHECK-NEXT:   macro READS_LOCAL: 2 lifted, inline:
// CHECK-NEXT:   macro RETURNS: 2 lifted, inline:
// CHECK-NEXT:   macro SHADOWS: 2 lifted, inline: no_statement_region 2
// CHECK-NEXT:   macro SPLIT: 2 lifted, inline:
// CHECK-NEXT:   macro STORE: 4 lifted, inline:
// CHECK-NEXT:   macro TWICE: 2 lifted, inline:
// CHECK-NEXT:   macro VARIADIC: 0 lifted, inline: variadic 2
// CHECK-NEXT:   macro WRITES_LOCAL: 2 lifted, inline:
// CHECK-NEXT:   macro print: 0 lifted, inline: reserved_macro_name 2
// CHECK-NEXT:   macro type: 0 lifted, inline: reserved_macro_name 2
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 4 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file __slate_macros.rs
// CHECK-NEXT: macro_rules! LOG {
// CHECK-NEXT:     ($x__0:expr, $x__1:expr $(, $x:expr)*) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::fprintf($x__0, ($x__1) as *const c_char, $($x),*) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: file macro_fold_helper.rs
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("helper {}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! ONLY_HERE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", ($a) * 3);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"helper %d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! ONLY_HERE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), ($a) * 3) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper(value: i32) -> i32 {
// CHECK-NEXT:     LOG!(
// CHECK-NEXT:         slate_support::stdout(),
// CHECK-NEXT:         c"helper %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         value
// CHECK-NEXT:     );
// CHECK-NEXT:     ONLY_HERE!(value);
// CHECK-NEXT:     DIFFERS!(value);
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! BUMPED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:         let __v: i32 = unsafe { bumps };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bumps = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("main {}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DROPS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! LOCALS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = $a;
// CHECK-NEXT:         println!("{}", t + 1);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! MOVED_CONST {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", ($a) + 1);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! READS_LOCAL {
// CHECK-NEXT:     ($a:expr, $count:expr) => {
// CHECK-NEXT:         println!("{}", ($a) + $count);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHADOWS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = 5;
// CHECK-NEXT:         println!("{}", ($a) + t);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SPLIT {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let split: i32 = $a;
// CHECK-NEXT:         println!("{}", split);
// CHECK-NEXT:         println!("{}", $a);
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
// CHECK-NEXT: macro_rules! BUMPED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         let __v: i32 = unsafe { bumps };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bumps = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"main %d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DROPS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! LOCALS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = $a;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), t + 1) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! MOVED_CONST {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), ($a) + 1) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! READS_LOCAL {
// CHECK-NEXT:     ($a:expr, $count:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), ($a) + $count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SHADOWS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = 5;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), ($a) + t) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SPLIT {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let split: i32 = $a;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), split) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[macro_use]
// CHECK-NEXT: mod __slate_macros;
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "macro_fold_helper.rs"]
// CHECK-NEXT: mod __slate_unit_macro_fold_helper;
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
// CHECK-NEXT: //! merge-sources: units/macro_fold_helper.c
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut count: i32 = 2;
// CHECK-NEXT:     let mut state: i32 = 0;
// CHECK-NEXT:     let a_value: i32 = 9;
// CHECK-NEXT:     let mut buffer: [c_char; 4] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = (0 as c_char);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut cursor: *mut c_char = buffer.as_mut_ptr() as *mut c_char;
// CHECK-NEXT:     LOG!(
// CHECK-NEXT:         slate_support::stdout(),
// CHECK-NEXT:         c"start %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         count
// CHECK-NEXT:     );
// CHECK-NEXT:     LOG!(
// CHECK-NEXT:         slate_support::stdout(),
// CHECK-NEXT:         c"no args\n".as_ptr() as *mut c_char
// CHECK-NEXT:     );
// CHECK-NEXT:     DIFFERS!(count);
// CHECK-NEXT:     ASSERT!(count == 3, c"count is not three".as_ptr() as *mut c_char);
// CHECK-NEXT:     ASSERT!(
// CHECK-NEXT:         next_id(unsafe { &mut (*std::ptr::addr_of_mut!(state)) }) == 1,
// CHECK-NEXT:         c"side effect".as_ptr() as *mut c_char
// CHECK-NEXT:     );
// CHECK-NEXT:     NOTE!(
// CHECK-NEXT:         count > 1,
// CHECK-NEXT:         slate_support::stdout(),
// CHECK-NEXT:         c"note %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         count
// CHECK-NEXT:     );
// CHECK-NEXT:     EARLY_LIST!(
// CHECK-NEXT:         slate_support::stdout(),
// CHECK-NEXT:         c"early list\n".as_ptr() as *mut c_char,
// CHECK-NEXT:         count != 0
// CHECK-NEXT:     );
// CHECK-NEXT:     TWICE!(count + 1);
// CHECK-NEXT:     DROPS!(count);
// CHECK-NEXT:     STORE!(cursor, 120i32);
// CHECK-NEXT:     STORE!(cursor, 121i32);
// CHECK-NEXT:     LOCALS!(count);
// CHECK-NEXT:     SHADOWS!(count);
// CHECK-NEXT:     let t: i32 = 5;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), t + t) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", t + t);
// CHECK-NEXT:     }
// CHECK-NEXT:     READS_LOCAL!(1i32, count);
// CHECK-NEXT:     WRITES_LOCAL!(1i32, count);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s %d\n".as_ptr(), c"count".as_ptr() as *mut c_char, count) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), a_value) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", a_value);
// CHECK-NEXT:         println!("{}", count);
// CHECK-NEXT:     }
// CHECK-NEXT:     if count > 0 {
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { slate_support::printf(c"opened\n".as_ptr()) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("opened");
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let declared: i32 = 7;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), declared) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", declared);
// CHECK-NEXT:     }
// CHECK-NEXT:     SPLIT!(count);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", count);
// CHECK-NEXT:         println!("{}", count);
// CHECK-NEXT:     }
// CHECK-NEXT:     MOVED_CONST!(1i32);
// CHECK-NEXT:     MOVED_CONST!(count);
// CHECK-NEXT:     BUMPED!(count);
// CHECK-NEXT:     BUMPED!(count + 1);
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         LOG!(
// CHECK-NEXT:             slate_support::stdout(),
// CHECK-NEXT:             c"loop %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:             i
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             buffer.as_mut_ptr() as *mut c_char,
// CHECK-NEXT:             state,
// CHECK-NEXT:             bumps,
// CHECK-NEXT:             count,
// CHECK-NEXT:             helper(count),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 early(-1i32),
// CHECK-NEXT:                 early(4i32),
// CHECK-NEXT:                 comma_return(6i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 with_goto(1i32, 1i32),
// CHECK-NEXT:                 with_switch(0i32, 0i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 with_switch(1i32, 5i32) + with_switch(9i32, 5i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {} {}", early(-1i32), early(4i32), comma_return(6i32));
// CHECK-NEXT:         println!("{} {}", with_goto(1i32, 1i32), with_switch(0i32, 0i32));
// CHECK-NEXT:         println!("{}", with_switch(1i32, 5i32) + with_switch(9i32, 5i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn comma_return(a: i32) -> i32 {
// CHECK-NEXT:     COMMA_RETURN!(a)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn early(a: i32) -> i32 {
// CHECK-NEXT:     RETURNS!(a);
// CHECK-NEXT:     a * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn next_id(state: &mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *state = *state + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *(state as *mut i32) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn report(message: *const c_char) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"error: %s\n".as_ptr(), message) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn with_goto(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         LOG!(
// CHECK-NEXT:             slate_support::stdout(),
// CHECK-NEXT:             c"goto entry\n".as_ptr() as *mut c_char
// CHECK-NEXT:         );
// CHECK-NEXT:         loop {
// CHECK-NEXT:             ASSERT!(a == b, c"goto mismatch".as_ptr() as *mut c_char);
// CHECK-NEXT:             LOG!(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"goto %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:                 i
// CHECK-NEXT:             );
// CHECK-NEXT:             NOTE!(
// CHECK-NEXT:                 i > 0,
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"goto note %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:                 i
// CHECK-NEXT:             );
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             if i >= 2 {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return i;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn with_switch(mode: i32, mut count: i32) -> i32 {
// CHECK-NEXT:     match mode {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             LOG!(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"case zero %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:                 count
// CHECK-NEXT:             );
// CHECK-NEXT:             count += 1;
// CHECK-NEXT:             ASSERT!(count != 0, c"count was zero".as_ptr() as *mut c_char);
// CHECK-NEXT:             LOG!(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"case one %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:                 count
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             ASSERT!(count != 0, c"count was zero".as_ptr() as *mut c_char);
// CHECK-NEXT:             LOG!(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"case one %d\n".as_ptr() as *mut c_char,
// CHECK-NEXT:                 count
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             LOG!(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"default\n".as_ptr() as *mut c_char
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ASSERT {
// CHECK-NEXT:     ($cond:expr, $msg:expr) => {
// CHECK-NEXT:         if !$cond {
// CHECK-NEXT:             report(($msg) as *const c_char);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! COMMA_RETURN {
// CHECK-NEXT:     ($a:expr) => {{[{][{]}}
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bumps = $a;
// CHECK-NEXT:         }
// CHECK-NEXT:         return $a;
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! EARLY_LIST {
// CHECK-NEXT:     ($x__0:expr, $x__1:expr, $c:expr $(, $x:expr)*) => {
// CHECK-NEXT:         if $c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::fprintf($x__0, ($x__1) as *const c_char, $($x),*)
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NOTE {
// CHECK-NEXT:     ($c:expr, $x__0:expr, $x__1:expr $(, $x:expr)*) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 && $c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::fprintf($x__0, ($x__1) as *const c_char, $($x),*)
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! RETURNS {
// CHECK-NEXT:     ($a:expr) => {{[{][{]}}
// CHECK-NEXT:         if ($a) < 0 {
// CHECK-NEXT:             return -1i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! STORE {
// CHECK-NEXT:     ($p:expr, $v:expr) => {
// CHECK-NEXT:         let __v: *mut c_char = $p;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut c_char = unsafe { __v.add(1) };
// CHECK-NEXT:         $p = [[V_0]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *__v = ($v) as c_char;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WRITES_LOCAL {
// CHECK-NEXT:     ($a:expr, $count:expr) => {
// CHECK-NEXT:         let __v: i32 = $count;
// CHECK-NEXT:         let [[V_0]]: i32 = __v + $a;
// CHECK-NEXT:         $count = [[V_0]];
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut bumps: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut verbose: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
