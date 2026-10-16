/* PR rtl-optimization/84071 */
/* Reported by Wilco <wilco@gcc.gnu.org> */

extern void abort(void);

typedef union {
  signed short   ss;
  unsigned short us;
  int            x;
} U;

int f(int x, int y, int z, int a, U u) __attribute__((noclone, noinline));

int f(int x, int y, int z, int a, U u) { return (u.ss <= 0) + u.us; }

int main(void) {
  U u = {.ss = -1};

  if (f(0, 0, 0, 0, u) != (1 << sizeof(short) * 8))
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/84071
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by Wilco <wilco@gcc.gnu.org>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     ss: i16,
// CHECK-NEXT:     us: u16,
// CHECK-NEXT:     x: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut x: i32, mut y: i32, mut z: i32, mut a: i32, mut u: [[SLATERECORD_0]]) -> i32 {
// CHECK-NEXT:     ((((unsafe { u.ss }) as i32) <= (0 as i32)) as i32) + (((unsafe { u.us }) as u32) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].ss = -(1 as i32) as i16;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     if f(0 as i32, 0 as i32, 0 as i32, 0 as i32, u)
// CHECK-NEXT:         != (1 as i32) << (2 as u64).wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
