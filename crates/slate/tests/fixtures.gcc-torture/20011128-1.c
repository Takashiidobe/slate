void exit(int);

int main(void) {
  char blah[33] = "01234567890123456789";
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     __SlateAlign16([
// CHECK-NEXT:         48 as i8,
// CHECK-NEXT:         49 as i8,
// CHECK-NEXT:         50 as i8,
// CHECK-NEXT:         51 as i8,
// CHECK-NEXT:         52 as i8,
// CHECK-NEXT:         53 as i8,
// CHECK-NEXT:         54 as i8,
// CHECK-NEXT:         55 as i8,
// CHECK-NEXT:         56 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         48 as i8,
// CHECK-NEXT:         49 as i8,
// CHECK-NEXT:         50 as i8,
// CHECK-NEXT:         51 as i8,
// CHECK-NEXT:         52 as i8,
// CHECK-NEXT:         53 as i8,
// CHECK-NEXT:         54 as i8,
// CHECK-NEXT:         55 as i8,
// CHECK-NEXT:         56 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ]);
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
