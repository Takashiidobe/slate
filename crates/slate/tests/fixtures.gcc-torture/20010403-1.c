void abort(void);
void exit(int);

void b(int *);
void c(int, int);
void d(int);

int e;

void a(int x, int y) {
  int f = x ? e : 0;
  int z = y;

  b(&y);
  c(z, y);
  d(f);
}

void b(int *y) { (*y)++; }

void c(int x, int y) {
  if (x == y)
    abort();
}

void d(int x) {}

int main(void) {
  a(0, 0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn a(mut x: i32, mut y: i32) {
// CHECK-NEXT:     let mut f: i32 = if x != (0 as i32) { unsafe { e } } else { 0 as i32 };
// CHECK-NEXT:     let mut z: i32 = y;
// CHECK-NEXT:     b(unsafe { &mut (*std::ptr::addr_of_mut!(y)) });
// CHECK-NEXT:     c(z, y);
// CHECK-NEXT:     d(f);
// CHECK-NEXT: }
// CHECK-NEXT: fn b(mut y: &mut i32) {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *y };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn c(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x == y {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn d(mut x: i32) {}
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     a(0 as i32, 0 as i32);
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
