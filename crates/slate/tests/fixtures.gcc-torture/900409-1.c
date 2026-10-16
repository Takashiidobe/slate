void abort(void);
void exit(int);

long f1(long a) { return a & 0xff000000L; }
long f2(long a) { return a & ~0xff000000L; }
long f3(long a) { return a & 0x000000ffL; }
long f4(long a) { return a & ~0x000000ffL; }
long f5(long a) { return a & 0x0000ffffL; }
long f6(long a) { return a & ~0x0000ffffL; }

int main(void) {
  long a = 0x89ABCDEF;

  if (f1(a) != 0x89000000L || f2(a) != 0x00ABCDEFL || f3(a) != 0x000000EFL ||
      f4(a) != 0x89ABCD00L || f5(a) != 0x0000CDEFL || f6(a) != 0x89AB0000L)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f1(mut a: i64) -> i64 {
// CHECK-NEXT:     a & (4278190080 as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn f2(mut a: i64) -> i64 {
// CHECK-NEXT:     a & !(4278190080 as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn f3(mut a: i64) -> i64 {
// CHECK-NEXT:     a & (255 as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn f4(mut a: i64) -> i64 {
// CHECK-NEXT:     a & !(255 as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn f5(mut a: i64) -> i64 {
// CHECK-NEXT:     a & (65535 as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn f6(mut a: i64) -> i64 {
// CHECK-NEXT:     a & !(65535 as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i64 = ((2309737967 as u32) as u64) as i64;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f1(a) != (2298478592 as i64) {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = f2(a) != (11259375 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = f3(a) != (239 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = f4(a) != (2309737728 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = f5(a) != (52719 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = f6(a) != (2309685248 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
