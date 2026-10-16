struct S {
  char  c;
  char  arr[4];
  float f;
};

char A[4] = {'1', '2', '3', '4'};

void foo(struct S s) {
  if (__builtin_memcmp(s.arr, A, 4))
    __builtin_abort();
}

int main(void) {
  struct S s;
  __builtin_memcpy(s.arr, A, 4);
  foo(s);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     arr: [i8; 4],
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut A: [i8; 4] = [
// CHECK-NEXT:     (49 as i32) as i8,
// CHECK-NEXT:     (50 as i32) as i8,
// CHECK-NEXT:     (51 as i32) as i8,
// CHECK-NEXT:     (52 as i32) as i8,
// CHECK-NEXT: ];
// CHECK-NEXT: fn foo(mut s: S) {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             s.arr.as_mut_ptr() as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(A) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             s.arr.as_mut_ptr() as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(A) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(s);
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
