void abort(void);
void exit(int);

long foo(long a, long b, long c) {
  if (a != 12 || b != 1 || c != 11)
    abort();
  return 0;
}
long bar(long a, long b) { return b; }
void baz(long a, long b, void *c) {
  long d;
  d = (long)c;
  foo(d, bar(a, 1), b);
}
int main() {
  baz(10, 11, (void *)12);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: i64, mut b: i64, mut c: i64) -> i64 {
// CHECK-NEXT:     if a != ((12 as i32) as i64) || b != ((1 as i32) as i64) || c != ((11 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     (0 as i32) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut a: i64, mut b: i64) -> i64 {
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-NEXT: fn baz(mut a: i64, mut b: i64, mut c: *mut ()) {
// CHECK-NEXT:     let mut d: i64 = c as i64;
// CHECK-NEXT:     foo(d, bar(a, (1 as i32) as i64), b);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     baz((10 as i32) as i64, (11 as i32) as i64, (12 as i32) as *mut ());
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
