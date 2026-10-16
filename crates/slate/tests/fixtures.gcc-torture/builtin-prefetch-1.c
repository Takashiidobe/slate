/* Test that __builtin_prefetch does no harm.

   Prefetch using all valid combinations of rw and locality values.
   These must be compile-time constants.  */

void exit(int);

#define NO_TEMPORAL_LOCALITY       0
#define LOW_TEMPORAL_LOCALITY      1
#define MODERATE_TEMPORAL_LOCALITY 1
#define HIGH_TEMPORAL_LOCALITY     3

#define READ_SHARED  2
#define WRITE_ACCESS 1
#define READ_ACCESS  0

enum locality { none, low, moderate, high };
enum rws { read, write, read_shared };

int arr[10];

void good_const(const int *p) {
  __builtin_prefetch(p, 0, 0);
  __builtin_prefetch(p, 0, 1);
  __builtin_prefetch(p, 0, 2);
  __builtin_prefetch(p, READ_ACCESS, 3);
  __builtin_prefetch(p, 1, NO_TEMPORAL_LOCALITY);
  __builtin_prefetch(p, 1, LOW_TEMPORAL_LOCALITY);
  __builtin_prefetch(p, 1, MODERATE_TEMPORAL_LOCALITY);
  __builtin_prefetch(p, WRITE_ACCESS, HIGH_TEMPORAL_LOCALITY);
}

void good_enum(const int *p) {
  __builtin_prefetch(p, read, none);
  __builtin_prefetch(p, read, low);
  __builtin_prefetch(p, read, moderate);
  __builtin_prefetch(p, read, high);
  __builtin_prefetch(p, write, none);
  __builtin_prefetch(p, write, low);
  __builtin_prefetch(p, write, moderate);
  __builtin_prefetch(p, write, high);
}

void good_expr(const int *p) {
  __builtin_prefetch(p, 1 - 1, 6 - (2 * 3));
  __builtin_prefetch(p, 1 + 0, 1 + 2);
}

void good_vararg(const int *p) {
  __builtin_prefetch(p, 0, 3);
  __builtin_prefetch(p, 0);
  __builtin_prefetch(p, 1);
  __builtin_prefetch(p);
}

int main() {
  good_const(arr);
  good_enum(arr);
  good_expr(arr);
  good_vararg(arr);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that __builtin_prefetch does no harm.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Prefetch using all valid combinations of rw and locality values.
// CHECK-NEXT: //! These must be compile-time constants.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn __builtin_prefetch([[V_1:__v[0-9]+]]: *const (), ...);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut arr: __SlateAlign16<[i32; 10]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn good_const(mut p: *const i32) {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn good_enum(mut p: *const i32) {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn good_expr(mut p: *const i32) {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn good_vararg(mut p: *const i32) {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     good_const((unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }) as *const i32);
// CHECK-NEXT:     good_enum((unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }) as *const i32);
// CHECK-NEXT:     good_expr((unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }) as *const i32);
// CHECK-NEXT:     good_vararg((unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i32 }) as *const i32);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
