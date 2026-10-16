/* { dg-do run } */
/* Like fp-cmp-4.c, but test that the setcc patterns are correct.  */

void abort(void);
void exit(int);

static int test_isunordered(double x, double y) {
  return __builtin_isunordered(x, y);
}

static int test_not_isunordered(double x, double y) {
  return !__builtin_isunordered(x, y);
}

static int test_isless(double x, double y) { return __builtin_isless(x, y); }

static int test_not_isless(double x, double y) {
  return !__builtin_isless(x, y);
}

static int test_islessequal(double x, double y) {
  return __builtin_islessequal(x, y);
}

static int test_not_islessequal(double x, double y) {
  return !__builtin_islessequal(x, y);
}

static int test_isgreater(double x, double y) {
  return __builtin_isgreater(x, y);
}

static int test_not_isgreater(double x, double y) {
  return !__builtin_isgreater(x, y);
}

static int test_isgreaterequal(double x, double y) {
  return __builtin_isgreaterequal(x, y);
}

static int test_not_isgreaterequal(double x, double y) {
  return !__builtin_isgreaterequal(x, y);
}

static int test_islessgreater(double x, double y) {
  return __builtin_islessgreater(x, y);
}

static int test_not_islessgreater(double x, double y) {
  return !__builtin_islessgreater(x, y);
}

static void one_test(double x, double y, int expected,
                     int (*pos)(double, double), int (*neg)(double, double)) {
  if ((*pos)(x, y) != expected)
    abort();
  if ((*neg)(x, y) != !expected)
    abort();
}

#define NAN (0.0 / 0.0)

int main() {
  struct try {
    double x, y;
    int    result[6];
  };

  static struct try const data[] = {
      {NAN, NAN, {1, 0, 0, 0, 0, 0}}, {0.0, NAN, {1, 0, 0, 0, 0, 0}},
      {NAN, 0.0, {1, 0, 0, 0, 0, 0}}, {0.0, 0.0, {0, 0, 1, 0, 1, 0}},
      {1.0, 2.0, {0, 1, 1, 0, 0, 1}}, {2.0, 1.0, {0, 0, 0, 1, 1, 1}},
  };

  struct test {
    int (*pos)(double, double);
    int (*neg)(double, double);
  };

  static struct test const tests[] = {
      {test_isunordered, test_not_isunordered},
      {test_isless, test_not_isless},
      {test_islessequal, test_not_islessequal},
      {test_isgreater, test_not_isgreater},
      {test_isgreaterequal, test_not_isgreaterequal},
      {test_islessgreater, test_not_islessgreater}};

  const int n = sizeof(data) / sizeof(data[0]);
  int       i, j;

  for (i = 0; i < n; ++i)
    for (j = 0; j < 6; ++j)
      one_test(data[i].x, data[i].y, data[i].result[j], tests[j].pos,
               tests[j].neg);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Like fp-cmp-4.c, but test that the setcc patterns are correct.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct r#try {
// CHECK-NEXT:     x: f64,
// CHECK-NEXT:     y: f64,
// CHECK-NEXT:     result: [i32; 6],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct test {
// CHECK-NEXT:     pos: Option<unsafe extern "C-unwind" fn(f64, f64) -> i32>,
// CHECK-NEXT:     neg: Option<unsafe extern "C-unwind" fn(f64, f64) -> i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_isunordered")]
// CHECK-NEXT: extern "C-unwind" fn test_isunordered(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     (x.is_nan() as i32) | (y.is_nan() as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_not_isunordered")]
// CHECK-NEXT: extern "C-unwind" fn test_not_isunordered(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     !((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_isless")]
// CHECK-NEXT: extern "C-unwind" fn test_isless(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     (x < y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_not_isless")]
// CHECK-NEXT: extern "C-unwind" fn test_not_isless(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     !(x < y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_islessequal")]
// CHECK-NEXT: extern "C-unwind" fn test_islessequal(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     (x <= y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_not_islessequal")]
// CHECK-NEXT: extern "C-unwind" fn test_not_islessequal(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     !(x <= y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_isgreater")]
// CHECK-NEXT: extern "C-unwind" fn test_isgreater(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     (x > y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_not_isgreater")]
// CHECK-NEXT: extern "C-unwind" fn test_not_isgreater(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     !(x > y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_isgreaterequal")]
// CHECK-NEXT: extern "C-unwind" fn test_isgreaterequal(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     (x >= y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_not_isgreaterequal")]
// CHECK-NEXT: extern "C-unwind" fn test_not_isgreaterequal(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     !(x >= y) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_islessgreater")]
// CHECK-NEXT: extern "C-unwind" fn test_islessgreater(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     ((x < y) as i32) | ((x > y) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_not_islessgreater")]
// CHECK-NEXT: extern "C-unwind" fn test_not_islessgreater(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     !(((x < y) as i32) | ((x > y) as i32) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn one_test(
// CHECK-NEXT:     mut x: f64,
// CHECK-NEXT:     mut y: f64,
// CHECK-NEXT:     mut expected: i32,
// CHECK-NEXT:     mut pos: Option<unsafe extern "C-unwind" fn(f64, f64) -> i32>,
// CHECK-NEXT:     mut neg: Option<unsafe extern "C-unwind" fn(f64, f64) -> i32>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if (unsafe { pos.unwrap()(x, y) }) != expected {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { neg.unwrap()(x, y) }) != (!(expected != (0 as i32)) as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut n: i32 = (((240 as u64) / (40 as u64)) as u32) as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         j = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (6 as i32) {
// CHECK-NEXT:             one_test(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of!(data.0) as *const r#try }.offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .x
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of!(data.0) as *const r#try }.offset(i as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .y
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe {
// CHECK-NEXT:                                 unsafe { std::ptr::addr_of!(data.0) as *const r#try }
// CHECK-NEXT:                                     .offset(i as isize)
// CHECK-NEXT:                             })
// CHECK-NEXT:                             .result
// CHECK-NEXT:                             .as_ptr() as *const i32
// CHECK-NEXT:                         }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of!(tests.0) as *const test }.offset(j as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .pos
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of!(tests.0) as *const test }.offset(j as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .neg
// CHECK-NEXT:                 },
// CHECK-NEXT:             );
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             j = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut data: __SlateAlign16<[r#try; 6]> = __SlateAlign16([
// CHECK-NEXT:     r#try {
// CHECK-NEXT:         x: 0.0f64 / 0.0f64,
// CHECK-NEXT:         y: 0.0f64 / 0.0f64,
// CHECK-NEXT:         result: [1 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32],
// CHECK-NEXT:     },
// CHECK-NEXT:     r#try {
// CHECK-NEXT:         x: 0.0f64,
// CHECK-NEXT:         y: 0.0f64 / 0.0f64,
// CHECK-NEXT:         result: [1 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32],
// CHECK-NEXT:     },
// CHECK-NEXT:     r#try {
// CHECK-NEXT:         x: 0.0f64 / 0.0f64,
// CHECK-NEXT:         y: 0.0f64,
// CHECK-NEXT:         result: [1 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32],
// CHECK-NEXT:     },
// CHECK-NEXT:     r#try {
// CHECK-NEXT:         x: 0.0f64,
// CHECK-NEXT:         y: 0.0f64,
// CHECK-NEXT:         result: [0 as i32, 0 as i32, 1 as i32, 0 as i32, 1 as i32, 0 as i32],
// CHECK-NEXT:     },
// CHECK-NEXT:     r#try {
// CHECK-NEXT:         x: 1.0f64,
// CHECK-NEXT:         y: 2.0f64,
// CHECK-NEXT:         result: [0 as i32, 1 as i32, 1 as i32, 0 as i32, 0 as i32, 1 as i32],
// CHECK-NEXT:     },
// CHECK-NEXT:     r#try {
// CHECK-NEXT:         x: 2.0f64,
// CHECK-NEXT:         y: 1.0f64,
// CHECK-NEXT:         result: [0 as i32, 0 as i32, 0 as i32, 1 as i32, 1 as i32, 1 as i32],
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut tests: __SlateAlign16<[test; 6]> = __SlateAlign16([
// CHECK-NEXT:     test {
// CHECK-NEXT:         pos: Some(test_isunordered),
// CHECK-NEXT:         neg: Some(test_not_isunordered),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         pos: Some(test_isless),
// CHECK-NEXT:         neg: Some(test_not_isless),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         pos: Some(test_islessequal),
// CHECK-NEXT:         neg: Some(test_not_islessequal),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         pos: Some(test_isgreater),
// CHECK-NEXT:         neg: Some(test_not_isgreater),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         pos: Some(test_isgreaterequal),
// CHECK-NEXT:         neg: Some(test_not_isgreaterequal),
// CHECK-NEXT:     },
// CHECK-NEXT:     test {
// CHECK-NEXT:         pos: Some(test_islessgreater),
// CHECK-NEXT:         neg: Some(test_not_islessgreater),
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
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
