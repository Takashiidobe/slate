#include <stdio.h>

static int flags;

static unsigned crc(unsigned c, int n) { return c * 31 + n; }
static unsigned adl(unsigned c, int n) { return c * 17 + n; }

#define DROPBITS(n) do { hold >>= (n); bits -= (unsigned)(n); } while (0)
#define BYTEBITS() do { hold >>= bits & 7; bits -= bits & 7; } while (0)
#define UPDATE_CHECK(check, len) (flags ? crc(check, len) : adl(check, len))
#define SWAP_LOCAL(a, b) do { unsigned t = (a); (a) = (b); (b) = t; } while (0)

static unsigned run(unsigned hold, unsigned bits, int k) {
    unsigned check = 1;
    int i = 0;
    if (k & 1) goto middle;
again:
    DROPBITS(1);
    BYTEBITS();
middle:
    check = UPDATE_CHECK(check, i);
    SWAP_LOCAL(hold, check);
    if (++i < k) goto again;
    if (k > 2) { DROPBITS(2); goto out; }
    DROPBITS(3);
out:
    return hold + bits + check;
}

int main(void) {
    printf("%u\n", run(0xffff, 16, 3));
    flags = 1;
    printf("%u\n", run(0xfff0, 12, 2));
    printf("%u\n", run(0xabcdef, 31, 5));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_dispatch_locals.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_dispatch_locals.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_dispatch_locals: 12 items, 0 gated, 0 duplicated functions
// CHECK-NEXT: macros: 4 function-like, 4 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro BYTEBITS: 2 lifted, inline:
// CHECK-NEXT:   macro DROPBITS: 6 lifted, inline:
// CHECK-NEXT:   macro SWAP_LOCAL: 2 lifted, inline:
// CHECK-NEXT:   macro UPDATE_CHECK: 2 lifted, inline:
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
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), run(65535u32, 16u32, 3i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         flags = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), run(65520u32, 12u32, 2i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), run(11259375u32, 31u32, 5i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn adl(c: u32, n: i32) -> u32 {
// CHECK-NEXT:     c.wrapping_mul(17).wrapping_add(n as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn crc(c: u32, n: i32) -> u32 {
// CHECK-NEXT:     c.wrapping_mul(31).wrapping_add(n as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn run(mut hold: u32, mut bits: u32, k: i32) -> u32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut check: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         check = 1;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         if k & 1 != 0 {
// CHECK-NEXT:             __slate_state = 4;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             __slate_state = 5;
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     check = UPDATE_CHECK!(check, i);
// CHECK-NEXT:                     SWAP_LOCAL!(hold, check);
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: i32 = i + 1;
// CHECK-NEXT:                     i = [[V_0]];
// CHECK-NEXT:                     if [[V_0]] < k {
// CHECK-NEXT:                         __slate_state = 5;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     DROPBITS!(1i32, hold, bits);
// CHECK-NEXT:                     BYTEBITS!(hold, bits);
// CHECK-NEXT:                     __slate_state = 4;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if k > 2 {
// CHECK-NEXT:             DROPBITS!(2i32, hold, bits);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             DROPBITS!(3i32, hold, bits);
// CHECK-NEXT:         }
// CHECK-NEXT:         return hold.wrapping_add(bits).wrapping_add(check);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BYTEBITS {
// CHECK-NEXT:     ($hold:expr, $bits:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __v: u32 = $hold;
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: u32 = __v >> (($bits) & 7);
// CHECK-NEXT:             $hold = [[V_1]];
// CHECK-NEXT:             let [[V_2:__v[0-9]+]]: u32 = $bits;
// CHECK-NEXT:             let [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_sub(($bits) & 7);
// CHECK-NEXT:             $bits = [[V_3]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! DROPBITS {
// CHECK-NEXT:     ($n:expr, $hold:expr, $bits:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __v: u32 = $hold;
// CHECK-NEXT:             let [[V_1]]: u32 = __v >> $n;
// CHECK-NEXT:             $hold = [[V_1]];
// CHECK-NEXT:             let [[V_2]]: u32 = $bits;
// CHECK-NEXT:             let [[V_3]]: u32 = [[V_2]].wrapping_sub(($n) as u32);
// CHECK-NEXT:             $bits = [[V_3]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SWAP_LOCAL {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let t: u32 = $a;
// CHECK-NEXT:             $a = $b;
// CHECK-NEXT:             $b = t;
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! UPDATE_CHECK {
// CHECK-NEXT:     ($check:expr, $len:expr) => {{[{][{]}}
// CHECK-NEXT:         let __v: u32 = if (unsafe { flags }) != 0 {
// CHECK-NEXT:             crc($check, $len)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             adl($check, $len)
// CHECK-NEXT:         };
// CHECK-NEXT:         __v
// CHECK-NEXT:     }};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut flags: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
