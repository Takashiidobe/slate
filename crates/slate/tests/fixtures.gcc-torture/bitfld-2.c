/* Test whether bit field boundaries aren't advanced if bit field type
   has alignment large enough.  */
extern void abort(void);
extern void exit(int);

struct A {
  unsigned short a : 5;
  unsigned short b : 5;
  unsigned short c : 6;
};

struct B {
  unsigned short a : 5;
  unsigned short b : 3;
  unsigned short c : 8;
};

int main() {
  /* If short is not at least 16 bits wide, don't test anything.  */
  if ((unsigned short)65521 != 65521)
    exit(0);

  if (sizeof(struct A) != sizeof(struct B))
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Test whether bit field boundaries aren't advanced if bit field type
// CHECK-NEXT: // has alignment large enough.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // If short is not at least 16 bits wide, don't test anything.
// CHECK-NEXT:     if (((((65521 as i32) as i16) as u16) as u32) as i32) != (65521 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (2 as u64) != (2 as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
