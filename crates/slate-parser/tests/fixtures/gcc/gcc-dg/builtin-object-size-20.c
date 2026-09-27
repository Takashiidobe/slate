/* PR middle-end/92815 - spurious -Wstringop-overflow writing into
   a flexible array of an extern struct
   { dg-do compile }
   { dg-options "-O -Wall -fdump-tree-optimized" }
   { dg-skip-if "test assumes that structs have padding" { default_packed } } */

#define ASSERT(expr) ((expr) ? (void)0 : fail (__LINE__))
#define bos0(expr) __builtin_object_size (expr, 1)
#define bos1(expr) __builtin_object_size (expr, 1)
#define bos2(expr) __builtin_object_size (expr, 2)
#define bos3(expr) __builtin_object_size (expr, 3)

typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;
typedef __SIZE_TYPE__  size_t;


extern void fail (int);




/* Verify sizes of a struct with a flexible array member and 1 byte
   of tail padding.  */

struct AI16CX { int16_t i; char n, a[]; };

struct AI16CX ai16c0 = { 0 };
struct AI16CX ai16c1 = { 0, 1, { 1 } };
struct AI16CX ai16c2 = { 0, 2, { 1, 2 } };
struct AI16CX ai16c3 = { 0, 3, { 1, 2, 3 } };
struct AI16CX ai16c4 = { 0, 4, { 1, 2, 3, 4 } };
struct AI16CX ai16c5 = { 0, 5, { 1, 2, 3, 4, 5 } };
struct AI16CX ai16c6 = { 0, 6, { 1, 2, 3, 4, 5, 6 } };
struct AI16CX ai16c7 = { 0, 7, { 1, 2, 3, 4, 5, 6, 7 } };
struct AI16CX ai16c8 = { 0, 8, { 1, 2, 3, 4, 5, 6, 7, 8 } };

extern struct AI16CX eai16cx;

void fai16cx (void)
{
  ASSERT (bos0 (&ai16c0) == sizeof ai16c0);
  ASSERT (bos0 (&ai16c1) == sizeof ai16c1);
  ASSERT (bos0 (&ai16c2) == sizeof ai16c2 + 1);
  ASSERT (bos0 (&ai16c3) == sizeof ai16c3 + 2);

  ASSERT (bos0 (&ai16c4) == sizeof ai16c4 + 3);
  ASSERT (bos0 (&ai16c5) == sizeof ai16c5 + 4);
  ASSERT (bos0 (&ai16c6) == sizeof ai16c6 + 5);
  ASSERT (bos0 (&ai16c7) == sizeof ai16c6 + 6);
  ASSERT (bos0 (&ai16c8) == sizeof ai16c6 + 7);

  ASSERT (bos0 (&eai16cx) == (size_t)-1);


  ASSERT (bos1 (&ai16c0) == sizeof ai16c0);
  ASSERT (bos1 (&ai16c1) == sizeof ai16c1);
  ASSERT (bos1 (&ai16c2) == sizeof ai16c2 + 1);
  ASSERT (bos1 (&ai16c3) == sizeof ai16c3 + 2);

  ASSERT (bos1 (&ai16c4) == sizeof ai16c4 + 3);
  ASSERT (bos1 (&ai16c5) == sizeof ai16c5 + 4);
  ASSERT (bos1 (&ai16c6) == sizeof ai16c6 + 5);
  ASSERT (bos1 (&ai16c7) == sizeof ai16c6 + 6);
  ASSERT (bos1 (&ai16c8) == sizeof ai16c6 + 7);

  ASSERT (bos1 (&eai16cx) == (size_t)-1);


  ASSERT (bos2 (&ai16c0) == sizeof ai16c0);
  ASSERT (bos2 (&ai16c1) == sizeof ai16c1);
  ASSERT (bos2 (&ai16c2) == sizeof ai16c2 + 1);
  ASSERT (bos2 (&ai16c3) == sizeof ai16c3 + 2);

  ASSERT (bos2 (&ai16c4) == sizeof ai16c4 + 3);
  ASSERT (bos2 (&ai16c5) == sizeof ai16c5 + 4);
  ASSERT (bos2 (&ai16c6) == sizeof ai16c6 + 5);
  ASSERT (bos2 (&ai16c7) == sizeof ai16c6 + 6);
  ASSERT (bos2 (&ai16c8) == sizeof ai16c6 + 7);

  ASSERT (bos2 (&eai16cx) == sizeof eai16cx);


  ASSERT (bos3 (&ai16c0) == sizeof ai16c0);
  ASSERT (bos3 (&ai16c1) == sizeof ai16c1);
  ASSERT (bos3 (&ai16c2) == sizeof ai16c2 + 1);
  ASSERT (bos3 (&ai16c3) == sizeof ai16c3 + 2);

  ASSERT (bos3 (&ai16c4) == sizeof ai16c4 + 3);
  ASSERT (bos3 (&ai16c5) == sizeof ai16c5 + 4);
  ASSERT (bos3 (&ai16c6) == sizeof ai16c6 + 5);
  ASSERT (bos3 (&ai16c7) == sizeof ai16c6 + 6);
  ASSERT (bos3 (&ai16c8) == sizeof ai16c6 + 7);

  ASSERT (bos3 (&eai16cx) == sizeof eai16cx);
}


/* Verify sizes of a struct with a flexible array member and 3 bytes
   of tail padding.  */

struct AI32CX { int32_t i; char n, a[]; };

struct AI32CX ai32c0 = { 0 };
struct AI32CX ai32c1 = { 0, 1, { 1 } };
struct AI32CX ai32c2 = { 0, 2, { 1, 2 } };
struct AI32CX ai32c3 = { 0, 3, { 1, 2, 3 } };
struct AI32CX ai32c4 = { 0, 4, { 1, 2, 3, 4 } };
struct AI32CX ai32c5 = { 0, 5, { 1, 2, 3, 4, 5 } };
struct AI32CX ai32c6 = { 0, 6, { 1, 2, 3, 4, 5, 6 } };
struct AI32CX ai32c7 = { 0, 7, { 1, 2, 3, 4, 5, 6, 7 } };
struct AI32CX ai32c8 = { 0, 8, { 1, 2, 3, 4, 5, 6, 7, 8 } };

extern struct AI32CX eai32cx;

void fai32cx (void)
{
  ASSERT (bos0 (&ai32c0) == sizeof ai32c0);
  ASSERT (bos0 (&ai32c1) == sizeof ai32c1);
  ASSERT (bos0 (&ai32c2) == sizeof ai32c2);
  ASSERT (bos0 (&ai32c3) == sizeof ai32c3);

  ASSERT (bos0 (&ai32c4) == sizeof ai32c4 + 1);
  ASSERT (bos0 (&ai32c5) == sizeof ai32c5 + 2);
  ASSERT (bos0 (&ai32c6) == sizeof ai32c6 + 3);
  ASSERT (bos0 (&ai32c7) == sizeof ai32c6 + 4);
  ASSERT (bos0 (&ai32c8) == sizeof ai32c6 + 5);

  ASSERT (bos0 (&eai32cx) == (size_t)-1);


  ASSERT (bos1 (&ai32c0) == sizeof ai32c0);
  ASSERT (bos1 (&ai32c1) == sizeof ai32c1);
  ASSERT (bos1 (&ai32c2) == sizeof ai32c2);
  ASSERT (bos1 (&ai32c3) == sizeof ai32c3);

  ASSERT (bos1 (&ai32c4) == sizeof ai32c4 + 1);
  ASSERT (bos1 (&ai32c5) == sizeof ai32c5 + 2);
  ASSERT (bos1 (&ai32c6) == sizeof ai32c6 + 3);
  ASSERT (bos1 (&ai32c7) == sizeof ai32c6 + 4);
  ASSERT (bos1 (&ai32c8) == sizeof ai32c6 + 5);

  ASSERT (bos1 (&eai32cx) == (size_t)-1);


  ASSERT (bos2 (&ai32c0) == sizeof ai32c0);
  ASSERT (bos2 (&ai32c1) == sizeof ai32c1);
  ASSERT (bos2 (&ai32c2) == sizeof ai32c2);
  ASSERT (bos2 (&ai32c3) == sizeof ai32c3);

  ASSERT (bos2 (&ai32c4) == sizeof ai32c4 + 1);
  ASSERT (bos2 (&ai32c5) == sizeof ai32c5 + 2);
  ASSERT (bos2 (&ai32c6) == sizeof ai32c6 + 3);
  ASSERT (bos2 (&ai32c7) == sizeof ai32c6 + 4);
  ASSERT (bos2 (&ai32c8) == sizeof ai32c6 + 5);

  ASSERT (bos2 (&eai32cx) == sizeof eai32cx);


  ASSERT (bos3 (&ai32c0) == sizeof ai32c0);
  ASSERT (bos3 (&ai32c1) == sizeof ai32c1);
  ASSERT (bos3 (&ai32c2) == sizeof ai32c2);
  ASSERT (bos3 (&ai32c3) == sizeof ai32c3);

  ASSERT (bos3 (&ai32c4) == sizeof ai32c4 + 1);
  ASSERT (bos3 (&ai32c5) == sizeof ai32c5 + 2);
  ASSERT (bos3 (&ai32c6) == sizeof ai32c6 + 3);
  ASSERT (bos3 (&ai32c7) == sizeof ai32c6 + 4);
  ASSERT (bos3 (&ai32c8) == sizeof ai32c6 + 5);

  ASSERT (bos3 (&eai32cx) == sizeof eai32cx);
}


/* Verify sizes of a struct with a flexible array member and 7 bytes
   of tail padding.  */

struct AI64CX { int64_t i __attribute__ ((aligned (8))); char n, a[]; };

struct AI64CX ai64c0 = { 0 };
struct AI64CX ai64c1 = { 0, 1, { 1 } };
struct AI64CX ai64c2 = { 0, 2, { 1, 2 } };
struct AI64CX ai64c3 = { 0, 3, { 1, 2, 3 } };
struct AI64CX ai64c4 = { 0, 4, { 1, 2, 3, 4 } };
struct AI64CX ai64c5 = { 0, 5, { 1, 2, 3, 4, 5 } };
struct AI64CX ai64c6 = { 0, 6, { 1, 2, 3, 4, 5, 6 } };
struct AI64CX ai64c7 = { 0, 7, { 1, 2, 3, 4, 5, 6, 7 } };
struct AI64CX ai64c8 = { 0, 8, { 1, 2, 3, 4, 5, 6, 7, 8 } };
struct AI64CX ai64c9 = { 0, 8, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } };

extern struct AI64CX eai64cx;

void fai64cx (void)
{
  ASSERT (bos0 (&ai64c0) == sizeof ai64c0);
  ASSERT (bos0 (&ai64c1) == sizeof ai64c1);
  ASSERT (bos0 (&ai64c2) == sizeof ai64c2);
  ASSERT (bos0 (&ai64c3) == sizeof ai64c3);
  ASSERT (bos0 (&ai64c4) == sizeof ai64c4);
  ASSERT (bos0 (&ai64c5) == sizeof ai64c5);
  ASSERT (bos0 (&ai64c6) == sizeof ai64c6);
  ASSERT (bos0 (&ai64c7) == sizeof ai64c7);

  ASSERT (bos0 (&ai64c8) == sizeof ai64c8 + 1);
  ASSERT (bos0 (&ai64c9) == sizeof ai64c9 + 2);

  ASSERT (bos0 (&eai64cx) == (size_t)-1);


  ASSERT (bos1 (&ai64c0) == sizeof ai64c0);
  ASSERT (bos1 (&ai64c1) == sizeof ai64c1);
  ASSERT (bos1 (&ai64c2) == sizeof ai64c2);
  ASSERT (bos1 (&ai64c3) == sizeof ai64c3);
  ASSERT (bos1 (&ai64c4) == sizeof ai64c4);
  ASSERT (bos1 (&ai64c5) == sizeof ai64c5);
  ASSERT (bos1 (&ai64c6) == sizeof ai64c6);
  ASSERT (bos1 (&ai64c7) == sizeof ai64c7);

  ASSERT (bos1 (&ai64c8) == sizeof ai64c8 + 1);
  ASSERT (bos1 (&ai64c9) == sizeof ai64c9 + 2);

  ASSERT (bos1 (&eai64cx) == (size_t)-1);


  ASSERT (bos2 (&ai64c0) == sizeof ai64c0);
  ASSERT (bos2 (&ai64c1) == sizeof ai64c1);
  ASSERT (bos2 (&ai64c2) == sizeof ai64c2);
  ASSERT (bos2 (&ai64c3) == sizeof ai64c3);
  ASSERT (bos2 (&ai64c4) == sizeof ai64c4);
  ASSERT (bos2 (&ai64c5) == sizeof ai64c5);
  ASSERT (bos2 (&ai64c6) == sizeof ai64c6);
  ASSERT (bos2 (&ai64c7) == sizeof ai64c7);

  ASSERT (bos2 (&ai64c8) == sizeof ai64c8 + 1);
  ASSERT (bos2 (&ai64c9) == sizeof ai64c9 + 2);

  ASSERT (bos2 (&eai64cx) == sizeof eai64cx);

  ASSERT (bos3 (&ai64c0) == sizeof ai64c0);
  ASSERT (bos3 (&ai64c1) == sizeof ai64c1);
  ASSERT (bos3 (&ai64c2) == sizeof ai64c2);
  ASSERT (bos3 (&ai64c3) == sizeof ai64c3);
  ASSERT (bos3 (&ai64c4) == sizeof ai64c4);
  ASSERT (bos3 (&ai64c5) == sizeof ai64c5);
  ASSERT (bos3 (&ai64c6) == sizeof ai64c6);
  ASSERT (bos3 (&ai64c7) == sizeof ai64c7);

  ASSERT (bos3 (&ai64c8) == sizeof ai64c8 + 1);
  ASSERT (bos3 (&ai64c9) == sizeof ai64c9 + 2);

  ASSERT (bos3 (&eai64cx) == sizeof eai64cx);
}

/* { dg-final { scan-tree-dump-not "fail" "optimized" } } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 int16_t = i16;
// DEFAULT-NEXT:     type @type1 int32_t = i32;
// DEFAULT-NEXT:     type @type2 int64_t = i64;
// DEFAULT-NEXT:     type @type3 size_t = u64;
// DEFAULT-NEXT:     type @type4 AI16CX = struct {
// DEFAULT-NEXT:         field0 i: i16;
// DEFAULT-NEXT:         field1 n: i8;
// DEFAULT-NEXT:         field2 a: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2, 3]];
// DEFAULT-NEXT:     type @type5 AI32CX = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 n: i8;
// DEFAULT-NEXT:         field2 a: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 5]];
// DEFAULT-NEXT:     type @type6 AI64CX = struct {
// DEFAULT-NEXT:         field0 i: i64;
// DEFAULT-NEXT:         field1 n: i8;
// DEFAULT-NEXT:         field2 a: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9]];
// DEFAULT-NEXT:     global %6 ai16c0: @type4 [storage=static] = aggregate<@type4, zero_fill=true>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %7 ai16c1: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = aggregate<array<i8, 1>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %8 ai16c2: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     global %9 ai16c3: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field2 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)))) [linkage=external];
// DEFAULT-NEXT:     global %10 ai16c4: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), field2 = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)))) [linkage=external];
// DEFAULT-NEXT:     global %11 ai16c5: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), field2 = aggregate<array<i8, 5>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)))) [linkage=external];
// DEFAULT-NEXT:     global %12 ai16c6: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), field2 = aggregate<array<i8, 6>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)))) [linkage=external];
// DEFAULT-NEXT:     global %13 ai16c7: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), field2 = aggregate<array<i8, 7>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)))) [linkage=external];
// DEFAULT-NEXT:     global %14 ai16c8: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(8)), field2 = aggregate<array<i8, 8>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(8)))) [linkage=external];
// DEFAULT-NEXT:     extern %15 eai16cx: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 ai32c0: @type5 [storage=static] = aggregate<@type5, zero_fill=true>(field0 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %19 ai32c1: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = aggregate<array<i8, 1>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %20 ai32c2: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     global %21 ai32c3: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field2 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)))) [linkage=external];
// DEFAULT-NEXT:     global %22 ai32c4: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), field2 = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)))) [linkage=external];
// DEFAULT-NEXT:     global %23 ai32c5: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), field2 = aggregate<array<i8, 5>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)))) [linkage=external];
// DEFAULT-NEXT:     global %24 ai32c6: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), field2 = aggregate<array<i8, 6>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)))) [linkage=external];
// DEFAULT-NEXT:     global %25 ai32c7: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), field2 = aggregate<array<i8, 7>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)))) [linkage=external];
// DEFAULT-NEXT:     global %26 ai32c8: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(8)), field2 = aggregate<array<i8, 8>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(8)))) [linkage=external];
// DEFAULT-NEXT:     extern %27 eai32cx: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %30 ai64c0: @type6 [storage=static] = aggregate<@type6, zero_fill=true>(field0 = widen<i64, reason=assign>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %31 ai64c1: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = aggregate<array<i8, 1>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %32 ai64c2: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     global %33 ai64c3: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), field2 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)))) [linkage=external];
// DEFAULT-NEXT:     global %34 ai64c4: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), field2 = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)))) [linkage=external];
// DEFAULT-NEXT:     global %35 ai64c5: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), field2 = aggregate<array<i8, 5>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)))) [linkage=external];
// DEFAULT-NEXT:     global %36 ai64c6: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), field2 = aggregate<array<i8, 6>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)))) [linkage=external];
// DEFAULT-NEXT:     global %37 ai64c7: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), field2 = aggregate<array<i8, 7>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)))) [linkage=external];
// DEFAULT-NEXT:     global %38 ai64c8: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(8)), field2 = aggregate<array<i8, 8>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(8)))) [linkage=external];
// DEFAULT-NEXT:     global %39 ai64c9: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(8)), field2 = aggregate<array<i8, 9>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(8)), index8 = truncate<i8, reason=assign, fits=always>(const<i32>(9)))) [linkage=external];
// DEFAULT-NEXT:     extern %40 eai64cx: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @fail(%42 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %45 @__builtin_object_size(%43 <unnamed>: ptr<const void>, %44 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fai16cx() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%6)), const<i32>(1)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(43));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%7)), const<i32>(1)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(44));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%8)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(45));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%9)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(46));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%10)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(48));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%11)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(49));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%12)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(50));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%13)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(51));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%14)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(52));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%15)), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(54));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%6)), const<i32>(1)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(57));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%7)), const<i32>(1)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(58));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%8)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(59));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%9)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(60));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%10)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(62));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%11)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(63));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%12)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(64));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%13)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(65));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%14)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(66));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%15)), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(68));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%6)), const<i32>(2)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(71));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%7)), const<i32>(2)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(72));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%8)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(73));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%9)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(74));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%10)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(76));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%11)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(77));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%12)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(78));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%13)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(79));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%14)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(80));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%15)), const<i32>(2)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(82));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%6)), const<i32>(3)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(85));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%7)), const<i32>(3)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(86));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%8)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(87));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%9)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(88));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%10)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(90));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%11)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(91));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%12)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(92));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%13)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(93));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%14)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(94));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type4>>(%15)), const<i32>(3)), const<u64>(4))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(96));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @fai32cx() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%18)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(119));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%19)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(120));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%20)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(121));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(122));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(124));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%23)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(125));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%24)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(126));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%25)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(127));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%26)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(128));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%27)), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(130));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%18)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(133));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%19)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(134));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%20)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(135));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(1)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(136));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(138));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%23)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(139));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%24)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(140));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%25)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(141));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%26)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(142));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%27)), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(144));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%18)), const<i32>(2)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(147));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%19)), const<i32>(2)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(148));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%20)), const<i32>(2)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(149));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(2)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(150));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(152));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%23)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(153));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%24)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(154));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%25)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(155));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%26)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(156));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%27)), const<i32>(2)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(158));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%18)), const<i32>(3)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(161));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%19)), const<i32>(3)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(162));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%20)), const<i32>(3)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(163));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(3)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(164));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(166));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%23)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(167));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%24)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(168));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%25)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(169));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%26)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(170));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%27)), const<i32>(3)), const<u64>(8))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(172));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @fai64cx() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%30)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(196));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%31)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(197));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%32)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(198));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%33)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(199));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%34)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(200));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%35)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(201));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%36)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(202));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%37)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(203));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%38)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(205));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%39)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(206));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%40)), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(208));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%30)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(211));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%31)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(212));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%32)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(213));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%33)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(214));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%34)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(215));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%35)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(216));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%36)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(217));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%37)), const<i32>(1)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(218));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%38)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(220));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%39)), const<i32>(1)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(221));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%40)), const<i32>(1)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(223));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%30)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(226));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%31)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(227));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%32)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(228));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%33)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(229));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%34)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(230));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%35)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(231));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%36)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(232));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%37)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(233));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%38)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(235));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%39)), const<i32>(2)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(236));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%40)), const<i32>(2)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(238));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%30)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(240));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%31)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(241));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%32)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(242));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%33)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(243));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%34)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(244));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%35)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(245));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%36)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(246));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%37)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(247));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%38)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(249));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%39)), const<i32>(3)), add<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(250));
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%45, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type6>>(%40)), const<i32>(3)), const<u64>(16))
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(252));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
