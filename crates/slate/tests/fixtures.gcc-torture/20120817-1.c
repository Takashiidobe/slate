void abort(void);
void exit(int);

typedef unsigned long long u64;
unsigned long              foo = 0;
u64                        f() __attribute__((noinline));

u64 f() { return ((u64)40) + ((u64)24) * (int)(foo - 1); }

int main() {
  if (f() != 16)
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
// CHECK-NEXT: static mut foo: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT: fn f() -> u64 {
// CHECK-NEXT:     (((40 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             (((24 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_mul(
// CHECK-NEXT:                     (((unsafe { foo }.wrapping_sub(((1 as i32) as i64) as u64) as u32)
// CHECK-NEXT:                         as i32) as i64) as u64,
// CHECK-NEXT:                 ),
// CHECK-NEXT:         )
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f() != (((16 as i32) as i64) as u64) {
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
