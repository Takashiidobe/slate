void abort(void);
void exit(int);

char a[10] = "deadbeef";

char acc_a(long i) { return a[i - 2000000000L]; }

int main(void) {
  if (acc_a(2000000000L) != 'd')
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
// CHECK-NEXT: static mut a: [i8; 10] = [
// CHECK-NEXT:     100 as i8,
// CHECK-NEXT:     101 as i8,
// CHECK-NEXT:     97 as i8,
// CHECK-NEXT:     100 as i8,
// CHECK-NEXT:     98 as i8,
// CHECK-NEXT:     101 as i8,
// CHECK-NEXT:     101 as i8,
// CHECK-NEXT:     102 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT: ];
// CHECK-NEXT: fn acc_a(mut i: i64) -> i8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i8 }
// CHECK-NEXT:                 .offset((i - (2000000000 as i64)) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (acc_a(2000000000 as i64) as i32) != (100 as i32) {
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
