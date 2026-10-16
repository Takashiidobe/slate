extern void           exit(int);
extern void           abort(void);
extern unsigned short f(short a) __attribute__((__noinline__));

unsigned short f(short a) {
  short b;

  if (a > 0)
    return 0;
  b = ((int)a) + -(int)32768;
  return b;
}

int main(void) {
  if (sizeof(short) < 2 || sizeof(short) >= sizeof(int))
    exit(0);

  if (f(-32767) != 1)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut a: i16) -> u16 {
// CHECK-NEXT:     let mut b: i16 = 0 as i16;
// CHECK-NEXT:     if (a as i32) > (0 as i32) {
// CHECK-NEXT:         return ((0 as i32) as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     b = ((a as i32) + -(32768 as i32)) as i16;
// CHECK-NEXT:     b as u16
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (2 as u64) < (((2 as i32) as i64) as u64) || (2 as u64) >= (4 as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((f(-(32767 as i32) as i16) as u32) as i32) != (1 as i32) {
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
