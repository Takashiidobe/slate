extern void abort();

void __attribute__((noinline)) bar(unsigned long long i) {
  if (i)
    abort();
}

static void __attribute__((always_inline)) foo(unsigned long long *r) {
  int i;

  for (i = 0;; i++)
    if (*r & ((unsigned long long)1 << (63 - i)))
      break;

  bar(i);
}

void __attribute__((noinline)) do_test(unsigned long long *r) {
  int i;

  for (i = 0; i < 2; ++i)
    foo(r);
}

int main() {
  unsigned long long r = 0x8000000000000001ull;

  do_test(&r);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut i: u64) {
// CHECK-NEXT:     if i != (0 as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut r: &u64) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         if (unsafe { *(r as *const u64) })
// CHECK-NEXT:             & (((1 as i32) as i64) as u64) << (63 as i32) - i != (0 as u64)
// CHECK-NEXT:         {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar((i as i64) as u64);
// CHECK-NEXT: }
// CHECK-NEXT: fn do_test(mut r: &u64) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         foo(r);
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: u64 = 9223372036854775809 as u64;
// CHECK-NEXT:     do_test(unsafe { &(*std::ptr::addr_of_mut!(r)) });
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
