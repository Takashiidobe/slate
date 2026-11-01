#include <stdio.h>

struct pair {
    int left;
    int right;
};

#define READ_HOLD (hold + 1)
#define BITS(n) (hold & ((1u << (n)) - 1))
#define DROP(n) do { hold >>= (n); bits -= (unsigned)(n); } while (0)
#define INIT() do { hold = 0; bits = 0; } while (0)
#define COLLIDE(n) (n + hold)
#define CHECK_DROP(n) do { if (bits >= (unsigned)(n)) DROP(n); } while (0)
#define SHIFT_THEN_READ(n) do { DROP(n); total += BITS(2); } while (0)
#define BODY_LOCAL(x) do { int saved = (x); READ_INTO(saved); } while (0)
#define READ_INTO(v) do { total += (v) + scale; } while (0)
#define FIELD_WRITE(v) do { pair.right = (v); } while (0)
#define INDEX_WRITE(i, v) do { table[i] = (v); } while (0)
#define DECAY_FIRST() (*table)
#define DEREF_WRITE(v) do { *cursor = (v); cursor++; } while (0)
#define ADDRESS_OF() (&hold)
#define KEYWORD_NAME(v) (type + (v))
#define TEMP_ARG(a) do { int temp = 1; total += temp + (a); } while (0)
#define PARAMETER_READ() (limit * 2)

static unsigned read_bits(unsigned hold, unsigned bits) {
    unsigned first = READ_HOLD;
    unsigned second = BITS(3);
    DROP(2);
    return first + second + hold + bits;
}

static unsigned drop_and_read(int limit) {
    unsigned hold = 0xabcd, bits = 16;
    int total = 0;
    int n = 5;
    int type = 3;
    int table[4] = {1, 2, 3, 4};
    int *cursor = table + 1;
    struct pair pair = {1, 2};
    int scale = 2;
    unsigned *address;
    DROP(4);
    CHECK_DROP(3);
    SHIFT_THEN_READ(1);
    total += COLLIDE(n);
    BODY_LOCAL(n + 1);
    FIELD_WRITE(pair.left + 7);
    INDEX_WRITE(2, 30);
    DEREF_WRITE(20);
    total += DECAY_FIRST() + table[1] + table[2] + pair.right;
    address = ADDRESS_OF();
    *address += 1;
    total += KEYWORD_NAME(4);
    TEMP_ARG(total);
    total += PARAMETER_READ();
    {
        unsigned hold = 7;
        total += READ_HOLD;
        total += BITS(1);
    }
    INIT();
    return hold + bits + (unsigned)total;
}

static double read_double(void) {
    double hold = 1.5;
    return READ_HOLD;
}

int main(void) {
    printf("%u\n", read_bits(0xf0f0, 12));
    printf("%u\n", drop_and_read(9));
    printf("%f\n", read_double());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_caller_locals.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_caller_locals.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_caller_locals: 25 items, 0 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 16 function-like, 16 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro ADDRESS_OF: 2 lifted, inline:
// CHECK-NEXT:   macro BITS: 6 lifted, inline:
// CHECK-NEXT:   macro BODY_LOCAL: 2 lifted, inline:
// CHECK-NEXT:   macro CHECK_DROP: 2 lifted, inline:
// CHECK-NEXT:   macro COLLIDE: 2 lifted, inline:
// CHECK-NEXT:   macro DECAY_FIRST: 2 lifted, inline:
// CHECK-NEXT:   macro DEREF_WRITE: 2 lifted, inline:
// CHECK-NEXT:   macro DROP: 8 lifted, inline:
// CHECK-NEXT:   macro FIELD_WRITE: 2 lifted, inline:
// CHECK-NEXT:   macro INDEX_WRITE: 2 lifted, inline:
// CHECK-NEXT:   macro INIT: 2 lifted, inline:
// CHECK-NEXT:   macro KEYWORD_NAME: 2 lifted, inline:
// CHECK-NEXT:   macro PARAMETER_READ: 2 lifted, inline:
// CHECK-NEXT:   macro READ_INTO: 2 lifted, inline:
// CHECK-NEXT:   macro SHIFT_THEN_READ: 2 lifted, inline:
// CHECK-NEXT:   macro TEMP_ARG: 2 lifted, inline:
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
// CHECK-NEXT: macro_rules! ADDRESS_OF {
// CHECK-NEXT:     ($hold:expr) => {
// CHECK-NEXT:         std::ptr::addr_of_mut!($hold)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BITS {
// CHECK-NEXT:     ($n:expr, $hold:expr) => {
// CHECK-NEXT:         ($hold) & (1u32 << $n).wrapping_sub(1)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BODY_LOCAL {
// CHECK-NEXT:     ($x:expr, $total:expr, $scale:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let saved: i32 = $x;
// CHECK-NEXT:             READ_INTO!(saved, $total, $scale);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CHECK_DROP {
// CHECK-NEXT:     ($n:expr, $bits:expr, $hold:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if ($bits) >= (($n) as u32) {
// CHECK-NEXT:                 DROP!($n, $hold, $bits);
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! COLLIDE {
// CHECK-NEXT:     ($n:expr, $hold:expr) => {
// CHECK-NEXT:         (($n) as u32).wrapping_add($hold)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! DECAY_FIRST {
// CHECK-NEXT:     ($table:expr) => {
// CHECK-NEXT:         unsafe { *($table.as_mut_ptr() as *mut i32) }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! DEREF_WRITE {
// CHECK-NEXT:     ($v:expr, $cursor:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *$cursor = $v;
// CHECK-NEXT:             }
// CHECK-NEXT:             let __v: *mut i32 = $cursor;
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { __v.add(1) };
// CHECK-NEXT:             $cursor = [[V_0]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! DROP {
// CHECK-NEXT:     ($n:expr, $hold:expr, $bits:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __v: u32 = $hold;
// CHECK-NEXT:             let [[V_0]]: u32 = __v >> $n;
// CHECK-NEXT:             $hold = [[V_0]];
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: u32 = $bits;
// CHECK-NEXT:             let [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_sub(($n) as u32);
// CHECK-NEXT:             $bits = [[V_2]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FIELD_WRITE {
// CHECK-NEXT:     ($v:expr, $pair:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             $pair.right = $v;
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! INDEX_WRITE {
// CHECK-NEXT:     ($i:expr, $v:expr, $table:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             $table[($i) as usize] = $v;
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! INIT {
// CHECK-NEXT:     ($hold:expr, $bits:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             $hold = 0;
// CHECK-NEXT:             $bits = 0;
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! KEYWORD_NAME {
// CHECK-NEXT:     ($v:expr, $type:expr) => {
// CHECK-NEXT:         ($type) + $v
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PARAMETER_READ {
// CHECK-NEXT:     ($limit:expr) => {
// CHECK-NEXT:         ($limit) * 2
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! READ_INTO {
// CHECK-NEXT:     ($v:expr, $total:expr, $scale:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let __v: i32 = $total;
// CHECK-NEXT:             let [[V_0]]: i32 = __v + (($v) + $scale);
// CHECK-NEXT:             $total = [[V_0]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHIFT_THEN_READ {
// CHECK-NEXT:     ($n:expr, $hold:expr, $bits:expr, $total:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             DROP!($n, $hold, $bits);
// CHECK-NEXT:             let __v: i32 = $total;
// CHECK-NEXT:             let [[V_0]]: i32 = (__v as u32).wrapping_add(BITS!(2i32, $hold)) as i32;
// CHECK-NEXT:             $total = [[V_0]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TEMP_ARG {
// CHECK-NEXT:     ($a:expr, $total:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let temp: i32 = 1;
// CHECK-NEXT:             let __v: i32 = $total;
// CHECK-NEXT:             let [[V_0]]: i32 = __v + (temp + $a);
// CHECK-NEXT:             $total = [[V_0]];
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn read_bits(mut hold: u32, mut bits: u32) -> u32 {
// CHECK-NEXT:     let first: u32 = hold.wrapping_add(1);
// CHECK-NEXT:     let second: u32 = BITS!(3i32, hold);
// CHECK-NEXT:     DROP!(2i32, hold, bits);
// CHECK-NEXT:     first
// CHECK-NEXT:         .wrapping_add(second)
// CHECK-NEXT:         .wrapping_add(hold)
// CHECK-NEXT:         .wrapping_add(bits)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn drop_and_read(limit: i32) -> u32 {
// CHECK-NEXT:     let mut hold: u32 = 43981;
// CHECK-NEXT:     let mut bits: u32 = 16;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let n: i32 = 5;
// CHECK-NEXT:     let r#type: i32 = 3;
// CHECK-NEXT:     let mut table: __SlateAlignLargeArray<[i32; 4]> =
// CHECK-NEXT:         __SlateAlignLargeArray([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     let mut cursor: *mut i32 = unsafe { (table.0.as_mut_ptr() as *mut i32).add(1) };
// CHECK-NEXT:     let mut pair: pair = pair { left: 1, right: 2 };
// CHECK-NEXT:     let scale: i32 = 2;
// CHECK-NEXT:     DROP!(4i32, hold, bits);
// CHECK-NEXT:     CHECK_DROP!(3i32, bits, hold);
// CHECK-NEXT:     SHIFT_THEN_READ!(1i32, hold, bits, total);
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = ([[V_3]] as u32).wrapping_add(COLLIDE!(n, hold)) as i32;
// CHECK-NEXT:     BODY_LOCAL!(n + 1, total, scale);
// CHECK-NEXT:     FIELD_WRITE!(pair.left + 7, pair);
// CHECK-NEXT:     INDEX_WRITE!(2i32, 30i32, table.0);
// CHECK-NEXT:     DEREF_WRITE!(20i32, cursor);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_4]] + (DECAY_FIRST!(table.0) + table.0[1] + table.0[2] + pair.right);
// CHECK-NEXT:     let address: *mut u32 = ADDRESS_OF!(hold);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: u32 = unsafe { *address };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *address = [[V_5]].wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_6]] + KEYWORD_NAME!(4i32, r#type);
// CHECK-NEXT:     TEMP_ARG!(total, total);
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_7]] + PARAMETER_READ!(limit);
// CHECK-NEXT:     {
// CHECK-NEXT:         let hold: u32 = 7;
// CHECK-NEXT:         total = (total as u32).wrapping_add(hold.wrapping_add(1)) as i32;
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:         total = ([[V_8]] as u32).wrapping_add(BITS!(1i32, hold)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     INIT!(hold, bits);
// CHECK-NEXT:     hold.wrapping_add(bits).wrapping_add(total as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn read_double() -> f64 {
// CHECK-NEXT:     let hold: f64 = 1.5f64;
// CHECK-NEXT:     hold + (1i32 as f64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%u\n".as_ptr(), read_bits(61680u32, 12u32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%u\n".as_ptr(), read_bits(61680u32, 12u32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%u\n".as_ptr(), drop_and_read(9i32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%u\n".as_ptr(), drop_and_read(9i32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%f\n".as_ptr(), read_double()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%f\n".as_ptr(), read_double()) };
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
