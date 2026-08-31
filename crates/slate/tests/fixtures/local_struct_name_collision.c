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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct TestCase {
// LOWERING-NEXT:     doc: *mut i8,
// LOWERING-NEXT:     expectedStatus: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct TestCase_0 {
// LOWERING-NEXT:     usesParameterEntities: i32,
// LOWERING-NEXT:     weight: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct TestCase_1 {
// LOWERING-NEXT:     expectedMovementInChars: i32,
// LOWERING-NEXT:     input: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_docs() -> i32 {
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase; 3]> = aligned::Aligned([TestCase { doc: std::ptr::null_mut(), expectedStatus: 0 }; 3]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *cases = [TestCase { doc: b"a\0".as_ptr() as *mut i8, expectedStatus: 1 }, TestCase { doc: b"bb\0".as_ptr() as *mut i8, expectedStatus: 2 }, TestCase { doc: b"ccc\0".as_ptr() as *mut i8, expectedStatus: 3 }];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = cases[({{_v[0-9]+}} as usize)].doc;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = cases[({{_v[0-9]+}} as usize)].expectedStatus;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 total = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_flags() -> i32 {
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase_0; 2]> = aligned::Aligned([TestCase_0 { usesParameterEntities: 0, weight: 0 }; 2]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *cases = [TestCase_0 { usesParameterEntities: 1, weight: 10 }, TestCase_0 { usesParameterEntities: 0, weight: 20 }];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = cases[({{_v[0-9]+}} as usize)].usesParameterEntities;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = cases[({{_v[0-9]+}} as usize)].weight;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         total = {{_v[0-9]+}};
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = cases[({{_v[0-9]+}} as usize)].weight;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:                         total = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_movements() -> i32 {
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase_1; 3]> = aligned::Aligned([TestCase_1 { expectedMovementInChars: 0, input: std::ptr::null_mut() }; 3]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     *cases = [TestCase_1 { expectedMovementInChars: 1, input: b"x\0".as_ptr() as *mut i8 }, TestCase_1 { expectedMovementInChars: 2, input: b"yy\0".as_ptr() as *mut i8 }, TestCase_1 { expectedMovementInChars: 3, input: b"zzz\0".as_ptr() as *mut i8 }];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = cases[({{_v[0-9]+}} as usize)].expectedMovementInChars;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = cases[({{_v[0-9]+}} as usize)].input;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 total = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_docs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_flags();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_movements();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct TestCase {
// REWRITES-NEXT:     doc: *mut i8,
// REWRITES-NEXT:     expectedStatus: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct TestCase_0 {
// REWRITES-NEXT:     usesParameterEntities: i32,
// REWRITES-NEXT:     weight: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct TestCase_1 {
// REWRITES-NEXT:     expectedMovementInChars: i32,
// REWRITES-NEXT:     input: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_docs() -> i32 {
// REWRITES-NEXT: let mut cases: aligned::Aligned<aligned::A16, [TestCase; 3]> = aligned::Aligned([TestCase { doc: std::ptr::null_mut(), expectedStatus: 0 }; 3]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: *cases = [TestCase { doc: b"a\0".as_ptr() as *mut i8, expectedStatus: 1 }, TestCase { doc: b"bb\0".as_ptr() as *mut i8, expectedStatus: 2 }, TestCase { doc: b"ccc\0".as_ptr() as *mut i8, expectedStatus: 3 }];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                     if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: *mut i8 = cases[((i as i64) as usize)].doc;
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:                                     total = total + ((unsafe { *{{_v[0-9]+}} }) as i32) * cases[((i as i64) as usize)].expectedStatus;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_flags() -> i32 {
// REWRITES-NEXT: let mut cases: aligned::Aligned<aligned::A16, [TestCase_0; 2]> = aligned::Aligned([TestCase_0 { usesParameterEntities: 0, weight: 0 }; 2]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: *cases = [TestCase_0 { usesParameterEntities: 1, weight: 10 }, TestCase_0 { usesParameterEntities: 0, weight: 20 }];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:                     if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: bool = cases[((i as i64) as usize)].usesParameterEntities != 0;
// REWRITES-NEXT:                                                         if {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                 total = total + cases[((i as i64) as usize)].weight;
// REWRITES-NEXT:                                                         } else {
// REWRITES-NEXT:                                                                                 total = total - cases[((i as i64) as usize)].weight;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_movements() -> i32 {
// REWRITES-NEXT: let mut cases: aligned::Aligned<aligned::A16, [TestCase_1; 3]> = aligned::Aligned([TestCase_1 { expectedMovementInChars: 0, input: std::ptr::null_mut() }; 3]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: *cases = [TestCase_1 { expectedMovementInChars: 1, input: b"x\0".as_ptr() as *mut i8 }, TestCase_1 { expectedMovementInChars: 2, input: b"yy\0".as_ptr() as *mut i8 }, TestCase_1 { expectedMovementInChars: 3, input: b"zzz\0".as_ptr() as *mut i8 }];
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                     if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: i32 = cases[((i as i64) as usize)].expectedMovementInChars;
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: *mut i8 = cases[((i as i64) as usize)].input;
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:                                     total = total + ({{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32));
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = sum_docs();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = sum_flags();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = sum_movements();
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
