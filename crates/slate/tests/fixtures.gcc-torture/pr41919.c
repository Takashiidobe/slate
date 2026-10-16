extern void abort(void);

#define assert(x)                                                              \
  if (!(x))                                                                    \
  abort()

struct S1 {
  signed char f0;
};

int g_23 = 0;

static struct S1 foo(void) {
  int      *l_100 = &g_23;
  int     **l_110 = &l_100;
  struct S1 l_128 = {1};
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  assert(l_100 == &g_23);
  return l_128;
}

static signed char bar(signed char si1, signed char si2) {
  return (si1 <= 0) ? si1 : (si2 * 2);
}
int main(void) {
  struct S1 s = foo();
  if (bar(0x99 ^ (s.f0 && 1), 1) != -104)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S1 {
// CHECK-NEXT:     f0: i8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut g_23: i32 = 0 as i32;
// CHECK-NEXT: fn foo() -> S1 {
// CHECK-NEXT:     let mut l_100: *mut i32 = unsafe { std::ptr::addr_of_mut!(g_23) };
// CHECK-NEXT:     let mut l_128: S1 = S1 { f0: (1 as i32) as i8 };
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(l_100 == unsafe { std::ptr::addr_of_mut!(g_23) }) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     return l_128;
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut si1: i8, mut si2: i8) -> i8 {
// CHECK-NEXT:     (if (si1 as i32) <= (0 as i32) { si1 as i32 } else { (si2 as i32) * (2 as i32) })
// CHECK-NEXT:         as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S1 = foo();
// CHECK-NEXT:     if (bar(
// CHECK-NEXT:         ((153 as i32) ^ ((s.f0 != (0 as i8) && (1 as i32) != (0 as i32)) as i32)) as i8,
// CHECK-NEXT:         (1 as i32) as i8,
// CHECK-NEXT:     ) as i32) != -(104 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
