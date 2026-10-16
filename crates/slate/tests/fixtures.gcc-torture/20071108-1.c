/* PR tree-optimization/32575 */

extern void abort(void);

struct S {
  void         *s1, *s2;
  unsigned char s3, s4, s5;
};

__attribute__((noinline)) void *foo(void) {
  static struct S s;
  return &s;
}

__attribute__((noinline)) void *bar() { return (void *)0; }

__attribute__((noinline)) struct S *test(void *a, void *b) {
  struct S *p, q;
  p = foo();
  if (p == 0) {
    p = &q;
    __builtin_memset(p, 0, sizeof(*p));
  }
  p->s1 = a;
  p->s2 = b;
  if (p == &q)
    p = 0;
  return p;
}

int main(void) {
  int       a;
  int       b;
  struct S *z = test((void *)&a, (void *)&b);
  if (z == 0 || z->s1 != (void *)&a || z->s2 != (void *)&b || z->s3 || z->s4)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/32575
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     s1: *mut (),
// CHECK-NEXT:     s2: *mut (),
// CHECK-NEXT:     s3: u8,
// CHECK-NEXT:     s4: u8,
// CHECK-NEXT:     s5: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> *mut () {
// CHECK-NEXT:     (unsafe { std::ptr::addr_of_mut!(s) }) as *mut ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> *mut () {
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut a: *mut (), mut b: *mut ()) -> *mut S {
// CHECK-NEXT:     let mut p: *mut S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p = foo() as *mut S;
// CHECK-NEXT:     if p == std::ptr::null_mut::<S>() {
// CHECK-NEXT:         p = std::ptr::addr_of_mut!(q);
// CHECK-NEXT:         unsafe { __builtin_memset(p as *mut (), 0 as i32, 24 as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).s1 = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).s2 = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     if p == std::ptr::addr_of_mut!(q) {
// CHECK-NEXT:         p = std::ptr::null_mut::<S>();
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     let mut z: *mut S = test(
// CHECK-NEXT:         std::ptr::addr_of_mut!(a) as *mut (),
// CHECK-NEXT:         std::ptr::addr_of_mut!(b) as *mut (),
// CHECK-NEXT:     );
// CHECK-NEXT:     if z == std::ptr::null_mut::<S>()
// CHECK-NEXT:         || (unsafe { (*z).s1 }) != (std::ptr::addr_of_mut!(a) as *mut ())
// CHECK-NEXT:         || (unsafe { (*z).s2 }) != (std::ptr::addr_of_mut!(b) as *mut ())
// CHECK-NEXT:         || (unsafe { (*z).s3 }) != (0 as u8)
// CHECK-NEXT:         || (unsafe { (*z).s4 }) != (0 as u8)
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
