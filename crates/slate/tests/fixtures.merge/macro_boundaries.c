// merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang
#include <stdio.h>
#include <stdlib.h>

static int verbose = 1;

static void report(const char *message) {
    printf("error: %s\n", message);
}

#define ASSERT(cond, msg) { if (!(cond)) report(msg); }
#define TRACE(x) { if (verbose > 0) printf x; }
#define NOTE(c, x) { if (verbose > 0 && (c)) printf x; }
#define QUIET(x)
#define BUMP(x) { count += (x); }
#define GUARD(x) { if (!(x)) return -1; }

static int next_id(int *state) {
    *state += 1;
    return *state;
}

static int checked(int a, int b) {
    int count = 0;
    int state = 0;
    ASSERT(a == b, "a and b differ");
    ASSERT(a + 1 == b + 1, "still differ");
    ASSERT((1 << 3) - 1 == 7, "folded");
    ASSERT(next_id(&state) == 1, "side effect");
    TRACE(("a %d b %d\n", a, b));
    NOTE(a != 0, ("nonzero %d\n", a));
    QUIET(("dropped %d\n", next_id(&state)));
    BUMP(a);
    GUARD(b >= 0);
    if (a > 100) ASSERT(b > 100, "nested");
    for (int i = 0; i < 2; i++) {
        TRACE(("loop %d\n", i));
    }
    printf("state %d count %d\n", state, count);
    return !(a == b);
}

int main(void) {
    printf("%d\n", checked(3, 3));
    printf("%d\n", checked(3, 4));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_boundaries.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_boundaries.c: [] 1 compiled
// CHECK-NEXT: target linux-i686: 1 compiles
// CHECK-NEXT:   macro_boundaries.c: [] 1 compiled
// CHECK-NEXT: total: 3 compiles across 3 targets
// CHECK-NEXT: unit macro_boundaries: 15 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:checked: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 6 function-like, 5 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline empty_expansion: 3
// CHECK-NEXT:   macro ASSERT: 15 lifted, inline:
// CHECK-NEXT:   macro BUMP: 3 lifted, inline:
// CHECK-NEXT:   macro GUARD: 3 lifted, inline:
// CHECK-NEXT:   macro NOTE: 3 lifted, inline:
// CHECK-NEXT:   macro QUIET: 0 lifted, inline: empty_expansion 3
// CHECK-NEXT:   macro TRACE: 6 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
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
// CHECK-NEXT: //! merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86", target_os = "linux", target_env = "gnu")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), checked(3i32, 3i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), checked(3i32, 4i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", checked(3i32, 3i32));
// CHECK-NEXT:         println!("{}", checked(3i32, 4i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn checked(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     let mut state: i32 = 0;
// CHECK-NEXT:     ASSERT!(a == b, c"a and b differ".as_ptr() as *mut c_char);
// CHECK-NEXT:     ASSERT!(a + 1 == b + 1, c"still differ".as_ptr() as *mut c_char);
// CHECK-NEXT:     ASSERT!(7i32 == 7, c"folded".as_ptr() as *mut c_char);
// CHECK-NEXT:     ASSERT!(
// CHECK-NEXT:         next_id(unsafe { &mut (*std::ptr::addr_of_mut!(state)) }) == 1,
// CHECK-NEXT:         c"side effect".as_ptr() as *mut c_char
// CHECK-NEXT:     );
// CHECK-NEXT:     TRACE!(c"a %d b %d\n".as_ptr() as *mut c_char, a, b);
// CHECK-NEXT:     NOTE!(a != 0, c"nonzero %d\n".as_ptr() as *mut c_char, a);
// CHECK-NEXT:     BUMP!(a, count);
// CHECK-NEXT:     GUARD!(b >= 0);
// CHECK-NEXT:     if a > 100 {
// CHECK-NEXT:         ASSERT!(b > 100, c"nested".as_ptr() as *mut c_char);
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         TRACE__2!(c"loop %d\n".as_ptr() as *mut c_char, i);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"state %d count %d\n".as_ptr(), state, count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("state {} count {}", state, count);
// CHECK-NEXT:     }
// CHECK-NEXT:     (a != b) as i32
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
// CHECK-NEXT: macro_rules! ASSERT {
// CHECK-NEXT:     ($cond:expr, $msg:expr) => {
// CHECK-NEXT:         if !$cond {
// CHECK-NEXT:             report(($msg) as *const c_char);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BUMP {
// CHECK-NEXT:     ($x:expr, $count:expr) => {
// CHECK-NEXT:         let __v: i32 = $count;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = __v + $x;
// CHECK-NEXT:         $count = [[V_0]];
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! GUARD {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         if !$x {
// CHECK-NEXT:             return -1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NOTE {
// CHECK-NEXT:     ($c:expr, $x__0:expr, $x__1:expr) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 && $c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(($x__0) as *const c_char, $x__1) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TRACE {
// CHECK-NEXT:     ($x__0:expr, $x__1:expr, $x__2:expr) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(($x__0) as *const c_char, $x__1, $x__2) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TRACE__2 {
// CHECK-NEXT:     ($x__0:expr, $x__1:expr) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(($x__0) as *const c_char, $x__1) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut verbose: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
