#define NULL 0

struct stuff {
  int   a;
  int   b;
  int   c;
  int   d;
  int   e;
  char *f;
  int   g;
};

void __attribute__((noinline)) bar(struct stuff *x) {
  if (x->g != 2)
    __builtin_abort();
}

int main(int argc, char **argv) {
  struct stuff x = {0, 0, 0, 0, 0, NULL, 0};
  x.a            = 100;
  x.d            = 100;
  x.g            = 2;
  /* Struct should now look like {100, 0, 0, 100, 0, 0, 0, 2}.  */
  bar(&x);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct stuff {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: i32,
// CHECK-NEXT:     e: i32,
// CHECK-NEXT:     f: *mut i8,
// CHECK-NEXT:     g: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &stuff) {
// CHECK-NEXT:     if (unsafe { (*(x as *const stuff)).g }) != (2 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut x: stuff = stuff {
// CHECK-NEXT:         a: 0 as i32,
// CHECK-NEXT:         b: 0 as i32,
// CHECK-NEXT:         c: 0 as i32,
// CHECK-NEXT:         d: 0 as i32,
// CHECK-NEXT:         e: 0 as i32,
// CHECK-NEXT:         f: std::ptr::null_mut::<i8>(),
// CHECK-NEXT:         g: 0 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     x.a = 100 as i32;
// CHECK-NEXT:     x.d = 100 as i32;
// CHECK-NEXT:     x.g = 2 as i32;
// CHECK-NEXT:     // Struct should now look like {100, 0, 0, 100, 0, 0, 0, 2}.
// CHECK-NEXT:     bar(unsafe { &(*std::ptr::addr_of_mut!(x)) });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
