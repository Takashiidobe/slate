struct A {
  char x[1];
};
extern void                             abort(void);
void __attribute__((noinline, noclone)) foo(struct A a) {
  if (a.x[0] != 'a')
    abort();
}
int main() {
  struct A a;
  int      i;
  for (i = 0; i < 1; ++i)
    a.x[i] = 'a';
  foo(a);
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
// CHECK-NEXT: struct A {
// CHECK-NEXT:     x: [i8; 1],
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: A) {
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe { (a.x.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as i32) != (97 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a.x.as_mut_ptr() as *mut i8).offset(i as isize) } = (97 as i32)
// CHECK-NEXT:                 as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(a);
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
