
void exit(int);
void abort(void);
int  a[1];
void (*terminate_me)(int);

__attribute__((noinline, noclone)) void t(int c) {
  int i;
  for (i = 0; i < c; i++) {
    if (i)
      terminate_me(0);
    a[i] = 0;
  }
}
int main(void) {
  terminate_me = exit;
  t(100);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [i32; 1] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut terminate_me: Option<unsafe extern "C-unwind" fn(i32)> = unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: };
// CHECK-NEXT: fn t(mut c: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < c {
// CHECK-NEXT:         if i != (0 as i32) {
// CHECK-NEXT:             unsafe { unsafe { terminate_me }.unwrap()(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:             } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         terminate_me = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32)>,
// CHECK-NEXT:             >(exit as *const ())
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     t(100 as i32);
// CHECK-NEXT:     unsafe { abort() };
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
