extern void abort(void);
struct test1 {
  int a;
  int b;
};
struct test2 {
  float        d;
  struct test1 sub;
};

int global;

int bla(struct test1 *xa, struct test2 *xb) {
  global    = 1;
  xb->sub.a = 1;
  xa->a     = 8;
  return xb->sub.a;
}

int main(void) {
  struct test2 pom;

  if (bla(&pom.sub, &pom) != 8)
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
// CHECK-NEXT: struct test1 {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct test2 {
// CHECK-NEXT:     d: f32,
// CHECK-NEXT:     sub: test1,
// CHECK-NEXT: }
// CHECK-NEXT: static mut global: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bla(mut xa: &mut test1, mut xb: &mut test2) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         global = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(xb as *mut test2)).sub.a = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(xa as *mut test1)).a = 8 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (*(xb as *mut test2)).sub.a }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut pom: test2 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if bla(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(pom.sub)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(pom)) },
// CHECK-NEXT:     ) != (8 as i32)
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
