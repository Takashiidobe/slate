struct f {
  int i;
};

int g(int i, int c, struct f *ff, int *p) {
  int *t;
  if (c)
    t = &i;
  else
    t = &ff->i;
  *p = 0;
  return *t;
}

extern void abort(void);

int main() {
  struct f f;
  f.i = 1;
  if (g(5, 0, &f, &f.i) != 0)
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
// CHECK-NEXT: struct f {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn g(mut i: i32, mut c: i32, mut ff: &mut f, mut p: &mut i32) -> i32 {
// CHECK-NEXT:     let mut t: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if c != (0 as i32) {
// CHECK-NEXT:         t = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         t = unsafe { std::ptr::addr_of_mut!((* (ff as * mut f)).i) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut i32) = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *t }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f.i = 1 as i32;
// CHECK-NEXT:     if g(
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(f)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(f.i)) },
// CHECK-NEXT:     ) != (0 as i32)
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
