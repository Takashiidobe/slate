#include <limits.h>

void abort(void);
void exit(int);

int __attribute__((noinline)) nge(int a, int b) { return -(a >= b); }
int __attribute__((noinline)) ngt(int a, int b) { return -(a > b); }
int __attribute__((noinline)) nle(int a, int b) { return -(a <= b); }
int __attribute__((noinline)) nlt(int a, int b) { return -(a < b); }
int __attribute__((noinline)) neq(int a, int b) { return -(a == b); }
int __attribute__((noinline)) nne(int a, int b) { return -(a != b); }
int __attribute__((noinline)) ngeu(unsigned a, unsigned b) { return -(a >= b); }
int __attribute__((noinline)) ngtu(unsigned a, unsigned b) { return -(a > b); }
int __attribute__((noinline)) nleu(unsigned a, unsigned b) { return -(a <= b); }
int __attribute__((noinline)) nltu(unsigned a, unsigned b) { return -(a < b); }

int main() {
  if (nge(INT_MIN, INT_MAX) != 0)
    abort();
  if (nge(INT_MAX, INT_MIN) != -1)
    abort();
  if (ngt(INT_MIN, INT_MAX) != 0)
    abort();
  if (ngt(INT_MAX, INT_MIN) != -1)
    abort();
  if (nle(INT_MIN, INT_MAX) != -1)
    abort();
  if (nle(INT_MAX, INT_MIN) != 0)
    abort();
  if (nlt(INT_MIN, INT_MAX) != -1)
    abort();
  if (nlt(INT_MAX, INT_MIN) != 0)
    abort();

  if (neq(INT_MIN, INT_MAX) != 0)
    abort();
  if (neq(INT_MAX, INT_MIN) != 0)
    abort();
  if (nne(INT_MIN, INT_MAX) != -1)
    abort();
  if (nne(INT_MAX, INT_MIN) != -1)
    abort();

  if (ngeu(0, ~0U) != 0)
    abort();
  if (ngeu(~0U, 0) != -1)
    abort();
  if (ngtu(0, ~0U) != 0)
    abort();
  if (ngtu(~0U, 0) != -1)
    abort();
  if (nleu(0, ~0U) != -1)
    abort();
  if (nleu(~0U, 0) != 0)
    abort();
  if (nltu(0, ~0U) != -1)
    abort();
  if (nltu(~0U, 0) != 0)
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
// CHECK-NEXT: fn nge(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     -((a >= b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ngt(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     -((a > b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn nle(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     -((a <= b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn nlt(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     -((a < b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn neq(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     -((a == b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn nne(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     -((a != b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ngeu(mut a: u32, mut b: u32) -> i32 {
// CHECK-NEXT:     -((a >= b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn ngtu(mut a: u32, mut b: u32) -> i32 {
// CHECK-NEXT:     -((a > b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn nleu(mut a: u32, mut b: u32) -> i32 {
// CHECK-NEXT:     -((a <= b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn nltu(mut a: u32, mut b: u32) -> i32 {
// CHECK-NEXT:     -((a < b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if nge(-(2147483647 as i32) - (1 as i32), 2147483647 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nge(2147483647 as i32, -(2147483647 as i32) - (1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ngt(-(2147483647 as i32) - (1 as i32), 2147483647 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ngt(2147483647 as i32, -(2147483647 as i32) - (1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nle(-(2147483647 as i32) - (1 as i32), 2147483647 as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nle(2147483647 as i32, -(2147483647 as i32) - (1 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nlt(-(2147483647 as i32) - (1 as i32), 2147483647 as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nlt(2147483647 as i32, -(2147483647 as i32) - (1 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if neq(-(2147483647 as i32) - (1 as i32), 2147483647 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if neq(2147483647 as i32, -(2147483647 as i32) - (1 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nne(-(2147483647 as i32) - (1 as i32), 2147483647 as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nne(2147483647 as i32, -(2147483647 as i32) - (1 as i32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ngeu((0 as i32) as u32, !(0 as u32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ngeu(!(0 as u32), (0 as i32) as u32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ngtu((0 as i32) as u32, !(0 as u32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ngtu(!(0 as u32), (0 as i32) as u32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nleu((0 as i32) as u32, !(0 as u32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nleu(!(0 as u32), (0 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nltu((0 as i32) as u32, !(0 as u32)) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if nltu(!(0 as u32), (0 as i32) as u32) != (0 as i32) {
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
