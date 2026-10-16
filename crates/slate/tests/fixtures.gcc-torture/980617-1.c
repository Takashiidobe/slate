void abort(void);
void exit(int);

void foo(unsigned int *p) {
  if ((signed char)(*p & 0xFF) == 17 || (signed char)(*p & 0xFF) == 18)
    return;
  else
    abort();
}

int main() {
  int i = 0x30011;
  foo(&i);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: &u32) {
// CHECK-NEXT:     if (((((unsafe { *(p as *const u32) }) & ((255 as i32) as u32)) as u8) as i8) as i32)
// CHECK-NEXT:         == (17 as i32)
// CHECK-NEXT:         || (((((unsafe { *(p as *const u32) }) & ((255 as i32) as u32)) as u8) as i8)
// CHECK-NEXT:             as i32) == (18 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 196625 as i32;
// CHECK-NEXT:     foo(unsafe { &(*(std::ptr::addr_of_mut!(i) as *mut u32)) });
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
