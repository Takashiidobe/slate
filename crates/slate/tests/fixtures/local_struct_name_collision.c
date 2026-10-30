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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct TestCase__4 {
// CHECK-NEXT:     doc: *const i8,
// CHECK-NEXT:     expectedStatus: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct TestCase__23 {
// CHECK-NEXT:     usesParameterEntities: i32,
// CHECK-NEXT:     weight: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct TestCase__45 {
// CHECK-NEXT:     expectedMovementInChars: i32,
// CHECK-NEXT:     input: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sum_docs() -> i32 {
// CHECK-NEXT:     let mut cases: __SlateAlign16<[TestCase__4; 3]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: TestCase__4 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].doc = c"a".as_ptr();
// CHECK-NEXT:             [[T_0]].expectedStatus = 1i32;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: TestCase__4 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].doc = c"bb".as_ptr();
// CHECK-NEXT:             [[T_1]].expectedStatus = 2i32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: TestCase__4 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].doc = c"ccc".as_ptr();
// CHECK-NEXT:             [[T_2]].expectedStatus = 3i32;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         total
// CHECK-NEXT:             += ((unsafe { *cases.0[i as usize].doc.add(0) }) as i32)
// CHECK-NEXT:                 * cases.0[i as usize].expectedStatus;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_flags() -> i32 {
// CHECK-NEXT:     let cases: __SlateAlign16<[TestCase__23; 2]> = __SlateAlign16([
// CHECK-NEXT:         TestCase__23 {
// CHECK-NEXT:             usesParameterEntities: 1,
// CHECK-NEXT:             weight: 10,
// CHECK-NEXT:         },
// CHECK-NEXT:         TestCase__23 {
// CHECK-NEXT:             usesParameterEntities: 0,
// CHECK-NEXT:             weight: 20,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         if cases.0[i as usize].usesParameterEntities != 0 {
// CHECK-NEXT:             total += cases.0[i as usize].weight;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             total -= cases.0[i as usize].weight;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_movements() -> i32 {
// CHECK-NEXT:     let mut cases: __SlateAlign16<[TestCase__45; 3]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0]]: TestCase__45 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].expectedMovementInChars = 1i32;
// CHECK-NEXT:             [[T_0]].input = c"x".as_ptr();
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1]]: TestCase__45 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].expectedMovementInChars = 2i32;
// CHECK-NEXT:             [[T_1]].input = c"yy".as_ptr();
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2]]: TestCase__45 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].expectedMovementInChars = 3i32;
// CHECK-NEXT:             [[T_2]].input = c"zzz".as_ptr();
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         total
// CHECK-NEXT:             += cases.0[i as usize].expectedMovementInChars
// CHECK-NEXT:                 + ((unsafe { *cases.0[i as usize].input.add(0) }) as i32);
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
