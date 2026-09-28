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
// DEFAULT-NEXT:     global %1 fail: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %78 .str78: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %82 .str82: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %84 .str84: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %85 .str85: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %87 .str87: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %89 .str89: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %93 .str93: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %94 .str94: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %98 .str98: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %138 .str138: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([115, 105, 122, 101, 111, 102, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %141 .str141: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([95, 95, 97, 108, 105, 103, 110, 111, 102, 95, 95, 32, 37, 115, 32, 102, 97, 105, 108, 101, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %143 .str143: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%63 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %64
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %65
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%66)), array_decay<ptr<i8>, length=Some(5)>(%67));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%68)), array_decay<ptr<i8>, length=Some(5)>(%69));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %70
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%71)), array_decay<ptr<i8>, length=Some(12)>(%72));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%73)), array_decay<ptr<i8>, length=Some(12)>(%74));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %75
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%76)), array_decay<ptr<i8>, length=Some(14)>(%77));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%78)), array_decay<ptr<i8>, length=Some(14)>(%79));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %80
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %81
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%82)), array_decay<ptr<i8>, length=Some(6)>(%83));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%84)), array_decay<ptr<i8>, length=Some(6)>(%85));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %86
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%87)), array_decay<ptr<i8>, length=Some(13)>(%88));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%89)), array_decay<ptr<i8>, length=Some(13)>(%90));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %91
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%92)), array_decay<ptr<i8>, length=Some(15)>(%93));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%94)), array_decay<ptr<i8>, length=Some(15)>(%95));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %96
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %97
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%98)), array_decay<ptr<i8>, length=Some(4)>(%99));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%100)), array_decay<ptr<i8>, length=Some(4)>(%101));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %102
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%103)), array_decay<ptr<i8>, length=Some(11)>(%104));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%105)), array_decay<ptr<i8>, length=Some(11)>(%106));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %107
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%108)), array_decay<ptr<i8>, length=Some(13)>(%109));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%110)), array_decay<ptr<i8>, length=Some(13)>(%111));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %112
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %113
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%114)), array_decay<ptr<i8>, length=Some(5)>(%115));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%116)), array_decay<ptr<i8>, length=Some(5)>(%117));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %118
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%119)), array_decay<ptr<i8>, length=Some(12)>(%120));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%121)), array_decay<ptr<i8>, length=Some(12)>(%122));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %123
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%124)), array_decay<ptr<i8>, length=Some(14)>(%125));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%126)), array_decay<ptr<i8>, length=Some(14)>(%127));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %128
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %129
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%130)), array_decay<ptr<i8>, length=Some(10)>(%131));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%132)), array_decay<ptr<i8>, length=Some(10)>(%133));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %134
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%135)), array_decay<ptr<i8>, length=Some(17)>(%136));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%137)), array_decay<ptr<i8>, length=Some(17)>(%138));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %139
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%140)), array_decay<ptr<i8>, length=Some(19)>(%141));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:                                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%142)), array_decay<ptr<i8>, length=Some(19)>(%143));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
