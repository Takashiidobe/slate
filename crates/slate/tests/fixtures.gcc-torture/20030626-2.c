char buf[40];

extern int  sprintf(char *, const char *, ...);
extern void abort(void);

int main() {
  int i = 0;
  int l = sprintf(buf, "%s", i++ ? "string" : "other string");
  if (l != sizeof("other string") - 1 || i != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn sprintf([[V_0:__v[0-9]+]]: *mut i8, [[V_1:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut buf: __SlateAlign16<[i8; 40]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut l: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     i = [[V_3]];
// CHECK-NEXT:     l = unsafe {
// CHECK-NEXT:         sprintf(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 },
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             if [[V_2]] != (0 as i32) {
// CHECK-NEXT:                 c"string".as_ptr() as *mut i8
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 c"other string".as_ptr() as *mut i8
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((l as i64) as u64) != (13 as u64).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         || i != (1 as i32)
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
