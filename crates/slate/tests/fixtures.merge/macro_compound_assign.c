#include <stdio.h>
#include <sys/types.h>

#define BASE 65521U
#define MOD(a) a %= BASE
#define MOD28(a) a %= BASE
#define MOD63(a) a %= BASE
#define SCALE(a, k) a *= (k)
#define ADD_TO(a, v) ((a) += (v))

static unsigned long adler(const unsigned char *buf, int len) {
    unsigned long a = 1;
    unsigned long b = 0;
    for (int i = 0; i < len; i++) {
        a += buf[i];
        b += a;
    }
    MOD(a);
    MOD28(b);
    return (b << 16) | a;
}

static off_t combine(off_t len, int y) {
    MOD63(len);
    MOD(y);
    SCALE(y, 3);
    return len + y;
}

int main(void) {
    const unsigned char text[] = "hello, world";
    int total = 0;
    int seen = ADD_TO(total, 5);
    ADD_TO(total, seen);
    printf("%lu\n", adler(text, (int)sizeof text - 1));
    printf("%lld\n", (long long)combine((off_t)100000, 70000));
    printf("%d %d\n", total, seen);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_compound_assign.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_compound_assign.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_compound_assign: 18 items, 3 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 5 function-like, 5 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro ADD_TO: 4 lifted, inline:
// CHECK-NEXT:   macro MOD: 4 lifted, inline:
// CHECK-NEXT:   macro MOD28: 2 lifted, inline:
// CHECK-NEXT:   macro MOD63: 2 lifted, inline:
// CHECK-NEXT:   macro SCALE: 2 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"), all(target_arch = "x86_64", windows, target_env = "msvc"))))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! MOD63 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         $a %= 65521;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! MOD63 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         $a = ((($a) as c_ulong) % 65521) as off_t;
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
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let text: [u8; 13] = [104, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 0];
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let seen: i32 = ADD_TO!(total, 5i32);
// CHECK-NEXT:     ADD_TO__2!(total, seen);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu\n".as_ptr(),
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]] = (core::mem::size_of::<[u8; 13]>() as i32) - 1;
// CHECK-NEXT:                 adler(text.as_slice(), [[T_0]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             combine(100000 as off_t, 70000i32) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), total, seen) };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", total, seen);
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn adler(buf: &[u8], len: i32) -> c_ulong {
// CHECK-NEXT:     let mut a: c_ulong = 1;
// CHECK-NEXT:     let mut b: c_ulong = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         a = a.wrapping_add((buf[i as usize] as i32) as c_ulong);
// CHECK-NEXT:         b = b.wrapping_add(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     MOD__2!(a);
// CHECK-NEXT:     MOD28!(b);
// CHECK-NEXT:     b << 16i32 | a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn combine(mut len: off_t, mut y: i32) -> off_t {
// CHECK-NEXT:     MOD63!(len);
// CHECK-NEXT:     MOD!(y);
// CHECK-NEXT:     SCALE!(y, 3i32);
// CHECK-NEXT:     len + (y as off_t)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD_TO {
// CHECK-NEXT:     ($a:expr, $v:expr) => {
// CHECK-NEXT:         {
// CHECK-NEXT:             let __v: i32 = ($a) + $v;
// CHECK-NEXT:             $a = __v;
// CHECK-NEXT:             __v
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD_TO__2 {
// CHECK-NEXT:     ($a:expr, $v:expr) => {
// CHECK-NEXT:         $a += $v;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MOD {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         $a = ((($a) as u32) % 65521) as i32;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MOD28 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         $a %= 65521;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MOD__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         $a %= 65521;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SCALE {
// CHECK-NEXT:     ($a:expr, $k:expr) => {
// CHECK-NEXT:         $a *= $k;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::off_t;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
