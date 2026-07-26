#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#define C89_JOIN_RAW(left, right) left##right
#define C89_JOIN(left, right) C89_JOIN_RAW(left, right)
#define C89_STRINGIFY_RAW(value) #value
#define C89_STRINGIFY(value) C89_STRINGIFY_RAW(value)
#define C89_MACRO_NUMBER 13

#if C89_MACRO_NUMBER == 12
#define C89_CONDITIONAL_VALUE 0
#elif defined(C89_MACRO_NUMBER)
#define C89_CONDITIONAL_VALUE C89_MACRO_NUMBER
#else
#define C89_CONDITIONAL_VALUE 0
#endif

enum C89Color {
  C89_RED = 3,
  C89_GREEN = 5,
  C89_BLUE = 7
};

struct C89Point {
  int x;
  int y;
};

struct C89Bits {
  unsigned low : 3;
  signed high : 4;
};

union C89Number {
  int integer;
  double real;
};

typedef signed int C89SignedInt;
typedef int (*C89BinaryOperation)(int, int);

int c89_external_value = 11;
static const int c89_const_global = 17;
static volatile int c89_volatile_global = 19;
static int C89_JOIN(c89_joined_, value) = 23;

static int c89_add(int left, int right);
static void c89_store(void *destination, const void *source);

static int c89_add(left, right)
int left;
int right;
{
  return left + right;
}

static void c89_store(void *destination, const void *source) {
  int *output;
  const int *input;
  output = (int *)destination;
  input = (const int *)source;
  *output = *input;
}

static int c89_variadic_sum(int count, ...) {
  va_list arguments;
  int index;
  int total;
  total = 0;
  va_start(arguments, count);
  for (index = 0; index < count; ++index) {
    total += va_arg(arguments, int);
  }
  va_end(arguments);
  return total;
}

static int c89_static_local(void) {
  static int calls = 0;
  calls += 1;
  return calls;
}

static int c89_control_flow(int value) {
  int result;
  int index;
  result = 0;
  index = 0;
  while (index < value) {
    if (index == 1) {
      ++index;
      continue;
    }
    result += index;
    if (result > 20) {
      break;
    }
    ++index;
  }
  do {
    --result;
  } while (result > 6);
  switch (value) {
  case 4:
    result += 10;
    break;
  default:
    result = 0;
    break;
  }
  if (result == 16) {
    goto c89_done;
  }
  result = -1;
c89_done:
  return result;
}

int main(void) {
  auto int automatic_value;
  register int register_value;
  extern int c89_external_value;
  C89SignedInt signed_value;
  unsigned long unsigned_value;
  signed char signed_character;
  float float_value;
  double double_value;
  long double long_double_value;
  wchar_t wide_character;
  enum C89Color color;
  struct C89Point point;
  struct C89Bits bits;
  union C89Number number;
  int array[4];
  int copied_value;
  int source_value;
  int *pointer;
  const int *const_pointer;
  C89BinaryOperation operation;
  int arithmetic;
  int bitwise;
  int logical;
  int conditional;
  int comma_value;
  int string_length;
  int static_calls;
  int variadic_total;
  int control_total;
  int standard_macro;

  automatic_value = 2;
  register_value = 3;
  signed_value = -5;
  unsigned_value = 29UL;
  signed_character = -7;
  float_value = 2.5F;
  double_value = 3.5;
  long_double_value = 4.5L;
  wide_character = L'Z';
  color = C89_GREEN;

  point.x = 31;
  point.y = 37;
  bits.low = 6;
  bits.high = -3;
  number.integer = 41;

  array[0] = 1;
  array[1] = 2;
  array[2] = 3;
  array[3] = 4;
  pointer = array;
  const_pointer = pointer;

  source_value = 43;
  copied_value = 0;
  c89_store(&copied_value, &source_value);
  operation = c89_add;

  arithmetic = (automatic_value + register_value) * 4 - 3;
  arithmetic /= 17;
  arithmetic %= 3;
  bitwise = ((1 << 5) | 3) ^ 2;
  bitwise &= 31;
  logical = (signed_value < 0 && unsigned_value > 0UL) || 0;
  conditional = logical ? 47 : 0;
  comma_value = (automatic_value += 1, automatic_value + 49);
  string_length = (int)sizeof(C89_STRINGIFY(C89_MACRO_NUMBER)) - 1;
  static_calls = c89_static_local() * 10 + c89_static_local();
  variadic_total = c89_variadic_sum(4, 2, 3, 5, 7);
  control_total = c89_control_flow(4);

#ifdef __STDC__
  standard_macro = __STDC__;
#else
  standard_macro = 0;
#endif

  printf(
      "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
      "%d %d %d %d %d %d %d %d %d\n",
      c89_external_value, c89_const_global, c89_volatile_global,
      c89_joined_value, C89_CONDITIONAL_VALUE, standard_macro, signed_value,
      (int)unsigned_value, (int)signed_character, (int)float_value,
      (int)double_value, (int)long_double_value, (int)wide_character, color,
      point.x + point.y, bits.low, bits.high, number.integer,
      const_pointer[0] + const_pointer[3], copied_value,
      operation(53, 6), arithmetic, bitwise, logical, conditional, comma_value,
      string_length, static_calls, variadic_total + control_total);
  return 0;
}
