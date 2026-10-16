/* { dg-skip-if "requires io" { freestanding } }  */

/* Program to test gcc's usage of the gofast library.  */

/* The main guiding themes are to make it trivial to add test cases over time
   and to make it easy for a program to parse the output to see if the right
   libcalls are being made.  */

#include <stdio.h>

void abort(void);
void exit(int);

float fp_add(float a, float b) { return a + b; }
float fp_sub(float a, float b) { return a - b; }
float fp_mul(float a, float b) { return a * b; }
float fp_div(float a, float b) { return a / b; }
float fp_neg(float a) { return -a; }

double dp_add(double a, double b) { return a + b; }
double dp_sub(double a, double b) { return a - b; }
double dp_mul(double a, double b) { return a * b; }
double dp_div(double a, double b) { return a / b; }
double dp_neg(double a) { return -a; }

double fp_to_dp(float f) { return f; }
float  dp_to_fp(double d) { return d; }

int eqsf2(float a, float b) { return a == b; }
int nesf2(float a, float b) { return a != b; }
int gtsf2(float a, float b) { return a > b; }
int gesf2(float a, float b) { return a >= b; }
int ltsf2(float a, float b) { return a < b; }
int lesf2(float a, float b) { return a <= b; }

int eqdf2(double a, double b) { return a == b; }
int nedf2(double a, double b) { return a != b; }
int gtdf2(double a, double b) { return a > b; }
int gedf2(double a, double b) { return a >= b; }
int ltdf2(double a, double b) { return a < b; }
int ledf2(double a, double b) { return a <= b; }

float        floatsisf(int i) { return i; }
double       floatsidf(int i) { return i; }
int          fixsfsi(float f) { return f; }
int          fixdfsi(double d) { return d; }
unsigned int fixunssfsi(float f) { return f; }
unsigned int fixunsdfsi(double d) { return d; }

int fail_count = 0;

int fail(char *msg) {
  fail_count++;
  fprintf(stderr, "Test failed: %s\n", msg);
}

int main() {
  if (fp_add(1, 1) != 2)
    fail("fp_add 1+1");
  if (fp_sub(3, 2) != 1)
    fail("fp_sub 3-2");
  if (fp_mul(2, 3) != 6)
    fail("fp_mul 2*3");
  if (fp_div(3, 2) != 1.5)
    fail("fp_div 3/2");
  if (fp_neg(1) != -1)
    fail("fp_neg 1");

  if (dp_add(1, 1) != 2)
    fail("dp_add 1+1");
  if (dp_sub(3, 2) != 1)
    fail("dp_sub 3-2");
  if (dp_mul(2, 3) != 6)
    fail("dp_mul 2*3");
  if (dp_div(3, 2) != 1.5)
    fail("dp_div 3/2");
  if (dp_neg(1) != -1)
    fail("dp_neg 1");

  if (fp_to_dp(1.5) != 1.5)
    fail("fp_to_dp 1.5");
  if (dp_to_fp(1.5) != 1.5)
    fail("dp_to_fp 1.5");

  if (floatsisf(1) != 1)
    fail("floatsisf 1");
  if (floatsidf(1) != 1)
    fail("floatsidf 1");
  if (fixsfsi(1.42) != 1)
    fail("fixsfsi 1.42");
  if (fixunssfsi(1.42) != 1)
    fail("fixunssfsi 1.42");
  if (fixdfsi(1.42) != 1)
    fail("fixdfsi 1.42");
  if (fixunsdfsi(1.42) != 1)
    fail("fixunsdfsi 1.42");

  if (eqsf2(1, 1) == 0)
    fail("eqsf2 1==1");
  if (eqsf2(1, 2) != 0)
    fail("eqsf2 1==2");
  if (nesf2(1, 2) == 0)
    fail("nesf2 1!=1");
  if (nesf2(1, 1) != 0)
    fail("nesf2 1!=1");
  if (gtsf2(2, 1) == 0)
    fail("gtsf2 2>1");
  if (gtsf2(1, 1) != 0)
    fail("gtsf2 1>1");
  if (gtsf2(0, 1) != 0)
    fail("gtsf2 0>1");
  if (gesf2(2, 1) == 0)
    fail("gesf2 2>=1");
  if (gesf2(1, 1) == 0)
    fail("gesf2 1>=1");
  if (gesf2(0, 1) != 0)
    fail("gesf2 0>=1");
  if (ltsf2(1, 2) == 0)
    fail("ltsf2 1<2");
  if (ltsf2(1, 1) != 0)
    fail("ltsf2 1<1");
  if (ltsf2(1, 0) != 0)
    fail("ltsf2 1<0");
  if (lesf2(1, 2) == 0)
    fail("lesf2 1<=2");
  if (lesf2(1, 1) == 0)
    fail("lesf2 1<=1");
  if (lesf2(1, 0) != 0)
    fail("lesf2 1<=0");

  if (fail_count != 0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stderr: *mut _IO_FILE;
// CHECK-NEXT:     fn fprintf(__stream: *mut _IO_FILE, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut i8,
// CHECK-NEXT:     _IO_read_end: *mut i8,
// CHECK-NEXT:     _IO_read_base: *mut i8,
// CHECK-NEXT:     _IO_write_base: *mut i8,
// CHECK-NEXT:     _IO_write_ptr: *mut i8,
// CHECK-NEXT:     _IO_write_end: *mut i8,
// CHECK-NEXT:     _IO_buf_base: *mut i8,
// CHECK-NEXT:     _IO_buf_end: *mut i8,
// CHECK-NEXT:     _IO_save_base: *mut i8,
// CHECK-NEXT:     _IO_backup_base: *mut i8,
// CHECK-NEXT:     _IO_save_end: *mut i8,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT:     _short_backupbuf: [i8; 1],
// CHECK-NEXT:     _old_offset: i64,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [i8; 1],
// CHECK-NEXT:     _lock: *mut (),
// CHECK-NEXT:     _offset: i64,
// CHECK-NEXT:     _codecvt: *mut _IO_codecvt,
// CHECK-NEXT:     _wide_data: *mut _IO_wide_data,
// CHECK-NEXT:     _freeres_list: *mut _IO_FILE,
// CHECK-NEXT:     _freeres_buf: *mut (),
// CHECK-NEXT:     _prevchain: *mut *mut _IO_FILE,
// CHECK-NEXT:     _mode: i32,
// CHECK-NEXT:     _unused3: i32,
// CHECK-NEXT:     _total_written: u64,
// CHECK-NEXT:     _unused2: [i8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Program to test gcc's usage of the gofast library.
// CHECK-NEXT: // The main guiding themes are to make it trivial to add test cases over time
// CHECK-NEXT: // and to make it easy for a program to parse the output to see if the right
// CHECK-NEXT: // libcalls are being made.
// CHECK-NEXT: fn fp_add(mut a: f32, mut b: f32) -> f32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fp_sub(mut a: f32, mut b: f32) -> f32 {
// CHECK-NEXT:     a - b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fp_mul(mut a: f32, mut b: f32) -> f32 {
// CHECK-NEXT:     a * b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fp_div(mut a: f32, mut b: f32) -> f32 {
// CHECK-NEXT:     a / b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fp_neg(mut a: f32) -> f32 {
// CHECK-NEXT:     -a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dp_add(mut a: f64, mut b: f64) -> f64 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dp_sub(mut a: f64, mut b: f64) -> f64 {
// CHECK-NEXT:     a - b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dp_mul(mut a: f64, mut b: f64) -> f64 {
// CHECK-NEXT:     a * b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dp_div(mut a: f64, mut b: f64) -> f64 {
// CHECK-NEXT:     a / b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dp_neg(mut a: f64) -> f64 {
// CHECK-NEXT:     -a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fp_to_dp(mut f: f32) -> f64 {
// CHECK-NEXT:     f as f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dp_to_fp(mut d: f64) -> f32 {
// CHECK-NEXT:     d as f32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn eqsf2(mut a: f32, mut b: f32) -> i32 {
// CHECK-NEXT:     (a == b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn nesf2(mut a: f32, mut b: f32) -> i32 {
// CHECK-NEXT:     (a != b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gtsf2(mut a: f32, mut b: f32) -> i32 {
// CHECK-NEXT:     (a > b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gesf2(mut a: f32, mut b: f32) -> i32 {
// CHECK-NEXT:     (a >= b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ltsf2(mut a: f32, mut b: f32) -> i32 {
// CHECK-NEXT:     (a < b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lesf2(mut a: f32, mut b: f32) -> i32 {
// CHECK-NEXT:     (a <= b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn eqdf2(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     (a == b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn nedf2(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     (a != b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gtdf2(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     (a > b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gedf2(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     (a >= b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ltdf2(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     (a < b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ledf2(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     (a <= b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn floatsisf(mut i: i32) -> f32 {
// CHECK-NEXT:     i as f32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn floatsidf(mut i: i32) -> f64 {
// CHECK-NEXT:     i as f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fixsfsi(mut f: f32) -> i32 {
// CHECK-NEXT:     f as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fixdfsi(mut d: f64) -> i32 {
// CHECK-NEXT:     d as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fixunssfsi(mut f: f32) -> u32 {
// CHECK-NEXT:     f as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fixunsdfsi(mut d: f64) -> u32 {
// CHECK-NEXT:     d as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut fail_count: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn fail(mut msg: *mut i8) -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { fail_count };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fail_count = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fprintf(
// CHECK-NEXT:             unsafe { stderr },
// CHECK-NEXT:             c"Test failed: %s\n".as_ptr() as *const i8,
// CHECK-NEXT:             msg,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if fp_add((1 as i32) as f32, (1 as i32) as f32) != ((2 as i32) as f32) {
// CHECK-NEXT:         fail(c"fp_add 1+1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fp_sub((3 as i32) as f32, (2 as i32) as f32) != ((1 as i32) as f32) {
// CHECK-NEXT:         fail(c"fp_sub 3-2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fp_mul((2 as i32) as f32, (3 as i32) as f32) != ((6 as i32) as f32) {
// CHECK-NEXT:         fail(c"fp_mul 2*3".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (fp_div((3 as i32) as f32, (2 as i32) as f32) as f64) != 1.5f64 {
// CHECK-NEXT:         fail(c"fp_div 3/2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fp_neg((1 as i32) as f32) != (-(1 as i32) as f32) {
// CHECK-NEXT:         fail(c"fp_neg 1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if dp_add((1 as i32) as f64, (1 as i32) as f64) != ((2 as i32) as f64) {
// CHECK-NEXT:         fail(c"dp_add 1+1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if dp_sub((3 as i32) as f64, (2 as i32) as f64) != ((1 as i32) as f64) {
// CHECK-NEXT:         fail(c"dp_sub 3-2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if dp_mul((2 as i32) as f64, (3 as i32) as f64) != ((6 as i32) as f64) {
// CHECK-NEXT:         fail(c"dp_mul 2*3".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if dp_div((3 as i32) as f64, (2 as i32) as f64) != 1.5f64 {
// CHECK-NEXT:         fail(c"dp_div 3/2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if dp_neg((1 as i32) as f64) != (-(1 as i32) as f64) {
// CHECK-NEXT:         fail(c"dp_neg 1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fp_to_dp(1.5f64 as f32) != 1.5f64 {
// CHECK-NEXT:         fail(c"fp_to_dp 1.5".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (dp_to_fp(1.5f64) as f64) != 1.5f64 {
// CHECK-NEXT:         fail(c"dp_to_fp 1.5".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if floatsisf(1 as i32) != ((1 as i32) as f32) {
// CHECK-NEXT:         fail(c"floatsisf 1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if floatsidf(1 as i32) != ((1 as i32) as f64) {
// CHECK-NEXT:         fail(c"floatsidf 1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fixsfsi(1.42f64 as f32) != (1 as i32) {
// CHECK-NEXT:         fail(c"fixsfsi 1.42".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fixunssfsi(1.42f64 as f32) != ((1 as i32) as u32) {
// CHECK-NEXT:         fail(c"fixunssfsi 1.42".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fixdfsi(1.42f64) != (1 as i32) {
// CHECK-NEXT:         fail(c"fixdfsi 1.42".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if fixunsdfsi(1.42f64) != ((1 as i32) as u32) {
// CHECK-NEXT:         fail(c"fixunsdfsi 1.42".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if eqsf2((1 as i32) as f32, (1 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"eqsf2 1==1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if eqsf2((1 as i32) as f32, (2 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"eqsf2 1==2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if nesf2((1 as i32) as f32, (2 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"nesf2 1!=1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if nesf2((1 as i32) as f32, (1 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"nesf2 1!=1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if gtsf2((2 as i32) as f32, (1 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"gtsf2 2>1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if gtsf2((1 as i32) as f32, (1 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"gtsf2 1>1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if gtsf2((0 as i32) as f32, (1 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"gtsf2 0>1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if gesf2((2 as i32) as f32, (1 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"gesf2 2>=1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if gesf2((1 as i32) as f32, (1 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"gesf2 1>=1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if gesf2((0 as i32) as f32, (1 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"gesf2 0>=1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ltsf2((1 as i32) as f32, (2 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"ltsf2 1<2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ltsf2((1 as i32) as f32, (1 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"ltsf2 1<1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ltsf2((1 as i32) as f32, (0 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"ltsf2 1<0".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if lesf2((1 as i32) as f32, (2 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"lesf2 1<=2".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if lesf2((1 as i32) as f32, (1 as i32) as f32) == (0 as i32) {
// CHECK-NEXT:         fail(c"lesf2 1<=1".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if lesf2((1 as i32) as f32, (0 as i32) as f32) != (0 as i32) {
// CHECK-NEXT:         fail(c"lesf2 1<=0".as_ptr() as *mut i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { fail_count }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
