extern void abort(void);

__attribute__((noinline)) void foo(void *p) {
  long l = (long)p;
  if (l < 0 || l > 6)
    abort();
}

int main() {
  short i;
  for (i = 6; i >= 0; i--)
    foo((void *)(long)i);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: *mut ()) {
// CHECK-NEXT:     let mut l: i64 = p as i64;
// CHECK-NEXT:     if l < ((0 as i32) as i64) || l > ((6 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i16 = (6 as i32) as i16;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (i as i32) >= (0 as i32) {
// CHECK-NEXT:         foo((i as i64) as *mut ());
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) - (1 as i32)) as i16;
// CHECK-NEXT:         i = [[V_1]];
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
