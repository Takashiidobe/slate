void abort(void);
void exit(int);

unsigned calc_mp(unsigned mod) {
  unsigned a, b, c;
  c = -1;
  a = c / mod;
  b = 0 - a * mod;
  if (b > mod) {
    a += 1;
    b -= mod;
  }
  return b;
}

int main(int argc, char *argv[]) {
  unsigned x = 1234;
  unsigned y = calc_mp(x);

  if ((sizeof(y) == 4 && y != 680) || (sizeof(y) == 2 && y != 134))
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
// CHECK-NEXT: fn calc_mp(mut r#mod: u32) -> u32 {
// CHECK-NEXT:     let mut a: u32 = 0 as u32;
// CHECK-NEXT:     let mut b: u32 = 0 as u32;
// CHECK-NEXT:     let mut c: u32 = -(1 as i32) as u32;
// CHECK-NEXT:     a = c / r#mod;
// CHECK-NEXT:     b = ((0 as i32) as u32).wrapping_sub(a.wrapping_mul(r#mod));
// CHECK-NEXT:     if b > r#mod {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = a;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         a = [[V_2]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = b;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = [[V_3]].wrapping_sub(r#mod);
// CHECK-NEXT:         b = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut x: u32 = (1234 as i32) as u32;
// CHECK-NEXT:     let mut y: u32 = calc_mp(x);
// CHECK-NEXT:     if (4 as u64) == (((4 as i32) as i64) as u64) && y != ((680 as i32) as u32)
// CHECK-NEXT:         || (4 as u64) == (((2 as i32) as i64) as u64) && y != ((134 as i32) as u32)
// CHECK-NEXT:     {
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
