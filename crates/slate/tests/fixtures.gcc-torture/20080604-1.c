struct barstruct {
  char const *some_string;
} x;
extern void                    abort(void);
void __attribute__((noinline)) foo(void) {
  if (!x.some_string)
    abort();
}
void baz(int b) {
  struct barstruct  bar;
  struct barstruct *barptr;
  if (b)
    barptr = &bar;
  else {
    barptr = &x + 1;
    barptr = barptr - 1;
  }
  barptr->some_string = "Everything OK";
  foo();
  barptr->some_string = "Everything OK";
}
int main() {
  x.some_string = (void *)0;
  baz(0);
  if (!x.some_string)
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
// CHECK-NEXT: struct barstruct {
// CHECK-NEXT:     some_string: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: barstruct = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     if !((unsafe { x.some_string }) != std::ptr::null::<i8>()) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn baz(mut b: i32) {
// CHECK-NEXT:     let mut bar: barstruct = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut barptr: *mut barstruct = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         barptr = std::ptr::addr_of_mut!(bar);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         barptr = unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(x) }.offset((1 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         barptr = unsafe { barptr.offset(-((1 as i32) as isize)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*barptr).some_string = c"Everything OK".as_ptr() as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*barptr).some_string = c"Everything OK".as_ptr() as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.some_string = std::ptr::null::<i8>();
// CHECK-NEXT:     }
// CHECK-NEXT:     baz(0 as i32);
// CHECK-NEXT:     if !((unsafe { x.some_string }) != std::ptr::null::<i8>()) {
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
