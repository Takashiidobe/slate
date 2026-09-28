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
// DEFAULT-NEXT:     type @type0 Field_c = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type1 Field_c = @type0;
// DEFAULT-NEXT:     type @type2 BitField_c = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type3 BitField_c = @type2;
// DEFAULT-NEXT:     type @type4 Field_sc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type5 Field_sc = @type4;
// DEFAULT-NEXT:     type @type6 BitField_sc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i8 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type7 BitField_sc = @type6;
// DEFAULT-NEXT:     type @type8 Field_uc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type9 Field_uc = @type8;
// DEFAULT-NEXT:     type @type10 BitField_uc = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u8 : 8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1], bit_offsets=[None, Some(8)], bit_units=[(1, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type11 BitField_uc = @type10;
// DEFAULT-NEXT:     type @type12 Field_s = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type13 Field_s = @type12;
// DEFAULT-NEXT:     type @type14 BitField_s = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16 : 16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type15 BitField_s = @type14;
// DEFAULT-NEXT:     type @type16 Field_ss = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type17 Field_ss = @type16;
// DEFAULT-NEXT:     type @type18 BitField_ss = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i16 : 16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type19 BitField_ss = @type18;
// DEFAULT-NEXT:     type @type20 Field_us = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type21 Field_us = @type20;
// DEFAULT-NEXT:     type @type22 BitField_us = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u16 : 16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type23 BitField_us = @type22;
// DEFAULT-NEXT:     type @type24 Field_i = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type25 Field_i = @type24;
// DEFAULT-NEXT:     type @type26 BitField_i = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type27 BitField_i = @type26;
// DEFAULT-NEXT:     type @type28 Field_si = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type29 Field_si = @type28;
// DEFAULT-NEXT:     type @type30 BitField_si = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type31 BitField_si = @type30;
// DEFAULT-NEXT:     type @type32 Field_ui = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type33 Field_ui = @type32;
// DEFAULT-NEXT:     type @type34 BitField_ui = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 4)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type35 BitField_ui = @type34;
// DEFAULT-NEXT:     type @type36 Field_l = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type37 Field_l = @type36;
// DEFAULT-NEXT:     type @type38 BitField_l = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type39 BitField_l = @type38;
// DEFAULT-NEXT:     type @type40 Field_sl = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type41 Field_sl = @type40;
// DEFAULT-NEXT:     type @type42 BitField_sl = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type43 BitField_sl = @type42;
// DEFAULT-NEXT:     type @type44 Field_ul = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type45 Field_ul = @type44;
// DEFAULT-NEXT:     type @type46 BitField_ul = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type47 BitField_ul = @type46;
// DEFAULT-NEXT:     type @type48 Field_ll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type49 Field_ll = @type48;
// DEFAULT-NEXT:     type @type50 BitField_ll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type51 BitField_ll = @type50;
// DEFAULT-NEXT:     type @type52 Field_sll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type53 Field_sll = @type52;
// DEFAULT-NEXT:     type @type54 BitField_sll = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: i64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type55 BitField_sll = @type54;
// DEFAULT-NEXT:     type @type56 Field_ull = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type57 Field_ull = @type56;
// DEFAULT-NEXT:     type @type58 BitField_ull = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: u64 : 64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type59 BitField_ull = @type58;
// DEFAULT-NEXT:     global %2 fail: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %75 .str75: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %80 .str80: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %91 .str91: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %96 .str96: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %139 .str139: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%64 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %65
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %66
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%67)), array_decay<ptr<i8>, length=Some(5)>(%68));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%69)), array_decay<ptr<i8>, length=Some(5)>(%70));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %71
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%72)), array_decay<ptr<i8>, length=Some(12)>(%73));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%74)), array_decay<ptr<i8>, length=Some(12)>(%75));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %76
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%77)), array_decay<ptr<i8>, length=Some(14)>(%78));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%79)), array_decay<ptr<i8>, length=Some(14)>(%80));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %81
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %82
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%83)), array_decay<ptr<i8>, length=Some(6)>(%84));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%85)), array_decay<ptr<i8>, length=Some(6)>(%86));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %87
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%88)), array_decay<ptr<i8>, length=Some(13)>(%89));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%90)), array_decay<ptr<i8>, length=Some(13)>(%91));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %92
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%93)), array_decay<ptr<i8>, length=Some(15)>(%94));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%95)), array_decay<ptr<i8>, length=Some(15)>(%96));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %97
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %98
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%99)), array_decay<ptr<i8>, length=Some(4)>(%100));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%101)), array_decay<ptr<i8>, length=Some(4)>(%102));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %103
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%104)), array_decay<ptr<i8>, length=Some(11)>(%105));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%106)), array_decay<ptr<i8>, length=Some(11)>(%107));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %108
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%109)), array_decay<ptr<i8>, length=Some(13)>(%110));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%111)), array_decay<ptr<i8>, length=Some(13)>(%112));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %113
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %114
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%115)), array_decay<ptr<i8>, length=Some(5)>(%116));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%117)), array_decay<ptr<i8>, length=Some(5)>(%118));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %119
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%120)), array_decay<ptr<i8>, length=Some(12)>(%121));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%122)), array_decay<ptr<i8>, length=Some(12)>(%123));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %124
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%125)), array_decay<ptr<i8>, length=Some(14)>(%126));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%127)), array_decay<ptr<i8>, length=Some(14)>(%128));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %129
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %130
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%131)), array_decay<ptr<i8>, length=Some(10)>(%132));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%133)), array_decay<ptr<i8>, length=Some(10)>(%134));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %135
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%136)), array_decay<ptr<i8>, length=Some(17)>(%137));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%138)), array_decay<ptr<i8>, length=Some(17)>(%139));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %140
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%141)), array_decay<ptr<i8>, length=Some(19)>(%142));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%143)), array_decay<ptr<i8>, length=Some(19)>(%144));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
