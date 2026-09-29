/* bf-ms-layout.c */

/* Test for MS bitfield layout */
/* Adapted from Donn Terry <donnte@microsoft.com> testcase
   posted to GCC-patches
   http://gcc.gnu.org/ml/gcc-patches/2000-08/msg00577.html */

/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-D_TEST_MS_LAYOUT" } */
/* This test uses the attribute instead of the command line option.  */

#include <stddef.h>
#include <string.h>

extern void abort();

#pragma pack(8)

#ifdef __GNUC__
#define ATTR __attribute__ ((ms_struct))
#endif

struct one {
  int d;
  unsigned char a;
  unsigned short b:7;
  char c;
} ATTR;

struct two {
  int d;
  unsigned char a;
  unsigned int b:7;
  char c;
} ATTR;

struct three {
  short d;
  unsigned short a:3;
  unsigned short b:9;
  unsigned char c:7;
} ATTR;


/* Bitfields of size 0 have some truly odd behaviors. */

struct four {
  unsigned short a:3;
  unsigned short b:9;
  unsigned int :0;  /* forces struct alignment to int */
  unsigned char c:7;
} ATTR;

struct five {
  char a;
  int :0;        /* ignored; prior field is not a bitfield. */
  char b;
  char c;
} ATTR;

struct six {
  char a :8;
  int :0;	/* not ignored; prior field IS a bitfield, causes
		   struct alignment as well. */
  char b;
  char c;
} ATTR;

struct seven {
  char a:8;
  char :0;
  int  :0;	/* Ignored; prior field is zero size bitfield. */
  char b;
  char c;
} ATTR;

struct eight { /* ms size 4 */
  short b:3;
  char  c;
} ATTR;

#ifdef _MSC_VER
#define LONGLONG __int64
#else
#define LONGLONG long long
#endif

union nine {   /* ms size 8 */
  LONGLONG a:3;
  char  c;
} ATTR;

struct ten {   /* ms size 16 */
  LONGLONG a:3;
  LONGLONG b:3;
  char  c;
} ATTR;


#define val(s,f) (s.f)

#define check_struct(_X) \
{ \
  if (sizeof (struct _X) != exp_sizeof_##_X )	\
    abort();					\
  memcpy(&test_##_X, filler, sizeof(test_##_X));\
  if (val(test_##_X,c) != exp_##_X##_c) 	\
     abort();					\
}

#define check_union(_X) \
{ \
  if (sizeof (union _X) != exp_sizeof_##_X )	\
    abort();                                    \
  memcpy(&test_##_X, filler, sizeof(test_##_X));\
  if (val(test_##_X,c) != exp_##_X##_c) 	\
     abort();					\
}

#define check_struct_size(_X) \
{ \
  if (sizeof (struct _X) != exp_sizeof_##_X )	\
    abort();                                    \
}

#define check_struct_off(_X) \
{ \
  memcpy(&test_##_X, filler, sizeof(test_##_X));\
  if (val(test_##_X,c) != exp_##_X##_c) 	\
    abort();                                    \
}

#define check_union_size(_X) \
{ \
  if (sizeof (union _X) != exp_sizeof_##_X )	\
    abort();                                    \
}

#define check_union_off(_X) \
{ \
  memcpy(&test_##_X, filler, sizeof(test_##_X));\
  if (val(test_##_X,c) != exp_##_X##_c) 	\
    abort();                                    \
}

int main(){

  unsigned char filler[16];
  struct one test_one;
  struct two test_two;
  struct three test_three;
  struct four test_four;
  struct five test_five;
  struct six test_six;
  struct seven test_seven;
  struct eight test_eight;
  union nine test_nine;
  struct ten test_ten;

#if defined (_TEST_MS_LAYOUT) || defined (_MSC_VER)
  size_t exp_sizeof_one = 12;
  size_t exp_sizeof_two = 16;
  size_t exp_sizeof_three =6;
  size_t exp_sizeof_four = 8;
  size_t exp_sizeof_five = 3;
  size_t exp_sizeof_six = 8;
  size_t exp_sizeof_seven = 3;
  size_t exp_sizeof_eight = 4;
  size_t exp_sizeof_nine = 8;
  size_t exp_sizeof_ten = 16;

  unsigned char exp_one_c = 8;
  unsigned char exp_two_c  = 12;
  unsigned char exp_three_c = 4;
  unsigned char exp_four_c = 4;
  char exp_five_c = 2;
  char exp_six_c = 5;
  char exp_seven_c = 2;
  char exp_eight_c = 2;
  char exp_nine_c = 0;
  char exp_ten_c = 8;

#else /* testing -mno-ms-bitfields */

  size_t exp_sizeof_one = 8;
  size_t exp_sizeof_two = 8;
  size_t exp_sizeof_three = 6;
  size_t exp_sizeof_four = 6;
  size_t exp_sizeof_five = 6;
  size_t exp_sizeof_six = 6;
  size_t exp_sizeof_seven = 6;
  size_t exp_sizeof_eight = 2;
  size_t exp_sizeof_nine = 8;
  size_t exp_sizeof_ten = 8;

  unsigned short exp_one_c = 6;
  unsigned int exp_two_c  = 6;
  unsigned char exp_three_c = 64;
  unsigned char exp_four_c = 4;
  char exp_five_c = 5;
  char exp_six_c = 5;
  char exp_seven_c = 5;
  char exp_eight_c = 1;
  char exp_nine_c = 0;
  char exp_ten_c = 1;

#endif

  unsigned char i;
  for ( i = 0; i < 16; i++ )
    filler[i] = i;

  check_struct_off (one);
  check_struct_off (two);
  check_struct_off (three);
  check_struct_off (four);
  check_struct_off (five);
  check_struct_off (six);
  check_struct_off (seven);
  check_struct_off (eight);
  check_union_off (nine);
  check_struct_off (ten);

  check_struct_size (one);
  check_struct_size (two);
  check_struct_size (three);
  check_struct_size (four);
  check_struct_size (five);
  check_struct_size (six);
  check_struct_size (seven);
  check_struct_size (eight);
  check_union_size (nine);
  check_struct_size (ten);

  return 0;
};

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT _TEST_MS_LAYOUT

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_one:[0-9]+]] one = struct {
// DEFAULT-NEXT:         field0 d: i32;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u16 : 7;
// DEFAULT-NEXT:         field3 c: i8;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 6, 8], bit_offsets=[None, None, Some(48), None], bit_units=[(6, 2)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_two:[0-9]+]] two = struct {
// DEFAULT-NEXT:         field0 d: i32;
// DEFAULT-NEXT:         field1 a: u8;
// DEFAULT-NEXT:         field2 b: u32 : 7;
// DEFAULT-NEXT:         field3 c: i8;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12], bit_offsets=[None, None, Some(64), None], bit_units=[(8, 4)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_three:[0-9]+]] three = struct {
// DEFAULT-NEXT:         field0 d: i16;
// DEFAULT-NEXT:         field1 a: u16 : 3;
// DEFAULT-NEXT:         field2 b: u16 : 9;
// DEFAULT-NEXT:         field3 c: u8 : 7;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 2, 2, 4], bit_offsets=[None, Some(16), Some(19), Some(32)], bit_units=[(2, 2), (4, 1)], field_units=[None, Some(0), Some(0), Some(1)]];
// DEFAULT-NEXT:     type @type[[TYPE_four:[0-9]+]] four = struct {
// DEFAULT-NEXT:         field0 a: u16 : 3;
// DEFAULT-NEXT:         field1 b: u16 : 9;
// DEFAULT-NEXT:         field2 <anonymous>: u32 : 0;
// DEFAULT-NEXT:         field3 c: u8 : 7;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 4, 4], bit_offsets=[Some(0), Some(3), Some(32), Some(32)], bit_units=[(0, 2), (4, 1)], field_units=[Some(0), Some(0), None, Some(1)]];
// DEFAULT-NEXT:     type @type[[TYPE_five:[0-9]+]] five = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field2 b: i8;
// DEFAULT-NEXT:         field3 c: i8;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1, 1, 2], bit_offsets=[None, Some(8), None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_six:[0-9]+]] six = struct {
// DEFAULT-NEXT:         field0 a: i8 : 8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field2 b: i8;
// DEFAULT-NEXT:         field3 c: i8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4, 5], bit_offsets=[Some(0), Some(32), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_seven:[0-9]+]] seven = struct {
// DEFAULT-NEXT:         field0 a: i8 : 8;
// DEFAULT-NEXT:         field1 <anonymous>: i8 : 0;
// DEFAULT-NEXT:         field2 <anonymous>: i32 : 0;
// DEFAULT-NEXT:         field3 b: i8;
// DEFAULT-NEXT:         field4 c: i8;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1, 1, 1, 2], bit_offsets=[Some(0), Some(8), Some(8), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_eight:[0-9]+]] eight = struct {
// DEFAULT-NEXT:         field0 b: i16 : 3;
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[Some(0), None], bit_units=[(0, 2)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_nine:[0-9]+]] nine = union {
// DEFAULT-NEXT:         field0 a: i64 : 3;
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 0], bit_offsets=[Some(0), None], bit_units=[(0, 8)], field_units=[Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_ten:[0-9]+]] ten = struct {
// DEFAULT-NEXT:         field0 a: i64 : 3;
// DEFAULT-NEXT:         field1 b: i64 : 3;
// DEFAULT-NEXT:         field2 c: i8;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 8], bit_offsets=[Some(0), Some(3), None], bit_units=[(0, 8)], field_units=[Some(0), Some(0), None]];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_filler:[0-9]+]] filler: array<u8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_test_one:[0-9]+]] test_one: @type[[TYPE_one]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_two:[0-9]+]] test_two: @type[[TYPE_two]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_three:[0-9]+]] test_three: @type[[TYPE_three]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_four:[0-9]+]] test_four: @type[[TYPE_four]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_five:[0-9]+]] test_five: @type[[TYPE_five]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_six:[0-9]+]] test_six: @type[[TYPE_six]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_seven:[0-9]+]] test_seven: @type[[TYPE_seven]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_eight:[0-9]+]] test_eight: @type[[TYPE_eight]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_nine:[0-9]+]] test_nine: @type[[TYPE_nine]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_test_ten:[0-9]+]] test_ten: @type[[TYPE_ten]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_one:[0-9]+]] exp_sizeof_one: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(12)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_two:[0-9]+]] exp_sizeof_two: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(16)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_three:[0-9]+]] exp_sizeof_three: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(6)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_four:[0-9]+]] exp_sizeof_four: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_five:[0-9]+]] exp_sizeof_five: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_six:[0-9]+]] exp_sizeof_six: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_seven:[0-9]+]] exp_sizeof_seven: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_eight:[0-9]+]] exp_sizeof_eight: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_nine:[0-9]+]] exp_sizeof_nine: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE_exp_sizeof_ten:[0-9]+]] exp_sizeof_ten: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(16)));
// DEFAULT-NEXT:         let %[[VALUE_exp_one_c:[0-9]+]] exp_one_c: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE_exp_two_c:[0-9]+]] exp_two_c: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:         let %[[VALUE_exp_three_c:[0-9]+]] exp_three_c: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_exp_four_c:[0-9]+]] exp_four_c: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_exp_five_c:[0-9]+]] exp_five_c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_exp_six_c:[0-9]+]] exp_six_c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_exp_seven_c:[0-9]+]] exp_seven_c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_exp_eight_c:[0-9]+]] exp_eight_c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_exp_nine_c:[0-9]+]] exp_nine_c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_exp_ten_c:[0-9]+]] exp_ten_c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(8));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u8 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u8>(%[[VALUE_i]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_i]]))), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE1]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(%[[VALUE_i]], read<u8>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_i]]))))), read<u8>(%[[VALUE_i]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_one]]>>(%[[VALUE_test_one]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(12));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_test_one]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_exp_one_c]]))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_two]]>>(%[[VALUE_test_two]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(16));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_test_two]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_exp_two_c]]))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_three]]>>(%[[VALUE_test_three]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(6));
// DEFAULT-NEXT:             if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield3<unit=1, bytes=4..5, bits=0..7>(%[[VALUE_test_three]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_exp_three_c]]))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_four]]>>(%[[VALUE_test_four]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(8));
// DEFAULT-NEXT:             if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield3<unit=1, bytes=4..5, bits=0..7>(%[[VALUE_test_four]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_exp_four_c]]))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_five]]>>(%[[VALUE_test_five]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(3));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_test_five]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_exp_five_c]])))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_six]]>>(%[[VALUE_test_six]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(8));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_test_six]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_exp_six_c]])))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_seven]]>>(%[[VALUE_test_seven]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(3));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field4(%[[VALUE_test_seven]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_exp_seven_c]])))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_eight]]>>(%[[VALUE_test_eight]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(4));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_test_eight]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_exp_eight_c]])))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_nine]]>>(%[[VALUE_test_nine]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(8));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_test_nine]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_exp_nine_c]])))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_ten]]>>(%[[VALUE_test_ten]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(16)>(%[[VALUE_filler]])), const<u64>(16));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(field2(%[[VALUE_test_ten]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_exp_ten_c]])))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(12), read<u64>(%[[VALUE_exp_sizeof_one]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(16), read<u64>(%[[VALUE_exp_sizeof_two]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(6), read<u64>(%[[VALUE_exp_sizeof_three]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(8), read<u64>(%[[VALUE_exp_sizeof_four]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(3), read<u64>(%[[VALUE_exp_sizeof_five]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(8), read<u64>(%[[VALUE_exp_sizeof_six]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(3), read<u64>(%[[VALUE_exp_sizeof_seven]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(4), read<u64>(%[[VALUE_exp_sizeof_eight]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(8), read<u64>(%[[VALUE_exp_sizeof_nine]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<u64>(const<u64>(16), read<u64>(%[[VALUE_exp_sizeof_ten]]))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
