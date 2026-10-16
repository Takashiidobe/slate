void big(long long u) {}

void doit(unsigned int a, unsigned int b, char *id) {
  big(*id);
  big(a);
  big(b);
}

int main(void) {
  doit(1, 1, "\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn big(mut u: i64) {}
// CHECK-NEXT: fn doit(mut a: u32, mut b: u32, mut id: *mut i8) {
// CHECK-NEXT:     big((unsafe { *id }) as i64);
// CHECK-NEXT:     big((a as u64) as i64);
// CHECK-NEXT:     big((b as u64) as i64);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     doit((1 as i32) as u32, (1 as i32) as u32, c"\n".as_ptr() as *mut i8);
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
