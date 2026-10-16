void abort(void);
void exit(int);

__extension__ union {
  double d;
  int    i[2];
} u = {d : -0.25};

/* This assumes the endianness of words in a long long is the same as
   that for doubles, which doesn't hold for a few platforms, but we
   can probably special case them here, as appropriate.  */
long long endianness_test = 1;
#define MSW (*(int *)&endianness_test)

int signbit(double x) {
  __extension__ union {
    double d;
    int    i[2];
  } u = {d : x};
  return u.i[MSW] < 0;
}

int main(void) {
  if (2 * sizeof(int) != sizeof(double) || u.i[MSW] >= 0)
    exit(0);

  if (!signbit(-0.25))
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     i: [i32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     i: [i32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: [[SLATERECORD_0]] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].d = -0.25f64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: /// This assumes the endianness of words in a long long is the same as
// CHECK-NEXT: /// that for doubles, which doesn't hold for a few platforms, but we
// CHECK-NEXT: /// can probably special case them here, as appropriate.
// CHECK-NEXT: static mut endianness_test: i64 = (1 as i32) as i64;
// CHECK-EMPTY:
// CHECK-NEXT: fn signbit(mut x: f64) -> i32 {
// CHECK-NEXT:     let mut [[U_0:u_[0-9]+]]: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_0]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].d = x;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { [[U_0]].i.as_mut_ptr() as *mut i32 }.offset(
// CHECK-NEXT:                 (unsafe { *((unsafe { std::ptr::addr_of_mut!(endianness_test) }) as *mut i32) })
// CHECK-NEXT:                     as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     }) < (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (((2 as i32) as i64) as u64).wrapping_mul(4 as u64) != (8 as u64)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(u.i) as *mut i32 }.offset(
// CHECK-NEXT:                     (unsafe { *((unsafe { std::ptr::addr_of_mut!(endianness_test) }) as *mut i32) })
// CHECK-NEXT:                         as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:         }) >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(signbit(-0.25f64) != (0 as i32)) {
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
