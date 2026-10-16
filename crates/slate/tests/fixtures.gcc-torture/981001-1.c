void abort(void);
void exit(int);

#define NG 0x100L

unsigned long flg = 0;

long sub(int n) {
  int a, b;

  if (n >= 2) {
    if (n % 2 == 0) {
      a = sub(n / 2);

      return (a + 2 * sub(n / 2 - 1)) * a;
    } else {
      a = sub(n / 2 + 1);
      b = sub(n / 2);

      return a * a + b * b;
    }
  } else
    return (long)n;
}

int main(void) {
  if (sub(30) != 832040L)
    flg |= NG;

  if (flg)
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
// CHECK-NEXT: static mut flg: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT: fn sub(mut n: i32) -> i64 {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     if n >= (2 as i32) {
// CHECK-NEXT:         if n % (2 as i32) == (0 as i32) {
// CHECK-NEXT:             a = sub(n / (2 as i32)) as i32;
// CHECK-NEXT:             return ((a as i64) + ((2 as i32) as i64) * sub(n / (2 as i32) - (1 as i32)))
// CHECK-NEXT:                 * (a as i64);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             a = sub(n / (2 as i32) + (1 as i32)) as i32;
// CHECK-NEXT:             b = sub(n / (2 as i32)) as i32;
// CHECK-NEXT:             return (a * a + b * b) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return n as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if sub(30 as i32) != (832040 as i64) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = unsafe { flg };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u64 = [[V_1]] | ((256 as i64) as u64);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flg = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { flg }) != (0 as u64) {
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
