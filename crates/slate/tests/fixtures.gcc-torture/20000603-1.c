/* It is not clear whether this test is conforming.  See DR#236
   http://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_236.htm.  However,
   there seems to be consensus that the presence of a union to aggregate
   struct s1 and struct s2 should make it conforming.  */
void abort(void);

struct s1 {
  double d;
};
struct s2 {
  double d;
};
union u {
  struct s1 x;
  struct s2 y;
};

double f(struct s1 *a, struct s2 *b) {
  a->d = 1.0;
  return b->d + 1.0;
}

int main() {
  union u a;
  a.x.d = 0.0;
  if (f(&a.x, &a.y) != 2.0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // It is not clear whether this test is conforming.  See DR#236
// CHECK-NEXT: // http://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_236.htm.  However,
// CHECK-NEXT: // there seems to be consensus that the presence of a union to aggregate
// CHECK-NEXT: // struct s1 and struct s2 should make it conforming.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s1 {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s2 {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union u {
// CHECK-NEXT:     x: s1,
// CHECK-NEXT:     y: s2,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: *mut s1, mut b: *mut s2) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*a).d = 1.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { (*b).d }) + 1.0f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.x.d = 0.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(unsafe { std::ptr::addr_of_mut!(a.x) }, unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(a.y)
// CHECK-NEXT:     }) != 2.0f64
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
