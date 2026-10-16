void abort(void);
void exit(int);

short x1 = 17;

struct {
  short i __attribute__((packed));
} t;

void f() {
  t.i = x1;
  if (t.i != 17)
    abort();
}

int main(void) {
  f();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut x1: i16 = (17 as i32) as i16;
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     i: i16,
// CHECK-NEXT: }
// CHECK-NEXT: static mut t: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t.i = unsafe { x1 };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { t.i }) as i32) != (17 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
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
