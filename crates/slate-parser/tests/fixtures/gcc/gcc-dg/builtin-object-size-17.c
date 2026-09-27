/* PR 71831 - __builtin_object_size poor results with no optimization
   Verify that even without optimization __builtin_object_size result
   is folded into a constant and dead code that depends on it is
   eliminated.  */
/* { dg-do compile } */
/* { dg-options "-O0 -fdump-tree-ssa" } */

#define concat(a, b)   a ## b
#define CAT(a, b)      concat (a, b)

/* Create a symbol name unique to each tes and object size type.  */
#define SYM(type)      CAT (CAT (CAT (failure_on_line_, __LINE__), _type_), type)

/* References to the following undefined symbol which is unique for each
   test case are expected to be eliminated.  */
#define TEST_FAILURE(type)			\
  do {						\
    extern void SYM (type)(void);		\
    SYM (type)();				\
  } while (0)

#define bos(obj, type) __builtin_object_size (obj, type)
#define size(obj, n) ((size_t)n == X ? sizeof *obj : (size_t)n)

#define test(expect, type, obj)			\
  do {						\
    if (bos (obj, type)	!= size (obj, expect))	\
      TEST_FAILURE (type);			\
  } while (0)

#define FOLD_ALL(r0, r1, r2, r3, obj)		\
  do {						\
    test (r0, 0, obj);				\
    test (r1, 1, obj);				\
    test (r2, 2, obj);				\
    test (r3, 3, obj);				\
  } while (0)

#define FOLD_0_2(r0, r1, r2, r3, obj)		\
  do {						\
    test (r0, 0, obj);				\
    test (r2, 2, obj);				\
  } while (0)

/* For convenience.  Substitute for 'sizeof object' in test cases where
   the size can vary from target to target.  */
#define X  (size_t)0xdeadbeef

typedef __SIZE_TYPE__ size_t;

extern char ax[];
#ifndef __builtin_object_size
char ax2[];               /* { dg-warning "assumed to have one element" } */
#endif

extern char a0[0];
static char a1[1];
static char a2[2];
static char a9[9];

#if __SIZEOF_SHORT__ == 4
extern short ia0[0];
static short ia1[1];
static short ia9[9];
#elif __SIZEOF_INT__ == 4
extern int ia0[0];
static int ia1[1];
static int ia9[9];
#elif __SIZEOF_LONG__ == 4
extern long ia0[0];
static long ia1[1];
static long ia9[9];
#endif

static char a2x2[2][2];
static char a3x5[3][5];

struct Sx { char n, a[]; } sx;
struct S0 { char n, a[0]; } s0;
struct S1 { char n, a[1]; } s1;
struct S2 { char n, a[2]; } s2;
struct S9 { char n, a[9]; } s9;

struct S2x2 { char n, a[2][2]; } s2x2;
struct S3x5 { char n, a[3][5]; } s3x5;

static __attribute__ ((noclone, noinline)) void
test_arrays ()
{
  FOLD_ALL (     1,       1,       1,       1,   ax2);

  FOLD_ALL (     1,       1,       1,       1,   a1);
  FOLD_ALL (     2,       2,       2,       2,   a2);
  FOLD_ALL (     9,       9,       9,       9,   a9);

  FOLD_ALL (     0,       0,       0,       0,   a0);
  FOLD_ALL (     1,       1,       1,       1,   ax2);

  FOLD_ALL (     0,       0,       0,       0,   ia0);
  FOLD_ALL (     4,       4,       4,       4,   ia1);
  FOLD_ALL (    36,      36,      36,      36,   ia9);

  /* Not all results for multidimensional arrays make sense (see
     bug 77293).  The expected results below simply reflect those
     obtained at -O2 (modulo the known limitations at -O1).  */
  FOLD_ALL (     4,       4,       4,       4,   a2x2);
  FOLD_ALL (     4,       4,       4,       4,   &a2x2[0]);
  FOLD_ALL (     4,       2,       4,       2,   &a2x2[0][0]);
  FOLD_0_2 (     0,  F1  (0),      0,       0,   &a2x2 + 1);
  FOLD_0_2 (     2,  F1 ( 2),      2,  F3 ( 2),  &a2x2[0] + 1);
  FOLD_0_2 (     3,  F1 ( 1),      3,  F3 ( 3),  &a2x2[0][0] + 1);

  FOLD_ALL (    15,      15,      15,      15,   a3x5);
  FOLD_ALL (    15,       5,      15,       5,   &a3x5[0][0] + 0);
  FOLD_0_2 (    14,  F1 ( 4),     14,  F3 (14),  &a3x5[0][0] + 1);

  FOLD_ALL (     1,       1,       1,       1,   a1 + 0);
  FOLD_0_2 (     0,  F1 ( 0),      0,       0,   &a1 + 1);
  FOLD_ALL (     2,       2,       2,       2,   a2 + 0);
  FOLD_0_2 (     1,  F1 ( 1),      1, F3 ( 1),   a2 + 1);
  FOLD_0_2 (     0,  F1 ( 0),      0,       0,   a2 + 2);
}

static __attribute__ ((noclone, noinline)) void
test_structs (void)
{
  /* The expected size of a declared object with a flexible array member
     is sizeof sx in all __builtin_object_size types.  */
  FOLD_ALL (     X,       X,       X,       X,   &sx);

  /* The expected size of a flexible array member of a declared object
     is zero.  */
  FOLD_ALL (     0,       0,       0,       0,   sx.a);

  /* The expected size of a declared object with a zero-length array member
     is sizeof sx in all __builtin_object_size types.  */
  FOLD_ALL (     X,       X,       X,       X,   &s0);

  /* The expected size of a zero-length array member of a declared object
     is zero.  */
  FOLD_ALL (     0,       0,       0,       0,   s0.a);

  FOLD_ALL (     X,       X,       X,       X,   &s1);
  FOLD_ALL (     1,       1,       1,       1,   s1.a);
  FOLD_0_2 (     0,  F1 (0),       0,       0,   s1.a + 1);

  FOLD_ALL (     X,       X,       X,       X,   &s9);
  FOLD_ALL (     9,       9,       9,       9,   s9.a);
  FOLD_ALL (     9,       9,       9,       9,   s9.a + 0);
  FOLD_0_2 (     8,  F1 ( 8),      8, F3 (  8),  s9.a + 1);
  FOLD_0_2 (     7,  F1 ( 7),      7, F3 (  7),  s9.a + 2);
  FOLD_0_2 (     0,  F1 ( 0),      0, F3 (  0),  s9.a + 9);
}

int
main()
{
  test_arrays ();
  test_structs ();

  return 0;
}

/* { dg-final { scan-tree-dump-not "failure_on_line" "ssa" } } */

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 Sx = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type2 S0 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 0>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type3 S1 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 1>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type4 S2 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 2>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type5 S9 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 9>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type6 S2x2 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<array<i8, 2>, 2>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type7 S3x5 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<array<i8, 5>, 3>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     extern %1 ax: array<i8, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 ax2: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %3 a0: array<i8, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 a1: array<i8, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 a2: array<i8, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 a9: array<i8, 9> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %7 ia0: array<i32, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 ia1: array<i32, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 ia9: array<i32, 9> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %10 a2x2: array<array<i8, 2>, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %11 a3x5: array<array<i8, 5>, 3> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %13 sx: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 s0: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 s1: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 s2: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %21 s9: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 s2x2: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 s3x5: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %155 @__builtin_object_size(%153 <unnamed>: ptr<const void>, %154 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %27 @failure_on_line_90_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %28 @failure_on_line_90_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %29 @failure_on_line_90_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %30 @failure_on_line_90_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %31 @failure_on_line_92_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %32 @failure_on_line_92_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %33 @failure_on_line_92_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %34 @failure_on_line_92_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %35 @failure_on_line_93_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %36 @failure_on_line_93_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %37 @failure_on_line_93_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %38 @failure_on_line_93_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %39 @failure_on_line_94_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %40 @failure_on_line_94_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %41 @failure_on_line_94_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %42 @failure_on_line_94_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %43 @failure_on_line_96_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %44 @failure_on_line_96_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %45 @failure_on_line_96_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %46 @failure_on_line_96_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %47 @failure_on_line_97_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %48 @failure_on_line_97_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %49 @failure_on_line_97_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %50 @failure_on_line_97_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %51 @failure_on_line_99_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %52 @failure_on_line_99_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %53 @failure_on_line_99_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %54 @failure_on_line_99_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %55 @failure_on_line_100_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %56 @failure_on_line_100_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %57 @failure_on_line_100_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %58 @failure_on_line_100_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %59 @failure_on_line_101_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %60 @failure_on_line_101_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %61 @failure_on_line_101_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %62 @failure_on_line_101_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %63 @failure_on_line_106_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %64 @failure_on_line_106_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %65 @failure_on_line_106_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %66 @failure_on_line_106_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %67 @failure_on_line_107_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %68 @failure_on_line_107_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %69 @failure_on_line_107_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %70 @failure_on_line_107_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %71 @failure_on_line_108_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %72 @failure_on_line_108_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %73 @failure_on_line_108_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %74 @failure_on_line_108_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %75 @failure_on_line_109_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %76 @failure_on_line_109_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %77 @failure_on_line_110_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %78 @failure_on_line_110_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %79 @failure_on_line_111_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %80 @failure_on_line_111_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %81 @failure_on_line_113_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %82 @failure_on_line_113_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %83 @failure_on_line_113_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %84 @failure_on_line_113_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %85 @failure_on_line_114_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %86 @failure_on_line_114_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %87 @failure_on_line_114_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %88 @failure_on_line_114_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %89 @failure_on_line_115_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %90 @failure_on_line_115_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %91 @failure_on_line_117_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %92 @failure_on_line_117_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %93 @failure_on_line_117_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %94 @failure_on_line_117_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %95 @failure_on_line_118_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %96 @failure_on_line_118_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %97 @failure_on_line_119_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %98 @failure_on_line_119_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %99 @failure_on_line_119_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %100 @failure_on_line_119_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %101 @failure_on_line_120_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %102 @failure_on_line_120_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %103 @failure_on_line_121_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %104 @failure_on_line_121_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %26 @test_arrays() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %151
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %152
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %156
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %157
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %158
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %159
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %160
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %161
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %162
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%30);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %163
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %164
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%4)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %165
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%31);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %166
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%4)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %167
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%32);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %168
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%4)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %169
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %170
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%4)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %171
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%34);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %172
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %173
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%5)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %174
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%35);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %175
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%5)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %176
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%36);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %177
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%5)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %178
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%37);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %179
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%5)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %180
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %181
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %182
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%6)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %183
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%39);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %184
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%6)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %185
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%40);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %186
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%6)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %187
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %188
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%6)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %189
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%42);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %190
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %191
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%3)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %192
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%43);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %193
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%3)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %194
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%44);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %195
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%3)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %196
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%45);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %197
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%3)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %198
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%46);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %199
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %200
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %201
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%47);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %202
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %203
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%48);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %204
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %205
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%49);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %206
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %207
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%50);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %208
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %209
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%7)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %210
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%51);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %211
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%7)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %212
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%52);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %213
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%7)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %214
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%53);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %215
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%7)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %216
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%54);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %217
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %218
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%8)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %219
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%55);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %220
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%8)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %221
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%56);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %222
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%8)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %223
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%57);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %224
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%8)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %225
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %226
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %227
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%9)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %228
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %229
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%9)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %230
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%60);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %231
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%9)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %232
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%61);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %233
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%9)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %234
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%62);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %235
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %236
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %237
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%63);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %238
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %239
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%64);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %240
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %241
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%65);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %242
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %243
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%66);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %244
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %245
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %246
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%67);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %247
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %248
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%68);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %249
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %250
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%69);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %251
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %252
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%70);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %253
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %254
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(0))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %255
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %256
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(0))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %257
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%72);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %258
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(0))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %259
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%73);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %260
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(0))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %261
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%74);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %262
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %263
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%10), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %264
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%75);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %265
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%10), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %266
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%76);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %267
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %268
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %269
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%77);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %270
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %271
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%78);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %272
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %273
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))))
// DEFAULT-NEXT:                             do %274
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%79);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %275
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%10), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))))
// DEFAULT-NEXT:                             do %276
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%80);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %277
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %278
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %279
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%81);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %280
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %281
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%82);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %282
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %283
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%83);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %284
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %285
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%84);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %286
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %287
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %288
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%85);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %289
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))))
// DEFAULT-NEXT:                             do %290
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%86);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %291
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %292
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%87);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %293
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))))
// DEFAULT-NEXT:                             do %294
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%88);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %295
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %296
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))))
// DEFAULT-NEXT:                             do %297
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%89);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %298
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))))
// DEFAULT-NEXT:                             do %299
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%90);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %300
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %301
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%4), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %302
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%91);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %303
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%4), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %304
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%92);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %305
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%4), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %306
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%93);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %307
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%4), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %308
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%94);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %309
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %310
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%4), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %311
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%95);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %312
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%4), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %313
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%96);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %314
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %315
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %316
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%97);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %317
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %318
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%98);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %319
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %320
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%99);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %321
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %322
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%100);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %323
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %324
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %325
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%101);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %326
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %327
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%102);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %328
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %329
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %330
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%103);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %331
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%5), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %332
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%104);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @failure_on_line_129_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %107 @failure_on_line_129_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %108 @failure_on_line_129_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %109 @failure_on_line_129_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %110 @failure_on_line_133_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %111 @failure_on_line_133_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %112 @failure_on_line_133_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %113 @failure_on_line_133_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %114 @failure_on_line_137_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %115 @failure_on_line_137_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %116 @failure_on_line_137_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %117 @failure_on_line_137_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %118 @failure_on_line_141_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %119 @failure_on_line_141_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %120 @failure_on_line_141_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %121 @failure_on_line_141_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %122 @failure_on_line_143_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %123 @failure_on_line_143_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %124 @failure_on_line_143_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %125 @failure_on_line_143_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %126 @failure_on_line_144_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %127 @failure_on_line_144_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %128 @failure_on_line_144_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %129 @failure_on_line_144_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %130 @failure_on_line_145_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %131 @failure_on_line_145_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %132 @failure_on_line_147_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %133 @failure_on_line_147_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %134 @failure_on_line_147_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %135 @failure_on_line_147_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %136 @failure_on_line_148_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %137 @failure_on_line_148_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %138 @failure_on_line_148_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %139 @failure_on_line_148_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %140 @failure_on_line_149_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %141 @failure_on_line_149_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %142 @failure_on_line_149_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %143 @failure_on_line_149_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %144 @failure_on_line_150_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %145 @failure_on_line_150_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %146 @failure_on_line_151_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %147 @failure_on_line_151_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %148 @failure_on_line_152_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %149 @failure_on_line_152_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %105 @test_structs() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %333
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %334
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%13)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %335
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%106);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %336
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%13)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %337
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%107);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %338
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%13)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %339
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%108);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %340
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%13)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %341
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%109);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %342
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %343
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%13))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %344
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%110);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %345
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%13))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %346
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%111);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %347
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%13))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %348
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%112);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %349
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%13))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %350
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%113);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %351
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %352
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%15)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %353
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%114);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %354
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%15)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %355
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%115);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %356
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%15)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %357
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%116);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %358
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%15)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %359
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%117);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %360
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %361
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%15))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %362
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%118);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %363
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%15))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %364
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%119);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %365
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%15))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %366
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%120);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %367
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%15))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %368
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%121);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %369
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %370
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%17)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %371
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%122);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %372
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%17)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %373
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%123);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %374
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%17)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %375
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%124);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %376
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%17)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %377
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%125);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %378
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %379
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%17))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %380
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%126);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %381
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%17))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %382
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%127);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %383
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%17))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %384
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%128);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %385
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%17))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %386
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%129);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %387
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %388
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%17)), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %389
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%130);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %390
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%17)), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %391
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%131);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %392
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %393
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %394
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%132);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %395
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %396
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%133);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %397
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %398
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%134);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %399
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%21)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %400
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%135);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %401
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %402
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%21))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %403
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%136);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %404
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%21))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %405
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%137);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %406
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%21))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %407
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%138);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %408
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%21))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %409
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%139);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %410
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %411
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %412
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%140);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %413
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %414
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%141);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %415
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %416
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%142);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %417
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %418
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %419
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %420
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))))
// DEFAULT-NEXT:                             do %421
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%144);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %422
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))))
// DEFAULT-NEXT:                             do %423
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%145);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %424
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %425
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))))
// DEFAULT-NEXT:                             do %426
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%146);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %427
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))))
// DEFAULT-NEXT:                             do %428
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%147);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %429
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %430
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(9))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %431
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%148);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %432
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%155, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%21)), const<i32>(9))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %433
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%149);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%26);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%105);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
