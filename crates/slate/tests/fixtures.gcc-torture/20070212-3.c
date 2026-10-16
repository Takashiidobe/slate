struct foo {
  int i;
  int j;
};

int bar(struct foo *k, int k2, int f, int f2) {
  int *p, *q;
  int  res;
  if (f)
    p = &k->i;
  else
    p = &k->j;
  res  = *p;
  k->i = 1;
  if (f2)
    q = p;
  else
    q = &k2;
  return res + *q;
}

extern void abort(void);

int main() {
  struct foo k;
  k.i = 0;
  k.j = 1;
  if (bar(&k, 1, 1, 1) != 1)
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
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut k: &mut foo, mut k2: i32, mut f: i32, mut f2: i32) -> i32 {
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut res: i32 = 0 as i32;
// CHECK-NEXT:     if f != (0 as i32) {
// CHECK-NEXT:         p = unsafe { std::ptr::addr_of_mut!((* (k as * mut foo)).i) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         p = unsafe { std::ptr::addr_of_mut!((* (k as * mut foo)).j) };
// CHECK-NEXT:     }
// CHECK-NEXT:     res = unsafe { *p };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(k as *mut foo)).i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if f2 != (0 as i32) {
// CHECK-NEXT:         q = p;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         q = std::ptr::addr_of_mut!(k2);
// CHECK-NEXT:     }
// CHECK-NEXT:     res + unsafe { *q }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut k: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     k.i = 0 as i32;
// CHECK-NEXT:     k.j = 1 as i32;
// CHECK-NEXT:     if bar(unsafe { &mut (*std::ptr::addr_of_mut!(k)) }, 1 as i32, 1 as i32, 1 as i32)
// CHECK-NEXT:         != (1 as i32)
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
