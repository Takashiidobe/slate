extern __attribute__((__noreturn__)) void exit(int);
extern __attribute__((__noreturn__)) void abort(void);
__attribute__((__noinline__)) int         add512(int a, int *b) {
  int c = a + 512;
  if (c != 0)
    *b = a;
  return c;
}

__attribute__((__noinline__)) int add513(int a, int *b) {
  int c = a + 513;
  if (c == 0)
    *b = a;
  return c;
}

int main(void) {
  int b0 = -1;
  int b1 = -1;
  if (add512(-512, &b0) != 0 || b0 != -1 || add513(-513, &b1) != 0 ||
      b1 != -513)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn add512(mut a: i32, mut b: &mut i32) -> i32 {
// CHECK-NEXT:     let mut c: i32 = a + (512 as i32);
// CHECK-NEXT:     if c != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(b as *mut i32) = a;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn add513(mut a: i32, mut b: &mut i32) -> i32 {
// CHECK-NEXT:     let mut c: i32 = a + (513 as i32);
// CHECK-NEXT:     if c == (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(b as *mut i32) = a;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b0: i32 = -(1 as i32);
// CHECK-NEXT:     let mut b1: i32 = -(1 as i32);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if add512(-(512 as i32), unsafe { &mut (*std::ptr::addr_of_mut!(b0)) }) != (0 as i32)
// CHECK-NEXT:         || b0 != -(1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = add513(-(513 as i32), unsafe { &mut (*std::ptr::addr_of_mut!(b1)) })
// CHECK-NEXT:             != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] || b1 != -(513 as i32) {
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
