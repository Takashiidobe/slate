void abort(void);
void exit(int);

unsigned int f1(int diff) { return ((unsigned int)(diff < 0 ? -diff : diff)); }

unsigned int f2(unsigned int diff) {
  return ((unsigned int)((signed int)diff < 0 ? -diff : diff));
}

unsigned long long f3(long long diff) {
  return ((unsigned long long)(diff < 0 ? -diff : diff));
}

unsigned long long f4(unsigned long long diff) {
  return ((unsigned long long)((signed long long)diff < 0 ? -diff : diff));
}

int main(void) {
  int i;
  for (i = 0; i <= 10; i++) {
    if (f1(i) != i)
      abort();
    if (f1(-i) != i)
      abort();
    if (f2(i) != i)
      abort();
    if (f2(-i) != i)
      abort();
    if (f3((long long)i) != i)
      abort();
    if (f3((long long)-i) != i)
      abort();
    if (f4((long long)i) != i)
      abort();
    if (f4((long long)-i) != i)
      abort();
  }
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f1(mut diff: i32) -> u32 {
// CHECK-NEXT:     (if diff < (0 as i32) { -diff } else { diff }) as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn f2(mut diff: u32) -> u32 {
// CHECK-NEXT:     if (diff as i32) < (0 as i32) { diff.wrapping_neg() } else { diff }
// CHECK-NEXT: }
// CHECK-NEXT: fn f3(mut diff: i64) -> u64 {
// CHECK-NEXT:     (if diff < ((0 as i32) as i64) { -diff } else { diff }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn f4(mut diff: u64) -> u64 {
// CHECK-NEXT:     if (diff as i64) < ((0 as i32) as i64) { diff.wrapping_neg() } else { diff }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (10 as i32) {
// CHECK-NEXT:         if f1(i) != (i as u32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f1(-i) != (i as u32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f2(i as u32) != (i as u32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f2(-i as u32) != (i as u32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f3(i as i64) != ((i as i64) as u64) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f3(-i as i64) != ((i as i64) as u64) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f4((i as i64) as u64) != ((i as i64) as u64) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if f4((-i as i64) as u64) != ((i as i64) as u64) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
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
