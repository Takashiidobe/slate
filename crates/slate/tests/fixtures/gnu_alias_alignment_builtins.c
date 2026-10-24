#include <stdio.h>

int        counter = 3;
extern int counter_alias __attribute__((alias("counter")));
static int hidden[4] = {1, 2, 3, 4};
extern int hidden_alias[4] __attribute__((alias("hidden")));

void aligned_function(void) __attribute__((aligned(64)));
void aligned_function(void) {}
void plain_function(void) {}

long long llabs(long long value) {
  return value + 1000;
}

int main(void) {
  counter_alias += 4;
  hidden_alias[2] = 30;
  printf("%d %d\n", counter, hidden[2]);
  printf("%d %d\n", (int)__alignof__(aligned_function), (int)__alignof__(plain_function));
  int  values[] = {0, -1, 1, 255, -256, 0x40000000};
  long wide[]   = {0L, -1L, (long)(1LL << 40), (long)-(1LL << 40)};
  for (int i = 0; i < 6; i++)
    printf("%d %d\n", __builtin_clrsb(values[i]), __builtin_parity(values[i]));
  for (int i = 0; i < 4; i++)
    printf("%d %d %d\n", __builtin_clrsbl(wide[i]), __builtin_clrsbll(wide[i]),
           __builtin_parityll(wide[i]));
  long long negative = -42;
  printf("%lld %lld\n", llabs(negative), __builtin_llabs(negative));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "llabs"]
// CHECK-NEXT:     fn __builtin_llabs([[V_0:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut counter: i32 = 3;
// CHECK-NEXT: static mut hidden: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT: fn aligned_function() {}
// CHECK-NEXT: fn plain_function() {}
// CHECK-NEXT: fn llabs(value: i64) -> i64 {
// CHECK-NEXT:     value + 1000
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = counter + 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         hidden.0[2] = 30;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", unsafe { counter }, unsafe { hidden.0[2] });
// CHECK-NEXT:     println!("{} {}", 64i32, 4i32);
// CHECK-NEXT:     let values: __SlateAlign16<[i32; 6]> = __SlateAlign16([
// CHECK-NEXT:         0i32,
// CHECK-NEXT:         -1i32,
// CHECK-NEXT:         1i32,
// CHECK-NEXT:         255i32,
// CHECK-NEXT:         -256i32,
// CHECK-NEXT:         1073741824i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let wide: __SlateAlign16<[i64; 4]> = __SlateAlign16([
// CHECK-NEXT:         0i64,
// CHECK-NEXT:         -1i64,
// CHECK-NEXT:         1099511627776i64,
// CHECK-NEXT:         -1099511627776i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < 6 {
// CHECK-NEXT:             println!(
// CHECK-NEXT:                 "{} {}", { let [[T_0:__t[0-9]+]] = values.0[i as usize]; ((if [[T_0]] < 0 { (! [[T_0]])
// CHECK-NEXT:                 .leading_zeros() } else { [[T_0]].leading_zeros() }) - 1) as i32 }, { let
// CHECK-NEXT:                 [[T_1:__t[0-9]+]] = values.0[i as usize] as u32; ([[T_1]].count_ones() & 1) as i32 }
// CHECK-NEXT:             );
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}", { let [[T_2:__t[0-9]+]] = wide.0[i as usize]; ((if [[T_2]] < 0 { (! [[T_2]])
// CHECK-NEXT:             .leading_zeros() } else { [[T_2]].leading_zeros() }) - 1) as i32 }, { let [[T_3:__t[0-9]+]] =
// CHECK-NEXT:             wide.0[i as usize]; ((if [[T_3]] < 0 { (! [[T_3]]).leading_zeros() } else { [[T_3]]
// CHECK-NEXT:             .leading_zeros() }) - 1) as i32 }, { let [[T_4:__t[0-9]+]] = wide.0[i as usize] as u64;
// CHECK-NEXT:             ([[T_4]].count_ones() & 1) as i32 }
// CHECK-NEXT:         );
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let negative: i64 = -42;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%lld %lld\n".as_ptr(), negative.wrapping_abs(), negative.wrapping_abs())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
