/* PR 71831 - __builtin_object_size poor results with no optimization
   Verify that even without optimization __builtin_object_size returns
   a meaningful result for a subset of simple expressins.  In cases
   where the result could not easily be made to match the one obtained
   with optimization the built-in was made to fail instead.  */
/* { dg-do run } */
/* { dg-options "-O0" } */

static int nfails;

#define TEST_FAILURE(line, obj, type, expect, result)		\
  __builtin_printf ("FAIL: line %i: __builtin_object_size("	\
		    #obj ", %i) == %zu, got %zu\n",		\
		    line, type, expect, result), ++nfails

#define bos(obj, type) __builtin_object_size (obj, type)
#define size(obj, n) ((size_t)n == X ? sizeof *obj : (size_t)n)

#define test(expect, type, obj)						\
  do {									\
    if (bos (obj, type)	!= size (obj, expect))				\
      TEST_FAILURE (__LINE__, obj, type, size (obj, expect), bos (obj, type)); \
  } while (0)

#define T(r0, r1, r2, r3, obj)			\
  do {						\
    test (r0, 0, obj);				\
    test (r1, 1, obj);				\
    test (r2, 2, obj);				\
    test (r3, 3, obj);				\
  } while (0)

/* For convenience.  Substitute for 'sizeof object' in test cases where
   the size can vary from target to target.  */
#define X  (size_t)0xdeadbeef

/* __builtin_object_size checking results are inconsistent for equivalent
   expressions (see bug 71831).  To avoid having duplicate the inconsistency
   at -O0 the built-in simply fails.  The results hardcoded in this test
   are those obtained with optimization (for easy comparison) but without
   optimization the macros below turn them into expected failures .  */
#if __OPTIMIZE__
#  define F0(n)    n
#  define F1(n)    n
#  define F2(n)    n
#  define F3(n)    n
#else
#  define F0(n)   -1
#  define F1(n)   -1
#  define F2(n)    0
#  define F3(n)    0
#endif

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
  T (    -1,      -1,       0,       0,   ax);

  T (     0,       0,       0,       0,   a0);
  T (     1,       1,       1,       1,   ax2);

  T (     1,       1,       1,       1,   a1);
  T (     2,       2,       2,       2,   a2);
  T (     9,       9,       9,       9,   a9);

  T (     0,       0,       0,       0,   a0);
  T (     1,       1,       1,       1,   ax2);

  T (     0,       0,       0,       0,   ia0);
  T (     4,       4,       4,       4,   ia1);
  T (    36,      36,      36,      36,   ia9);

  /* Not all results for multidimensional arrays make sense (see
     bug 77293).  The expected results below simply reflect those
     obtained at -O2 (modulo the known limitations at -O1).  */
  T (     4,       4,       4,       4,   a2x2);
  T (     4,       4,       4,       4,   &a2x2[0]);
  T (     4,       2,       4,       2,   &a2x2[0][0]);
  T (     0,  F1  (0),      0,       0,   &a2x2 + 1);
  T (     2,  F1 ( 2),      2,  F3 ( 2),  &a2x2[0] + 1);
  T (     3,  F1 ( 1),      3,  F3 ( 3),  &a2x2[0][0] + 1);

  T (    15,      15,      15,      15,   a3x5);
  T (    15,       5,      15,       5,   &a3x5[0][0] + 0);
  T (    14,  F1 ( 4),     14,  F3 (14),  &a3x5[0][0] + 1);

  T (     1,       1,       1,       1,   a1 + 0);
  T (     0,  F1  (0),      0,       0,   a1 + 1);
  T (     0,  F1 ( 0),      0,       0,   &a1 + 1);
  /* In the following the offset is out of bounds which makes
     the expression undefined.  Still, verify that the returned
     size is zero (and not some large number).  */
  T (     0,  F1  (0),      0,       0,   a1 + 2);

  T (     2,       2,       2,       2,   a2 + 0);
  T (     1,  F1 ( 1),      1, F3 ( 1),   a2 + 1);
  T (     0,  F1 ( 0),      0,       0,   a2 + 2);
}

static __attribute__ ((noclone, noinline)) void
test_structs (struct Sx *psx, struct S0 *ps0, struct S1 *ps1, struct S9 *ps9)
{
  /* The expected size of a declared object with a flexible array member
     is sizeof sx in all __builtin_object_size types.  */
  T (     X,       X,       X,       X,   &sx);

  /* The expected size of an unknown object with a flexible array member
     is unknown in all __builtin_object_size types.  */
  T (    -1,      -1,       0,       0,   psx);

  /* The expected size of a flexible array member of a declared object
     is zero.  */
  T (     0,       0,       0,       0,   sx.a);

  /* The expected size of a flexible array member of an unknown object
     is unknown.  */
  T (    -1,      -1,       0,       0,   psx->a);

  /* The expected size of a declared object with a zero-length array member
     is sizeof sx in all __builtin_object_size types.  */
  T (     X,       X,       X,       X,   &s0);

  /* The expected size of an unknown object with a zero-length array member
     is unknown in all __builtin_object_size types.  */
  T (    -1,      -1,       0,       0,   ps0);

  /* The expected size of a zero-length array member of a declared object
     is zero.  */
  T (     0,       0,       0,       0,   s0.a);

  /* The expected size of a zero-length array member of an unknown object
     is unknown.  */
  T (    -1,      -1,       0,       0,   ps0->a);

  T (     X,       X,       X,       X,   &s1);
  T (     1,       1,       1,       1,   s1.a);
  T (     0,  F1 (0),       0,       0,   s1.a + 1);

  /* GCC treats arrays of all sizes that are the last member of a struct
     as flexible array members.  */
  T (    -1,      -1,       0,       0,   ps1);
  T (    -1,      -1,       0,       0,   ps1->a);
  T (    -1,      -1,       0,       0,   ps1->a + 1);

  T (     X,       X,       X,       X,   &s9);
  T (     9,       9,       9,       9,   s9.a);
  T (     9,       9,       9,       9,   s9.a + 0);
  T (     8,  F1 ( 8),      8, F3 (  8),  s9.a + 1);
  T (     7,  F1 ( 7),      7, F3 (  7),  s9.a + 2);
  T (     0,  F1 ( 0),      0, F3 (  0),  s9.a + 9);

  /* The following make little sense but see bug 77301.  */
  T (    -1,      -1,       0,       0,   ps9);
  T (    -1,      -1,       0,       0,   ps9->a);
  T (    -1,      -1,       0,       0,   ps9->a + 1);
}

int
main()
{
  test_arrays ();

  test_structs (&sx, &s0, &s1, &s9);

  if (nfails)
    __builtin_abort ();

  return 0;
}

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
// DEFAULT-NEXT:     global %0 nfails: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %2 ax: array<i8, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ax2: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %4 a0: array<i8, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 a1: array<i8, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 a2: array<i8, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 a9: array<i8, 9> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %8 ia0: array<i32, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 ia1: array<i32, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %10 ia9: array<i32, 9> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %11 a2x2: array<array<i8, 2>, 2> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %12 a3x5: array<array<i8, 5>, 3> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %14 sx: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 s0: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 s1: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 s2: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %22 s9: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %24 s2x2: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %26 s3x5: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %47 .str47: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %50 .str50: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %52 .str52: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %54 .str54: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %74 .str74: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %77 .str77: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %79 .str79: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %81 .str81: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %83 .str83: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %86 .str86: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %88 .str88: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %90 .str90: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %92 .str92: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %95 .str95: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %97 .str97: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %99 .str99: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %137 .str137: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 105, 97, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %140 .str140: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %142 .str142: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %144 .str144: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %146 .str146: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 120, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %149 .str149: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %151 .str151: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %153 .str153: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %155 .str155: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %158 .str158: array<i8, 71> [storage=static] = code_units<array<i8, 71>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %160 .str160: array<i8, 71> [storage=static] = code_units<array<i8, 71>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %162 .str162: array<i8, 71> [storage=static] = code_units<array<i8, 71>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %164 .str164: array<i8, 71> [storage=static] = code_units<array<i8, 71>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %167 .str167: array<i8, 69> [storage=static] = code_units<array<i8, 69>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %169 .str169: array<i8, 69> [storage=static] = code_units<array<i8, 69>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %171 .str171: array<i8, 69> [storage=static] = code_units<array<i8, 69>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %173 .str173: array<i8, 69> [storage=static] = code_units<array<i8, 69>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %176 .str176: array<i8, 72> [storage=static] = code_units<array<i8, 72>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %178 .str178: array<i8, 72> [storage=static] = code_units<array<i8, 72>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %180 .str180: array<i8, 72> [storage=static] = code_units<array<i8, 72>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %182 .str182: array<i8, 72> [storage=static] = code_units<array<i8, 72>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %189 .str189: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %191 .str191: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 50, 120, 50, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %194 .str194: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 51, 120, 53, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %196 .str196: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 51, 120, 53, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %198 .str198: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 51, 120, 53, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %200 .str200: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 51, 120, 53, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %203 .str203: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %205 .str205: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %207 .str207: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %209 .str209: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %212 .str212: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %214 .str214: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %216 .str216: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %218 .str218: array<i8, 75> [storage=static] = code_units<array<i8, 75>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 51, 120, 53, 91, 48, 93, 91, 48, 93, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %221 .str221: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %223 .str223: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %225 .str225: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %227 .str227: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %230 .str230: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %232 .str232: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %234 .str234: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %236 .str236: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %239 .str239: array<i8, 67> [storage=static] = code_units<array<i8, 67>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %241 .str241: array<i8, 67> [storage=static] = code_units<array<i8, 67>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %243 .str243: array<i8, 67> [storage=static] = code_units<array<i8, 67>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 67> [storage=static] = code_units<array<i8, 67>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 97, 49, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %248 .str248: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %250 .str250: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 49, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %266 .str266: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %268 .str268: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %270 .str270: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %272 .str272: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %275 .str275: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %277 .str277: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %279 .str279: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %281 .str281: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 97, 50, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %284 .str284: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %286 .str286: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %288 .str288: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %290 .str290: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %293 .str293: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %295 .str295: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %297 .str297: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %299 .str299: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %302 .str302: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 120, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %304 .str304: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 120, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %306 .str306: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 120, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %308 .str308: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 120, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %311 .str311: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %313 .str313: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %315 .str315: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %317 .str317: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 120, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %320 .str320: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %322 .str322: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %324 .str324: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %326 .str326: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %329 .str329: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %331 .str331: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %333 .str333: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %335 .str335: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %338 .str338: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 48, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %340 .str340: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 48, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %342 .str342: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 48, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %344 .str344: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 48, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %347 .str347: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %349 .str349: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %351 .str351: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %353 .str353: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 48, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %356 .str356: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %358 .str358: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %360 .str360: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %362 .str362: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %365 .str365: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %367 .str367: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %369 .str369: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %371 .str371: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %374 .str374: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %376 .str376: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %378 .str378: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %380 .str380: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 49, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %383 .str383: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %385 .str385: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %387 .str387: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %389 .str389: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %392 .str392: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %394 .str394: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %396 .str396: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %398 .str398: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %401 .str401: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %403 .str403: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %405 .str405: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %407 .str407: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 49, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %410 .str410: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %412 .str412: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %414 .str414: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %416 .str416: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 38, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %419 .str419: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %421 .str421: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %423 .str423: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %425 .str425: array<i8, 64> [storage=static] = code_units<array<i8, 64>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %428 .str428: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %430 .str430: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %432 .str432: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %434 .str434: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 48, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %437 .str437: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %439 .str439: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %441 .str441: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %443 .str443: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %446 .str446: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %448 .str448: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %450 .str450: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %452 .str452: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 50, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %455 .str455: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %457 .str457: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %459 .str459: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %461 .str461: array<i8, 68> [storage=static] = code_units<array<i8, 68>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 115, 57, 46, 97, 32, 43, 32, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %464 .str464: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %466 .str466: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %468 .str468: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %470 .str470: array<i8, 63> [storage=static] = code_units<array<i8, 63>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %473 .str473: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %475 .str475: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %477 .str477: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %479 .str479: array<i8, 66> [storage=static] = code_units<array<i8, 66>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %482 .str482: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %484 .str484: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %486 .str486: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %488 .str488: array<i8, 70> [storage=static] = code_units<array<i8, 70>>([70, 65, 73, 76, 58, 32, 108, 105, 110, 101, 32, 37, 105, 58, 32, 95, 95, 98, 117, 105, 108, 116, 105, 110, 95, 111, 98, 106, 101, 99, 116, 95, 115, 105, 122, 101, 40, 112, 115, 57, 45, 62, 97, 32, 43, 32, 49, 44, 32, 37, 105, 41, 32, 61, 61, 32, 37, 122, 117, 44, 32, 103, 111, 116, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %38 @__builtin_object_size(%36 <unnamed>: ptr<const void>, %37 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %40 @__builtin_printf(%39 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @test_arrays() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %34
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %35
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%41)), const<i32>(95), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(0)));
// DEFAULT-NEXT:                             let %490: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %491: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%490), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%491));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %42
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%43)), const<i32>(95), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(1)));
// DEFAULT-NEXT:                             let %492: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %493: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%492), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%493));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %44
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%45)), const<i32>(95), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(2)));
// DEFAULT-NEXT:                             let %494: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %495: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%494), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%495));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %46
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%47)), const<i32>(95), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%2)), const<i32>(3)));
// DEFAULT-NEXT:                             let %496: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %497: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%496), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%497));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %48
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %49
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%50)), const<i32>(97), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(0)));
// DEFAULT-NEXT:                             let %498: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %499: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%498), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%499));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %51
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%52)), const<i32>(97), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(1)));
// DEFAULT-NEXT:                             let %500: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %501: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%500), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%501));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %53
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%54)), const<i32>(97), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(2)));
// DEFAULT-NEXT:                             let %502: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %503: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%502), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%503));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %55
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%56)), const<i32>(97), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(3)));
// DEFAULT-NEXT:                             let %504: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %505: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%504), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%505));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %57
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %58
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%59)), const<i32>(98), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(0)));
// DEFAULT-NEXT:                             let %506: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %507: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%506), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%507));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %60
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%61)), const<i32>(98), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(1)));
// DEFAULT-NEXT:                             let %508: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %509: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%508), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%509));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %62
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%63)), const<i32>(98), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(2)));
// DEFAULT-NEXT:                             let %510: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %511: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%510), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%511));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %64
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%65)), const<i32>(98), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(3)));
// DEFAULT-NEXT:                             let %512: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %513: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%512), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%513));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %66
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %67
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%68)), const<i32>(100), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(0)));
// DEFAULT-NEXT:                             let %514: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %515: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%514), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%515));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %69
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%70)), const<i32>(100), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(1)));
// DEFAULT-NEXT:                             let %516: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %517: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%516), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%517));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %71
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%72)), const<i32>(100), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(2)));
// DEFAULT-NEXT:                             let %518: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %519: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%518), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%519));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %73
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%74)), const<i32>(100), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%5)), const<i32>(3)));
// DEFAULT-NEXT:                             let %520: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %521: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%520), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%521));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %75
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %76
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%77)), const<i32>(101), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(0)));
// DEFAULT-NEXT:                             let %522: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %523: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%522), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%523));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %78
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%79)), const<i32>(101), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(1)));
// DEFAULT-NEXT:                             let %524: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %525: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%524), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%525));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %80
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%81)), const<i32>(101), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(2)));
// DEFAULT-NEXT:                             let %526: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %527: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%526), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%527));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %82
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%83)), const<i32>(101), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%6)), const<i32>(3)));
// DEFAULT-NEXT:                             let %528: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %529: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%528), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%529));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %84
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %85
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%86)), const<i32>(102), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(0)));
// DEFAULT-NEXT:                             let %530: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %531: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%530), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%531));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %87
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%88)), const<i32>(102), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(1)));
// DEFAULT-NEXT:                             let %532: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %533: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%532), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%533));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %89
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%90)), const<i32>(102), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(2)));
// DEFAULT-NEXT:                             let %534: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %535: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%534), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%535));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %91
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%92)), const<i32>(102), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)), const<i32>(3)));
// DEFAULT-NEXT:                             let %536: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %537: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%536), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%537));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %93
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %94
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%95)), const<i32>(104), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(0)));
// DEFAULT-NEXT:                             let %538: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %539: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%538), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%539));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %96
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%97)), const<i32>(104), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(1)));
// DEFAULT-NEXT:                             let %540: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %541: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%540), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%541));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %98
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%99)), const<i32>(104), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(2)));
// DEFAULT-NEXT:                             let %542: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %543: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%542), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%543));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %100
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%101)), const<i32>(104), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(%4)), const<i32>(3)));
// DEFAULT-NEXT:                             let %544: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %545: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%544), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%545));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %102
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %103
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%104)), const<i32>(105), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(0)));
// DEFAULT-NEXT:                             let %546: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %547: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%546), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%547));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %105
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%106)), const<i32>(105), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(1)));
// DEFAULT-NEXT:                             let %548: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %549: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%548), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%549));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %107
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%108)), const<i32>(105), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(2)));
// DEFAULT-NEXT:                             let %550: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %551: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%550), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%551));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %109
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%110)), const<i32>(105), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(%3)), const<i32>(3)));
// DEFAULT-NEXT:                             let %552: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %553: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%552), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%553));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %111
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %112
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%113)), const<i32>(107), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(0)));
// DEFAULT-NEXT:                             let %554: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %555: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%554), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%555));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %114
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%115)), const<i32>(107), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(1)));
// DEFAULT-NEXT:                             let %556: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %557: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%556), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%557));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %116
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%117)), const<i32>(107), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(2)));
// DEFAULT-NEXT:                             let %558: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %559: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%558), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%559));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %118
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%119)), const<i32>(107), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(0)>(%8)), const<i32>(3)));
// DEFAULT-NEXT:                             let %560: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %561: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%560), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%561));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %120
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %121
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%122)), const<i32>(108), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(0)));
// DEFAULT-NEXT:                             let %562: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %563: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%562), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%563));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %123
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%124)), const<i32>(108), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(1)));
// DEFAULT-NEXT:                             let %564: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %565: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%564), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%565));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %125
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%126)), const<i32>(108), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(2)));
// DEFAULT-NEXT:                             let %566: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %567: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%566), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%567));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %127
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%128)), const<i32>(108), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(1)>(%9)), const<i32>(3)));
// DEFAULT-NEXT:                             let %568: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %569: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%568), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%569));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %129
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %130
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%131)), const<i32>(109), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(0)));
// DEFAULT-NEXT:                             let %570: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %571: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%570), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%571));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %132
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%133)), const<i32>(109), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(1)));
// DEFAULT-NEXT:                             let %572: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %573: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%572), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%573));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %134
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%135)), const<i32>(109), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(2)));
// DEFAULT-NEXT:                             let %574: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %575: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%574), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%575));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %136
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%137)), const<i32>(109), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(4), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(36)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(9)>(%10)), const<i32>(3)));
// DEFAULT-NEXT:                             let %576: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %577: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%576), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%577));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %138
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %139
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%140)), const<i32>(114), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(0)));
// DEFAULT-NEXT:                             let %578: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %579: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%578), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%579));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %141
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%142)), const<i32>(114), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(1)));
// DEFAULT-NEXT:                             let %580: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %581: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%580), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%581));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %143
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%144)), const<i32>(114), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(2)));
// DEFAULT-NEXT:                             let %582: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %583: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%582), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%583));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %145
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%146)), const<i32>(114), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11)), const<i32>(3)));
// DEFAULT-NEXT:                             let %584: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %585: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%584), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%585));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %147
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %148
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%149)), const<i32>(115), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(0)));
// DEFAULT-NEXT:                             let %586: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %587: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%586), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%587));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %150
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%151)), const<i32>(115), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(1)));
// DEFAULT-NEXT:                             let %588: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %589: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%588), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%589));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %152
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%153)), const<i32>(115), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(2)));
// DEFAULT-NEXT:                             let %590: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %591: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%590), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%591));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %154
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%155)), const<i32>(115), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0))))), const<i32>(3)));
// DEFAULT-NEXT:                             let %592: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %593: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%592), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%593));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %156
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %157
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(71)>(%158)), const<i32>(116), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(0)));
// DEFAULT-NEXT:                             let %594: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %595: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%594), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%595));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %159
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(71)>(%160)), const<i32>(116), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(1)));
// DEFAULT-NEXT:                             let %596: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %597: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%596), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%597));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %161
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(71)>(%162)), const<i32>(116), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(4)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(2)));
// DEFAULT-NEXT:                             let %598: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %599: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%598), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%599));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %163
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(71)>(%164)), const<i32>(116), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0))))), const<i32>(3)));
// DEFAULT-NEXT:                             let %600: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %601: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%600), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%601));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %165
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %166
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(69)>(%167)), const<i32>(117), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %602: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %603: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%602), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%603));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %168
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(69)>(%169)), const<i32>(117), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %604: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %605: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%604), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%605));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %170
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(69)>(%171)), const<i32>(117), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %606: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %607: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%606), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%607));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %172
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(69)>(%173)), const<i32>(117), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<array<i8, 2>, 2>>, subtract=false, element=array<array<i8, 2>, 2>, overflow=ub>(addr_of<ptr<array<array<i8, 2>, 2>>>(%11), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %608: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %609: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%608), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%609));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %174
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %175
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(72)>(%176)), const<i32>(118), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %610: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %611: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%610), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%611));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %177
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(72)>(%178)), const<i32>(118), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %612: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %613: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%612), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%613));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %179
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(72)>(%180)), const<i32>(118), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %614: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %615: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%614), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%615));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %181
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(72)>(%182)), const<i32>(118), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(addr_of<ptr<array<i8, 2>>>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %616: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %617: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%616), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%617));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %183
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %184
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%185)), const<i32>(119), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %618: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %619: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%618), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%619));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %186
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%187)), const<i32>(119), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %620: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %621: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%620), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%621));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %188
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%189)), const<i32>(119), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %622: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %623: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%622), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%623));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %190
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%191)), const<i32>(119), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(deref(ptr_offset<ptr<array<i8, 2>>, subtract=false, element=array<i8, 2>, overflow=ub>(array_decay<ptr<array<i8, 2>>, length=Some(2)>(%11), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %624: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %625: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%624), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%625));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %192
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %193
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%194)), const<i32>(121), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(0)));
// DEFAULT-NEXT:                             let %626: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %627: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%626), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%627));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %195
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%196)), const<i32>(121), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(1)));
// DEFAULT-NEXT:                             let %628: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %629: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%628), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%629));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %197
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%198)), const<i32>(121), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(2)));
// DEFAULT-NEXT:                             let %630: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %631: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%630), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%631));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %199
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%200)), const<i32>(121), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(5), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12)), const<i32>(3)));
// DEFAULT-NEXT:                             let %632: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %633: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%632), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%633));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %201
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %202
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%203)), const<i32>(122), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(0)));
// DEFAULT-NEXT:                             let %634: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %635: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%634), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%635));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %204
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%205)), const<i32>(122), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(1)));
// DEFAULT-NEXT:                             let %636: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %637: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%636), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%637));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %206
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%207)), const<i32>(122), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(15)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(2)));
// DEFAULT-NEXT:                             let %638: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %639: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%638), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%639));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %208
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%209)), const<i32>(122), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(5)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(0))), const<i32>(3)));
// DEFAULT-NEXT:                             let %640: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %641: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%640), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%641));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %210
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %211
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%212)), const<i32>(123), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %642: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %643: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%642), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%643));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %213
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%214)), const<i32>(123), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %644: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %645: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%644), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%645));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %215
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%216)), const<i32>(123), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(14)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %646: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %647: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%646), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%647));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %217
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(75)>(%218)), const<i32>(123), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(deref(ptr_offset<ptr<array<i8, 5>>, subtract=false, element=array<i8, 5>, overflow=ub>(array_decay<ptr<array<i8, 5>>, length=Some(3)>(%12), const<i32>(0)))), const<i32>(0)))), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %648: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %649: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%648), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%649));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %219
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %220
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%221)), const<i32>(125), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(0)));
// DEFAULT-NEXT:                             let %650: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %651: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%650), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%651));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %222
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%223)), const<i32>(125), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(1)));
// DEFAULT-NEXT:                             let %652: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %653: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%652), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%653));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %224
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%225)), const<i32>(125), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(2)));
// DEFAULT-NEXT:                             let %654: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %655: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%654), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%655));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %226
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%227)), const<i32>(125), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(0))), const<i32>(3)));
// DEFAULT-NEXT:                             let %656: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %657: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%656), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%657));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %228
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %229
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%230)), const<i32>(126), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %658: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %659: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%658), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%659));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %231
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%232)), const<i32>(126), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %660: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %661: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%660), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%661));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %233
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%234)), const<i32>(126), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %662: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %663: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%662), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%663));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %235
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%236)), const<i32>(126), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %664: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %665: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%664), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%665));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %237
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %238
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(67)>(%239)), const<i32>(127), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %666: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %667: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%666), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%667));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %240
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(67)>(%241)), const<i32>(127), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %668: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %669: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%668), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%669));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %242
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(67)>(%243)), const<i32>(127), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %670: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %671: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%670), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%671));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %244
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(67)>(%245)), const<i32>(127), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<array<i8, 1>>, subtract=false, element=array<i8, 1>, overflow=ub>(addr_of<ptr<array<i8, 1>>>(%5), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %672: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %673: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%672), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%673));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %246
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %247
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%248)), const<i32>(131), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(0)));
// DEFAULT-NEXT:                             let %674: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %675: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%674), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%675));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %249
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%250)), const<i32>(131), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(1)));
// DEFAULT-NEXT:                             let %676: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %677: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%676), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%677));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %251
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%252)), const<i32>(131), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(2)));
// DEFAULT-NEXT:                             let %678: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %679: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%678), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%679));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %253
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%254)), const<i32>(131), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(%5), const<i32>(2))), const<i32>(3)));
// DEFAULT-NEXT:                             let %680: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %681: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%680), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%681));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %255
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %256
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%257)), const<i32>(133), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(0)));
// DEFAULT-NEXT:                             let %682: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %683: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%682), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%683));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %258
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%259)), const<i32>(133), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(1)));
// DEFAULT-NEXT:                             let %684: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %685: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%684), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%685));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %260
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%261)), const<i32>(133), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(2)));
// DEFAULT-NEXT:                             let %686: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %687: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%686), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%687));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %262
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%263)), const<i32>(133), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(0))), const<i32>(3)));
// DEFAULT-NEXT:                             let %688: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %689: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%688), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%689));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %264
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %265
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%266)), const<i32>(134), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %690: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %691: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%690), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%691));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %267
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%268)), const<i32>(134), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %692: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %693: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%692), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%693));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %269
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%270)), const<i32>(134), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %694: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %695: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%694), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%695));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %271
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%272)), const<i32>(134), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %696: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %697: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%696), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%697));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %273
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %274
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%275)), const<i32>(135), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(0)));
// DEFAULT-NEXT:                             let %698: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %699: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%698), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%699));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %276
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%277)), const<i32>(135), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(1)));
// DEFAULT-NEXT:                             let %700: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %701: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%700), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%701));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %278
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%279)), const<i32>(135), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(2)));
// DEFAULT-NEXT:                             let %702: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %703: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%702), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%703));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %280
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%281)), const<i32>(135), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(%6), const<i32>(2))), const<i32>(3)));
// DEFAULT-NEXT:                             let %704: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %705: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%704), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%705));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_structs(%29 psx: ptr<@type1>, %30 ps0: ptr<@type2>, %31 ps1: ptr<@type3>, %32 ps9: ptr<@type5>) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %282
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %283
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%284)), const<i32>(143), const<i32>(0), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(0)));
// DEFAULT-NEXT:                             let %706: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %707: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%706), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%707));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %285
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%286)), const<i32>(143), const<i32>(1), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(1)));
// DEFAULT-NEXT:                             let %708: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %709: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%708), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%709));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %287
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%288)), const<i32>(143), const<i32>(2), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(2)));
// DEFAULT-NEXT:                             let %710: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %711: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%710), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%711));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %289
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%290)), const<i32>(143), const<i32>(3), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%14)), const<i32>(3)));
// DEFAULT-NEXT:                             let %712: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %713: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%712), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%713));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %291
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %292
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%293)), const<i32>(147), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(0)));
// DEFAULT-NEXT:                             let %714: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %715: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%714), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%715));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %294
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%295)), const<i32>(147), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(1)));
// DEFAULT-NEXT:                             let %716: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %717: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%716), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%717));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %296
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%297)), const<i32>(147), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(2)));
// DEFAULT-NEXT:                             let %718: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %719: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%718), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%719));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %298
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%299)), const<i32>(147), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%29)), const<i32>(3)));
// DEFAULT-NEXT:                             let %720: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %721: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%720), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%721));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %300
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %301
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%302)), const<i32>(151), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(0)));
// DEFAULT-NEXT:                             let %722: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %723: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%722), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%723));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %303
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%304)), const<i32>(151), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(1)));
// DEFAULT-NEXT:                             let %724: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %725: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%724), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%725));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %305
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%306)), const<i32>(151), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(2)));
// DEFAULT-NEXT:                             let %726: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %727: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%726), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%727));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %307
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%308)), const<i32>(151), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(%14))), const<i32>(3)));
// DEFAULT-NEXT:                             let %728: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %729: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%728), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%729));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %309
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %310
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%311)), const<i32>(155), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(0)));
// DEFAULT-NEXT:                             let %730: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %731: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%730), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%731));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %312
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%313)), const<i32>(155), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(1)));
// DEFAULT-NEXT:                             let %732: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %733: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%732), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%733));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %314
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%315)), const<i32>(155), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(2)));
// DEFAULT-NEXT:                             let %734: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %735: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%734), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%735));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %316
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%317)), const<i32>(155), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=None>(field1(deref(read<ptr<@type1>>(%29))))), const<i32>(3)));
// DEFAULT-NEXT:                             let %736: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %737: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%736), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%737));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %318
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %319
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%320)), const<i32>(159), const<i32>(0), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(0)));
// DEFAULT-NEXT:                             let %738: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %739: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%738), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%739));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %321
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%322)), const<i32>(159), const<i32>(1), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(1)));
// DEFAULT-NEXT:                             let %740: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %741: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%740), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%741));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %323
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%324)), const<i32>(159), const<i32>(2), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(2)));
// DEFAULT-NEXT:                             let %742: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %743: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%742), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%743));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %325
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%326)), const<i32>(159), const<i32>(3), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%16)), const<i32>(3)));
// DEFAULT-NEXT:                             let %744: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %745: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%744), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%745));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %327
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %328
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%329)), const<i32>(163), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(0)));
// DEFAULT-NEXT:                             let %746: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %747: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%746), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%747));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %330
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%331)), const<i32>(163), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(1)));
// DEFAULT-NEXT:                             let %748: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %749: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%748), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%749));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %332
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%333)), const<i32>(163), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(2)));
// DEFAULT-NEXT:                             let %750: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %751: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%750), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%751));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %334
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%335)), const<i32>(163), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type2>>(%30)), const<i32>(3)));
// DEFAULT-NEXT:                             let %752: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %753: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%752), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%753));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %336
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %337
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%338)), const<i32>(167), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(0)));
// DEFAULT-NEXT:                             let %754: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %755: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%754), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%755));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %339
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%340)), const<i32>(167), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(1)));
// DEFAULT-NEXT:                             let %756: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %757: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%756), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%757));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %341
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%342)), const<i32>(167), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(2)));
// DEFAULT-NEXT:                             let %758: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %759: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%758), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%759));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %343
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%344)), const<i32>(167), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(%16))), const<i32>(3)));
// DEFAULT-NEXT:                             let %760: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %761: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%760), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%761));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %345
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %346
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%347)), const<i32>(171), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(0)));
// DEFAULT-NEXT:                             let %762: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %763: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%762), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%763));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %348
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%349)), const<i32>(171), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(1)));
// DEFAULT-NEXT:                             let %764: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %765: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%764), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%765));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %350
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%351)), const<i32>(171), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(2)));
// DEFAULT-NEXT:                             let %766: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %767: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%766), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%767));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %352
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%353)), const<i32>(171), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type2>>(%30))))), const<i32>(3)));
// DEFAULT-NEXT:                             let %768: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %769: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%768), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%769));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %354
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %355
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%356)), const<i32>(173), const<i32>(0), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(0)));
// DEFAULT-NEXT:                             let %770: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %771: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%770), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%771));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %357
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%358)), const<i32>(173), const<i32>(1), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(1)));
// DEFAULT-NEXT:                             let %772: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %773: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%772), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%773));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %359
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%360)), const<i32>(173), const<i32>(2), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(2)));
// DEFAULT-NEXT:                             let %774: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %775: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%774), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%775));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %361
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%362)), const<i32>(173), const<i32>(3), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type3>>(%18)), const<i32>(3)));
// DEFAULT-NEXT:                             let %776: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %777: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%776), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%777));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %363
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %364
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%365)), const<i32>(174), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(0)));
// DEFAULT-NEXT:                             let %778: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %779: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%778), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%779));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %366
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%367)), const<i32>(174), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(1)));
// DEFAULT-NEXT:                             let %780: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %781: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%780), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%781));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %368
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%369)), const<i32>(174), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(2)));
// DEFAULT-NEXT:                             let %782: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %783: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%782), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%783));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %370
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%371)), const<i32>(174), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(%18))), const<i32>(3)));
// DEFAULT-NEXT:                             let %784: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %785: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%784), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%785));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %372
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %373
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%374)), const<i32>(175), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %786: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %787: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%786), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%787));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %375
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%376)), const<i32>(175), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %788: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %789: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%788), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%789));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %377
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%378)), const<i32>(175), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %790: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %791: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%790), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%791));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %379
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%380)), const<i32>(175), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(%18)), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %792: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %793: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%792), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%793));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %381
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %382
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%383)), const<i32>(179), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(0)));
// DEFAULT-NEXT:                             let %794: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %795: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%794), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%795));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %384
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%385)), const<i32>(179), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(1)));
// DEFAULT-NEXT:                             let %796: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %797: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%796), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%797));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %386
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%387)), const<i32>(179), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(2)));
// DEFAULT-NEXT:                             let %798: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %799: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%798), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%799));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %388
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%389)), const<i32>(179), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(2), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type3>>(%31)), const<i32>(3)));
// DEFAULT-NEXT:                             let %800: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %801: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%800), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%801));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %390
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %391
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%392)), const<i32>(180), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(0)));
// DEFAULT-NEXT:                             let %802: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %803: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%802), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%803));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %393
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%394)), const<i32>(180), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(1)));
// DEFAULT-NEXT:                             let %804: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %805: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%804), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%805));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %395
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%396)), const<i32>(180), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(2)));
// DEFAULT-NEXT:                             let %806: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %807: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%806), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%807));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %397
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%398)), const<i32>(180), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31))))), const<i32>(3)));
// DEFAULT-NEXT:                             let %808: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %809: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%808), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%809));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %399
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %400
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%401)), const<i32>(181), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %810: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %811: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%810), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%811));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %402
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%403)), const<i32>(181), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %812: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %813: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%812), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%813));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %404
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%405)), const<i32>(181), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %814: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %815: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%814), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%815));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %406
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%407)), const<i32>(181), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type3>>(%31)))), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %816: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %817: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%816), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%817));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %408
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %409
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(0)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%410)), const<i32>(183), const<i32>(0), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(0)));
// DEFAULT-NEXT:                             let %818: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %819: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%818), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%819));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %411
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(1)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%412)), const<i32>(183), const<i32>(1), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(1)));
// DEFAULT-NEXT:                             let %820: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %821: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%820), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%821));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %413
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(2)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%414)), const<i32>(183), const<i32>(2), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(2)));
// DEFAULT-NEXT:                             let %822: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %823: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%822), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%823));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %415
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(3)), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%416)), const<i32>(183), const<i32>(3), conditional<u64>(eq<u64>(widen<u64, reason=explicit>(const<u32>(3735928559)), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), widen<u64, reason=explicit>(const<u32>(3735928559))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type5>>(%22)), const<i32>(3)));
// DEFAULT-NEXT:                             let %824: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %825: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%824), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%825));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %417
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %418
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%419)), const<i32>(184), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(0)));
// DEFAULT-NEXT:                             let %826: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %827: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%826), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%827));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %420
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%421)), const<i32>(184), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(1)));
// DEFAULT-NEXT:                             let %828: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %829: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%828), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%829));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %422
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%423)), const<i32>(184), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(2)));
// DEFAULT-NEXT:                             let %830: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %831: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%830), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%831));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %424
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%425)), const<i32>(184), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(%22))), const<i32>(3)));
// DEFAULT-NEXT:                             let %832: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %833: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%832), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%833));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %426
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %427
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%428)), const<i32>(185), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(0)));
// DEFAULT-NEXT:                             let %834: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %835: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%834), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%835));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %429
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%430)), const<i32>(185), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(1)));
// DEFAULT-NEXT:                             let %836: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %837: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%836), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%837));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %431
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%432)), const<i32>(185), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(2)));
// DEFAULT-NEXT:                             let %838: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %839: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%838), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%839));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %433
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%434)), const<i32>(185), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(9)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(0))), const<i32>(3)));
// DEFAULT-NEXT:                             let %840: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %841: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%840), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%841));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %435
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %436
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%437)), const<i32>(186), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %842: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %843: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%842), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%843));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %438
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%439)), const<i32>(186), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %844: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %845: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%844), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%845));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %440
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%441)), const<i32>(186), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(8)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %846: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %847: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%846), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%847));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %442
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%443)), const<i32>(186), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %848: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %849: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%848), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%849));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %444
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %445
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%446)), const<i32>(187), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(0)));
// DEFAULT-NEXT:                             let %850: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %851: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%850), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%851));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %447
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%448)), const<i32>(187), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(1)));
// DEFAULT-NEXT:                             let %852: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %853: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%852), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%853));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %449
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%450)), const<i32>(187), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(7)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(2)));
// DEFAULT-NEXT:                             let %854: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %855: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%854), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%855));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %451
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%452)), const<i32>(187), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(2))), const<i32>(3)));
// DEFAULT-NEXT:                             let %856: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %857: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%856), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%857));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %453
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %454
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%455)), const<i32>(188), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(0)));
// DEFAULT-NEXT:                             let %858: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %859: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%858), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%859));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %456
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%457)), const<i32>(188), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(1)));
// DEFAULT-NEXT:                             let %860: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %861: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%860), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%861));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %458
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%459)), const<i32>(188), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(2)));
// DEFAULT-NEXT:                             let %862: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %863: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%862), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%863));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %460
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68)>(%461)), const<i32>(188), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(9)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(%22)), const<i32>(9))), const<i32>(3)));
// DEFAULT-NEXT:                             let %864: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %865: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%864), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%865));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %462
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %463
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%464)), const<i32>(191), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(0)));
// DEFAULT-NEXT:                             let %866: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %867: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%866), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%867));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %465
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%466)), const<i32>(191), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(1)));
// DEFAULT-NEXT:                             let %868: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %869: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%868), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%869));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %467
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%468)), const<i32>(191), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(2)));
// DEFAULT-NEXT:                             let %870: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %871: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%870), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%871));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %469
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(63)>(%470)), const<i32>(191), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(10), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type5>>(%32)), const<i32>(3)));
// DEFAULT-NEXT:                             let %872: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %873: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%872), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%873));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %471
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %472
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%473)), const<i32>(192), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(0)));
// DEFAULT-NEXT:                             let %874: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %875: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%874), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%875));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %474
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%475)), const<i32>(192), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(1)));
// DEFAULT-NEXT:                             let %876: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %877: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%876), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%877));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %476
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%477)), const<i32>(192), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(2)));
// DEFAULT-NEXT:                             let %878: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %879: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%878), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%879));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %478
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(66)>(%479)), const<i32>(192), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), const<u64>(1), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32))))), const<i32>(3)));
// DEFAULT-NEXT:                             let %880: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %881: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%880), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%881));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %480
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %481
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(0)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%482)), const<i32>(193), const<i32>(0), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:                             let %882: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %883: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%882), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%883));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %483
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(1)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%484)), const<i32>(193), const<i32>(1), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(1)));
// DEFAULT-NEXT:                             let %884: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %885: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%884), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%885));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %485
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(2)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%486)), const<i32>(193), const<i32>(2), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(2)));
// DEFAULT-NEXT:                             let %886: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %887: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%886), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%887));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                 do %487
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(3)), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))))
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(70)>(%488)), const<i32>(193), const<i32>(3), conditional<u64>(eq<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))), widen<u64, reason=explicit>(const<u32>(3735928559))), add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%38, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field1(deref(read<ptr<@type5>>(%32)))), const<i32>(1))), const<i32>(3)));
// DEFAULT-NEXT:                             let %888: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %889: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%888), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%889));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %489 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, ptr<@type2>, ptr<@type3>, ptr<@type5>) -> void>(%28, addr_of<ptr<@type1>>(%14), addr_of<ptr<@type2>>(%16), addr_of<ptr<@type3>>(%18), addr_of<ptr<@type5>>(%22));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%489);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
