void abort(void);
void exit(int);

struct x {
  int a, b;
} z = {-4028, 4096};

int foo(struct x *p, int y) {
  if ((y & 0xff) != y || -p->b >= p->a)
    return 1;
  return 0;
}

int main(void) {
  if (foo(&z, 10))
    abort();
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
// CHECK-NEXT: struct x {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut z: x = x {
// CHECK-NEXT:     a: -(4028 as i32),
// CHECK-NEXT:     b: 4096 as i32,
// CHECK-NEXT: };
// CHECK-NEXT: fn foo(mut p: *mut x, mut y: i32) -> i32 {
// CHECK-NEXT:     if y & (255 as i32) != y || -unsafe { (*p).b } >= unsafe { (*p).a } {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(unsafe { std::ptr::addr_of_mut!(z) }, 10 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
