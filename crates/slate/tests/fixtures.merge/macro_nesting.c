#include <stdio.h>

typedef unsigned short ush;
typedef unsigned char uch;

static ush buffer;
static uch bytes[32];
static int used;
static int total;
static int reads = 2;

struct code {
    ush code;
    ush len;
};

#define PUT_BYTE(c) { bytes[used++] = (uch)(c); }
#define PUT_SHORT(w) { \
    PUT_BYTE((uch)((w) & 0xff)); \
    PUT_BYTE((uch)((ush)(w) >> 8)); \
}
#define SEND(value, length) \
{ int width = length; \
  if (width > 8) { \
    int val = (int)value; \
    buffer |= (ush)val; \
    PUT_SHORT(buffer); \
    total += width; \
  } else { \
    buffer |= (ush)(value); \
    total += width; \
  } \
}
#define SEND_CODE(c, tree) SEND(tree[c].code, tree[c].len)
#define SEND_TWO(a, b) { SEND(a, 9); SEND(b, 3); }
#define ADD(a) { total += (a); }
#define SAME_BODY(a) { total += (a); }
#define ADD_READS(a) { total += (a) + reads; }
#define LOCAL_READS(a) { int reads = 40; ADD_READS(a); total += reads; }
#define LEAKS(a) int leaked = (a); total += leaked;
#define USES_LEAK(a) { LEAKS(a); total += leaked * 2; }
#define TEMP(a) { int temp = 5; total += (a) + temp; }
#define ARG_NAMES_TEMP(a) { int temp = 7; TEMP(temp + (a)); }
#define ONLY_NESTED(a) { total += (a) * 3; }
#define CALLS_ONLY_NESTED(a) { ONLY_NESTED(a); total += 1; }

static void direct(int value) {
    PUT_BYTE(value);
    PUT_SHORT(value + 1);
    SEND(value, value + 5);
    ADD(value);
    SAME_BODY(value + 1);
    ADD_READS(value);
    TEMP(value);
}

static void nested(const struct code *tree, int index) {
    SEND_CODE(index, tree);
    SEND_CODE(index + 1, tree);
    SEND_TWO(index, index + 2);
    LOCAL_READS(index);
    USES_LEAK(index);
    ARG_NAMES_TEMP(index);
    CALLS_ONLY_NESTED(index);
}

int main(void) {
    static const struct code tree[3] = {{0x1234, 9}, {0x0102, 3}, {0x00ff, 12}};
    direct(4);
    nested(tree, 0);
    nested(tree, 1);
    printf("%d %d %d\n", buffer, used, total);
    for (int i = 0; i < used; i++) {
        printf("%d\n", bytes[i]);
    }
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_nesting.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_nesting.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_nesting: 31 items, 1 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 15 function-like, 13 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline split_argument: 4
// CHECK-NEXT:   inline local_used_after_macro: 2
// CHECK-NEXT:   inline no_statement_region: 2
// CHECK-NEXT:   macro ADD: 2 lifted, inline:
// CHECK-NEXT:   macro ADD_READS: 4 lifted, inline:
// CHECK-NEXT:   macro ARG_NAMES_TEMP: 2 lifted, inline:
// CHECK-NEXT:   macro CALLS_ONLY_NESTED: 2 lifted, inline:
// CHECK-NEXT:   macro LEAKS: 0 lifted, inline: local_used_after_macro 2
// CHECK-NEXT:   macro LOCAL_READS: 2 lifted, inline:
// CHECK-NEXT:   macro ONLY_NESTED: 2 lifted, inline:
// CHECK-NEXT:   macro PUT_BYTE: 26 lifted, inline:
// CHECK-NEXT:   macro PUT_SHORT: 12 lifted, inline:
// CHECK-NEXT:   macro SAME_BODY: 2 lifted, inline:
// CHECK-NEXT:   macro SEND: 8 lifted, inline: split_argument 2
// CHECK-NEXT:   macro SEND_CODE: 4 lifted, inline:
// CHECK-NEXT:   macro SEND_TWO: 0 lifted, inline: split_argument 2
// CHECK-NEXT:   macro TEMP: 2 lifted, inline: no_statement_region 2
// CHECK-NEXT:   macro USES_LEAK: 2 lifted, inline:
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
// CHECK-NEXT: macro_rules! ADD_READS {
// CHECK-NEXT:     ($a:expr, $reads__1:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + (($a) + $reads__1);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD_READS__2 {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + (($a) + reads);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + $a;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ARG_NAMES_TEMP {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let temp: i32 = 5;
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + (temp + $a + temp);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALLS_ONLY_NESTED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         ONLY_NESTED!($a);
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! LOCAL_READS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let reads__1: i32 = 40;
// CHECK-NEXT:         ADD_READS!($a, reads__1);
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + reads__1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ONLY_NESTED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + ($a) * 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT_BYTE {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { used };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             used = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bytes.0[__v as usize] = $c;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT_BYTE__2 {
// CHECK-NEXT:     ($c:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { used };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             used = __v + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bytes.0[__v as usize] = (($c) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT_SHORT {
// CHECK-NEXT:     ($w:expr) => {
// CHECK-NEXT:         PUT_BYTE!(((((($w) as u32) as i32) & 255) as i8) as u8);
// CHECK-NEXT:         PUT_BYTE!(((((($w) as u32) as i32) >> 8i32) as i8) as u8);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUT_SHORT__2 {
// CHECK-NEXT:     ($w:expr) => {
// CHECK-NEXT:         PUT_BYTE!(((($w) & 255) as i8) as u8);
// CHECK-NEXT:         PUT_BYTE!(((((((($w) as i16) as u16) as u32) as i32) >> 8i32) as i8) as u8);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SAME_BODY {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + $a;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SEND_CODE {
// CHECK-NEXT:     ($c:expr, $tree:expr) => {
// CHECK-NEXT:         SEND__2!(unsafe { (*$tree.offset(($c) as isize)).code }, unsafe {
// CHECK-NEXT:             (*$tree.offset(($c) as isize)).len
// CHECK-NEXT:         });
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SEND__2 {
// CHECK-NEXT:     ($value:expr, $length:expr) => {
// CHECK-NEXT:         let width: i32 = (($length) as u32) as i32;
// CHECK-NEXT:         if width > 8 {
// CHECK-NEXT:             let val: i32 = (($value) as u32) as i32;
// CHECK-NEXT:             let __v: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = ((((__v as u32) as i32) | ((((val as i16) as u16) as u32) as i32)) as i16)
// CHECK-NEXT:                     as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             PUT_SHORT!(unsafe { buffer });
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 total = total + width;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = (((([[V_0]] as u32) as i32) | ((($value) as u32) as i32)) as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 total = total + width;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SEND {
// CHECK-NEXT:     ($value:expr, $length:expr) => {
// CHECK-NEXT:         let width: i32 = $length;
// CHECK-NEXT:         if width > 8 {
// CHECK-NEXT:             let val: i32 = $value;
// CHECK-NEXT:             let __v: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = ((((__v as u32) as i32) | ((((val as i16) as u16) as u32) as i32)) as i16)
// CHECK-NEXT:                     as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             PUT_SHORT!(unsafe { buffer });
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 total = total + width;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_0]]: u16 = unsafe { buffer };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 buffer = (((([[V_0]] as u32) as i32) | ((((($value) as i16) as u16) as u32) as i32))
// CHECK-NEXT:                     as i16) as u16;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 total = total + width;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TEMP {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let temp: i32 = 5;
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + (($a) + temp);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! USES_LEAK {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let leaked: i32 = $a;
// CHECK-NEXT:         let __v: i32 = unsafe { total };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = __v + leaked;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = total + leaked * 2;
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut bytes: __SlateAlignLargeArray<[u8; 32]> =
// CHECK-NEXT:     __SlateAlignLargeArray(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut used: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut reads: i32 = 2;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct code {
// CHECK-NEXT:     code: u16,
// CHECK-NEXT:     len: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn direct(value: i32) {
// CHECK-NEXT:     PUT_BYTE__2!(value);
// CHECK-NEXT:     PUT_SHORT__2!(value + 1);
// CHECK-NEXT:     SEND!(value, value + 5);
// CHECK-NEXT:     ADD!(value);
// CHECK-NEXT:     SAME_BODY!(value + 1);
// CHECK-NEXT:     ADD_READS__2!(value);
// CHECK-NEXT:     TEMP!(value);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn nested(mut tree__0: *const code, index: i32) {
// CHECK-NEXT:     SEND_CODE!(index, tree__0);
// CHECK-NEXT:     SEND_CODE!(index + 1, tree__0);
// CHECK-NEXT:     SEND!(index, 9i32);
// CHECK-NEXT:     let width: i32 = 3;
// CHECK-NEXT:     if width > 8 {
// CHECK-NEXT:         let val: i32 = index + 2;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32) | ((((val as i16) as u16) as u32) as i32)) as i16)
// CHECK-NEXT:                 as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         PUT_SHORT!(unsafe { buffer });
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = total + width;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             buffer = ((((buffer as u32) as i32) | (((((index + 2) as i16) as u16) as u32) as i32))
// CHECK-NEXT:                 as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             total = total + width;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     LOCAL_READS!(index);
// CHECK-NEXT:     USES_LEAK!(index);
// CHECK-NEXT:     ARG_NAMES_TEMP!(index);
// CHECK-NEXT:     CALLS_ONLY_NESTED!(index);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     direct(4i32);
// CHECK-NEXT:     nested(unsafe { std::ptr::addr_of!(tree) as *const code }, 0i32);
// CHECK-NEXT:     nested(unsafe { std::ptr::addr_of!(tree) as *const code }, 1i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"%d %d %d\n".as_ptr(), (buffer as u32) as i32, used, total)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             ((unsafe { buffer }) as u32) as i32,
// CHECK-NEXT:             unsafe { used },
// CHECK-NEXT:             unsafe { total }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < unsafe { used } {
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { slate_support::printf(c"%d\n".as_ptr(), (bytes.0[i as usize] as u32) as i32) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("{}", ((unsafe { bytes.0[i as usize] }) as u32) as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut tree: [code; 3] = [
// CHECK-NEXT:     code {
// CHECK-NEXT:         code: 4660u16,
// CHECK-NEXT:         len: 9u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         code: 258u16,
// CHECK-NEXT:         len: 3u16,
// CHECK-NEXT:     },
// CHECK-NEXT:     code {
// CHECK-NEXT:         code: 255u16,
// CHECK-NEXT:         len: 12u16,
// CHECK-NEXT:     },
// CHECK-NEXT: ];
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
