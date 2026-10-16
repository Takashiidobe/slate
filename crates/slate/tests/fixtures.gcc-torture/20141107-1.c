#define bool _Bool

bool f(int a, bool c) __attribute__((noinline));
bool f(int a, bool c) {
  if (!a)
    c = !c;
  return c;
}

void checkf(int a, bool b) {
  bool c = f(a, b);
  char d;
  __builtin_memcpy(&d, &c, 1);
  if (d != (a == 0) ^ b)
    __builtin_abort();
}

int main(void) {
  checkf(0, 0);
  checkf(0, 1);
  checkf(1, 1);
  checkf(1, 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut a: i32, mut c: bool) -> bool {
// CHECK-NEXT:     if !(a != (0 as i32)) {
// CHECK-NEXT:         c = !c;
// CHECK-NEXT:     }
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn checkf(mut a: i32, mut b: bool) {
// CHECK-NEXT:     let mut c: bool = f(a, b);
// CHECK-NEXT:     let mut d: i8 = 0 as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(d) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(c) as *const (),
// CHECK-NEXT:             ((1 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (((d as i32) != ((a == (0 as i32)) as i32)) as i32) ^ (b as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     checkf(0 as i32, (0 as i32) != (0 as i32));
// CHECK-NEXT:     checkf(0 as i32, (1 as i32) != (0 as i32));
// CHECK-NEXT:     checkf(1 as i32, (1 as i32) != (0 as i32));
// CHECK-NEXT:     checkf(1 as i32, (0 as i32) != (0 as i32));
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
