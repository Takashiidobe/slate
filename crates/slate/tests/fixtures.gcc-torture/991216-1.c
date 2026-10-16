void abort(void);
void exit(int);

#define VALUE 0x123456789abcdefLL
#define AFTER 0x55

void test1(int a, long long value, int after) {
  if (a != 1 || value != VALUE || after != AFTER)
    abort();
}

void test2(int a, int b, long long value, int after) {
  if (a != 1 || b != 2 || value != VALUE || after != AFTER)
    abort();
}

void test3(int a, int b, int c, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || value != VALUE || after != AFTER)
    abort();
}

void test4(int a, int b, int c, int d, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || value != VALUE || after != AFTER)
    abort();
}

void test5(int a, int b, int c, int d, int e, long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || value != VALUE ||
      after != AFTER)
    abort();
}

void test6(int a, int b, int c, int d, int e, int f, long long value,
           int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 ||
      value != VALUE || after != AFTER)
    abort();
}

void test7(int a, int b, int c, int d, int e, int f, int g, long long value,
           int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 || g != 7 ||
      value != VALUE || after != AFTER)
    abort();
}

void test8(int a, int b, int c, int d, int e, int f, int g, int h,
           long long value, int after) {
  if (a != 1 || b != 2 || c != 3 || d != 4 || e != 5 || f != 6 || g != 7 ||
      h != 8 || value != VALUE || after != AFTER)
    abort();
}

int main() {
  test1(1, VALUE, AFTER);
  test2(1, 2, VALUE, AFTER);
  test3(1, 2, 3, VALUE, AFTER);
  test4(1, 2, 3, 4, VALUE, AFTER);
  test5(1, 2, 3, 4, 5, VALUE, AFTER);
  test6(1, 2, 3, 4, 5, 6, VALUE, AFTER);
  test7(1, 2, 3, 4, 5, 6, 7, VALUE, AFTER);
  test8(1, 2, 3, 4, 5, 6, 7, 8, VALUE, AFTER);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn test1(mut a: i32, mut value: i64, mut after: i32) {
// CHECK-NEXT:     if a != (1 as i32) || value != (81985529216486895 as i64) || after != (85 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut a: i32, mut b: i32, mut value: i64, mut after: i32) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || value != (81985529216486895 as i64)
// CHECK-NEXT:         || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test3(mut a: i32, mut b: i32, mut c: i32, mut value: i64, mut after: i32) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || c != (3 as i32)
// CHECK-NEXT:         || value != (81985529216486895 as i64) || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test4(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut value: i64,
// CHECK-NEXT:     mut after: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || c != (3 as i32) || d != (4 as i32)
// CHECK-NEXT:         || value != (81985529216486895 as i64) || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test5(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut e: i32,
// CHECK-NEXT:     mut value: i64,
// CHECK-NEXT:     mut after: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || c != (3 as i32) || d != (4 as i32)
// CHECK-NEXT:         || e != (5 as i32) || value != (81985529216486895 as i64) || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test6(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut e: i32,
// CHECK-NEXT:     mut f: i32,
// CHECK-NEXT:     mut value: i64,
// CHECK-NEXT:     mut after: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || c != (3 as i32) || d != (4 as i32)
// CHECK-NEXT:         || e != (5 as i32) || f != (6 as i32) || value != (81985529216486895 as i64)
// CHECK-NEXT:         || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test7(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut e: i32,
// CHECK-NEXT:     mut f: i32,
// CHECK-NEXT:     mut g: i32,
// CHECK-NEXT:     mut value: i64,
// CHECK-NEXT:     mut after: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || c != (3 as i32) || d != (4 as i32)
// CHECK-NEXT:         || e != (5 as i32) || f != (6 as i32) || g != (7 as i32)
// CHECK-NEXT:         || value != (81985529216486895 as i64) || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test8(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut e: i32,
// CHECK-NEXT:     mut f: i32,
// CHECK-NEXT:     mut g: i32,
// CHECK-NEXT:     mut h: i32,
// CHECK-NEXT:     mut value: i64,
// CHECK-NEXT:     mut after: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if a != (1 as i32) || b != (2 as i32) || c != (3 as i32) || d != (4 as i32)
// CHECK-NEXT:         || e != (5 as i32) || f != (6 as i32) || g != (7 as i32) || h != (8 as i32)
// CHECK-NEXT:         || value != (81985529216486895 as i64) || after != (85 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1(1 as i32, 81985529216486895 as i64, 85 as i32);
// CHECK-NEXT:     test2(1 as i32, 2 as i32, 81985529216486895 as i64, 85 as i32);
// CHECK-NEXT:     test3(1 as i32, 2 as i32, 3 as i32, 81985529216486895 as i64, 85 as i32);
// CHECK-NEXT:     test4(1 as i32, 2 as i32, 3 as i32, 4 as i32, 81985529216486895 as i64, 85 as i32);
// CHECK-NEXT:     test5(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         81985529216486895 as i64,
// CHECK-NEXT:         85 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test6(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         81985529216486895 as i64,
// CHECK-NEXT:         85 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test7(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         81985529216486895 as i64,
// CHECK-NEXT:         85 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     test8(
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:         81985529216486895 as i64,
// CHECK-NEXT:         85 as i32,
// CHECK-NEXT:     );
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
