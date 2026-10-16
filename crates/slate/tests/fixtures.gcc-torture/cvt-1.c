void abort(void);
void exit(int);

static inline long g1(double x) { return (double)(long)x; }

long g2(double f) { return f; }

double f(long i) {
  if (g1(i) != g2(i))
    abort();
  return g2(i);
}

int main(void) {
  if (f(123456789L) != 123456789L)
    abort();
  if (f(123456789L) != g2(123456789L))
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
// CHECK-NEXT: fn g1(mut x: f64) -> i64 {
// CHECK-NEXT:     ((x as i64) as f64) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn g2(mut f: f64) -> i64 {
// CHECK-NEXT:     f as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut i: i64) -> f64 {
// CHECK-NEXT:     if g1(i as f64) != g2(i as f64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     g2(i as f64) as f64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(123456789 as i64) != ((123456789 as i64) as f64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(123456789 as i64) != (g2((123456789 as i64) as f64) as f64) {
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
