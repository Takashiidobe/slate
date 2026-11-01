#include <stdio.h>

typedef unsigned short ush;
typedef unsigned char uch;

static ush buffer;
static uch bytes[8];
static int used;
static int flips;

#define PUT(value, room) \
{ if (room) { \
    int val = (int)value; \
    buffer |= (ush)val << 1; \
    flips += val; \
  } else { \
    buffer |= (ush)(value) << 1; \
  } \
}

#define STORE_BYTE(c) { bytes[used++] = (uch)(c); }
#define SUM_TWICE(a) { flips += (a) + (a); }
#define MIXED(a) { flips += (a) * 2; buffer ^= (ush)(a); }
#define UNHYGIENIC(a) { flips += a * a; }

int main(void) {
    unsigned lc = 70000u;
    int wide = 70001;
    ush small = 3;
    uch code = 200;

    PUT(wide, flips > 0);
    PUT(wide - 1, flips < 0);
    PUT(wide + 2, flips == 0);
    PUT(small, flips == 0);
    PUT(lc, flips > 0);
    PUT(lc - 1, 1);
    PUT(code + 1, flips);
    PUT((int)lc, flips != 0);

    STORE_BYTE(wide);
    STORE_BYTE(small);
    STORE_BYTE(code);
    STORE_BYTE(lc >> 3);

    SUM_TWICE(wide);
    SUM_TWICE(code);
    MIXED(wide);
    MIXED(small + 1);
    UNHYGIENIC(small + 1);
    UNHYGIENIC(code);

    printf("%d %d %d %d %d\n", buffer, flips, used, bytes[0], bytes[3]);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_arg_casts.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_arg_casts.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_arg_casts: 19 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 5 function-like, 5 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline split_argument: 10
// CHECK-NEXT:   macro MIXED: 4 lifted, inline:
// CHECK-NEXT:   macro PUT: 8 lifted, inline: split_argument 8
// CHECK-NEXT:   macro STORE_BYTE: 8 lifted, inline:
// CHECK-NEXT:   macro SUM_TWICE: 4 lifted, inline:
// CHECK-NEXT:   macro UNHYGIENIC: 2 lifted, inline: split_argument 2
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
// CHECK-NEXT: macro_rules! MIXED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { flips };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = __v + ($a) * 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: u16 = unsafe { buffer };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer =
// CHECK-NEXT:                 (((([[V_0]] as u32) as i32) ^ ((((($a) as i16) as u16) as u32) as i32)) as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT__2 {
// CHECK-NEXT:     ($value:expr, $room:expr) => {
// CHECK-NEXT:         if $room {
// CHECK-NEXT:             let val: i32 = (($value) as u32) as i32;
// CHECK-NEXT:             let __v: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = ((((__v as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                     as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 flips = flips + val;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer =
// CHECK-NEXT:                     (((([[V_1]] as u32) as i32) | ((($value) as u32) as i32) << 1i32) as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT {
// CHECK-NEXT:     ($value:expr, $room:expr) => {
// CHECK-NEXT:         if $room {
// CHECK-NEXT:             let val: i32 = $value;
// CHECK-NEXT:             let __v: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = ((((__v as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                     as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 flips = flips + val;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_1]]: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = (((([[V_1]] as u32) as i32)
// CHECK-NEXT:                     | ((((($value) as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                     as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT__3 {
// CHECK-NEXT:     ($value:expr, $room:expr) => {
// CHECK-NEXT:         if $room {
// CHECK-NEXT:             let val: i32 = ($value) as i32;
// CHECK-NEXT:             let __v: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = ((((__v as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                     as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 flips = flips + val;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_1]]: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = (((([[V_1]] as u32) as i32) | (((($value) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                     as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! STORE_BYTE {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { used };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             used = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bytes[__v as usize] = ($c) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! STORE_BYTE__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { used };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             used = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bytes[__v as usize] = (($c) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! STORE_BYTE__3 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { used };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             used = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bytes[__v as usize] = $c;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! STORE_BYTE__4 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { used };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             used = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bytes[__v as usize] = ($c) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SUM_TWICE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { flips };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = __v + (($a) + $a);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SUM_TWICE__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { flips };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = __v + (((($a) as u32) as i32) + ((($a) as u32) as i32));
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! UNHYGIENIC {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { flips };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = __v + ((($a) as u32) as i32) * ((($a) as u32) as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut buffer: u16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut bytes: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut used: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut flips: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let lc: u32 = 70000;
// CHECK-NEXT:     let wide: i32 = 70001;
// CHECK-NEXT:     let small: u16 = 3;
// CHECK-NEXT:     let code: u8 = 200u8;
// CHECK-NEXT:     PUT!(wide, (unsafe { flips }) > 0);
// CHECK-NEXT:     if (unsafe { flips }) < 0 {
// CHECK-NEXT:         let val: i32 = wide - 1;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = flips + val;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32)
// CHECK-NEXT:                 | (((((wide - 1) as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { flips }) == 0 {
// CHECK-NEXT:         let val: i32 = wide + 2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = flips + val;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32)
// CHECK-NEXT:                 | (((((wide + 2) as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     PUT__2!(small, (unsafe { flips }) == 0);
// CHECK-NEXT:     PUT__3!(lc, (unsafe { flips }) > 0);
// CHECK-NEXT:     if true {
// CHECK-NEXT:         let val: i32 = (lc as i32) - 1;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = flips + val;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32)
// CHECK-NEXT:                 | (((lc.wrapping_sub(1) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { flips }) != 0 {
// CHECK-NEXT:         let val: i32 = ((code as u32) as i32) + 1;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32) | ((((val as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flips = flips + val;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32)
// CHECK-NEXT:                 | (((((((code as u32) as i32) + 1) as i16) as u16) as u32) as i32) << 1i32)
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     PUT!(lc as i32, (unsafe { flips }) != 0);
// CHECK-NEXT:     STORE_BYTE__2!(wide);
// CHECK-NEXT:     STORE_BYTE!(small);
// CHECK-NEXT:     STORE_BYTE__3!(code);
// CHECK-NEXT:     STORE_BYTE__4!(lc >> 3i32);
// CHECK-NEXT:     SUM_TWICE!(wide);
// CHECK-NEXT:     SUM_TWICE__2!(code);
// CHECK-NEXT:     MIXED!(wide);
// CHECK-NEXT:     MIXED!(((small as u32) as i32) + 1);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         flips = flips + (((small as u32) as i32) + 1 * ((small as u32) as i32) + 1);
// CHECK-NEXT:     }
// CHECK-NEXT:     UNHYGIENIC!(code);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 (buffer as u32) as i32,
// CHECK-NEXT:                 flips,
// CHECK-NEXT:                 used,
// CHECK-NEXT:                 (bytes[0] as u32) as i32,
// CHECK-NEXT:                 (bytes[3] as u32) as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {}",
// CHECK-NEXT:             ((unsafe { buffer }) as u32) as i32,
// CHECK-NEXT:             unsafe { flips },
// CHECK-NEXT:             unsafe { used },
// CHECK-NEXT:             ((unsafe { bytes[0] }) as u32) as i32,
// CHECK-NEXT:             ((unsafe { bytes[3] }) as u32) as i32
// CHECK-NEXT:         );
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
