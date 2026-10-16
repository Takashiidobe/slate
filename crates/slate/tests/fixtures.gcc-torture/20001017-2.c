void abort(void);

void fn_4parms(unsigned char a, long *b, long *c, unsigned int *d) {
  if (*b != 1 || *c != 2 || *d != 3)
    abort();
}

int main() {
  unsigned char a = 0;
  unsigned long b = 1, c = 2;
  unsigned int  d = 3;

  fn_4parms(a, &b, &c, &d);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn fn_4parms(mut a: u8, mut b: &i64, mut c: &i64, mut d: &u32) {
// CHECK-NEXT:     if (unsafe { *(b as *const i64) }) != ((1 as i32) as i64)
// CHECK-NEXT:         || (unsafe { *(c as *const i64) }) != ((2 as i32) as i64)
// CHECK-NEXT:         || (unsafe { *(d as *const u32) }) != ((3 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u8 = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     let mut b: u64 = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     let mut c: u64 = ((2 as i32) as i64) as u64;
// CHECK-NEXT:     let mut d: u32 = (3 as i32) as u32;
// CHECK-NEXT:     fn_4parms(
// CHECK-NEXT:         a,
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(b) as *mut i64)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(c) as *mut i64)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(d)) },
// CHECK-NEXT:     );
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
