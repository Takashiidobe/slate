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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct TestCase {
// COMMON-LOWERING-NEXT:     expectedStatus: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct TestCase_0 {
// COMMON-LOWERING-NEXT:     usesParameterEntities: i32,
// COMMON-LOWERING-NEXT:     weight: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct TestCase_1 {
// COMMON-LOWERING-NEXT:     expectedMovementInChars: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_docs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_flags();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_movements();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sum_docs() -> i32 {
// COMMON-LOWERING-NEXT:         [TestCase {
// COMMON-LOWERING-NEXT:             doc: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:             expectedStatus: 0,
// COMMON-LOWERING-NEXT:         }; 3],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [TestCase; 3] = [
// COMMON-LOWERING-NEXT:         TestCase {
// COMMON-LOWERING-NEXT:             expectedStatus: 1,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         TestCase {
// COMMON-LOWERING-NEXT:             expectedStatus: 2,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         TestCase {
// COMMON-LOWERING-NEXT:             expectedStatus: 3,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *cases = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = cases[({{__v[0-9]+}} as usize)].expectedStatus;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sum_flags() -> i32 {
// COMMON-LOWERING-NEXT:         [TestCase_0 {
// COMMON-LOWERING-NEXT:             usesParameterEntities: 0,
// COMMON-LOWERING-NEXT:             weight: 0,
// COMMON-LOWERING-NEXT:         }; 2],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [TestCase_0; 2] = [
// COMMON-LOWERING-NEXT:         TestCase_0 {
// COMMON-LOWERING-NEXT:             usesParameterEntities: 1,
// COMMON-LOWERING-NEXT:             weight: 10,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         TestCase_0 {
// COMMON-LOWERING-NEXT:             usesParameterEntities: 0,
// COMMON-LOWERING-NEXT:             weight: 20,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *cases = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = cases[({{__v[0-9]+}} as usize)].usesParameterEntities;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = cases[({{__v[0-9]+}} as usize)].weight;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     } else {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = cases[({{__v[0-9]+}} as usize)].weight;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sum_movements() -> i32 {
// COMMON-LOWERING-NEXT:         [TestCase_1 {
// COMMON-LOWERING-NEXT:             expectedMovementInChars: 0,
// COMMON-LOWERING-NEXT:             input: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         }; 3],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [TestCase_1; 3] = [
// COMMON-LOWERING-NEXT:         TestCase_1 {
// COMMON-LOWERING-NEXT:             expectedMovementInChars: 1,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         TestCase_1 {
// COMMON-LOWERING-NEXT:             expectedMovementInChars: 2,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         TestCase_1 {
// COMMON-LOWERING-NEXT:             expectedMovementInChars: 3,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *cases = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = cases[({{__v[0-9]+}} as usize)].expectedMovementInChars;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     doc: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     input: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase; 3]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:             doc: b"a\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             doc: b"bb\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             doc: b"ccc\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = cases[({{__v[0-9]+}} as usize)].doc;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase_0; 2]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase_1; 3]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:             input: b"x\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             input: b"yy\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:             input: b"zzz\0".as_ptr() as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = cases[({{__v[0-9]+}} as usize)].input;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     doc: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     input: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A8, [TestCase; 3]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:             doc: b"a\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             doc: b"bb\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             doc: b"ccc\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = cases[({{__v[0-9]+}} as usize)].doc;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [TestCase_0; 2]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A8, [TestCase_1; 3]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:             input: b"x\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             input: b"yy\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:             input: b"zzz\0".as_ptr() as *mut u8,
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = cases[({{__v[0-9]+}} as usize)].input;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct TestCase {
// COMMON-REWRITES-NEXT:     expectedStatus: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct TestCase_0 {
// COMMON-REWRITES-NEXT:     usesParameterEntities: i32,
// COMMON-REWRITES-NEXT:     weight: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct TestCase_1 {
// COMMON-REWRITES-NEXT:     expectedMovementInChars: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             sum_docs(),
// COMMON-REWRITES-NEXT:             sum_flags(),
// COMMON-REWRITES-NEXT:             sum_movements(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sum_docs() -> i32 {
// COMMON-REWRITES-NEXT:         [TestCase {
// COMMON-REWRITES-NEXT:             doc: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:             expectedStatus: 0,
// COMMON-REWRITES-NEXT:         }; 3],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     *cases = [
// COMMON-REWRITES-NEXT:         TestCase {
// COMMON-REWRITES-NEXT:             expectedStatus: 1,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         TestCase {
// COMMON-REWRITES-NEXT:             expectedStatus: 2,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         TestCase {
// COMMON-REWRITES-NEXT:             expectedStatus: 3,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     for i in 0..3 {
// COMMON-REWRITES-NEXT:         total += ((unsafe { *{{__v[0-9]+}} }) as i32) * cases[((i as i64) as usize)].expectedStatus;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sum_flags() -> i32 {
// COMMON-REWRITES-NEXT:         [TestCase_0 {
// COMMON-REWRITES-NEXT:             usesParameterEntities: 0,
// COMMON-REWRITES-NEXT:             weight: 0,
// COMMON-REWRITES-NEXT:         }; 2],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     *cases = [
// COMMON-REWRITES-NEXT:         TestCase_0 {
// COMMON-REWRITES-NEXT:             usesParameterEntities: 1,
// COMMON-REWRITES-NEXT:             weight: 10,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         TestCase_0 {
// COMMON-REWRITES-NEXT:             usesParameterEntities: 0,
// COMMON-REWRITES-NEXT:             weight: 20,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     for i in 0..2 {
// COMMON-REWRITES-NEXT:         if cases[((i as i64) as usize)].usesParameterEntities != 0 {
// COMMON-REWRITES-NEXT:             total += cases[((i as i64) as usize)].weight;
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             total -= cases[((i as i64) as usize)].weight;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sum_movements() -> i32 {
// COMMON-REWRITES-NEXT:         [TestCase_1 {
// COMMON-REWRITES-NEXT:             expectedMovementInChars: 0,
// COMMON-REWRITES-NEXT:             input: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         }; 3],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     *cases = [
// COMMON-REWRITES-NEXT:         TestCase_1 {
// COMMON-REWRITES-NEXT:             expectedMovementInChars: 1,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         TestCase_1 {
// COMMON-REWRITES-NEXT:             expectedMovementInChars: 2,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         TestCase_1 {
// COMMON-REWRITES-NEXT:             expectedMovementInChars: 3,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     for i in 0..3 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = cases[((i as i64) as usize)].expectedMovementInChars;
// COMMON-REWRITES-NEXT:         total += {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     doc: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     input: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase; 3]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:             doc: c"a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             doc: c"bb".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             doc: c"ccc".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = cases[((i as i64) as usize)].doc;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase_0; 2]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [TestCase_1; 3]> = aligned::Aligned(
// REWRITES-X86_64-GNU-NEXT:             input: c"x".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             input: c"yy".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             input: c"zzz".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = cases[((i as i64) as usize)].input;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     doc: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     input: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A8, [TestCase; 3]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:             doc: c"a".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             doc: c"bb".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             doc: c"ccc".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = cases[((i as i64) as usize)].doc;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [TestCase_0; 2]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A8, [TestCase_1; 3]> = aligned::Aligned(
// REWRITES-AARCH64-GNU-NEXT:             input: c"x".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             input: c"yy".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             input: c"zzz".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = cases[((i as i64) as usize)].input;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
