void abort(void);

int movegt(int x, int y, long long a) {
  int i;
  int ret = 0;
  for (i = 0; i < y; i++) {
    if (a >= (long long)0xf000000000000000LL)
      ret = x;
    else
      ret = y;
  }
  return ret;
}

struct test {
  long long val;
  int       ret;
} tests[] = {
    {0xf000000000000000LL, -1}, {0xefffffffffffffffLL, 1},
    {0xf000000000000001LL, -1}, {0x0000000000000000LL, -1},
    {0x8000000000000000LL, 1},
};

int main() {
  int i;
  for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    if (movegt(-1, 1, tests[i].val) != tests[i].ret)
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn movegt(mut x: i32, mut y: i32, mut a: i64) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut ret: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < y {
// CHECK-NEXT:         if a >= ((17293822569102704640 as u64) as i64) {
// CHECK-NEXT:             ret = x;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             ret = y;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     ret
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct test {
// CHECK-NEXT:     val: i64,
// CHECK-NEXT:     ret: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut tests: __SlateAlign16<[test; 5]> = __SlateAlign16([
// CHECK-NEXT:     test {
// CHECK-NEXT:         val: (17293822569102704640 as u64) as i64,
// CHECK-NEXT:         ret: -(1 as i32),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         val: (17293822569102704639 as u64) as i64,
// CHECK-NEXT:         ret: 1 as i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         val: (17293822569102704641 as u64) as i64,
// CHECK-NEXT:         ret: -(1 as i32),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         val: 0 as i64,
// CHECK-NEXT:         ret: -(1 as i32),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         val: (9223372036854775808 as u64) as i64,
// CHECK-NEXT:         ret: 1 as i32,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (80 as u64) / (16 as u64) {
// CHECK-NEXT:         if movegt(
// CHECK-NEXT:             -(1 as i32),
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(tests.0) as *mut test }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .val
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(tests.0) as *mut test }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .ret
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
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
