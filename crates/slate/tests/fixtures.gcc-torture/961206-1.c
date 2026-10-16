void abort(void);
void exit(int);

int sub1(unsigned long long i) {
  if (i < 0x80000000)
    return 1;
  else
    return 0;
}

int sub2(unsigned long long i) {
  if (i <= 0x7FFFFFFF)
    return 1;
  else
    return 0;
}

int sub3(unsigned long long i) {
  if (i >= 0x80000000)
    return 0;
  else
    return 1;
}

int sub4(unsigned long long i) {
  if (i > 0x7FFFFFFF)
    return 0;
  else
    return 1;
}

int main(void) {
  if (sub1(0x80000000ULL))
    abort();

  if (sub2(0x80000000ULL))
    abort();

  if (sub3(0x80000000ULL))
    abort();

  if (sub4(0x80000000ULL))
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
// CHECK-NEXT: fn sub1(mut i: u64) -> i32 {
// CHECK-NEXT:     if i < ((2147483648 as u32) as u64) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn sub2(mut i: u64) -> i32 {
// CHECK-NEXT:     if i <= (((2147483647 as i32) as i64) as u64) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn sub3(mut i: u64) -> i32 {
// CHECK-NEXT:     if i >= ((2147483648 as u32) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn sub4(mut i: u64) -> i32 {
// CHECK-NEXT:     if i > (((2147483647 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if sub1(2147483648 as u64) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if sub2(2147483648 as u64) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if sub3(2147483648 as u64) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if sub4(2147483648 as u64) != (0 as i32) {
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
