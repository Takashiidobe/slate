/* We used to mis-compile this testcase as we did not know that
   &a+offsetof(b,a) was the same as &a.b */

void abort(void);

struct A {
  int t;
  int i;
};

void bar(float *p) { *p = 5.2; }

int foo(struct A *locp, int i, int str) {
  float f, g, *p;
  int   T355;
  int  *T356;
  /* Currently, the alias analyzer has limited support for handling
     aliases of structure fields when no other variables are aliased.
     Introduce additional aliases to confuse it.  */
  p = i ? &g : &f;
  bar(p);
  if (*p > 0.0)
    str = 1;

  T355  = locp->i;
  T356  = &locp->i;
  *T356 = str;
  T355  = locp->i;

  return T355;
}

int main(void) {
  struct A loc;
  int      str;

  loc.i = 2;
  str   = foo(&loc, 10, 3);
  if (str != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! We used to mis-compile this testcase as we did not know that
// CHECK-NEXT: //! &a+offsetof(b,a) was the same as &a.b
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     t: i32,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: *mut f32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 5.2f64 as f32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut locp: &mut A, mut i: i32, mut str: i32) -> i32 {
// CHECK-NEXT:     let mut f: f32 = 0 as f32;
// CHECK-NEXT:     let mut g: f32 = 0 as f32;
// CHECK-NEXT:     let mut p: *mut f32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut T355: i32 = 0 as i32;
// CHECK-NEXT:     let mut T356: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     // Currently, the alias analyzer has limited support for handling
// CHECK-NEXT:     // aliases of structure fields when no other variables are aliased.
// CHECK-NEXT:     // Introduce additional aliases to confuse it.
// CHECK-NEXT:     p = if i != (0 as i32) {
// CHECK-NEXT:         std::ptr::addr_of_mut!(g)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::addr_of_mut!(f)
// CHECK-NEXT:     };
// CHECK-NEXT:     bar(p);
// CHECK-NEXT:     if ((unsafe { *p }) as f64) > 0.0f64 {
// CHECK-NEXT:         str = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     T355 = unsafe { (*(locp as *mut A)).i };
// CHECK-NEXT:     T356 = unsafe { std::ptr::addr_of_mut!((*(locp as *mut A)).i) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *T356 = str;
// CHECK-NEXT:     }
// CHECK-NEXT:     T355 = unsafe { (*(locp as *mut A)).i };
// CHECK-NEXT:     T355
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut loc: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut str: i32 = 0 as i32;
// CHECK-NEXT:     loc.i = 2 as i32;
// CHECK-NEXT:     str = foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(loc)) },
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     if str != (1 as i32) {
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
