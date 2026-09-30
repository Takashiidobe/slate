/* Verify that GCC's internal notions of types in <stdint.h> agree
   with any system header (which GCC will use by default for hosted
   compilations).  */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */
/* { dg-additional-options "-DSIGNAL_SUPPRESS" { target { ! signal } } } */

#include <stdint.h>
#ifndef SIGNAL_SUPPRESS
#include <signal.h>
#endif

#define CHECK_TYPES(TYPE1, TYPE2) \
  do { TYPE1 a; TYPE2 *b = &a; TYPE2 c; TYPE1 *d = &c; } while (0)

void
check_types (void)
{
#ifdef __INT8_TYPE__
  CHECK_TYPES(__INT8_TYPE__, int8_t);
#endif
#ifdef __INT16_TYPE__
  CHECK_TYPES(__INT16_TYPE__, int16_t);
#endif
#ifdef __INT32_TYPE__
  CHECK_TYPES(__INT32_TYPE__, int32_t);
#endif
#ifdef __INT64_TYPE__
  CHECK_TYPES(__INT64_TYPE__, int64_t);
#endif
#ifdef __UINT8_TYPE__
  CHECK_TYPES(__UINT8_TYPE__, uint8_t);
#endif
#ifdef __UINT16_TYPE__
  CHECK_TYPES(__UINT16_TYPE__, uint16_t);
#endif
#ifdef __UINT32_TYPE__
  CHECK_TYPES(__UINT32_TYPE__, uint32_t);
#endif
#ifdef __UINT64_TYPE__
  CHECK_TYPES(__UINT64_TYPE__, uint64_t);
#endif
  CHECK_TYPES(__INT_LEAST8_TYPE__, int_least8_t);
  CHECK_TYPES(__INT_LEAST16_TYPE__, int_least16_t);
  CHECK_TYPES(__INT_LEAST32_TYPE__, int_least32_t);
  CHECK_TYPES(__INT_LEAST64_TYPE__, int_least64_t);
  CHECK_TYPES(__UINT_LEAST8_TYPE__, uint_least8_t);
  CHECK_TYPES(__UINT_LEAST16_TYPE__, uint_least16_t);
  CHECK_TYPES(__UINT_LEAST32_TYPE__, uint_least32_t);
  CHECK_TYPES(__UINT_LEAST64_TYPE__, uint_least64_t);
  CHECK_TYPES(__INT_FAST8_TYPE__, int_fast8_t);
  CHECK_TYPES(__INT_FAST16_TYPE__, int_fast16_t);
  CHECK_TYPES(__INT_FAST32_TYPE__, int_fast32_t);
  CHECK_TYPES(__INT_FAST64_TYPE__, int_fast64_t);
  CHECK_TYPES(__UINT_FAST8_TYPE__, uint_fast8_t);
  CHECK_TYPES(__UINT_FAST16_TYPE__, uint_fast16_t);
  CHECK_TYPES(__UINT_FAST32_TYPE__, uint_fast32_t);
  CHECK_TYPES(__UINT_FAST64_TYPE__, uint_fast64_t);
#ifdef __INTPTR_TYPE__
  CHECK_TYPES(__INTPTR_TYPE__, intptr_t);
#endif
#ifdef __UINTPTR_TYPE__
  CHECK_TYPES(__UINTPTR_TYPE__, uintptr_t);
#endif
  CHECK_TYPES(__INTMAX_TYPE__, intmax_t);
  CHECK_TYPES(__UINTMAX_TYPE__, uintmax_t);
#ifndef SIGNAL_SUPPRESS
  CHECK_TYPES(__SIG_ATOMIC_TYPE__, sig_atomic_t);
#endif
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type[[TYPE___int8_t:[0-9]+]] __int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___int16_t:[0-9]+]] __int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___int64_t:[0-9]+]] __int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___int_least8_t:[0-9]+]] __int_least8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least8_t:[0-9]+]] __uint_least8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___int_least16_t:[0-9]+]] __int_least16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least16_t:[0-9]+]] __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___int_least32_t:[0-9]+]] __int_least32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least32_t:[0-9]+]] __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___int_least64_t:[0-9]+]] __int_least64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least64_t:[0-9]+]] __uint_least64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___intmax_t:[0-9]+]] __intmax_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___uintmax_t:[0-9]+]] __uintmax_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___sig_atomic_t:[0-9]+]] __sig_atomic_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int8_t:[0-9]+]] int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_int16_t:[0-9]+]] int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int64_t:[0-9]+]] int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint16_t:[0-9]+]] uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_int_least8_t:[0-9]+]] int_least8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_int_least16_t:[0-9]+]] int_least16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE_int_least32_t:[0-9]+]] int_least32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int_least64_t:[0-9]+]] int_least64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least8_t:[0-9]+]] uint_least8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least16_t:[0-9]+]] uint_least16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least32_t:[0-9]+]] uint_least32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least64_t:[0-9]+]] uint_least64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_int_fast8_t:[0-9]+]] int_fast8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_int_fast16_t:[0-9]+]] int_fast16_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_int_fast32_t:[0-9]+]] int_fast32_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_int_fast64_t:[0-9]+]] int_fast64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint_fast8_t:[0-9]+]] uint_fast8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint_fast16_t:[0-9]+]] uint_fast16_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uint_fast32_t:[0-9]+]] uint_fast32_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uint_fast64_t:[0-9]+]] uint_fast64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_intptr_t:[0-9]+]] intptr_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_intmax_t:[0-9]+]] intmax_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uintmax_t:[0-9]+]] uintmax_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_sig_atomic_t:[0-9]+]] sig_atomic_t = i32;
// DEFAULT-NEXT:     fn %[[VALUE_check_types:[0-9]+]] @check_types() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b:[0-9]+]] b: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d:[0-9]+]] d: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_c]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_2:[0-9]+]] a: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_2:[0-9]+]] b: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_2:[0-9]+]] c: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_2:[0-9]+]] d: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_3:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_3:[0-9]+]] b: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_a_3]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_3:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_3:[0-9]+]] d: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_c_3]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_4:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_4:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_4]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_4:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_4:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_4]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_5:[0-9]+]] a: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_5:[0-9]+]] b: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%[[VALUE_a_5]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_5:[0-9]+]] c: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_5:[0-9]+]] d: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%[[VALUE_c_5]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_6:[0-9]+]] a: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_6:[0-9]+]] b: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%[[VALUE_a_6]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_6:[0-9]+]] c: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_6:[0-9]+]] d: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%[[VALUE_c_6]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_7:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_7:[0-9]+]] b: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%[[VALUE_a_7]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_7:[0-9]+]] c: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_7:[0-9]+]] d: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%[[VALUE_c_7]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_8:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_8:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_8]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_8:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_8:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_8]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_9:[0-9]+]] a: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_9:[0-9]+]] b: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_a_9]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_9:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_9:[0-9]+]] d: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_c_9]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_10:[0-9]+]] a: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_10:[0-9]+]] b: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%[[VALUE_a_10]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_10:[0-9]+]] c: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_10:[0-9]+]] d: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%[[VALUE_c_10]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_11:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_11:[0-9]+]] b: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_a_11]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_11:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_11:[0-9]+]] d: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_c_11]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_12:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_12:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_12]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_12:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_12:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_12]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_13:[0-9]+]] a: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_13:[0-9]+]] b: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%[[VALUE_a_13]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_13:[0-9]+]] c: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_13:[0-9]+]] d: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%[[VALUE_c_13]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_14:[0-9]+]] a: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_14:[0-9]+]] b: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%[[VALUE_a_14]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_14:[0-9]+]] c: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_14:[0-9]+]] d: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%[[VALUE_c_14]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_15:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_15:[0-9]+]] b: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%[[VALUE_a_15]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_15:[0-9]+]] c: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_15:[0-9]+]] d: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%[[VALUE_c_15]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_16:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_16:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_16]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_16:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_16:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_16]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_17:[0-9]+]] a: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_17:[0-9]+]] b: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_a_17]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_17:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_17:[0-9]+]] d: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_c_17]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_18:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_18:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_18]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_18:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_18:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_18]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_19:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_19:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_19]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_19:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_19:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_19]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_20:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_20:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_20]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_20:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_20:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_20]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_21:[0-9]+]] a: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_21:[0-9]+]] b: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%[[VALUE_a_21]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_21:[0-9]+]] c: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_21:[0-9]+]] d: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%[[VALUE_c_21]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_22:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_22:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_22]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_22:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_22:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_22]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_23:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_23:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_23]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_23:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_23:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_23]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_24:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_24:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_24]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_24:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_24:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_24]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_25:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_25:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_25]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_25:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_25:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_25]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_26:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_26:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_26]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_26:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_26:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_26]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_27:[0-9]+]] a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_27:[0-9]+]] b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_a_27]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_27:[0-9]+]] c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_27:[0-9]+]] d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%[[VALUE_c_27]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_28:[0-9]+]] a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_28:[0-9]+]] b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_a_28]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_28:[0-9]+]] c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_28:[0-9]+]] d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_c_28]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_29:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_b_29:[0-9]+]] b: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_a_29]]);
// DEFAULT-NEXT:                 let %[[VALUE_c_29:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_d_29:[0-9]+]] d: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_c_29]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
