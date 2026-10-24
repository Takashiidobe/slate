int main(void) {
  int values[4];
  int *p = &values[1];
  int *q = &values[3];
  return (int)(q - p);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// CHECK-NEXT:     let p: *mut i32 = unsafe { (values.0.as_mut_ptr() as *mut i32).add(1) };
// CHECK-NEXT:     let q: *mut i32 = unsafe { (values.0.as_mut_ptr() as *mut i32).add(3) };
// CHECK-NEXT:     ((unsafe { q.offset_from(p as *mut i32) }) as i64) as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
