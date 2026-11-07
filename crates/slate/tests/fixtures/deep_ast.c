#include <stdio.h>

#define STEP_1(value) ((value) + 1)
#define STEP_2(value) STEP_1(STEP_1(value))
#define STEP_3(value) STEP_2(STEP_2(value))
#define STEP_4(value) STEP_3(STEP_3(value))
#define STEP_5(value) STEP_4(STEP_4(value))
#define STEP_6(value) STEP_5(STEP_5(value))
#define STEP_7(value) STEP_6(STEP_6(value))
#define STEP_8(value) STEP_7(STEP_7(value))

int main(void) {
  printf("%d\n", STEP_8(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! STEP_1 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         ($value) + 1
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_2__2 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_1!(($value) + 1)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_2 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_1!(STEP_1!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_3__2 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_2!(STEP_2__2!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_3 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_2!(STEP_2!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_4__2 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_3!(STEP_3__2!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_4 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_3!(STEP_3!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_5 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_4!(STEP_4__2!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_5__2 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_4!(STEP_4!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_6 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_5__2!(STEP_5!($value))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_7 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_6!(($value) + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1
// CHECK-NEXT:         + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! STEP_8 {
// CHECK-NEXT:     ($value:expr) => {
// CHECK-NEXT:         STEP_7!(($value) + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1
// CHECK-NEXT:         + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
// CHECK-NEXT:         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1
// CHECK-NEXT:         + 1 + 1 + 1 + 1 + 1 + 1 + 1)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", STEP_8!(0i32));
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
