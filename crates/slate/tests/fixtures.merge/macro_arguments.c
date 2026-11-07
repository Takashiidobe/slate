#include <stdio.h>

#define SWAP(T, a, b) do { T tmp = (a); (a) = (b); (b) = tmp; } while (0)
#define FIELD(p, name) ((p)->name)
#define NESTED_FIELD(p, name) (FIELD(p, name) + 1)
#define SET_FIELD(p, name, v) do { (p)->name = (v); } while (0)
#define CAST_TO(T, x) ((T)(x))
#define NESTED_CAST(T, x) (CAST_TO(T, x) + 1)
#define SIZE_PLUS(T, n) (sizeof(T) + (n))
#define APPLY(f, x) ((f)(x))
#define CALL2(f, a, b) f(a, b)
#define ARGS_OF(f, args) f args
#define DECLARE(name, v) int name = (v)
#define MEMBER_NAME(name) (holder.name + 1)
#define TRACE(x) do { printf x; } while (0)
#define READ_THROUGH(T, p) (*(T *)(p))

struct pair {
    int left;
    int right;
    long wide;
};

struct only_cast {
    int value;
};

struct odd_fields {
    int type;
    unsigned flag : 2;
    int count;
    union {
        int inner_value;
    };
};

typedef unsigned int my_uint;

struct pair holder = {1, 2, 3};

static int twice(int x) { return x * 2; }
static int add(int a, int b) { return a + b; }
static int thrice(int x) { return x * 3; }

int main(void) {
    int i = 1, j = 2;
    long m = 3, n = 4;
    my_uint u1 = 5, u2 = 6;
    struct pair s = {5, 6, 7};
    struct pair t = {8, 9, 10};
    struct pair *p = &s;
    int (*fn)(int) = thrice;
    SWAP(int, i, j);
    SWAP(long, m, n);
    SWAP(my_uint, u1, u2);
    SWAP(struct pair, s, t);
    SWAP(struct pair, t, s);
    int f1 = FIELD(p, left);
    int f2 = FIELD(p, right);
    long f3 = FIELD(p, wide);
    int f4 = NESTED_FIELD(p, right);
    SET_FIELD(p, left, 10);
    SET_FIELD(p, right, 11);
    long c = CAST_TO(long, i);
    int d = CAST_TO(int, m);
    long c2 = NESTED_CAST(long, i);
    unsigned long z = SIZE_PLUS(int, 1);
    int e = APPLY(twice, i);
    int e2 = APPLY(fn, i);
    int g = CALL2(add, i, j);
    int h = ARGS_OF(add, (i, j));
    DECLARE(q, 9);
    int r = MEMBER_NAME(left);
    struct only_cast oc = {42};
    void *erased = &oc;
    int v = READ_THROUGH(struct only_cast, erased).value;
    struct odd_fields odd = {1, 2, 3, {4}};
    struct odd_fields *op = &odd;
    SET_FIELD(op, type, 20);
    SET_FIELD(op, flag, 1);
    SET_FIELD(op, inner_value, 30);
    printf("%d %d %d %d\n", FIELD(op, type), FIELD(op, flag), FIELD(op, count), FIELD(op, inner_value));
    TRACE(("plain\n"));
    TRACE(("%d %d\n", i, j));
    printf("%d %d %ld %ld %u %u %d %d %ld %d %d %d %ld %d %ld %lu %d %d %d %d %d %d %d\n", i, j, m, n, u1, u2, f1, f2, f3, f4, s.left, s.right, c, d, c2, z, e, e2, g, h, q, r, v);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_arguments.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_arguments.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_arguments: 33 items, 2 gated, 0 duplicated functions
// CHECK-NEXT:   spliced record:odd_fields: 2 variants
// CHECK-NEXT:   spliced fn:thrice: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: macros: 14 function-like, 12 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   inline bit_field_write: 2
// CHECK-NEXT:   inline local_used_after_macro: 2
// CHECK-NEXT:   inline no_statement_region: 2
// CHECK-NEXT:   macro APPLY: 4 lifted, inline:
// CHECK-NEXT:   macro ARGS_OF: 2 lifted, inline:
// CHECK-NEXT:   macro CALL2: 2 lifted, inline:
// CHECK-NEXT:   macro CAST_TO: 6 lifted, inline:
// CHECK-NEXT:   macro DECLARE: 0 lifted, inline: local_used_after_macro 2
// CHECK-NEXT:   macro FIELD: 16 lifted, inline:
// CHECK-NEXT:   macro MEMBER_NAME: 2 lifted, inline:
// CHECK-NEXT:   macro NESTED_CAST: 2 lifted, inline:
// CHECK-NEXT:   macro NESTED_FIELD: 2 lifted, inline:
// CHECK-NEXT:   macro READ_THROUGH: 0 lifted, inline: no_statement_region 2
// CHECK-NEXT:   macro SET_FIELD: 8 lifted, inline: bit_field_write 2
// CHECK-NEXT:   macro SIZE_PLUS: 2 lifted, inline:
// CHECK-NEXT:   macro SWAP: 10 lifted, inline:
// CHECK-NEXT:   macro TRACE: 4 lifted, inline:
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
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub flag: u32,
// CHECK-NEXT:         #[bits(6, access = na)]
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
// CHECK-NEXT:     pub struct __SlateBits10U0 {
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub flag: u32,
// CHECK-NEXT:         #[bits(30, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg_attr(
// CHECK-NEXT:     unix,
// CHECK-NEXT:     unsafe(link_section = ".text.slate_distinct.macro_arguments.thrice")
// CHECK-NEXT: )]
// CHECK-NEXT: extern "C-unwind" fn thrice(x: i32) -> i32 {
// CHECK-NEXT:     x * 3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct odd_fields {
// CHECK-NEXT:     r#type: i32,
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits10U0,
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_macro_arguments_c_32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct only_cast {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT:     wide: c_long,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_macro_arguments_c_32 {
// CHECK-NEXT:     inner_value: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut holder: pair = pair {
// CHECK-NEXT:     left: 1i32,
// CHECK-NEXT:     right: 2i32,
// CHECK-NEXT:     wide: (3 as c_long),
// CHECK-NEXT: };
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
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     let mut j: i32 = 2;
// CHECK-NEXT:     let mut m: c_long = 3;
// CHECK-NEXT:     let mut n: c_long = 4;
// CHECK-NEXT:     let mut u1: u32 = 5;
// CHECK-NEXT:     let mut u2: u32 = 6;
// CHECK-NEXT:     let mut s: pair = pair {
// CHECK-NEXT:         left: 5,
// CHECK-NEXT:         right: 6,
// CHECK-NEXT:         wide: 7,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut t: pair = pair {
// CHECK-NEXT:         left: 8,
// CHECK-NEXT:         right: 9,
// CHECK-NEXT:         wide: 10,
// CHECK-NEXT:     };
// CHECK-NEXT:     let p: *mut pair = std::ptr::addr_of_mut!(s);
// CHECK-NEXT:     let r#fn: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(thrice);
// CHECK-NEXT:     SWAP!(i32, i, j);
// CHECK-NEXT:     SWAP!(c_long, m, n);
// CHECK-NEXT:     SWAP!(u32, u1, u2);
// CHECK-NEXT:     SWAP!(pair, s, t);
// CHECK-NEXT:     SWAP!(pair, t, s);
// CHECK-NEXT:     let f1: i32 = FIELD!(p, left);
// CHECK-NEXT:     let f2: i32 = FIELD!(p, right);
// CHECK-NEXT:     let f3: c_long = FIELD!(p, wide);
// CHECK-NEXT:     let f4: i32 = NESTED_FIELD!(p, right);
// CHECK-NEXT:     SET_FIELD!(p, left, 10i32);
// CHECK-NEXT:     SET_FIELD!(p, right, 11i32);
// CHECK-NEXT:     let c: c_long = CAST_TO!(c_long, i);
// CHECK-NEXT:     let d: i32 = CAST_TO!(i32, m);
// CHECK-NEXT:     let c2: c_long = NESTED_CAST!(c_long, i);
// CHECK-NEXT:     let z: c_ulong = SIZE_PLUS!(i32, 1i32) as c_ulong;
// CHECK-NEXT:     let e: i32 = APPLY!(i);
// CHECK-NEXT:     let e2: i32 = APPLY__2!(r#fn, i);
// CHECK-NEXT:     let g: i32 = CALL2!(i, j);
// CHECK-NEXT:     let h: i32 = ARGS_OF!(i, j);
// CHECK-NEXT:     let q: i32 = 9;
// CHECK-NEXT:     let r: i32 = MEMBER_NAME!(left);
// CHECK-NEXT:     let mut oc: only_cast = only_cast { value: 42 };
// CHECK-NEXT:     let erased: *mut () = std::ptr::addr_of_mut!(oc) as *mut ();
// CHECK-NEXT:     let v: i32 = unsafe { (*(erased as *mut only_cast)).value };
// CHECK-NEXT:     let mut odd: odd_fields = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: odd_fields = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].r#type = 1i32;
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_flag(2u32);
// CHECK-NEXT:         [[T_0]].count = 3i32;
// CHECK-NEXT:         [[T_0]].[[SLATE_ANON_0]] = __SlateRecord_macro_arguments_c_32 { inner_value: 4 };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let op: *mut odd_fields = std::ptr::addr_of_mut!(odd);
// CHECK-NEXT:     SET_FIELD!(op, r#type, 20i32);
// CHECK-NEXT:     loop {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*op).__slate_bits_0.__set_flag(1u32);
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SET_FIELD__2!(op, inner_value, 30i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 FIELD!(op, r#type),
// CHECK-NEXT:                 FIELD__2!(op),
// CHECK-NEXT:                 FIELD!(op, count),
// CHECK-NEXT:                 FIELD__3!(op, inner_value),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {}",
// CHECK-NEXT:             FIELD!(op, r#type),
// CHECK-NEXT:             FIELD__2!(op),
// CHECK-NEXT:             FIELD!(op, count),
// CHECK-NEXT:             FIELD__3!(op, inner_value)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     TRACE!(c"plain\n".as_ptr() as *mut c_char);
// CHECK-NEXT:     TRACE!(c"%d %d\n".as_ptr() as *mut c_char, i, j);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %ld %ld %u %u %d %d %ld %d %d %d %ld %d %ld %lu %d %d %d %d %d %d %d\n"
// CHECK-NEXT:                 .as_ptr(),
// CHECK-NEXT:             i,
// CHECK-NEXT:             j,
// CHECK-NEXT:             m,
// CHECK-NEXT:             n,
// CHECK-NEXT:             u1,
// CHECK-NEXT:             u2,
// CHECK-NEXT:             f1,
// CHECK-NEXT:             f2,
// CHECK-NEXT:             f3,
// CHECK-NEXT:             f4,
// CHECK-NEXT:             s.left,
// CHECK-NEXT:             s.right,
// CHECK-NEXT:             c,
// CHECK-NEXT:             d,
// CHECK-NEXT:             c2,
// CHECK-NEXT:             z,
// CHECK-NEXT:             e,
// CHECK-NEXT:             e2,
// CHECK-NEXT:             g,
// CHECK-NEXT:             h,
// CHECK-NEXT:             q,
// CHECK-NEXT:             r,
// CHECK-NEXT:             v,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn twice(x: i32) -> i32 {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! APPLY {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         twice($x)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! APPLY__2 {
// CHECK-NEXT:     ($f:expr, $x:expr) => {
// CHECK-NEXT:         unsafe { $f.unwrap()($x) }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ARGS_OF {
// CHECK-NEXT:     ($($args:expr),*) => {
// CHECK-NEXT:         add($($args),*)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CALL2 {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         add($a, $b)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! CAST_TO {
// CHECK-NEXT:     ($T:ty, $x:expr) => {
// CHECK-NEXT:         ($x) as $T
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FIELD {
// CHECK-NEXT:     ($p:expr, $name:ident) => {
// CHECK-NEXT:         unsafe { (*$p).$name }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FIELD__2 {
// CHECK-NEXT:     ($p:expr) => {
// CHECK-NEXT:         (unsafe { (*$p).__slate_bits_0.__get_flag() }) as i32
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! FIELD__3 {
// CHECK-NEXT:     ($p:expr, $name:ident) => {
// CHECK-NEXT:         unsafe { (*$p).[[SLATE_ANON_0]].$name }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! MEMBER_NAME {
// CHECK-NEXT:     ($name:ident) => {
// CHECK-NEXT:         (unsafe { holder.$name }) + 1
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NESTED_CAST {
// CHECK-NEXT:     ($T:ty, $x:expr) => {
// CHECK-NEXT:         CAST_TO!($T, $x) + (1i32 as $T)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NESTED_FIELD {
// CHECK-NEXT:     ($p:expr, $name:ident) => {
// CHECK-NEXT:         FIELD!($p, $name) + 1
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SET_FIELD {
// CHECK-NEXT:     ($p:expr, $name:ident, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*$p).$name = $v;
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SET_FIELD__2 {
// CHECK-NEXT:     ($p:expr, $name:ident, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*$p).[[SLATE_ANON_0]].$name = $v;
// CHECK-NEXT:             }
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SIZE_PLUS {
// CHECK-NEXT:     ($T:ty, $n:expr) => {
// CHECK-NEXT:         (std::mem::size_of::<$T>() as size_t).wrapping_add(($n) as size_t)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SWAP {
// CHECK-NEXT:     ($T:ty, $a:expr, $b:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let tmp: $T = $a;
// CHECK-NEXT:             $a = $b;
// CHECK-NEXT:             $b = tmp;
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TRACE {
// CHECK-NEXT:     ($x__0:expr $(, $x:expr)*) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { slate_support::printf(($x__0) as *const c_char, $($x),*) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
