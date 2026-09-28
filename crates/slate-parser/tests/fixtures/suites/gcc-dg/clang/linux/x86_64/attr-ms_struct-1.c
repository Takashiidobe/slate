/* Test for MS structure sizes.  */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-std=gnu99" } */

extern void abort ();

#define ATTR __attribute__((__ms_struct__))

#define size_struct_0 1
#define size_struct_1 4
#define size_struct_2 24
#define size_struct_3 8
#define size_struct_4 32
#define size_struct_5 12
#define size_struct_6 40
#define size_struct_7 8
#define size_struct_8 20
#define size_struct_9 32

struct _struct_0
{
  char member_0;
} ATTR;
typedef struct _struct_0 struct_0;

struct _struct_1
{
  char member_0;
  short member_1:13;
} ATTR;
typedef struct _struct_1 struct_1;

struct _struct_2
{
  double member_0;
  unsigned char member_1:8;
  long member_2:32;
  unsigned char member_3:5;
  short member_4:14;
  short member_5:13;
  unsigned char:0;
} ATTR;
typedef struct _struct_2 struct_2;

struct _struct_3
{
  unsigned long member_0:26;
  unsigned char member_1:2;

} ATTR;
typedef struct _struct_3 struct_3;

struct _struct_4
{
  unsigned char member_0:7;
  double member_1;
  double member_2;
  short member_3:5;
  char member_4:2;

} ATTR;
typedef struct _struct_4 struct_4;

struct _struct_5
{
  unsigned short member_0:12;
  long member_1:1;
  unsigned short member_2:6;

} ATTR;
typedef struct _struct_5 struct_5;

struct _struct_6
{
  unsigned char member_0:7;
  unsigned long member_1:25;
  char member_2:1;
  double member_3;
  short member_4:9;
  double member_5;

} ATTR;
typedef struct _struct_6 struct_6;

struct _struct_7
{
  double member_0;

} ATTR;
typedef struct _struct_7 struct_7;

struct _struct_8
{
  unsigned char member_0:7;
  long member_1:11;
  long member_2:5;
  long:0;
  char member_4:8;
  unsigned short member_5:4;
  unsigned char member_6:3;
  long member_7:23;

} ATTR;
typedef struct _struct_8 struct_8;

struct _struct_9
{
  double member_0;
  unsigned long member_1:6;
  long member_2:17;
  double member_3;
  unsigned long member_4:22;

} ATTR;
typedef struct _struct_9 struct_9;

struct_0 test_struct_0 = { 123 };
struct_1 test_struct_1 = { 82, 1081 };
struct_2 test_struct_2 = { 20.0, 31, 407760, 1, 14916, 6712 };
struct_3 test_struct_3 = { 64616999, 1 };
struct_4 test_struct_4 = { 61, 20.0, 20.0, 12, 0 };
struct_5 test_struct_5 = { 909, 1, 57 };
struct_6 test_struct_6 = { 12, 21355796, 0, 20.0, 467, 20.0 };
struct_7 test_struct_7 = { 20.0 };
struct_8 test_struct_8 = { 126, 1821, 22, 125, 6, 0, 2432638 };
struct_9 test_struct_9 = { 20.0, 3, 23957, 20.0, 1001631 };


int
main (void)
{

  if (size_struct_0 != sizeof (struct_0))
    abort ();

  if (size_struct_1 != sizeof (struct_1))
    abort ();

  if (size_struct_2 != sizeof (struct_2))
    abort ();

  if (size_struct_3 != sizeof (struct_3))
    abort ();

  if (size_struct_4 != sizeof (struct_4))
    abort ();

  if (size_struct_5 != sizeof (struct_5))
    abort ();

  if (size_struct_6 != sizeof (struct_6))
    abort ();

  if (size_struct_7 != sizeof (struct_7))
    abort ();

  if (size_struct_8 != sizeof (struct_8))
    abort ();

  if (size_struct_9 != sizeof (struct_9))
    abort ();

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type0 _struct_0 = struct {
// DEFAULT-NEXT:         field0 member_0: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 struct_0 = @type0;
// DEFAULT-NEXT:     type @type2 _struct_1 = struct {
// DEFAULT-NEXT:         field0 member_0: i8;
// DEFAULT-NEXT:         field1 member_1: i16 : 13;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2], bit_offsets=[None, Some(16)], bit_units=[(2, 2)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type3 struct_1 = @type2;
// DEFAULT-NEXT:     type @type4 _struct_2 = struct {
// DEFAULT-NEXT:         field0 member_0: f64;
// DEFAULT-NEXT:         field1 member_1: u8 : 8;
// DEFAULT-NEXT:         field2 member_2: i64 : 32;
// DEFAULT-NEXT:         field3 member_3: u8 : 5;
// DEFAULT-NEXT:         field4 member_4: i16 : 14;
// DEFAULT-NEXT:         field5 member_5: i16 : 13;
// DEFAULT-NEXT:         field6 <anonymous>: u8 : 0;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24, 26, 28, 30], bit_offsets=[None, Some(64), Some(128), Some(192), Some(208), Some(224), Some(240)], bit_units=[(8, 1), (16, 8), (24, 1), (26, 2), (28, 2)], field_units=[None, Some(0), Some(1), Some(2), Some(3), Some(4), None]];
// DEFAULT-NEXT:     type @type5 struct_2 = @type4;
// DEFAULT-NEXT:     type @type6 _struct_3 = struct {
// DEFAULT-NEXT:         field0 member_0: u64 : 26;
// DEFAULT-NEXT:         field1 member_1: u8 : 2;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), Some(64)], bit_units=[(0, 8), (8, 1)], field_units=[Some(0), Some(1)]];
// DEFAULT-NEXT:     type @type7 struct_3 = @type6;
// DEFAULT-NEXT:     type @type8 _struct_4 = struct {
// DEFAULT-NEXT:         field0 member_0: u8 : 7;
// DEFAULT-NEXT:         field1 member_1: f64;
// DEFAULT-NEXT:         field2 member_2: f64;
// DEFAULT-NEXT:         field3 member_3: i16 : 5;
// DEFAULT-NEXT:         field4 member_4: i8 : 2;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24, 26], bit_offsets=[Some(0), None, None, Some(192), Some(208)], bit_units=[(0, 1), (24, 2), (26, 1)], field_units=[Some(0), None, None, Some(1), Some(2)]];
// DEFAULT-NEXT:     type @type9 struct_4 = @type8;
// DEFAULT-NEXT:     type @type10 _struct_5 = struct {
// DEFAULT-NEXT:         field0 member_0: u16 : 12;
// DEFAULT-NEXT:         field1 member_1: i64 : 1;
// DEFAULT-NEXT:         field2 member_2: u16 : 6;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), Some(64), Some(128)], bit_units=[(0, 2), (8, 8), (16, 2)], field_units=[Some(0), Some(1), Some(2)]];
// DEFAULT-NEXT:     type @type11 struct_5 = @type10;
// DEFAULT-NEXT:     type @type12 _struct_6 = struct {
// DEFAULT-NEXT:         field0 member_0: u8 : 7;
// DEFAULT-NEXT:         field1 member_1: u64 : 25;
// DEFAULT-NEXT:         field2 member_2: i8 : 1;
// DEFAULT-NEXT:         field3 member_3: f64;
// DEFAULT-NEXT:         field4 member_4: i16 : 9;
// DEFAULT-NEXT:         field5 member_5: f64;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 8, 16, 24, 32, 40], bit_offsets=[Some(0), Some(64), Some(128), None, Some(256), None], bit_units=[(0, 1), (8, 8), (16, 1), (32, 2)], field_units=[Some(0), Some(1), Some(2), None, Some(3), None]];
// DEFAULT-NEXT:     type @type13 struct_6 = @type12;
// DEFAULT-NEXT:     type @type14 _struct_7 = struct {
// DEFAULT-NEXT:         field0 member_0: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type15 struct_7 = @type14;
// DEFAULT-NEXT:     type @type16 _struct_8 = struct {
// DEFAULT-NEXT:         field0 member_0: u8 : 7;
// DEFAULT-NEXT:         field1 member_1: i64 : 11;
// DEFAULT-NEXT:         field2 member_2: i64 : 5;
// DEFAULT-NEXT:         field3 <anonymous>: i64 : 0;
// DEFAULT-NEXT:         field4 member_4: i8 : 8;
// DEFAULT-NEXT:         field5 member_5: u16 : 4;
// DEFAULT-NEXT:         field6 member_6: u8 : 3;
// DEFAULT-NEXT:         field7 member_7: i64 : 23;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 9, 16, 16, 18, 20, 24], bit_offsets=[Some(0), Some(64), Some(75), Some(128), Some(128), Some(144), Some(160), Some(192)], bit_units=[(0, 1), (8, 8), (16, 1), (18, 2), (20, 1), (24, 8)], field_units=[Some(0), Some(1), Some(1), None, Some(2), Some(3), Some(4), Some(5)]];
// DEFAULT-NEXT:     type @type17 struct_8 = @type16;
// DEFAULT-NEXT:     type @type18 _struct_9 = struct {
// DEFAULT-NEXT:         field0 member_0: f64;
// DEFAULT-NEXT:         field1 member_1: u64 : 6;
// DEFAULT-NEXT:         field2 member_2: i64 : 17;
// DEFAULT-NEXT:         field3 member_3: f64;
// DEFAULT-NEXT:         field4 member_4: u64 : 22;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 8, 16, 24], bit_offsets=[None, Some(64), Some(70), None, Some(192)], bit_units=[(8, 8), (24, 8)], field_units=[None, Some(0), Some(0), None, Some(1)]];
// DEFAULT-NEXT:     type @type19 struct_9 = @type18;
// DEFAULT-NEXT:     global %21 test_struct_0: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(123))) [linkage=external];
// DEFAULT-NEXT:     global %22 test_struct_1: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(82)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(1081))) [linkage=external];
// DEFAULT-NEXT:     global %23 test_struct_2: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = const<f64>(20.0), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(31))), field2 = widen<i64, reason=assign>(const<i32>(407760)), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field4 = truncate<i16, reason=assign, fits=always>(const<i32>(14916)), field5 = truncate<i16, reason=assign, fits=always>(const<i32>(6712))) [linkage=external];
// DEFAULT-NEXT:     global %24 test_struct_3: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(64616999))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %25 test_struct_4: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(61))), field1 = const<f64>(20.0), field2 = const<f64>(20.0), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(12)), field4 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %26 test_struct_5: @type10 [storage=static] = aggregate<@type10, zero_fill=false>(field0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(909))), field1 = widen<i64, reason=assign>(const<i32>(1)), field2 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(57)))) [linkage=external];
// DEFAULT-NEXT:     global %27 test_struct_6: @type12 [storage=static] = aggregate<@type12, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(12))), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(21355796))), field2 = truncate<i8, reason=assign, fits=always>(const<i32>(0)), field3 = const<f64>(20.0), field4 = truncate<i16, reason=assign, fits=always>(const<i32>(467)), field5 = const<f64>(20.0)) [linkage=external];
// DEFAULT-NEXT:     global %28 test_struct_7: @type14 [storage=static] = aggregate<@type14, zero_fill=false>(field0 = const<f64>(20.0)) [linkage=external];
// DEFAULT-NEXT:     global %29 test_struct_8: @type16 [storage=static] = aggregate<@type16, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(126))), field1 = widen<i64, reason=assign>(const<i32>(1821)), field2 = widen<i64, reason=assign>(const<i32>(22)), field4 = truncate<i8, reason=assign, fits=always>(const<i32>(125)), field5 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(6))), field6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), field7 = widen<i64, reason=assign>(const<i32>(2432638))) [linkage=external];
// DEFAULT-NEXT:     global %30 test_struct_9: @type18 [storage=static] = aggregate<@type18, zero_fill=false>(field0 = const<f64>(20.0), field1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))), field2 = widen<i64, reason=assign>(const<i32>(23957)), field3 = const<f64>(20.0), field4 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1001631)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort(unprototyped) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %31 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), const<u64>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(24))), const<u64>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))), const<u64>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12))), const<u64>(24))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(40))), const<u64>(48))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(20))), const<u64>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))), const<u64>(32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
