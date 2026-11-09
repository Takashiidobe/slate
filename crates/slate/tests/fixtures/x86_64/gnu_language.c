// { dg-options "-std=gnu23" }
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define GNU_NAMED_SUM(first, rest...) ((first) + (rest))
#define GNU_COMMA_OPTION(first, ...)  ((first), ##__VA_ARGS__)

struct GNUZeroArray {
  int           count;
  unsigned char data[0];
};

struct GNUEmpty {};

union GNUFlexibleUnion {
  int           value;
  unsigned char data[];
};

struct GNUOnlyFlexible {
  unsigned char data[];
};

union GNUCast {
  int    integer;
  double real;
};

enum GNUForward;

enum GNUForward { GNU_FORWARD_VALUE = 23 };

static int gnu_statement_expression(int input) {
  return ({
    int local = input + 2;
    local * 3;
  });
}

static int gnu_local_label(int input) {
  __label__ finished;
  int result = input;
  if (result > 0) {
    result += 5;
    goto finished;
  }
  result = 0;
finished:
  return result;
}

static int gnu_computed_goto(unsigned int selector) {
  static void *targets[] = {&&zero, &&one, &&other};
  if (selector > 1) {
    selector = 2;
  }
  goto *targets[selector];
zero:
  return 7;
one:
  return 11;
other:
  return 13;
}

static int gnu_case_range(int value) {
  switch (value) {
  case 1 ... 4:
    return 17;
  case 5 ... 8:
    return 19;
  default:
    return 23;
  }
}

static int gnu_function_name(void) {
  return (__FUNCTION__[0] == 'g') + (__PRETTY_FUNCTION__[0] == 'i');
}

static int gnu_function_pointer(void) {
  ptrdiff_t distance = (gnu_function_name + 3) - gnu_function_name;
  return distance == 3;
}

static int gnu_syntax(void) {
  int                  gnu$dollar      = 29;
  int                  side_effect     = 0;
  int                  omitted         = ++side_effect ?: 31;
  __auto_type          inferred        = 37;
  __typeof__(inferred) same_type       = 41;
  const char          *raw             = R"tag(gnu\n)tag";
  int                  range_values[6] = {[1 ... 3] = 5, [5] = 7};
  int                  old_array[3]    = {[1] 11};
  struct GNUOldInitializer {
    int first;
    int second;
  };
  struct GNUOldInitializer old_record = {first : 13, second : 17};
  int                      macro_total =
      GNU_NAMED_SUM(43, 47) + GNU_COMMA_OPTION(53) + GNU_COMMA_OPTION(59, 61);
  int initializer_total = range_values[0] + range_values[1] + range_values[3] +
                          range_values[5] + old_array[1] + old_record.first +
                          old_record.second;
  int spelling_total =
      __extension__({
        __const__ int value = 67;
        value;
      }) +
      (__alignof__(long) >= __alignof__(int)) + ('\e' == 27) + (raw[0] == 'g') +
      (raw[3] == '\\') + (raw[4] == 'n') + (int)_Countof(range_values) +
      (int)__builtin_offsetof(struct GNUOldInitializer, second);
  return gnu$dollar + omitted + side_effect + inferred + same_type +
         macro_total + initializer_total + spelling_total;
}

static int gnu_aggregate_types(void) {
  union GNUCast   cast_value = (union GNUCast)71;
  enum GNUForward forward    = GNU_FORWARD_VALUE;
  return (int)sizeof(struct GNUZeroArray) + (int)sizeof(struct GNUEmpty) +
         (int)sizeof(union GNUFlexibleUnion) +
         (int)sizeof(struct GNUOnlyFlexible) + cast_value.integer + forward;
}

static int gnu_pointer_semantics(void) {
  unsigned char values[8] = {};
  void         *begin     = values;
  void         *middle    = begin + 3;
  return (middle - begin == 3) + (middle == values + 3) + (sizeof(void) == 1) +
         gnu_function_pointer();
}

static int gnu_numeric_types(void) {
  __int128        wide_integer    = ((__int128)1 << 100) + 73;
  __complex__ int integer_complex = 5 + 7i;
  __float128      wide_float      = 3.25Q;
  int complex_total = __real__ integer_complex + __imag__ integer_complex;
  return (int)(wide_integer >> 100) + (int)(wide_integer & 127) +
         complex_total + (int)(wide_float * 4);
}

int main(void) {
  printf("%d %d %d %d %d %d %d\n", gnu_statement_expression(3),
         gnu_local_label(5), gnu_computed_goto(0) + gnu_computed_goto(1),
         gnu_case_range(3) + gnu_case_range(7) + gnu_case_range(9),
         gnu_syntax(), gnu_aggregate_types(),
         gnu_pointer_semantics() + gnu_numeric_types() + gnu_function_name());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=gnu23" }
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUZeroArray {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     data: [u8; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUEmpty {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union GNUCast {
// CHECK-NEXT:     integer: i32,
// CHECK-NEXT:     real: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUOldInitializer {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_statement_expression(input: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     let local: i32 = input + 2;
// CHECK-NEXT:     [[V_0]] = local * 3;
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_local_label(input: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         result = input;
// CHECK-NEXT:         if result > 0 {
// CHECK-NEXT:             result += 5;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             result = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         return result;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_computed_goto(mut selector: u32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 5usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 13;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 11;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     return 7;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => match targets.0[selector as usize] as usize {
// CHECK-NEXT:                     19 => {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     20 => {
// CHECK-NEXT:                         __slate_state = 1usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     21 => {
// CHECK-NEXT:                         __slate_state = 0usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         unreachable!();
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     selector = 2;
// CHECK-NEXT:                     __slate_state = 3usize;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     if selector > 1 {
// CHECK-NEXT:                         __slate_state = 4usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut targets: __SlateAlign16<[*mut (); 3]> =
// CHECK-NEXT:     __SlateAlign16([19usize as *mut (), 20usize as *mut (), 21usize as *mut ()]);
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_case_range(value: i32) -> i32 {
// CHECK-NEXT:     match value {
// CHECK-NEXT:         1..=4 => {
// CHECK-NEXT:             return 17;
// CHECK-NEXT:         }
// CHECK-NEXT:         5..=8 => {
// CHECK-NEXT:             return 19;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 23;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.gnu_function_name")]
// CHECK-NEXT: extern "C-unwind" fn gnu_function_name() -> i32 {
// CHECK-NEXT:     ((((unsafe { *(c"gnu_function_name".as_ptr() as *mut i8).add(0) }) as i32) == 103) as i32)
// CHECK-NEXT:         + ((((unsafe { *(c"int gnu_function_name(void)".as_ptr() as *mut i8).add(0) }) as i32)
// CHECK-NEXT:             == 105) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_function_pointer() -> i32 {
// CHECK-NEXT:     let distance: i64 = ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, *const u8>(unsafe {
// CHECK-NEXT:             std::mem::transmute::<*const u8, Option<unsafe extern "C-unwind" fn() -> i32>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, *const u8>(
// CHECK-NEXT:                         Some(gnu_function_name),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .wrapping_offset(3 as isize),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     }) as isize)
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, *const u8>(
// CHECK-NEXT:                     Some(gnu_function_name),
// CHECK-NEXT:                 )
// CHECK-NEXT:             }) as isize,
// CHECK-NEXT:         ) as i64;
// CHECK-NEXT:     (distance == 3) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_syntax() -> i32 {
// CHECK-NEXT:     let gnu_dollar_dollar: i32 = 29;
// CHECK-NEXT:     let mut side_effect: i32 = 0;
// CHECK-NEXT:     let mut omitted: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = side_effect + 1;
// CHECK-NEXT:     side_effect = [[V_1]];
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = [[V_1]];
// CHECK-NEXT:     omitted = if [[V_2]] != 0 { [[V_2]] } else { 31 };
// CHECK-NEXT:     let inferred: i32 = 37;
// CHECK-NEXT:     let same_type: i32 = 41;
// CHECK-NEXT:     let raw: *const i8 = c"gnu\\n".as_ptr();
// CHECK-NEXT:     let range_values: __SlateAlign16<[i32; 6]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 6] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = 5i32;
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: usize = 1;
// CHECK-NEXT:         while [[T_2]] <= 3 {
// CHECK-NEXT:             [[T_0]][[[T_2]]] = [[T_1]];
// CHECK-NEXT:             [[T_2]] += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]][5] = 7i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let old_array: [i32; 3] = {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [i32; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]][1] = 11i32;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let old_record: GNUOldInitializer = GNUOldInitializer {
// CHECK-NEXT:         first: 13,
// CHECK-NEXT:         second: 17,
// CHECK-NEXT:     };
// CHECK-NEXT:     let macro_total: i32 = 43 + 47 + 53 + 61;
// CHECK-NEXT:     let initializer_total: i32 = range_values.0[0]
// CHECK-NEXT:         + range_values.0[1]
// CHECK-NEXT:         + range_values.0[3]
// CHECK-NEXT:         + range_values.0[5]
// CHECK-NEXT:         + old_array[1]
// CHECK-NEXT:         + old_record.first
// CHECK-NEXT:         + old_record.second;
// CHECK-NEXT:     let mut spelling_total: i32 = 0;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32;
// CHECK-NEXT:     let value: i32 = 67;
// CHECK-NEXT:     [[V_3]] = value;
// CHECK-NEXT:     spelling_total = [[V_3]]
// CHECK-NEXT:         + (((std::mem::align_of::<i64>() as u64) >= (std::mem::align_of::<i32>() as u64)) as i32)
// CHECK-NEXT:         + (true as i32)
// CHECK-NEXT:         + ((((unsafe { *raw.add(0) }) as i32) == 103) as i32)
// CHECK-NEXT:         + ((((unsafe { *raw.add(3) }) as i32) == 92) as i32)
// CHECK-NEXT:         + ((((unsafe { *raw.add(4) }) as i32) == 110) as i32)
// CHECK-NEXT:         + 6
// CHECK-NEXT:         + (std::mem::offset_of!(GNUOldInitializer, second) as i32);
// CHECK-NEXT:     gnu_dollar_dollar
// CHECK-NEXT:         + omitted
// CHECK-NEXT:         + side_effect
// CHECK-NEXT:         + inferred
// CHECK-NEXT:         + same_type
// CHECK-NEXT:         + macro_total
// CHECK-NEXT:         + initializer_total
// CHECK-NEXT:         + spelling_total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_aggregate_types() -> i32 {
// CHECK-NEXT:     let cast_value: GNUCast = {
// CHECK-NEXT:         let mut [[T_0]]: GNUCast = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].integer = 71i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let forward: u32 = 23;
// CHECK-NEXT:     (((std::mem::size_of::<GNUZeroArray>() as i32)
// CHECK-NEXT:         + (std::mem::size_of::<GNUEmpty>() as i32)
// CHECK-NEXT:         + 4
// CHECK-NEXT:         + 0
// CHECK-NEXT:         + unsafe { cast_value.integer }) as u32)
// CHECK-NEXT:         .wrapping_add(forward as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_pointer_semantics() -> i32 {
// CHECK-NEXT:     let mut values: [u8; 8] = {
// CHECK-NEXT:         let mut [[T_0]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let begin: *mut () = values.as_mut_ptr() as *mut ();
// CHECK-NEXT:     let middle: *mut () = (unsafe { (begin as *const u8).add(3) }) as *mut ();
// CHECK-NEXT:     ((((unsafe { (middle as *const u8).offset_from(begin as *const u8) }) as i64) == 3) as i32)
// CHECK-NEXT:         + ((middle == ((unsafe { (values.as_mut_ptr() as *mut u8).add(3) }) as *mut ())) as i32)
// CHECK-NEXT:         + (true as i32)
// CHECK-NEXT:         + gnu_function_pointer()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_numeric_types() -> i32 {
// CHECK-NEXT:     let wide_integer: i128 = ((1 as i128) << 100i32) + 73;
// CHECK-NEXT:     let integer_complex: num_complex::Complex<i32> = 5 + num_complex::Complex { re: 0, im: 7 };
// CHECK-NEXT:     let wide_float: f128 = 3.25f128;
// CHECK-NEXT:     let complex_total: i32 = integer_complex.re + integer_complex.im;
// CHECK-NEXT:     ((wide_integer >> 100i32) as i32)
// CHECK-NEXT:         + ((wide_integer & 127) as i32)
// CHECK-NEXT:         + complex_total
// CHECK-NEXT:         + ((wide_float * (4i32 as f128)) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {}",
// CHECK-NEXT:         gnu_statement_expression(3i32),
// CHECK-NEXT:         gnu_local_label(5i32),
// CHECK-NEXT:         gnu_computed_goto(0u32) + gnu_computed_goto(1u32),
// CHECK-NEXT:         gnu_case_range(3i32) + gnu_case_range(7i32) + gnu_case_range(9i32),
// CHECK-NEXT:         gnu_syntax(),
// CHECK-NEXT:         gnu_aggregate_types(),
// CHECK-NEXT:         gnu_pointer_semantics() + gnu_numeric_types() + gnu_function_name()
// CHECK-NEXT:     );
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
