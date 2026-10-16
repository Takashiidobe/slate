#if __SIZEOF_INT__ < 4
__extension__ typedef __UINT32_TYPE__ uint32_t;
__extension__ typedef __INT32_TYPE__  int32_t;
#else
typedef unsigned uint32_t;
typedef int      int32_t;
#endif

extern void abort(void);

__attribute__((noinline, noclone)) unsigned long long f1(int32_t x) {
  return ((unsigned long long)x) << 4;
}

__attribute__((noinline, noclone)) long long f2(uint32_t x) {
  return ((long long)x) << 4;
}

__attribute__((noinline, noclone)) unsigned long long f3(uint32_t x) {
  return ((unsigned long long)x) << 4;
}

__attribute__((noinline, noclone)) long long f4(int32_t x) {
  return ((long long)x) << 4;
}

int main() {
  if (f1(0xf0000000) != 0xffffffff00000000)
    abort();
  if (f2(0xf0000000) != 0xf00000000)
    abort();
  if (f3(0xf0000000) != 0xf00000000)
    abort();
  if (f4(0xf0000000) != 0xffffffff00000000)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f1(mut x: i32) -> u64 {
// CHECK-NEXT:     ((x as i64) as u64) << (4 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn f2(mut x: u32) -> i64 {
// CHECK-NEXT:     ((x as u64) as i64) << (4 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn f3(mut x: u32) -> u64 {
// CHECK-NEXT:     (x as u64) << (4 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn f4(mut x: i32) -> i64 {
// CHECK-NEXT:     (x as i64) << (4 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f1((4026531840 as u32) as i32) != (18446744069414584320 as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f2(4026531840 as u32) != (64424509440 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f3(4026531840 as u32) != ((64424509440 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (f4((4026531840 as u32) as i32) as u64) != (18446744069414584320 as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
