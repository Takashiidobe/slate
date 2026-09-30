/* { dg-do run { target { pcc_bitfield_type_matters || default_packed } } } */
/* { dg-options "" } */

/* Check bitfields and non-bitfields are aligned & sized similarly.

   Copyright (C) 2002 Free Software Foundation Inc
   Contributed by Nathan Sidwell <nathan@codesourcery.com>
*/

#include <limits.h>
#include <stdio.h>

static int fail;

#define CHECK1(N, T) do { \
  typedef struct Field_##N { char c; T f; } Field_##N; \
  typedef struct BitField_##N { char c; T f : sizeof (T) * CHAR_BIT; } BitField_##N; \
  if (sizeof (Field_##N) != sizeof (BitField_##N)) { \
    fail = 1; printf ("sizeof %s failed\n", #T); \
  } \
  if (__alignof__ (Field_##N) != __alignof__ (BitField_##N)) { \
    fail = 1; printf ("__alignof__ %s failed\n", #T); \
  } \
} while (0)

#define CHECK(N, T) do { \
  CHECK1(N, T); \
  CHECK1 (s##N, signed T); \
  CHECK1 (u##N, unsigned T); \
} while (0)
 
int main ()
{
  
  CHECK (c, char);
  CHECK (s, short);
  CHECK (i, int);
  CHECK (l, long);
  CHECK (ll, long long);
  
  return fail;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_Field_c:[0-9]+]] Field_c = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_c_2:[0-9]+]] Field_c = @type[[TYPE_Field_c]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_c:[0-9]+]] BitField_c = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_c_2:[0-9]+]] BitField_c = @type[[TYPE_BitField_c]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_sc:[0-9]+]] Field_sc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_sc_2:[0-9]+]] Field_sc = @type[[TYPE_Field_sc]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_sc:[0-9]+]] BitField_sc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_sc_2:[0-9]+]] BitField_sc = @type[[TYPE_BitField_sc]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_uc:[0-9]+]] Field_uc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_uc_2:[0-9]+]] Field_uc = @type[[TYPE_Field_uc]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_uc:[0-9]+]] BitField_uc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u8 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_uc_2:[0-9]+]] BitField_uc = @type[[TYPE_BitField_uc]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_s:[0-9]+]] Field_s = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_s_2:[0-9]+]] Field_s = @type[[TYPE_Field_s]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_s:[0-9]+]] BitField_s = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16 : 16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_s_2:[0-9]+]] BitField_s = @type[[TYPE_BitField_s]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ss:[0-9]+]] Field_ss = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ss_2:[0-9]+]] Field_ss = @type[[TYPE_Field_ss]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ss:[0-9]+]] BitField_ss = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16 : 16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ss_2:[0-9]+]] BitField_ss = @type[[TYPE_BitField_ss]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_us:[0-9]+]] Field_us = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_us_2:[0-9]+]] Field_us = @type[[TYPE_Field_us]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_us:[0-9]+]] BitField_us = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u16 : 16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_us_2:[0-9]+]] BitField_us = @type[[TYPE_BitField_us]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_i:[0-9]+]] Field_i = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_i_2:[0-9]+]] Field_i = @type[[TYPE_Field_i]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_i:[0-9]+]] BitField_i = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_i_2:[0-9]+]] BitField_i = @type[[TYPE_BitField_i]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_si:[0-9]+]] Field_si = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_si_2:[0-9]+]] Field_si = @type[[TYPE_Field_si]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_si:[0-9]+]] BitField_si = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_si_2:[0-9]+]] BitField_si = @type[[TYPE_BitField_si]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ui:[0-9]+]] Field_ui = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ui_2:[0-9]+]] Field_ui = @type[[TYPE_Field_ui]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ui:[0-9]+]] BitField_ui = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ui_2:[0-9]+]] BitField_ui = @type[[TYPE_BitField_ui]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_l:[0-9]+]] Field_l = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_l_2:[0-9]+]] Field_l = @type[[TYPE_Field_l]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_l:[0-9]+]] BitField_l = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_l_2:[0-9]+]] BitField_l = @type[[TYPE_BitField_l]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_sl:[0-9]+]] Field_sl = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_sl_2:[0-9]+]] Field_sl = @type[[TYPE_Field_sl]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_sl:[0-9]+]] BitField_sl = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_sl_2:[0-9]+]] BitField_sl = @type[[TYPE_BitField_sl]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ul:[0-9]+]] Field_ul = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ul_2:[0-9]+]] Field_ul = @type[[TYPE_Field_ul]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ul:[0-9]+]] BitField_ul = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ul_2:[0-9]+]] BitField_ul = @type[[TYPE_BitField_ul]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ll:[0-9]+]] Field_ll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ll_2:[0-9]+]] Field_ll = @type[[TYPE_Field_ll]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ll:[0-9]+]] BitField_ll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ll_2:[0-9]+]] BitField_ll = @type[[TYPE_BitField_ll]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_sll:[0-9]+]] Field_sll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_sll_2:[0-9]+]] Field_sll = @type[[TYPE_Field_sll]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_sll:[0-9]+]] BitField_sll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_sll_2:[0-9]+]] BitField_sll = @type[[TYPE_BitField_sll]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ull:[0-9]+]] Field_ull = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Field_ull_2:[0-9]+]] Field_ull = @type[[TYPE_Field_ull]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ull:[0-9]+]] BitField_ull = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_BitField_ull_2:[0-9]+]] BitField_ull = @type[[TYPE_BitField_ull]];
// DEFAULT-NEXT:     global %[[VALUE_fail:[0-9]+]] fail: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str]])), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_3]])), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_5]])), array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_6]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_7]])), array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_8]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_9]])), array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_10]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_11]])), array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_12]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_13]])), array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_15]])), array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_16]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_17]])), array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_19]])), array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_20]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_21]])), array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_23]])), array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_25]])), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_26]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_27]])), array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_28]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_29]])), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_30]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_31]])), array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_32]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_33]])), array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_34]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_35]])), array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_36]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_37]])), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_38]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_39]])), array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_40]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_41]])), array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_42]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_43]])), array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_44]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_45]])), array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_46]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_47]])), array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_48]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_49]])), array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_50]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_51]])), array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_52]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_53]])), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_54]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_55]])), array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_56]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_57]])), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_58]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_fail]], const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_59]])), array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_60]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_fail]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
