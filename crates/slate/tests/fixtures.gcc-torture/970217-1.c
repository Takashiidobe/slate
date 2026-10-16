void exit(int);

int sub(int i, int array[i++]) { return i; }

int main(void) {
  int array[10];
  exit(sub(10, array) != 11);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sub(mut i: i32, mut array: *mut i32) -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     i = [[V_2]];
// CHECK-NEXT:     i
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut array: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         exit((sub(10 as i32, array.0.as_mut_ptr() as *mut i32) != (11 as i32)) as i32)
// CHECK-NEXT:     };
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
