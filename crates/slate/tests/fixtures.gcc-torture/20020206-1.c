struct A {
  unsigned int a, b, c;
};

extern void abort(void);
extern void exit(int);

struct A bar(void) { return (struct A){176, 52, 31}; }

void baz(struct A *a) {
  if (a->a != 176 || a->b != 52 || a->c != 31)
    abort();
}

int main() {
  struct A d;

  d = ({ ({ bar(); }); });
  baz(&d);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() -> A {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = A {
// CHECK-NEXT:                 a: (176 as i32) as u32,
// CHECK-NEXT:                 b: (52 as i32) as u32,
// CHECK-NEXT:                 c: (31 as i32) as u32,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn baz(mut a: &A) {
// CHECK-NEXT:     if (unsafe { (*(a as *const A)).a }) != ((176 as i32) as u32)
// CHECK-NEXT:         || (unsafe { (*(a as *const A)).b }) != ((52 as i32) as u32)
// CHECK-NEXT:         || (unsafe { (*(a as *const A)).c }) != ((31 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: A;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: A;
// CHECK-NEXT:     [[V_2]] = bar();
// CHECK-NEXT:     [[V_1]] = [[V_2]];
// CHECK-NEXT:     d = [[V_1]];
// CHECK-NEXT:     baz(unsafe { &(*std::ptr::addr_of_mut!(d)) });
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
