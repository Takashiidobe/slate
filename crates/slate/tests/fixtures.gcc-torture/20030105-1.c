void abort(void);
void exit(int);

int __attribute__((noinline)) foo() {
  const int a[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  int       i, sum;

  sum = 0;
  for (i = 0; i < sizeof(a) / sizeof(*a); i++)
    sum += a[i];

  return sum;
}

int main() {
  if (foo() != 28)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 8]> = __SlateAlign16([
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (32 as u64) / (4 as u64) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]]
// CHECK-NEXT:             + unsafe { *unsafe { (a.0.as_ptr() as *const i32).offset(i as isize) } };
// CHECK-NEXT:         sum = [[V_2]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (28 as i32) {
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
