extern void                            abort(void);
static char *__attribute__((noinline)) itos(int num) { return (char *)0; }
static void __attribute__((noinline))  foo(int i, const char *x) {
  if (i >= 4)
    abort();
}
int main() {
  int x = -__INT_MAX__ + 3;
  int i;

  for (i = 0; i < 4; ++i) {
    char *p;
    --x;
    p = itos(x);
    foo(i, p);
  }

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn itos(mut num: i32) -> *mut i8 {
// CHECK-NEXT:     std::ptr::null_mut::<i8>()
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut i: i32, mut x: *const i8) {
// CHECK-NEXT:     if i >= (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = -(2147483647 as i32) + (3 as i32);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = x;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         x = [[V_1]];
// CHECK-NEXT:         p = itos(x);
// CHECK-NEXT:         foo(i, p as *const i8);
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
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
