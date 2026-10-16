void abort(void);
void exit(int);

int a3[3];

void f(int *a) {
  int i;

  for (i = 3; --i;)
    a[i] = 42 / i;
}

int main() {
  f(a3);

  if (a3[1] != 42 || a3[2] != 21)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a3: [i32; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f(mut a: *mut i32) {
// CHECK-NEXT:     let mut i: i32 = 3 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:         if !([[V_2]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { a.offset(i as isize) } = (42 as i32) / i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f(unsafe { std::ptr::addr_of_mut!(a3) as *mut i32 });
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a3) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (42 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a3) as *mut i32 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (21 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
