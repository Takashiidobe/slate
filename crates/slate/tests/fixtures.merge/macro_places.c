#include <stdio.h>

struct state {
    int count;
    int bits[4];
    unsigned flag : 3;
};

#pragma pack(push, 1)
struct packed {
    char tag;
    int value;
};
#pragma pack(pop)

static int global;
static struct state shared;

#define SET(p) { (p) = 7; }
#define INC(x) { (x) = (x) + 1; }
#define ADD(x, n) { (x) += (n); }
#define BUMP(x) { (x)++; }
#define PUSH(stack, top, v) { (stack)[(top)] = (v); (top)++; }
#define SET_COUNT(s) { (s).count = 3; }
#define SHOW_COUNT(s) { printf("%d\n", (s).count); }
#define THROUGH(x) { int *q = &(x); *q = 9; }
#define ADDRESS_ONLY(x) { printf("%d\n", &(x) != 0); }
#define NESTED_INC(x) { INC(x); SET_TWICE(x); }
#define SET_TWICE(x) { (x) = (x) * 2; }
#define SHOW(a) { printf("%d\n", (int)(a)); }
#define DOUBLE_READ(a) { printf("%d %d\n", (int)(a), (int)(a)); }
#define WRITE_DEREF(p) { *(p) = 11; }
#define SET_LOCAL_THEN(x) { (x) = 5; printf("%d\n", (x)); }

static int sum(const int *values, int length) {
    int total = 0;
    for (int i = 0; i < length; i++) {
        total += values[i];
    }
    return total;
}

static void fill(int *values, int length) {
    SHOW(length);
    for (int i = 0; i < length; i++) {
        values[i] = i + 1;
    }
}

static int reads_only(int value) {
    SHOW(value);
    return value;
}

static int writes_param(int value) {
    INC(value);
    return value;
}

static void writes_through(int *out) {
    WRITE_DEREF(out);
    *out += 1;
}

static void pushes(int *stack, int *top) {
    PUSH(stack, *top, 4);
}

int main(void) {
    int n = 1;
    int unused_write = 0;
    int temp_source = 10;
    int stack[4] = {0, 0, 0, 0};
    int top = 0;
    int array[3] = {1, 2, 3};
    int index = 1;
    struct state s = {0, {0, 0, 0, 0}, 0};
    struct state *ps = &s;
    struct packed tight = {1, 2};
    volatile int shaky = 1;
    int through = 0;
    int deref_target = 0;
    int ro = 8;

    SET(n);
    INC(n);
    ADD(n, 2);
    BUMP(n);
    SET(unused_write);
    int saved = temp_source;
    INC(temp_source);
    printf("%d %d\n", saved, temp_source);
    PUSH(stack, top, 5);
    PUSH(stack, top, 6);
    pushes(stack, &top);
    SET_COUNT(s);
    SHOW_COUNT(s);
    SHOW_COUNT(*ps);
    SET_COUNT(*ps);
    INC(ps->count);
    INC(array[index]);
    INC(s.bits[2]);
    BUMP(*ps->bits);
    INC(global);
    SET_COUNT(shared);
    INC(shared.bits[1]);
    THROUGH(through);
    ADDRESS_ONLY(ro);
    NESTED_INC(n);
    INC(s.flag);
    INC(tight.value);
    INC(shaky);
    SET_LOCAL_THEN(ro);
    writes_through(&deref_target);
    DOUBLE_READ(n);

    int buffer[4];
    fill(buffer, 4);
    SHOW(sum(buffer, 4));

    printf("%d %d %d %d %d %d\n", n, unused_write, top, stack[0], stack[1], stack[2]);
    printf("%d %d %d %d %d\n", s.count, s.bits[0], s.bits[2], array[1], global);
    int tight_value = tight.value;
    printf("%d %d %d %d %d\n", shared.count, shared.bits[1], through, (int)s.flag, tight_value);
    printf("%d %d %d %d %d\n", shaky, ro, deref_target, reads_only(3), writes_param(3));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_places.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_places.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_places: 42 items, 15 gated, 0 duplicated functions
// CHECK-NEXT:   spliced record:state: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 15 function-like, 15 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline reads_caller_local: 6
// CHECK-NEXT:   macro ADD: 2 lifted, inline:
// CHECK-NEXT:   macro ADDRESS_ONLY: 2 lifted, inline:
// CHECK-NEXT:   macro BUMP: 4 lifted, inline:
// CHECK-NEXT:   macro DOUBLE_READ: 2 lifted, inline:
// CHECK-NEXT:   macro INC: 20 lifted, inline: reads_caller_local 4
// CHECK-NEXT:   macro NESTED_INC: 2 lifted, inline:
// CHECK-NEXT:   macro PUSH: 4 lifted, inline: reads_caller_local 2
// CHECK-NEXT:   macro SET: 4 lifted, inline:
// CHECK-NEXT:   macro SET_COUNT: 6 lifted, inline:
// CHECK-NEXT:   macro SET_LOCAL_THEN: 2 lifted, inline:
// CHECK-NEXT:   macro SET_TWICE: 2 lifted, inline:
// CHECK-NEXT:   macro SHOW: 6 lifted, inline:
// CHECK-NEXT:   macro SHOW_COUNT: 4 lifted, inline:
// CHECK-NEXT:   macro THROUGH: 2 lifted, inline:
// CHECK-NEXT:   macro WRITE_DEREF: 2 lifted, inline:
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
// CHECK-NEXT: macro_rules! BUMP {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = $x;
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = unsafe { *[[V_0]] };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = [[V_1]] + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! ADDRESS_ONLY {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         println!("{}", !std::ptr::addr_of_mut!($x).is_null() as i32);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! ADDRESS_ONLY {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 !std::ptr::addr_of_mut!($x).is_null() as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADD {
// CHECK-NEXT:     ($x:expr, $n:expr) => {
// CHECK-NEXT:         let __v: i32 = $x;
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = __v + $n;
// CHECK-NEXT:         $x = [[V_2]];
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! BUMP__2 {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         let __v: i32 = $x;
// CHECK-NEXT:         let [[V_2]]: i32 = __v + 1;
// CHECK-NEXT:         $x = [[V_2]];
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DOUBLE_READ {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{} {}", $a, $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DOUBLE_READ {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), $a, $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! INC {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         $x = ($x) + 1;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! INC__2 {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             $x = ($x) + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NESTED_INC {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         INC!($x);
// CHECK-NEXT:         SET_TWICE!($x);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! PUSH {
// CHECK-NEXT:     ($stack:expr, $top:expr, $v:expr) => {
// CHECK-NEXT:         $stack[($top) as usize] = $v;
// CHECK-NEXT:         let __v: i32 = $top;
// CHECK-NEXT:         let [[V_2]]: i32 = __v + 1;
// CHECK-NEXT:         $top = [[V_2]];
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SET_COUNT {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             $s.count = 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SET_COUNT__2 {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         $s.count = 3;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SET_LOCAL_THEN {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         $x = 5;
// CHECK-NEXT:         println!("{}", $x);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SET_LOCAL_THEN {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         $x = 5;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $x) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SET_TWICE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         $x = ($x) * 2;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SET {
// CHECK-NEXT:     ($p:expr) => {
// CHECK-NEXT:         $p = 7;
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHOW_COUNT {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         println!("{}", $s.count);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SHOW_COUNT {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $s.count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHOW_COUNT__2 {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         println!("{}", unsafe { $s.count });
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SHOW_COUNT__2 {
// CHECK-NEXT:     ($s:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d\n".as_ptr(), $s.count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! THROUGH {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         let q: *mut i32 = std::ptr::addr_of_mut!($x);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *q = 9;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! WRITE_DEREF {
// CHECK-NEXT:     ($p:expr) => {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *$p = 11;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct state {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     bits: [i32; 4],
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct packed {
// CHECK-NEXT:     tag: c_char,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub flag: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub flag: u32,
// CHECK-NEXT:         #[bits(29, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut global: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut shared: state = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..values.len() as i32 {
// CHECK-NEXT:         total += unsafe { values[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fill(values: *mut i32, length: i32) {
// CHECK-NEXT:     SHOW!(length);
// CHECK-NEXT:     for i in 0..length {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *values.offset(i as isize) = i + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reads_only(value: i32) -> i32 {
// CHECK-NEXT:     SHOW!(value);
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn writes_param(mut value: i32) -> i32 {
// CHECK-NEXT:     INC!(value);
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn writes_through(out: *mut i32) {
// CHECK-NEXT:     WRITE_DEREF!(out);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out = *out + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pushes(stack: *mut i32, top: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *stack.offset(*(top as *mut i32) as isize) = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *top = *top + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut n: i32 = 1;
// CHECK-NEXT:     let mut unused_write: i32 = 0;
// CHECK-NEXT:     let mut temp_source: i32 = 10;
// CHECK-NEXT:     let mut stack: __SlateAlignLargeArray<[i32; 4]> =
// CHECK-NEXT:         __SlateAlignLargeArray([0i32, 0i32, 0i32, 0i32]);
// CHECK-NEXT:     let mut top: i32 = 0;
// CHECK-NEXT:     let mut array: [i32; 3] = [1, 2, 3];
// CHECK-NEXT:     let index: i32 = 1;
// CHECK-NEXT:     let mut s: state = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: state = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].count = 0i32;
// CHECK-NEXT:         [[T_0]].bits = [0i32, 0i32, 0i32, 0i32];
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_flag(0u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let ps: *mut state = std::ptr::addr_of_mut!(s);
// CHECK-NEXT:     let mut tight: packed = packed { tag: 1, value: 2 };
// CHECK-NEXT:     let mut shaky: i32 = 1;
// CHECK-NEXT:     let mut through: i32 = 0;
// CHECK-NEXT:     let mut deref_target: i32 = 0;
// CHECK-NEXT:     let mut ro: i32 = 8;
// CHECK-NEXT:     SET!(n);
// CHECK-NEXT:     INC!(n);
// CHECK-NEXT:     ADD!(n, 2i32);
// CHECK-NEXT:     BUMP__2!(n);
// CHECK-NEXT:     SET!(unused_write);
// CHECK-NEXT:     let saved: i32 = temp_source;
// CHECK-NEXT:     INC!(temp_source);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"%d %d\n".as_ptr(), saved, temp_source) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {}", saved, temp_source);
// CHECK-NEXT:     }
// CHECK-NEXT:     PUSH!(stack.0, top, 5i32);
// CHECK-NEXT:     PUSH!(stack.0, top, 6i32);
// CHECK-NEXT:     pushes(stack.0.as_mut_ptr() as *mut i32, unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(top))
// CHECK-NEXT:     });
// CHECK-NEXT:     SET_COUNT__2!(s);
// CHECK-NEXT:     SHOW_COUNT!(s);
// CHECK-NEXT:     SHOW_COUNT__2!(*ps);
// CHECK-NEXT:     SET_COUNT!(*ps);
// CHECK-NEXT:     INC__2!((*ps).count);
// CHECK-NEXT:     INC!(array[index as usize]);
// CHECK-NEXT:     INC!(s.bits[2]);
// CHECK-NEXT:     BUMP!(unsafe { (*ps).bits.as_mut_ptr() as *mut i32 });
// CHECK-NEXT:     INC__2!(global);
// CHECK-NEXT:     SET_COUNT!(shared);
// CHECK-NEXT:     INC__2!(shared.bits[1]);
// CHECK-NEXT:     THROUGH!(through);
// CHECK-NEXT:     ADDRESS_ONLY!(ro);
// CHECK-NEXT:     NESTED_INC!(n);
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = ((s.__slate_bits_0.__get_flag() as i32) + 1) as u32;
// CHECK-NEXT:     s.__slate_bits_0.__set_flag([[T_1]]);
// CHECK-NEXT:     INC!(tight.value);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(shaky),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(shaky)) + 1,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     SET_LOCAL_THEN!(ro);
// CHECK-NEXT:     writes_through(std::ptr::addr_of_mut!(deref_target));
// CHECK-NEXT:     DOUBLE_READ!(n);
// CHECK-NEXT:     let mut buffer: __SlateAlignLargeArray<[i32; 4]> = __SlateAlignLargeArray([0i32; 4]);
// CHECK-NEXT:     fill(buffer.0.as_mut_ptr() as *mut i32, 4i32);
// CHECK-NEXT:     SHOW!(sum(&buffer.0));
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 n,
// CHECK-NEXT:                 unused_write,
// CHECK-NEXT:                 top,
// CHECK-NEXT:                 stack.0[0],
// CHECK-NEXT:                 stack.0[1],
// CHECK-NEXT:                 stack.0[2],
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
// CHECK-NEXT:                 c"%d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 s.count,
// CHECK-NEXT:                 s.bits[0],
// CHECK-NEXT:                 s.bits[2],
// CHECK-NEXT:                 array[1],
// CHECK-NEXT:                 global,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {} {}",
// CHECK-NEXT:             n, unused_write, top, stack.0[0], stack.0[1], stack.0[2]
// CHECK-NEXT:         );
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {}",
// CHECK-NEXT:             s.count,
// CHECK-NEXT:             s.bits[0],
// CHECK-NEXT:             s.bits[2],
// CHECK-NEXT:             array[1],
// CHECK-NEXT:             unsafe { global }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let tight_value: i32 = tight.value;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 shared.count,
// CHECK-NEXT:                 shared.bits[1],
// CHECK-NEXT:                 through,
// CHECK-NEXT:                 s.__slate_bits_0.__get_flag() as i32,
// CHECK-NEXT:                 tight_value,
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
// CHECK-NEXT:                 c"%d %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(shaky)),
// CHECK-NEXT:                 ro,
// CHECK-NEXT:                 deref_target,
// CHECK-NEXT:                 reads_only(3i32),
// CHECK-NEXT:                 writes_param(3i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {}",
// CHECK-NEXT:             unsafe { shared.count },
// CHECK-NEXT:             unsafe { shared.bits[1] },
// CHECK-NEXT:             through,
// CHECK-NEXT:             s.__slate_bits_0.__get_flag() as i32,
// CHECK-NEXT:             tight_value
// CHECK-NEXT:         );
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {}",
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(shaky)) },
// CHECK-NEXT:             ro,
// CHECK-NEXT:             deref_target,
// CHECK-NEXT:             reads_only(3i32),
// CHECK-NEXT:             writes_param(3i32)
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
