/* If some target has a Max alignment less than 32, please create
   a #ifdef around the alignment and add your alignment.  */
#ifdef __pdp11__
#define alignment 2
#else
#define alignment 32
#endif

void abort(void);
void exit(int);

typedef struct x {
  int a;
  int b;
} __attribute__((aligned(alignment))) X;
typedef struct y {
  X   x;
  X   y[31];
  int c;
} Y;

Y y[2];

int main(void) {
  if (((char *)&y[1] - (char *)&y[0]) & 31)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! If some target has a Max alignment less than 32, please create
// CHECK-NEXT: //! a #ifdef around the alignment and add your alignment.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct x {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct y {
// CHECK-NEXT:     x: x,
// CHECK-NEXT:     y: [x; 31],
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut y: [y; 2] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         ((unsafe { unsafe { std::ptr::addr_of_mut!(y) as *mut y }.offset((1 as i32) as isize) })
// CHECK-NEXT:             as *mut i8)
// CHECK-NEXT:             .offset_from(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(y) as *mut y }.offset((0 as i32) as isize)
// CHECK-NEXT:                 }) as *mut i8,
// CHECK-NEXT:             )
// CHECK-NEXT:     }) as i64)
// CHECK-NEXT:         & ((31 as i32) as i64)
// CHECK-NEXT:         != (0 as i64)
// CHECK-NEXT:     {
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
