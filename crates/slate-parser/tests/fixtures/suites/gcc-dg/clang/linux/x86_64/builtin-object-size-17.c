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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_Sx:[0-9]+]] Sx = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S0:[0-9]+]] S0 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 0>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 1>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S2:[0-9]+]] S2 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 2>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S9:[0-9]+]] S9 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<i8, 9>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S2x2:[0-9]+]] S2x2 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<array<i8, 2>, 2>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_S3x5:[0-9]+]] S3x5 = struct {
// DEFAULT-NEXT:         field0 n: i8;
// DEFAULT-NEXT:         field1 a: array<array<i8, 5>, 3>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     extern %[[VALUE_ax:[0-9]+]] ax: array<i8, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ax2:[0-9]+]] ax2: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_a0:[0-9]+]] a0: array<i8, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: array<i8, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: array<i8, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a9:[0-9]+]] a9: array<i8, 9> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_ia0:[0-9]+]] ia0: array<i32, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ia1:[0-9]+]] ia1: array<i32, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ia9:[0-9]+]] ia9: array<i32, 9> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a2x2:[0-9]+]] a2x2: array<array<i8, 2>, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a3x5:[0-9]+]] a3x5: array<array<i8, 5>, 3> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_sx:[0-9]+]] sx: @type[[TYPE_Sx]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s0:[0-9]+]] s0: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_S1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: @type[[TYPE_S2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s9:[0-9]+]] s9: @type[[TYPE_S9]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s2x2:[0-9]+]] s2x2: @type[[TYPE_S2x2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s3x5:[0-9]+]] s3x5: @type[[TYPE_S3x5]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_90_type_0:[0-9]+]] @failure_on_line_90_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_90_type_1:[0-9]+]] @failure_on_line_90_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_90_type_2:[0-9]+]] @failure_on_line_90_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_90_type_3:[0-9]+]] @failure_on_line_90_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_92_type_0:[0-9]+]] @failure_on_line_92_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_92_type_1:[0-9]+]] @failure_on_line_92_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_92_type_2:[0-9]+]] @failure_on_line_92_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_92_type_3:[0-9]+]] @failure_on_line_92_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_93_type_0:[0-9]+]] @failure_on_line_93_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_93_type_1:[0-9]+]] @failure_on_line_93_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_93_type_2:[0-9]+]] @failure_on_line_93_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_93_type_3:[0-9]+]] @failure_on_line_93_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_94_type_0:[0-9]+]] @failure_on_line_94_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_94_type_1:[0-9]+]] @failure_on_line_94_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_94_type_2:[0-9]+]] @failure_on_line_94_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_94_type_3:[0-9]+]] @failure_on_line_94_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_96_type_0:[0-9]+]] @failure_on_line_96_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_96_type_1:[0-9]+]] @failure_on_line_96_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_96_type_2:[0-9]+]] @failure_on_line_96_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_96_type_3:[0-9]+]] @failure_on_line_96_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_97_type_0:[0-9]+]] @failure_on_line_97_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_97_type_1:[0-9]+]] @failure_on_line_97_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_97_type_2:[0-9]+]] @failure_on_line_97_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_97_type_3:[0-9]+]] @failure_on_line_97_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_99_type_0:[0-9]+]] @failure_on_line_99_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_99_type_1:[0-9]+]] @failure_on_line_99_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_99_type_2:[0-9]+]] @failure_on_line_99_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_99_type_3:[0-9]+]] @failure_on_line_99_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_100_type_0:[0-9]+]] @failure_on_line_100_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_100_type_1:[0-9]+]] @failure_on_line_100_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_100_type_2:[0-9]+]] @failure_on_line_100_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_100_type_3:[0-9]+]] @failure_on_line_100_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_101_type_0:[0-9]+]] @failure_on_line_101_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_101_type_1:[0-9]+]] @failure_on_line_101_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_101_type_2:[0-9]+]] @failure_on_line_101_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_101_type_3:[0-9]+]] @failure_on_line_101_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_106_type_0:[0-9]+]] @failure_on_line_106_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_106_type_1:[0-9]+]] @failure_on_line_106_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_106_type_2:[0-9]+]] @failure_on_line_106_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_106_type_3:[0-9]+]] @failure_on_line_106_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_107_type_0:[0-9]+]] @failure_on_line_107_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_107_type_1:[0-9]+]] @failure_on_line_107_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_107_type_2:[0-9]+]] @failure_on_line_107_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_107_type_3:[0-9]+]] @failure_on_line_107_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_108_type_0:[0-9]+]] @failure_on_line_108_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_108_type_1:[0-9]+]] @failure_on_line_108_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_108_type_2:[0-9]+]] @failure_on_line_108_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_108_type_3:[0-9]+]] @failure_on_line_108_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_109_type_0:[0-9]+]] @failure_on_line_109_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_109_type_2:[0-9]+]] @failure_on_line_109_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_110_type_0:[0-9]+]] @failure_on_line_110_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_110_type_2:[0-9]+]] @failure_on_line_110_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_111_type_0:[0-9]+]] @failure_on_line_111_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_111_type_2:[0-9]+]] @failure_on_line_111_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_113_type_0:[0-9]+]] @failure_on_line_113_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_113_type_1:[0-9]+]] @failure_on_line_113_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_113_type_2:[0-9]+]] @failure_on_line_113_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_113_type_3:[0-9]+]] @failure_on_line_113_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_114_type_0:[0-9]+]] @failure_on_line_114_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_114_type_1:[0-9]+]] @failure_on_line_114_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_114_type_2:[0-9]+]] @failure_on_line_114_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_114_type_3:[0-9]+]] @failure_on_line_114_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_115_type_0:[0-9]+]] @failure_on_line_115_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_115_type_2:[0-9]+]] @failure_on_line_115_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_117_type_0:[0-9]+]] @failure_on_line_117_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_117_type_1:[0-9]+]] @failure_on_line_117_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_117_type_2:[0-9]+]] @failure_on_line_117_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_117_type_3:[0-9]+]] @failure_on_line_117_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_118_type_0:[0-9]+]] @failure_on_line_118_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_118_type_2:[0-9]+]] @failure_on_line_118_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_119_type_0:[0-9]+]] @failure_on_line_119_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_119_type_1:[0-9]+]] @failure_on_line_119_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_119_type_2:[0-9]+]] @failure_on_line_119_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_119_type_3:[0-9]+]] @failure_on_line_119_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_120_type_0:[0-9]+]] @failure_on_line_120_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_120_type_2:[0-9]+]] @failure_on_line_120_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_121_type_0:[0-9]+]] @failure_on_line_121_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_121_type_2:[0-9]+]] @failure_on_line_121_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_arrays:[0-9]+]] @test_arrays() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_90_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_90_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_90_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_90_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_92_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_92_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_92_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_92_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_93_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_93_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_93_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_93_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_a9]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_94_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_a9]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_94_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_a9]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_94_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_a9]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_94_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%[[VALUE_a0]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_96_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%[[VALUE_a0]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_96_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%[[VALUE_a0]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_96_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%[[VALUE_a0]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_96_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_97_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_97_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_97_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_ax2]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_97_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%[[VALUE_ia0]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_99_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE59:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%[[VALUE_ia0]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_99_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%[[VALUE_ia0]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_99_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE63:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%[[VALUE_ia0]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_99_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE65:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_ia1]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_100_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE68:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_ia1]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE69:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_100_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_ia1]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE71:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_100_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%[[VALUE_ia1]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE73:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_100_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE75:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%[[VALUE_ia9]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %[[VALUE76:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_101_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%[[VALUE_ia9]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_101_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%[[VALUE_ia9]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %[[VALUE80:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_101_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE81:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%[[VALUE_ia9]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             do %[[VALUE82:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_101_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE83:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE84:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE85:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_106_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE86:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE87:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_106_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE88:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_106_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE90:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_106_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE92:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE93:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE94:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_107_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE95:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE96:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_107_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE97:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE98:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_107_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE99:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE100:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_107_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE101:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE102:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(0))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE103:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_108_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE104:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(0))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE105:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_108_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE106:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(0))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             do %[[VALUE107:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_108_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE108:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(0))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE109:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_108_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE110:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE111:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%[[VALUE_a2x2]]), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE112:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_109_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE113:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%[[VALUE_a2x2]]), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE114:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_109_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE115:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE116:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE117:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_110_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE118:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE119:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_110_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE120:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE121:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))))
// DEFAULT-NEXT:                             do %[[VALUE122:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_111_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE123:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%[[VALUE_a2x2]]), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))))
// DEFAULT-NEXT:                             do %[[VALUE124:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_111_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE125:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE126:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]])), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %[[VALUE127:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_113_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE128:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]])), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %[[VALUE129:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_113_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE130:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]])), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %[[VALUE131:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_113_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE132:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]])), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %[[VALUE133:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_113_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE134:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE135:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %[[VALUE136:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_114_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE137:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))))
// DEFAULT-NEXT:                             do %[[VALUE138:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_114_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE139:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             do %[[VALUE140:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_114_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE141:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]]), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))))
// DEFAULT-NEXT:                             do %[[VALUE142:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_114_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE143:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE144:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]]), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))))
// DEFAULT-NEXT:                             do %[[VALUE145:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_115_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE146:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%[[VALUE_a3x5]]), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))))
// DEFAULT-NEXT:                             do %[[VALUE147:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_115_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE148:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE149:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]]), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE150:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_117_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE151:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]]), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE152:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_117_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE153:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]]), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE154:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_117_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE155:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_a1]]), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE156:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_117_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE157:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE158:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%[[VALUE_a1]]), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE159:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_118_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE160:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%[[VALUE_a1]]), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE161:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_118_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE162:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE163:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE164:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_119_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE165:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE166:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_119_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE167:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE168:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_119_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE169:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             do %[[VALUE170:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_119_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE171:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE172:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE173:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_120_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE174:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE175:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_120_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE176:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE177:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE178:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_121_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE179:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE180:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_121_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_129_type_0:[0-9]+]] @failure_on_line_129_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_129_type_1:[0-9]+]] @failure_on_line_129_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_129_type_2:[0-9]+]] @failure_on_line_129_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_129_type_3:[0-9]+]] @failure_on_line_129_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_133_type_0:[0-9]+]] @failure_on_line_133_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_133_type_1:[0-9]+]] @failure_on_line_133_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_133_type_2:[0-9]+]] @failure_on_line_133_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_133_type_3:[0-9]+]] @failure_on_line_133_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_137_type_0:[0-9]+]] @failure_on_line_137_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_137_type_1:[0-9]+]] @failure_on_line_137_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_137_type_2:[0-9]+]] @failure_on_line_137_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_137_type_3:[0-9]+]] @failure_on_line_137_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_141_type_0:[0-9]+]] @failure_on_line_141_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_141_type_1:[0-9]+]] @failure_on_line_141_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_141_type_2:[0-9]+]] @failure_on_line_141_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_141_type_3:[0-9]+]] @failure_on_line_141_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_143_type_0:[0-9]+]] @failure_on_line_143_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_143_type_1:[0-9]+]] @failure_on_line_143_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_143_type_2:[0-9]+]] @failure_on_line_143_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_143_type_3:[0-9]+]] @failure_on_line_143_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_144_type_0:[0-9]+]] @failure_on_line_144_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_144_type_1:[0-9]+]] @failure_on_line_144_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_144_type_2:[0-9]+]] @failure_on_line_144_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_144_type_3:[0-9]+]] @failure_on_line_144_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_145_type_0:[0-9]+]] @failure_on_line_145_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_145_type_2:[0-9]+]] @failure_on_line_145_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_147_type_0:[0-9]+]] @failure_on_line_147_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_147_type_1:[0-9]+]] @failure_on_line_147_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_147_type_2:[0-9]+]] @failure_on_line_147_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_147_type_3:[0-9]+]] @failure_on_line_147_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_148_type_0:[0-9]+]] @failure_on_line_148_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_148_type_1:[0-9]+]] @failure_on_line_148_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_148_type_2:[0-9]+]] @failure_on_line_148_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_148_type_3:[0-9]+]] @failure_on_line_148_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_149_type_0:[0-9]+]] @failure_on_line_149_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_149_type_1:[0-9]+]] @failure_on_line_149_type_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_149_type_2:[0-9]+]] @failure_on_line_149_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_149_type_3:[0-9]+]] @failure_on_line_149_type_3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_150_type_0:[0-9]+]] @failure_on_line_150_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_150_type_2:[0-9]+]] @failure_on_line_150_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_151_type_0:[0-9]+]] @failure_on_line_151_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_151_type_2:[0-9]+]] @failure_on_line_151_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_152_type_0:[0-9]+]] @failure_on_line_152_type_0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_failure_on_line_152_type_2:[0-9]+]] @failure_on_line_152_type_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_structs:[0-9]+]] @test_structs() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE181:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE182:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_Sx]]>>(%[[VALUE_sx]])), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE183:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_129_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE184:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_Sx]]>>(%[[VALUE_sx]])), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE185:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_129_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE186:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_Sx]]>>(%[[VALUE_sx]])), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE187:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_129_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE188:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_Sx]]>>(%[[VALUE_sx]])), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE189:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_129_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE190:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE191:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_sx]]))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE192:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_133_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE193:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_sx]]))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE194:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_133_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE195:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_sx]]))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE196:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_133_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE197:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_sx]]))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE198:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_133_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE199:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE200:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s0]])), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE201:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_137_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE202:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s0]])), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE203:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_137_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE204:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s0]])), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE205:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_137_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE206:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s0]])), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE207:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_137_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE208:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE209:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%[[VALUE_s0]]))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE210:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_141_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE211:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%[[VALUE_s0]]))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE212:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_141_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE213:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%[[VALUE_s0]]))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE214:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_141_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE215:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%[[VALUE_s0]]))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE216:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_141_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE217:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE218:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s1]])), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE219:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_143_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE220:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s1]])), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE221:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_143_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE222:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s1]])), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE223:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_143_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE224:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s1]])), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE225:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_143_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE226:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE227:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%[[VALUE_s1]]))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE228:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_144_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE229:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%[[VALUE_s1]]))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE230:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_144_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE231:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%[[VALUE_s1]]))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE232:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_144_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE233:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%[[VALUE_s1]]))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             do %[[VALUE234:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_144_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE235:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE236:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%[[VALUE_s1]])), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE237:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_145_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE238:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%[[VALUE_s1]])), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE239:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_145_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE240:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE241:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s9]])), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE242:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_147_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE243:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s9]])), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE244:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_147_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE245:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s9]])), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE246:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_147_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE247:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s9]])), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             do %[[VALUE248:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_147_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE249:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE250:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]]))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE251:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_148_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE252:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]]))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE253:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_148_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE254:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]]))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE255:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_148_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE256:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]]))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE257:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_148_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE258:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE259:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE260:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_149_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE261:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE262:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_149_type_1]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE263:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE264:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_149_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE265:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             do %[[VALUE266:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_149_type_3]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE267:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE268:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))))
// DEFAULT-NEXT:                             do %[[VALUE269:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_150_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE270:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))))
// DEFAULT-NEXT:                             do %[[VALUE271:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_150_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE272:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE273:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))))
// DEFAULT-NEXT:                             do %[[VALUE274:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_151_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE275:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))))
// DEFAULT-NEXT:                             do %[[VALUE276:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_151_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE277:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE278:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(9))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE279:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_152_type_0]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %[[VALUE280:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%[[VALUE_s9]])), const<i32>(9))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             do %[[VALUE281:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_failure_on_line_152_type_2]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_arrays]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_structs]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
