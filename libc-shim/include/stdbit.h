#ifndef __SLATE_STDBIT_H
#define __SLATE_STDBIT_H

#include <stdbool.h>

#define __STDC_VERSION_STDBIT_H__ 202311L

#define __STDC_ENDIAN_LITTLE__ __ORDER_LITTLE_ENDIAN__
#define __STDC_ENDIAN_BIG__    __ORDER_BIG_ENDIAN__
#define __STDC_ENDIAN_NATIVE__ __BYTE_ORDER__

unsigned int stdc_leading_zeros_uc(unsigned char value);
unsigned int stdc_leading_zeros_us(unsigned short value);
unsigned int stdc_leading_zeros_ui(unsigned int value);
unsigned int stdc_leading_zeros_ul(unsigned long int value);
unsigned int stdc_leading_zeros_ull(unsigned long long int value);
#define stdc_leading_zeros(value)                                            \
  _Generic((value),                                                          \
      unsigned char: stdc_leading_zeros_uc,                                  \
      unsigned short: stdc_leading_zeros_us,                                 \
      unsigned int: stdc_leading_zeros_ui,                                   \
      unsigned long int: stdc_leading_zeros_ul,                              \
      unsigned long long int: stdc_leading_zeros_ull)(value)

unsigned int stdc_leading_ones_uc(unsigned char value);
unsigned int stdc_leading_ones_us(unsigned short value);
unsigned int stdc_leading_ones_ui(unsigned int value);
unsigned int stdc_leading_ones_ul(unsigned long int value);
unsigned int stdc_leading_ones_ull(unsigned long long int value);
#define stdc_leading_ones(value)                                             \
  _Generic((value),                                                          \
      unsigned char: stdc_leading_ones_uc,                                   \
      unsigned short: stdc_leading_ones_us,                                  \
      unsigned int: stdc_leading_ones_ui,                                    \
      unsigned long int: stdc_leading_ones_ul,                               \
      unsigned long long int: stdc_leading_ones_ull)(value)

unsigned int stdc_trailing_zeros_uc(unsigned char value);
unsigned int stdc_trailing_zeros_us(unsigned short value);
unsigned int stdc_trailing_zeros_ui(unsigned int value);
unsigned int stdc_trailing_zeros_ul(unsigned long int value);
unsigned int stdc_trailing_zeros_ull(unsigned long long int value);
#define stdc_trailing_zeros(value)                                           \
  _Generic((value),                                                          \
      unsigned char: stdc_trailing_zeros_uc,                                 \
      unsigned short: stdc_trailing_zeros_us,                                \
      unsigned int: stdc_trailing_zeros_ui,                                  \
      unsigned long int: stdc_trailing_zeros_ul,                             \
      unsigned long long int: stdc_trailing_zeros_ull)(value)

unsigned int stdc_trailing_ones_uc(unsigned char value);
unsigned int stdc_trailing_ones_us(unsigned short value);
unsigned int stdc_trailing_ones_ui(unsigned int value);
unsigned int stdc_trailing_ones_ul(unsigned long int value);
unsigned int stdc_trailing_ones_ull(unsigned long long int value);
#define stdc_trailing_ones(value)                                            \
  _Generic((value),                                                          \
      unsigned char: stdc_trailing_ones_uc,                                  \
      unsigned short: stdc_trailing_ones_us,                                 \
      unsigned int: stdc_trailing_ones_ui,                                   \
      unsigned long int: stdc_trailing_ones_ul,                              \
      unsigned long long int: stdc_trailing_ones_ull)(value)

unsigned int stdc_first_leading_zero_uc(unsigned char value);
unsigned int stdc_first_leading_zero_us(unsigned short value);
unsigned int stdc_first_leading_zero_ui(unsigned int value);
unsigned int stdc_first_leading_zero_ul(unsigned long int value);
unsigned int stdc_first_leading_zero_ull(unsigned long long int value);
#define stdc_first_leading_zero(value)                                       \
  _Generic((value),                                                          \
      unsigned char: stdc_first_leading_zero_uc,                             \
      unsigned short: stdc_first_leading_zero_us,                            \
      unsigned int: stdc_first_leading_zero_ui,                              \
      unsigned long int: stdc_first_leading_zero_ul,                         \
      unsigned long long int: stdc_first_leading_zero_ull)(value)

unsigned int stdc_first_leading_one_uc(unsigned char value);
unsigned int stdc_first_leading_one_us(unsigned short value);
unsigned int stdc_first_leading_one_ui(unsigned int value);
unsigned int stdc_first_leading_one_ul(unsigned long int value);
unsigned int stdc_first_leading_one_ull(unsigned long long int value);
#define stdc_first_leading_one(value)                                        \
  _Generic((value),                                                          \
      unsigned char: stdc_first_leading_one_uc,                              \
      unsigned short: stdc_first_leading_one_us,                             \
      unsigned int: stdc_first_leading_one_ui,                               \
      unsigned long int: stdc_first_leading_one_ul,                          \
      unsigned long long int: stdc_first_leading_one_ull)(value)

unsigned int stdc_first_trailing_zero_uc(unsigned char value);
unsigned int stdc_first_trailing_zero_us(unsigned short value);
unsigned int stdc_first_trailing_zero_ui(unsigned int value);
unsigned int stdc_first_trailing_zero_ul(unsigned long int value);
unsigned int stdc_first_trailing_zero_ull(unsigned long long int value);
#define stdc_first_trailing_zero(value)                                      \
  _Generic((value),                                                          \
      unsigned char: stdc_first_trailing_zero_uc,                            \
      unsigned short: stdc_first_trailing_zero_us,                           \
      unsigned int: stdc_first_trailing_zero_ui,                             \
      unsigned long int: stdc_first_trailing_zero_ul,                        \
      unsigned long long int: stdc_first_trailing_zero_ull)(value)

unsigned int stdc_first_trailing_one_uc(unsigned char value);
unsigned int stdc_first_trailing_one_us(unsigned short value);
unsigned int stdc_first_trailing_one_ui(unsigned int value);
unsigned int stdc_first_trailing_one_ul(unsigned long int value);
unsigned int stdc_first_trailing_one_ull(unsigned long long int value);
#define stdc_first_trailing_one(value)                                       \
  _Generic((value),                                                          \
      unsigned char: stdc_first_trailing_one_uc,                             \
      unsigned short: stdc_first_trailing_one_us,                            \
      unsigned int: stdc_first_trailing_one_ui,                              \
      unsigned long int: stdc_first_trailing_one_ul,                         \
      unsigned long long int: stdc_first_trailing_one_ull)(value)

unsigned int stdc_count_zeros_uc(unsigned char value);
unsigned int stdc_count_zeros_us(unsigned short value);
unsigned int stdc_count_zeros_ui(unsigned int value);
unsigned int stdc_count_zeros_ul(unsigned long int value);
unsigned int stdc_count_zeros_ull(unsigned long long int value);
#define stdc_count_zeros(value)                                              \
  _Generic((value),                                                          \
      unsigned char: stdc_count_zeros_uc,                                    \
      unsigned short: stdc_count_zeros_us,                                   \
      unsigned int: stdc_count_zeros_ui,                                     \
      unsigned long int: stdc_count_zeros_ul,                                \
      unsigned long long int: stdc_count_zeros_ull)(value)

unsigned int stdc_count_ones_uc(unsigned char value);
unsigned int stdc_count_ones_us(unsigned short value);
unsigned int stdc_count_ones_ui(unsigned int value);
unsigned int stdc_count_ones_ul(unsigned long int value);
unsigned int stdc_count_ones_ull(unsigned long long int value);
#define stdc_count_ones(value)                                               \
  _Generic((value),                                                          \
      unsigned char: stdc_count_ones_uc,                                     \
      unsigned short: stdc_count_ones_us,                                    \
      unsigned int: stdc_count_ones_ui,                                      \
      unsigned long int: stdc_count_ones_ul,                                 \
      unsigned long long int: stdc_count_ones_ull)(value)

bool stdc_has_single_bit_uc(unsigned char value);
bool stdc_has_single_bit_us(unsigned short value);
bool stdc_has_single_bit_ui(unsigned int value);
bool stdc_has_single_bit_ul(unsigned long int value);
bool stdc_has_single_bit_ull(unsigned long long int value);
#define stdc_has_single_bit(value)                                           \
  _Generic((value),                                                          \
      unsigned char: stdc_has_single_bit_uc,                                 \
      unsigned short: stdc_has_single_bit_us,                                \
      unsigned int: stdc_has_single_bit_ui,                                  \
      unsigned long int: stdc_has_single_bit_ul,                             \
      unsigned long long int: stdc_has_single_bit_ull)(value)

unsigned int stdc_bit_width_uc(unsigned char value);
unsigned int stdc_bit_width_us(unsigned short value);
unsigned int stdc_bit_width_ui(unsigned int value);
unsigned int stdc_bit_width_ul(unsigned long int value);
unsigned int stdc_bit_width_ull(unsigned long long int value);
#define stdc_bit_width(value)                                                \
  _Generic((value),                                                          \
      unsigned char: stdc_bit_width_uc,                                      \
      unsigned short: stdc_bit_width_us,                                     \
      unsigned int: stdc_bit_width_ui,                                       \
      unsigned long int: stdc_bit_width_ul,                                  \
      unsigned long long int: stdc_bit_width_ull)(value)

unsigned char          stdc_bit_floor_uc(unsigned char value);
unsigned short         stdc_bit_floor_us(unsigned short value);
unsigned int           stdc_bit_floor_ui(unsigned int value);
unsigned long int      stdc_bit_floor_ul(unsigned long int value);
unsigned long long int stdc_bit_floor_ull(unsigned long long int value);
#define stdc_bit_floor(value)                                                \
  _Generic((value),                                                          \
      unsigned char: stdc_bit_floor_uc,                                      \
      unsigned short: stdc_bit_floor_us,                                     \
      unsigned int: stdc_bit_floor_ui,                                       \
      unsigned long int: stdc_bit_floor_ul,                                  \
      unsigned long long int: stdc_bit_floor_ull)(value)

unsigned char          stdc_bit_ceil_uc(unsigned char value);
unsigned short         stdc_bit_ceil_us(unsigned short value);
unsigned int           stdc_bit_ceil_ui(unsigned int value);
unsigned long int      stdc_bit_ceil_ul(unsigned long int value);
unsigned long long int stdc_bit_ceil_ull(unsigned long long int value);
#define stdc_bit_ceil(value)                                                 \
  _Generic((value),                                                          \
      unsigned char: stdc_bit_ceil_uc,                                       \
      unsigned short: stdc_bit_ceil_us,                                      \
      unsigned int: stdc_bit_ceil_ui,                                        \
      unsigned long int: stdc_bit_ceil_ul,                                   \
      unsigned long long int: stdc_bit_ceil_ull)(value)

unsigned char  stdc_rotate_left_uc(unsigned char value, unsigned int count);
unsigned short stdc_rotate_left_us(unsigned short value, unsigned int count);
unsigned int   stdc_rotate_left_ui(unsigned int value, unsigned int count);
unsigned long  stdc_rotate_left_ul(unsigned long value, unsigned int count);
unsigned long long stdc_rotate_left_ull(unsigned long long value,
                                        unsigned int       count);
#define stdc_rotate_left(value, count)                                       \
  _Generic((value),                                                          \
      unsigned char: stdc_rotate_left_uc,                                    \
      unsigned short: stdc_rotate_left_us,                                   \
      unsigned int: stdc_rotate_left_ui,                                     \
      unsigned long int: stdc_rotate_left_ul,                                \
      unsigned long long int: stdc_rotate_left_ull)(value, count)

unsigned char  stdc_rotate_right_uc(unsigned char value, unsigned int count);
unsigned short stdc_rotate_right_us(unsigned short value, unsigned int count);
unsigned int   stdc_rotate_right_ui(unsigned int value, unsigned int count);
unsigned long  stdc_rotate_right_ul(unsigned long value, unsigned int count);
unsigned long long stdc_rotate_right_ull(unsigned long long value,
                                         unsigned int       count);
#define stdc_rotate_right(value, count)                                      \
  _Generic((value),                                                          \
      unsigned char: stdc_rotate_right_uc,                                   \
      unsigned short: stdc_rotate_right_us,                                  \
      unsigned int: stdc_rotate_right_ui,                                    \
      unsigned long int: stdc_rotate_right_ul,                               \
      unsigned long long int: stdc_rotate_right_ull)(value, count)

#endif
