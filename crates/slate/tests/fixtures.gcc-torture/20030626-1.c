char buf[10];

extern void abort(void);
extern int  sprintf(char *, const char *, ...);

int main() {
  int l = sprintf(buf, "foo\0bar");
  if (l != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn sprintf([[V_0:__v[0-9]+]]: *mut i8, [[V_1:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut buf: [i8; 10] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut l: i32 = unsafe {
// CHECK-NEXT:         sprintf(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(buf) as *mut i8 },
// CHECK-NEXT:             b"foo\0bar\0".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if l != (3 as i32) {
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
