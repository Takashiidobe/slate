void abort(void);
void exit(int);

long val = 0x5e000000;

long f1(void) { return 0x132; }

long f2(void) { return 0x5e000000; }

void f3(long b) { val = b; }

void f4() {
  long v = f1();
  long o = f2();
  v      = (v & 0x00ffffff) | (o & 0xff000000);
  f3(v);
}

int main(void) {
  f4();
  if (val != 0x5e000132)
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
// CHECK-NEXT: static mut val: i64 = (1577058304 as i32) as i64;
// CHECK-NEXT: fn f1() -> i64 {
// CHECK-NEXT:     (306 as i32) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn f2() -> i64 {
// CHECK-NEXT:     (1577058304 as i32) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn f3(mut b: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         val = b;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f4() {
// CHECK-NEXT:     let mut v: i64 = f1();
// CHECK-NEXT:     let mut o: i64 = f2();
// CHECK-NEXT:     v = v & ((16777215 as i32) as i64) | o & (((4278190080 as u32) as u64) as i64);
// CHECK-NEXT:     f3(v);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f4();
// CHECK-NEXT:     if (unsafe { val }) != ((1577058610 as i32) as i64) {
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
