extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);
struct S {
  int i[16];
};
struct S                               *p;
void __attribute__((noinline, noclone)) foo(struct S *a, struct S *b) {
  a->i[0] = -1;
  p       = b;
}
void test(void) {
  struct S a, b;
  memset(&a.i[0], '\0', sizeof(a.i));
  memset(&b.i[0], '\0', sizeof(b.i));
  foo(&a, &b);
  *p = a;
  *p = b;
  if (b.i[0] != -1)
    abort();
}
int main() {
  test();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memset(s: *mut (), c: i32, n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     i: [i32; 16],
// CHECK-NEXT: }
// CHECK-NEXT: static mut p: *mut S = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut a: &mut S, mut b: *mut S) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(a as *mut S)).i.as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = b;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test() {
// CHECK-NEXT:     let mut a: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { (a.i.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { (b.i.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(a)) }, std::ptr::addr_of_mut!(b));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p } = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (b.i.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) != -(1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test();
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
