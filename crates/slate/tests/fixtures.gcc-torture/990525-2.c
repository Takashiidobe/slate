void abort(void);
void exit(int);

typedef struct {
  int v[4];
} Test1;

Test1 func2();

int func1() {
  Test1 test;
  test = func2();

  if (test.v[0] != 10)
    abort();
  if (test.v[1] != 20)
    abort();
  if (test.v[2] != 30)
    abort();
  if (test.v[3] != 40)
    abort();
}

Test1 func2() {
  Test1 tmp;
  tmp.v[0] = 10;
  tmp.v[1] = 20;
  tmp.v[2] = 30;
  tmp.v[3] = 40;
  return tmp;
}

int main() {
  func1();
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
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: [i32; 4],
// CHECK-NEXT: }
// CHECK-NEXT: fn func1() -> i32 {
// CHECK-NEXT:     let mut test: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     test = func2();
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (test.v.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) != (10 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (test.v.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) }
// CHECK-NEXT:     }) != (20 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (test.v.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) }
// CHECK-NEXT:     }) != (30 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (test.v.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) }
// CHECK-NEXT:     }) != (40 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn func2() -> [[SLATERECORD_0]] {
// CHECK-NEXT:     let mut tmp: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (tmp.v.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = 10
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (tmp.v.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = 20
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (tmp.v.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = 30
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (tmp.v.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = 40
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     return tmp;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func1();
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
