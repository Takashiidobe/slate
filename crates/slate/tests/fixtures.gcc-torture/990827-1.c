void abort(void);
void exit(int);

unsigned test(unsigned one, unsigned bit) {
  unsigned val  = bit & 1;
  unsigned zero = one >> 1;

  val++;
  return zero + (val >> 1);
}

int main() {
  if (test(1, 0) != 0)
    abort();
  if (test(1, 1) != 1)
    abort();
  if (test(1, 65535) != 1)
    abort();
  exit(0);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut one: u32, mut bit: u32) -> u32 {
// CHECK-NEXT:     let mut val: u32 = bit & ((1 as i32) as u32);
// CHECK-NEXT:     let mut zero: u32 = one >> (1 as i32);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = val;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = [[V_1]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     val = [[V_2]];
// CHECK-NEXT:     zero.wrapping_add(val >> (1 as i32))
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test((1 as i32) as u32, (0 as i32) as u32) != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((1 as i32) as u32, (1 as i32) as u32) != ((1 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((1 as i32) as u32, (65535 as i32) as u32) != ((1 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0 as i32
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
