#include <stdio.h>
#include <stdlib.h>

static int trace;

#define CHECK(x) { if ((x) < 0) return -1; }
#define FAIL(x) return (trace = (x), (x))
#define BAIL() return
#define BAIL_IF(c) do { if (c) return; } while (0)
#define CHECK_BOTH(x) { CHECK(x); CHECK((x) - 1); }
#define NULL_GUARD(p) { if (!(p)) return NULL; }
#define NEG_NULL(n) { if ((n) < 0) return NULL; }
#define TAKE(x) ({ if ((x) < 0) return -2; (x) + 1; })
#define PICK(x) { if (x) return 1; else return 2; }
#define OWN_LOOP(n) do { for (int i = 0; i < (n); i++) { if (i == 3) break; if (i == 1) continue; trace += i; } } while (0)
#define SKIP_IF(c) { if (c) continue; }
#define STOP_IF(c) { if (c) break; }

static int checks(int a) {
    CHECK(a);
    return a + 1;
}

static int tail_only(int a) {
    FAIL(a);
}

static void bails(int a) {
    printf("bails %d\n", a);
    BAIL_IF(a);
    printf("kept going\n");
    BAIL();
}

static int nested(int a) {
    CHECK_BOTH(a);
    return a * 3;
}

static char *guarded_pointer(char *s, int n) {
    NULL_GUARD(s);
    return s + n;
}

static int *guarded_buffer(int n) {
    int *p = n < 0 ? NULL : malloc(4 * sizeof(int));
    NULL_GUARD(p);
    p[0] = n;
    return p;
}

static int *hidden_null_return(int *p, int n) {
    NEG_NULL(n);
    *p += n;
    return p;
}

static int *hidden_buffer_return(int n) {
    NEG_NULL(n);
    int *p = malloc(n * sizeof(int));
    p[0] = n;
    return p;
}

static int value_position(int a) {
    int t = TAKE(a);
    return t * 2;
}

static int in_switch(int m) {
    switch (m) {
    case 0:
        PICK(m);
    case 1:
        return 7;
    default:
        break;
    }
    return 9;
}

static int in_loop(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        SKIP_IF(i == 1);
        STOP_IF(i == 3);
        CHECK(sum - 100);
        sum += i;
    }
    OWN_LOOP(n);
    return sum;
}

static int with_goto(int a) {
    int i = 0;
again:
    CHECK(a);
    if (++i < 2) goto again;
    return i;
}

int main(void) {
    char text[] = "abc";
    int *buffer = guarded_buffer(5);
    printf("%d %d\n", checks(-1), checks(1));
    printf("%d %d\n", tail_only(4), trace);
    bails(1);
    bails(0);
    printf("%d %d %d\n", nested(0), nested(2), nested(-1));
    printf("%s %d\n", guarded_pointer(text, 1), guarded_pointer(NULL, 1) == NULL);
    printf("%d %d\n", buffer[0], guarded_buffer(-1) == NULL);
    int cell = 1;
    int *fresh = hidden_buffer_return(8);
    printf("%d %d\n", *hidden_null_return(&cell, 2), hidden_null_return(&cell, -1) == NULL);
    printf("%d %d\n", fresh[0], hidden_buffer_return(-1) == NULL);
    free(fresh);
    printf("%d %d\n", value_position(-1), value_position(2));
    printf("%d %d %d\n", in_switch(0), in_switch(1), in_switch(2));
    printf("%d %d\n", in_loop(6), trace);
    printf("%d %d\n", with_goto(1), with_goto(-1));
    free(buffer);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_control_flow.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_control_flow.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_control_flow: 31 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:bails: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 12 function-like, 10 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline break_out_of_macro: 4
// CHECK-NEXT:   macro BAIL: 2 lifted, inline:
// CHECK-NEXT:   macro BAIL_IF: 2 lifted, inline:
// CHECK-NEXT:   macro CHECK: 10 lifted, inline:
// CHECK-NEXT:   macro CHECK_BOTH: 2 lifted, inline:
// CHECK-NEXT:   macro FAIL: 2 lifted, inline:
// CHECK-NEXT:   macro NEG_NULL: 4 lifted, inline:
// CHECK-NEXT:   macro NULL_GUARD: 4 lifted, inline:
// CHECK-NEXT:   macro OWN_LOOP: 2 lifted, inline:
// CHECK-NEXT:   macro PICK: 2 lifted, inline:
// CHECK-NEXT:   macro SKIP_IF: 0 lifted, inline: break_out_of_macro 2
// CHECK-NEXT:   macro STOP_IF: 0 lifted, inline: break_out_of_macro 2
// CHECK-NEXT:   macro TAKE: 2 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut text: [c_char; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let buffer: *mut i32 = guarded_buffer(5i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), checks(-1i32), checks(1i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), tail_only(4i32), trace) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", checks(-1i32), checks(1i32));
// CHECK-NEXT:         println!("{} {}", tail_only(4i32), unsafe { trace });
// CHECK-NEXT:     }
// CHECK-NEXT:     bails(1i32);
// CHECK-NEXT:     bails(0i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 nested(0i32),
// CHECK-NEXT:                 nested(2i32),
// CHECK-NEXT:                 nested(-1i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {} {}", nested(0i32), nested(2i32), nested(-1i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %d\n".as_ptr(),
// CHECK-NEXT:             guarded_pointer(text.as_mut_ptr() as *mut c_char, 1i32),
// CHECK-NEXT:             guarded_pointer(std::ptr::null_mut::<c_char>(), 1i32).is_null() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 *buffer.add(0),
// CHECK-NEXT:                 guarded_buffer(-1i32).is_null() as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             unsafe { *buffer.add(0) },
// CHECK-NEXT:             guarded_buffer(-1i32).is_null() as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut cell: i32 = 1;
// CHECK-NEXT:     let fresh: *mut i32 = hidden_buffer_return(8i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 *hidden_null_return(std::ptr::addr_of_mut!(cell), 2i32),
// CHECK-NEXT:                 hidden_null_return(std::ptr::addr_of_mut!(cell), -1i32).is_null() as i32,
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
// CHECK-NEXT:                 *fresh.add(0),
// CHECK-NEXT:                 hidden_buffer_return(-1i32).is_null() as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             unsafe { *hidden_null_return(std::ptr::addr_of_mut!(cell), 2i32) },
// CHECK-NEXT:             hidden_null_return(std::ptr::addr_of_mut!(cell), -1i32).is_null() as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             unsafe { *fresh.add(0) },
// CHECK-NEXT:             hidden_buffer_return(-1i32).is_null() as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free((fresh as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 value_position(-1i32),
// CHECK-NEXT:                 value_position(2i32),
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
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 in_switch(0i32),
// CHECK-NEXT:                 in_switch(1i32),
// CHECK-NEXT:                 in_switch(2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), in_loop(6i32), trace) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), with_goto(1i32), with_goto(-1i32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", value_position(-1i32), value_position(2i32));
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             in_switch(0i32),
// CHECK-NEXT:             in_switch(1i32),
// CHECK-NEXT:             in_switch(2i32)
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{} {}", in_loop(6i32), unsafe { trace });
// CHECK-NEXT:         println!("{} {}", with_goto(1i32), with_goto(-1i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free((buffer as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bails(a: i32) {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"bails %d\n".as_ptr(), a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("bails {}", a);
// CHECK-NEXT:     }
// CHECK-NEXT:     BAIL_IF!(a != 0);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"kept going\n".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("kept going");
// CHECK-NEXT:     }
// CHECK-NEXT:     BAIL!();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn checks(a: i32) -> i32 {
// CHECK-NEXT:     CHECK!(a);
// CHECK-NEXT:     a + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn guarded_buffer(n: i32) -> *mut i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut () = if n < 0 {
// CHECK-NEXT:         std::ptr::null_mut::<()>()
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             malloc((4 as size_t).wrapping_mul(std::mem::size_of::<i32>() as size_t) as usize)
// CHECK-NEXT:                 as *mut ()
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let p: *mut i32 = [[V_0]] as *mut i32;
// CHECK-NEXT:     NULL_GUARD__2!(p);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.add(0) = n;
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn guarded_pointer(s: *mut c_char, n: i32) -> *mut c_char {
// CHECK-NEXT:     NULL_GUARD!(s);
// CHECK-NEXT:     unsafe { s.offset(n as isize) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn hidden_buffer_return(n: i32) -> *mut i32 {
// CHECK-NEXT:     NEG_NULL!(n);
// CHECK-NEXT:     let p: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc((n as size_t).wrapping_mul(std::mem::size_of::<i32>() as size_t) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.add(0) = n;
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn hidden_null_return(p: *mut i32, n: i32) -> *mut i32 {
// CHECK-NEXT:     NEG_NULL!(n);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = *p + n;
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn in_loop(n: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if i != 1 {
// CHECK-NEXT:             if i == 3 {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             CHECK!(sum - 100);
// CHECK-NEXT:             sum += i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     OWN_LOOP!(n);
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn in_switch(m: i32) -> i32 {
// CHECK-NEXT:     match m {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             PICK!(m != 0);
// CHECK-NEXT:             return 7;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return 7;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     9
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn nested(a: i32) -> i32 {
// CHECK-NEXT:     CHECK_BOTH!(a);
// CHECK-NEXT:     a * 3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn tail_only(a: i32) -> i32 {
// CHECK-NEXT:     FAIL!(a)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn value_position(a: i32) -> i32 {
// CHECK-NEXT:     let t: i32 = TAKE!(a);
// CHECK-NEXT:     t * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn with_goto(a: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             CHECK!(a);
// CHECK-NEXT:             let [[V_0]]: i32 = i + 1;
// CHECK-NEXT:             i = [[V_0]];
// CHECK-NEXT:             if [[V_0]] >= 2 {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return i;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BAIL {
// CHECK-NEXT:     () => {{[{][{]}}
// CHECK-NEXT:         return;
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BAIL_IF {
// CHECK-NEXT:     ($c:expr) => {{[{][{]}}
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if $c {
// CHECK-NEXT:                 return;
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         if ($x) < 0 {
// CHECK-NEXT:             return -1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CHECK_BOTH {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         CHECK!($x);
// CHECK-NEXT:         CHECK!(($x) - 1);
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FAIL {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             trace = $x;
// CHECK-NEXT:         }
// CHECK-NEXT:         return $x;
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NEG_NULL {
// CHECK-NEXT:     ($n:expr) => {{[{][{]}}
// CHECK-NEXT:         if ($n) < 0 {
// CHECK-NEXT:             return std::ptr::null_mut::<i32>();
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NULL_GUARD {
// CHECK-NEXT:     ($p:expr) => {{[{][{]}}
// CHECK-NEXT:         if $p.is_null() {
// CHECK-NEXT:             return std::ptr::null_mut::<c_char>();
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NULL_GUARD__2 {
// CHECK-NEXT:     ($p:expr) => {{[{][{]}}
// CHECK-NEXT:         if $p.is_null() {
// CHECK-NEXT:             return std::ptr::null_mut::<i32>();
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! OWN_LOOP {
// CHECK-NEXT:     ($n:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut i: i32 = 0;
// CHECK-NEXT:             while i < $n {
// CHECK-NEXT:                 if i == 3 {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if i != 1 {
// CHECK-NEXT:                     let __v: i32 = unsafe { trace };
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         trace = __v + i;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 i += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PICK {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         if $x {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TAKE {
// CHECK-NEXT:     ($x:expr) => {{[{][{]}}
// CHECK-NEXT:         let mut __v: i32;
// CHECK-NEXT:         if ($x) < 0 {
// CHECK-NEXT:             return -2;
// CHECK-NEXT:         }
// CHECK-NEXT:         __v = ($x) + 1;
// CHECK-NEXT:         __v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut trace: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
