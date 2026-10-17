#include <stdio.h>

static int sum_docs(void) {
  struct TestCase {
    const char *doc;
    int         expectedStatus;
  };

  const struct TestCase cases[] = {
      {"a", 1},
      {"bb", 2},
      {"ccc", 3},
  };

  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += (int)cases[i].doc[0] * cases[i].expectedStatus;
  }
  return total;
}

static int sum_flags(void) {
  struct TestCase {
    int usesParameterEntities;
    int weight;
  };

  const struct TestCase cases[] = {
      {1, 10},
      {0, 20},
  };

  int total = 0;
  for (int i = 0; i < 2; i++) {
    if (cases[i].usesParameterEntities) {
      total += cases[i].weight;
    } else {
      total -= cases[i].weight;
    }
  }
  return total;
}

static int sum_movements(void) {
  struct TestCase {
    int         expectedMovementInChars;
    const char *input;
  };

  struct TestCase cases[] = {
      {1, "x"},
      {2, "yy"},
      {3, "zzz"},
  };

  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += cases[i].expectedMovementInChars + (int)cases[i].input[0];
  }
  return total;
}

int main(void) {
  printf("%d %d %d\n", sum_docs(), sum_flags(), sum_movements());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[TESTCASE_0:TestCase_[0-9]+]] {
// CHECK-NEXT:     doc: *const i8,
// CHECK-NEXT:     expectedStatus: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[TESTCASE_1:TestCase_[0-9]+]] {
// CHECK-NEXT:     usesParameterEntities: i32,
// CHECK-NEXT:     weight: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[TESTCASE_2:TestCase_[0-9]+]] {
// CHECK-NEXT:     expectedMovementInChars: i32,
// CHECK-NEXT:     input: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sum_docs() -> i32 {
// CHECK-NEXT:     let mut cases: __SlateAlign16<[[[TESTCASE_0]]; 3]> = __SlateAlign16([
// CHECK-NEXT:         [[TESTCASE_0]] {
// CHECK-NEXT:             doc: c"a".as_ptr(),
// CHECK-NEXT:             expectedStatus: 1,
// CHECK-NEXT:         },
// CHECK-NEXT:         [[TESTCASE_0]] {
// CHECK-NEXT:             doc: c"bb".as_ptr(),
// CHECK-NEXT:             expectedStatus: 2,
// CHECK-NEXT:         },
// CHECK-NEXT:         [[TESTCASE_0]] {
// CHECK-NEXT:             doc: c"ccc".as_ptr(),
// CHECK-NEXT:             expectedStatus: 3,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         total
// CHECK-NEXT:             += ((unsafe { *unsafe { cases.0[i as usize].doc.offset(0 as isize) } })
// CHECK-NEXT:                 as i32) * cases.0[i as usize].expectedStatus;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_flags() -> i32 {
// CHECK-NEXT:     let cases: __SlateAlign16<[[[TESTCASE_1]]; 2]> = __SlateAlign16([
// CHECK-NEXT:         [[TESTCASE_1]] {
// CHECK-NEXT:             usesParameterEntities: 1,
// CHECK-NEXT:             weight: 10,
// CHECK-NEXT:         },
// CHECK-NEXT:         [[TESTCASE_1]] {
// CHECK-NEXT:             usesParameterEntities: 0,
// CHECK-NEXT:             weight: 20,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..2 as i32 {
// CHECK-NEXT:         if cases.0[i as usize].usesParameterEntities != 0 {
// CHECK-NEXT:             total += cases.0[i as usize].weight;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             total -= cases.0[i as usize].weight;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_movements() -> i32 {
// CHECK-NEXT:     let mut cases: __SlateAlign16<[[[TESTCASE_2]]; 3]> = __SlateAlign16([
// CHECK-NEXT:         [[TESTCASE_2]] {
// CHECK-NEXT:             expectedMovementInChars: 1,
// CHECK-NEXT:             input: c"x".as_ptr(),
// CHECK-NEXT:         },
// CHECK-NEXT:         [[TESTCASE_2]] {
// CHECK-NEXT:             expectedMovementInChars: 2,
// CHECK-NEXT:             input: c"yy".as_ptr(),
// CHECK-NEXT:         },
// CHECK-NEXT:         [[TESTCASE_2]] {
// CHECK-NEXT:             expectedMovementInChars: 3,
// CHECK-NEXT:             input: c"zzz".as_ptr(),
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         total
// CHECK-NEXT:             += cases.0[i as usize].expectedMovementInChars
// CHECK-NEXT:                 + ((unsafe { *unsafe { cases.0[i as usize].input.offset(0 as isize) } })
// CHECK-NEXT:                     as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", sum_docs(), sum_flags(), sum_movements());
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
