#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define GNU_NAMED_SUM(first, rest...) ((first) + (rest))
#define GNU_COMMA_OPTION(first, ...) ((first), ##__VA_ARGS__)

struct GNUZeroArray {
  int count;
  unsigned char data[0];
};

struct GNUEmpty {};

union GNUFlexibleUnion {
  int value;
  unsigned char data[];
};

struct GNUOnlyFlexible {
  unsigned char data[];
};

union GNUCast {
  int integer;
  double real;
};

enum GNUForward;

enum GNUForward {
  GNU_FORWARD_VALUE = 23
};

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
  int gnu$dollar = 29;
  int side_effect = 0;
  int omitted = ++side_effect ?: 31;
  __auto_type inferred = 37;
  __typeof__(inferred) same_type = 41;
  const char *raw = R"tag(gnu\n)tag";
  int range_values[6] = {[1 ... 3] = 5, [5] = 7};
  int old_array[3] = {[1] 11};
  struct GNUOldInitializer {
    int first;
    int second;
  };
  struct GNUOldInitializer old_record = {first : 13, second : 17};
  int macro_total =
      GNU_NAMED_SUM(43, 47) + GNU_COMMA_OPTION(53) +
      GNU_COMMA_OPTION(59, 61);
  int initializer_total =
      range_values[0] + range_values[1] + range_values[3] + range_values[5] +
      old_array[1] + old_record.first + old_record.second;
  int spelling_total =
      __extension__({ __const__ int value = 67; value; }) +
      (__alignof__(long) >= __alignof__(int)) + ('\e' == 27) +
      (raw[0] == 'g') + (raw[3] == '\\') + (raw[4] == 'n') +
      (int)_Countof(range_values) +
      (int)__builtin_offsetof(struct GNUOldInitializer, second);
  return gnu$dollar + omitted + side_effect + inferred + same_type +
         macro_total + initializer_total + spelling_total;
}

static int gnu_aggregate_types(void) {
  union GNUCast cast_value = (union GNUCast)71;
  enum GNUForward forward = GNU_FORWARD_VALUE;
  return (int)sizeof(struct GNUZeroArray) +
         (int)sizeof(struct GNUEmpty) +
         (int)sizeof(union GNUFlexibleUnion) +
         (int)sizeof(struct GNUOnlyFlexible) + cast_value.integer + forward;
}

static int gnu_pointer_semantics(void) {
  unsigned char values[8] = {};
  void *begin = values;
  void *middle = begin + 3;
  return (middle - begin == 3) + (middle == values + 3) +
         (sizeof(void) == 1) + gnu_function_pointer();
}

static int gnu_numeric_types(void) {
  __int128 wide_integer = ((__int128)1 << 100) + 73;
  __complex__ int integer_complex = 5 + 7i;
  __float128 wide_float = 3.25Q;
  int complex_total =
      __real__ integer_complex + __imag__ integer_complex;
  return (int)(wide_integer >> 100) + (int)(wide_integer & 127) +
         complex_total + (int)(wide_float * 4);
}

int main(void) {
  printf("%d %d %d %d %d %d %d\n", gnu_statement_expression(3),
         gnu_local_label(5), gnu_computed_goto(0) + gnu_computed_goto(1),
         gnu_case_range(3) + gnu_case_range(7) + gnu_case_range(9),
         gnu_syntax(), gnu_aggregate_types(),
         gnu_pointer_semantics() + gnu_numeric_types() +
             gnu_function_name());
  return 0;
}
