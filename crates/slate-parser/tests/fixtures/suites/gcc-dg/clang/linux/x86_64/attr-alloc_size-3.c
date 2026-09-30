/* PR c/77531 - __attribute__((alloc_size(1,2))) could also warn on
   multiplication overflow
   PR c/78284 - warn on malloc with very large arguments
   Test exercising the ability to detect and diagnose calls to allocation
   functions decorated with attribute alloc_size that either overflow or
   exceed the default maximum object size (with -Walloc-size-larger-than
   not explicitly specified).  */
/* { dg-do compile { target size32plus } } */
/* { dg-options "-O2 -Wall" } */

#define SCHAR_MAX  __SCHAR_MAX__
#define SCHAR_MIN  (-SCHAR_MAX - 1)
#define UCHAR_MAX  (SCHAR_MAX * 2 + 1)

#define SHRT_MAX   __SHRT_MAX__
#define SHRT_MIN   (-SHRT_MAX - 1)
#define USHRT_MAX  (SHRT_MAX * 2U + 1)

#define INT_MAX    __INT_MAX__
#define INT_MIN    (-INT_MAX - 1)
#define UINT_MAX   (INT_MAX * 2U + 1)

#define LONG_MAX   __LONG_MAX__
#define LONG_MIN   (-LONG_MAX - 1L)
#define ULONG_MAX  (LONG_MAX * 2LU + 1)

#define LLONG_MAX  __LLONG_MAX__
#define LLONG_MIN  (-LLONG_MAX - 1LL)
#define ULLONG_MAX (ULLONG_MAX * 2LLU + 1)

#define PTRDIFF_MAX __PTRDIFF_MAX__
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1)
#define SIZE_MAX    __SIZE_MAX__

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__    size_t;

#define ALLOC_SIZE(...) __attribute__ ((alloc_size (__VA_ARGS__)))

void* f_uchar_1 (unsigned char) ALLOC_SIZE (1);
void* f_uchar_2 (unsigned char, unsigned char) ALLOC_SIZE (1, 2);
void* f_schar_1 (signed char) ALLOC_SIZE (1);
void* f_schar_2 (signed char, signed char) ALLOC_SIZE (1, 2);

void* f_ushrt_1 (unsigned short) ALLOC_SIZE (1);
void* f_ushrt_2 (unsigned short, unsigned short) ALLOC_SIZE (1, 2);
void* f_shrt_1 (signed short) ALLOC_SIZE (1);
void* f_shrt_2 (signed short, signed short) ALLOC_SIZE (1, 2);

void* f_uint_1 (unsigned) ALLOC_SIZE (1);
void* f_uint_2 (unsigned, unsigned) ALLOC_SIZE (1, 2);
void* f_int_1 (int) ALLOC_SIZE (1);
void* f_int_2 (int, int) ALLOC_SIZE (1, 2);

void* f_ulong_1 (unsigned long) ALLOC_SIZE (1);
void* f_ulong_2 (unsigned long, unsigned long) ALLOC_SIZE (1, 2);
void* f_long_1 (long) ALLOC_SIZE (1);
void* f_long_2 (long, long) ALLOC_SIZE (1, 2);

void* f_ullong_1 (unsigned long long) ALLOC_SIZE (1);
void* f_ullong_2 (unsigned long long, unsigned long long) ALLOC_SIZE (1, 2);
void* f_llong_1 (long long) ALLOC_SIZE (1);
void* f_llong_2 (long long, long long) ALLOC_SIZE (1, 2);

void* f_size_1 (size_t) ALLOC_SIZE (1);
void* f_size_2 (size_t, size_t) ALLOC_SIZE (1, 2);

static size_t
unsigned_range (size_t min, size_t max)
{
  extern size_t random_unsigned_value (void);
  size_t val = random_unsigned_value ();
  if (val < min || max < val) val = min;
  return val;
}

static long long
signed_range (long long min, long long max)
{
  extern long long random_signed_value (void);
  long long val = random_signed_value ();
  if (val < min || max < val) val = min;
  return val;
}

static size_t
unsigned_anti_range (size_t min, size_t max)
{
  extern size_t random_unsigned_value (void);
  size_t val = random_unsigned_value ();
  if (min <= val && val <= max)
    val = min - 1;
  return val;
}

#define UR(min, max) unsigned_range (min, max)
#define SR(min, max) signed_range (min, max)

#define UAR(min, max) unsigned_anti_range (min, max)
#define SAR(min, max) signed_anti_range (min, max)


void sink (void*);

void
test_uchar_cst (void)
{
  const unsigned char max = UCHAR_MAX;

  sink (f_uchar_1 (0));
  sink (f_uchar_1 (1));
  sink (f_uchar_1 (max));

  sink (f_uchar_2 (0, 0));
  sink (f_uchar_2 (0, 1));
  sink (f_uchar_2 (1, 0));
  sink (f_uchar_2 (1, 1));
  sink (f_uchar_2 (0, max));
  sink (f_uchar_2 (max, 0));
  sink (f_uchar_2 (max, max));
}

void
test_uchar_range (unsigned char n, int i)
{
  const unsigned char max = UCHAR_MAX;

  sink (f_uchar_1 (n));

  sink (f_uchar_1 (UR (0, 1)));
  sink (f_uchar_1 (UR (1, max)));
  sink (f_uchar_1 (UR (0, max - 1)));

  sink (f_uchar_1 (UAR (1, 1)));
  sink (f_uchar_1 (UAR (1, max - 1)));
  sink (f_uchar_1 (UAR (max - 2, max - 1)));

  sink (f_uchar_2 (0, n));
  sink (f_uchar_2 (0, i));
  sink (f_uchar_2 (n, 0));
  sink (f_uchar_2 (i, 0));
  sink (f_uchar_2 (1, n));
  sink (f_uchar_2 (1, i));
  sink (f_uchar_2 (n, 1));
  sink (f_uchar_2 (i, 1));
  sink (f_uchar_2 (max, n));
  sink (f_uchar_2 (max, i));
  sink (f_uchar_2 (n, max));
  sink (f_uchar_2 (i, max));
  sink (f_uchar_2 (n, n));
  sink (f_uchar_2 (i, i));

  sink (f_uchar_2 (UR (0, 1), UR (0, 1)));
  sink (f_uchar_2 (UR (1, 2), UR (1, 2)));
  sink (f_uchar_2 (UR (1, max), UR (0, 1)));
  sink (f_uchar_2 (UR (0, 1), UR (1, max)));
}

void
test_schar_cst (void)
{
  const signed char min = SCHAR_MIN;
  const signed char max = SCHAR_MAX;

  sink (f_schar_1 (min));     /* { dg-warning "argument 1 value .-\[0-9\]+. is negative" } */
  sink (f_schar_1 (-1));      /* { dg-warning "argument 1 value .-1. is negative" } */
  sink (f_schar_1 (0));
  sink (f_schar_1 (1));
  sink (f_schar_1 (max));

  sink (f_schar_2 (0, min));     /* { dg-warning "argument 2 value .-\[0-9\]+. is negative" } */
  sink (f_schar_2 (min, 0));     /* { dg-warning "argument 1 value .-\[0-9\]+. is negative" } */
  sink (f_schar_2 (0, -1));      /* { dg-warning "argument 2 value .-1. is negative" } */
  sink (f_schar_2 (-1, 0));      /* { dg-warning "argument 1 value .-1. is negative" } */

}

void
test_schar_range (signed char n)
{
  const signed char min = SCHAR_MIN;
  const signed char max = SCHAR_MAX;

  sink (f_schar_1 (n));

  sink (f_schar_1 (SR (min, min + 1)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_schar_1 (SR (min, 0)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_schar_1 (SR (-1, 0)));   /* { dg-warning "argument 1 range \\\[-1, 0\\\] is negative" } */
  sink (f_schar_1 (SR (-1, 1)));
  sink (f_schar_1 (SR (0, 1)));
  sink (f_schar_1 (SR (0, max - 1)));
  sink (f_schar_1 (SR (1, max)));
  sink (f_schar_1 (SR (max - 1, max)));

  sink (f_schar_2 (n, n));

  sink (f_schar_2 (SR (min, min + 1), n));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_schar_2 (n, SR (min, min + 1)));   /* { dg-warning "argument 2 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_schar_2 (SR (min, min + 1), 0));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_schar_2 (0, SR (min, min + 1)));   /* { dg-warning "argument 2 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_schar_2 (SR (min, min + 1), min));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  /* { dg-warning "argument 2 value .-\[0-9\]+. is negative" "argument 2" { target *-*-* } .-1 } */
  sink (f_schar_2 (min, SR (min, min + 1)));   /* { dg-warning "argument 2 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  /* { dg-warning "argument 1 value .-\[0-9\]+. is negative" "argument 1" { target *-*-* } .-1 } */

  sink (f_schar_2 (SR (-1, 0), 0));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_schar_2 (0, SR (-1, 0)));   /* { dg-warning "argument 2 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_schar_2 (SR (-1, 0), 1));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_schar_2 (1, SR (-1, 0)));   /* { dg-warning "argument 2 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_schar_2 (SR (-1, 0), n));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_schar_2 (n, SR (-1, 0)));   /* { dg-warning "argument 2 range \\\[-\[0-9\]+, 0\\\] is negative" } */

  sink (f_schar_2 (max, SR (1, max)));
  sink (f_schar_2 (SR (1, max), max));
}

void
test_ushrt_cst (void)
{
  const unsigned short max = USHRT_MAX;

  sink (f_ushrt_1 (0));
  sink (f_ushrt_1 (1));
  sink (f_ushrt_1 (max));

  sink (f_ushrt_2 (0, 0));
  sink (f_ushrt_2 (0, 1));
  sink (f_ushrt_2 (1, 0));
  sink (f_ushrt_2 (1, 1));
  sink (f_ushrt_2 (0, max));
  sink (f_ushrt_2 (max, 0));

  if (max < SIZE_MAX && (size_t)max * max < SIZE_MAX / 2)
    sink (f_ushrt_2 (max, max));
}

void
test_ushrt_range (unsigned short n)
{
  const unsigned short max = USHRT_MAX;

  sink (f_ushrt_1 (n));
  sink (f_ushrt_1 (UR (0, 1)));
  sink (f_ushrt_1 (UR (1, max - 1)));
  sink (f_ushrt_1 (UR (1, max)));
  sink (f_ushrt_1 (UR (0, max - 1)));
}

void
test_shrt_cst (void)
{
  const short min = SHRT_MIN;
  const short max = SHRT_MAX;

  sink (f_shrt_1 (min));   /* { dg-warning "argument 1 value .-\[0-9\]+. is negative" } */
  sink (f_shrt_1 (-1));         /* { dg-warning "argument 1 value .-1. is negative" } */
  sink (f_shrt_1 (0));
  sink (f_shrt_1 (1));
  sink (f_shrt_1 (max));
}

void
test_shrt_range (short n)
{
  const short min = SHRT_MIN;
  const short max = SHRT_MAX;

  sink (f_shrt_1 (n));

  sink (f_shrt_1 (SR (min, min + 1)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_shrt_1 (SR (min, 0)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_shrt_1 (SR (-1, 0)));   /* { dg-warning "argument 1 range \\\[-1, 0\\\] is negative" } */
  sink (f_shrt_1 (SR (-1, 1)));
  sink (f_shrt_1 (SR (0, 1)));
  sink (f_shrt_1 (SR (0, max - 1)));
  sink (f_shrt_1 (SR (1, max)));
  sink (f_shrt_1 (SR (max - 1, max)));
}

void
test_uint_cst (void)
{
  const unsigned max = UINT_MAX;

  sink (f_uint_1 (0));
  sink (f_uint_1 (1));

  if (max < SIZE_MAX)
    {
      sink (f_uint_1 (max - 1));
      sink (f_uint_1 (max));
    }
}

void
test_uint_range (unsigned n)
{
  const unsigned max = UINT_MAX;

  sink (f_uint_1 (n));
  sink (f_uint_1 (UR (0, 1)));
  sink (f_uint_1 (UR (0, max - 1)));
  sink (f_uint_1 (UR (1, max - 1)));
  sink (f_uint_1 (UR (1, max)));
}

void
test_int_cst (void)
{
  const int min = INT_MIN;
  const int max = INT_MAX;

  sink (f_int_1 (min));   /* { dg-warning "argument 1 value .-\[0-9\]+. is negative" } */
  sink (f_int_1 (-1));         /* { dg-warning "argument 1 value .-1. is negative" } */
  sink (f_int_1 (0));
  sink (f_int_1 (1));
  sink (f_int_1 (max));
}

void
test_int_range (int n)
{
  const int min = INT_MIN;
  const int max = INT_MAX;

  sink (f_int_1 (n));

  sink (f_int_1 (SR (min, min + 1)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, -\[0-9\]+\\\] is negative" } */
  sink (f_int_1 (SR (min, 0)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+, 0\\\] is negative" } */
  sink (f_int_1 (SR (-1, 0)));   /* { dg-warning "argument 1 range \\\[-1, 0\\\] is negative" } */
  sink (f_int_1 (SR (-1, 1)));
  sink (f_int_1 (SR (0, 1)));
  sink (f_int_1 (SR (0, max - 1)));
  sink (f_int_1 (SR (1, max)));
  sink (f_int_1 (SR (max - 1, max)));
}

void
test_ulong_cst (void)
{
  const unsigned long max = ULONG_MAX;

  sink (f_ulong_1 (0));
  sink (f_ulong_1 (1));
#if ULONG_MAX < SIZE_MAX
  sink (f_ulong_1 (max - 1));
  sink (f_ulong_1 (max));
#else
  (void)&max;
#endif
}

void
test_ulong_range (unsigned long n)
{
  const unsigned long max = ULONG_MAX;

  sink (f_ulong_1 (n));
  sink (f_ulong_1 (UR (0, 1)));
  sink (f_ulong_1 (UR (0, max - 1)));
  sink (f_ulong_1 (UR (1, max - 1)));
  sink (f_ulong_1 (UR (1, max)));
}

void
test_long_cst (void)
{
  const long min = LONG_MIN;
  const long max = LONG_MAX;

  sink (f_long_1 (min));   /* { dg-warning "argument 1 value .-\[0-9\]+l*. is negative" } */
  sink (f_long_1 (-1));         /* { dg-warning "argument 1 value .-1l*. is negative" } */
  sink (f_long_1 (0));
  sink (f_long_1 (1));
  sink (f_long_1 (max));
}

void
test_long_range (long n)
{
  const long min = LONG_MIN;
  const long max = LONG_MAX;

  sink (f_long_1 (n));

  sink (f_long_1 (SR (min, min + 1)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+l*, -\[0-9\]+l*\\\] is negative" } */
  sink (f_long_1 (SR (min, 0)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]+l*, 0l*\\\] is negative" } */
  sink (f_long_1 (SR (-1, 0)));   /* { dg-warning "argument 1 range \\\[-1l*, 0l*\\\] is negative" } */
  sink (f_long_1 (SR (-1, 1)));
  sink (f_long_1 (SR (0, 1)));
  sink (f_long_1 (SR (0, max - 1)));
  sink (f_long_1 (SR (1, max)));
  sink (f_long_1 (SR (max - 1, max)));
}

void
test_size_cst (void)
{
  const size_t max = __SIZE_MAX__;

  sink (f_size_1 (0));
  sink (f_size_1 (1));
  sink (f_size_1 (max - 1));  /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  sink (f_size_1 (max));      /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */

  sink (f_size_2 (0, max - 1));  /* { dg-warning "argument 2 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (max - 1, 0));  /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (1, max - 1));  /* { dg-warning "argument 2 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (max - 1, 1));  /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (max - 1, max - 1));  /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  /* { dg-warning "argument 2 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" "argument 2" { target *-*-* } .-1 } */

  sink (f_size_2 (0, max));      /* { dg-warning "argument 2 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (max, 0));      /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size \[0-9\]+" } */

  sink (f_size_2 (max / 2, 2));      /* { dg-warning "product .\[0-9\]+ \\* \[0-9\]+. of arguments 1 and 2 exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (max / 2, 3));      /* { dg-warning "product .\[0-9\]+ \\* \[0-9\]+. of arguments 1 and 2 exceeds .SIZE_MAX." } */
}

void
test_size_range (size_t ui, ptrdiff_t si)
{
  const ptrdiff_t smin = PTRDIFF_MIN;
  const ptrdiff_t smax = PTRDIFF_MAX;
  const size_t umax = SIZE_MAX;

  sink (f_size_1 (ui));
  sink (f_size_1 (si));

  sink (f_size_1 (UR (0, 1)));
  sink (f_size_1 (UR (0, umax - 1)));
  sink (f_size_1 (UR (1, umax - 1)));
  sink (f_size_1 (UR (1, umax)));

  sink (f_size_1 (UAR (1, 1)));
  /* Since the only valid argument in the anti-range below is zero
     a warning is expected even though -Walloc-zero is not specified.  */
  sink (f_size_1 (UAR (1, umax / 2)));   /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " } */
  /* The only valid argument in this range is 1.  */
  sink (f_size_1 (UAR (2, umax / 2)));

  sink (f_size_2 (ui, ui));
  sink (f_size_2 (si, si));
  sink (f_size_2 (ui, umax / 2));
  sink (f_size_2 (si, umax / 2));
  sink (f_size_2 (umax / 2, ui));
  sink (f_size_2 (umax / 2, si));

  sink (f_size_2 (UR (0, 1), umax));   /* { dg-warning "argument 2 value .\[0-9\]+. exceeds maximum object size " } */
  sink (f_size_2 (UR (0, 1), umax / 2));
  sink (f_size_2 (UR (0, umax / 2), umax / 2));

  sink (f_size_2 (UR (umax / 2 + 1, umax / 2 + 2), ui));  /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " } */
  sink (f_size_2 (ui, UR (umax / 2 + 1, umax / 2 + 2)));  /* { dg-warning "argument 2 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " } */
  sink (f_size_2 (UR (umax / 2 + 1, umax), UR (umax / 2 + 1, umax)));  /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " } */
/* { dg-warning "argument 2 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " "argument 2" { target *-*-* } .-1 } */

  sink (f_size_2 (SR (smin, 1), 1));
  sink (f_size_2 (SR (smin, 1), umax / 2));
  sink (f_size_2 (SR (-1, smax), 1));
  sink (f_size_2 (SR (-1, smax), umax / 2));
  sink (f_size_2 (SR (-1, 1), 1));
  sink (f_size_2 (SR (-1, 1), umax / 2));
  sink (f_size_2 (SR (-9, 9), 1));
  sink (f_size_2 (SR (-9, 9), umax / 2));
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
// DEFAULT-NEXT:     type @type[[TYPE_ptrdiff_t:[0-9]+]] ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_f_uchar_1:[0-9]+]] @f_uchar_1(%[[VALUE0:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_uchar_2:[0-9]+]] @f_uchar_2(%[[VALUE1:[0-9]+]] <unnamed>: u8, %[[VALUE2:[0-9]+]] <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_schar_1:[0-9]+]] @f_schar_1(%[[VALUE3:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_schar_2:[0-9]+]] @f_schar_2(%[[VALUE4:[0-9]+]] <unnamed>: i8, %[[VALUE5:[0-9]+]] <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ushrt_1:[0-9]+]] @f_ushrt_1(%[[VALUE6:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ushrt_2:[0-9]+]] @f_ushrt_2(%[[VALUE7:[0-9]+]] <unnamed>: u16, %[[VALUE8:[0-9]+]] <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_shrt_1:[0-9]+]] @f_shrt_1(%[[VALUE9:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_shrt_2:[0-9]+]] @f_shrt_2(%[[VALUE10:[0-9]+]] <unnamed>: i16, %[[VALUE11:[0-9]+]] <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_uint_1:[0-9]+]] @f_uint_1(%[[VALUE12:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_uint_2:[0-9]+]] @f_uint_2(%[[VALUE13:[0-9]+]] <unnamed>: u32, %[[VALUE14:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_int_1:[0-9]+]] @f_int_1(%[[VALUE15:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_int_2:[0-9]+]] @f_int_2(%[[VALUE16:[0-9]+]] <unnamed>: i32, %[[VALUE17:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ulong_1:[0-9]+]] @f_ulong_1(%[[VALUE18:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ulong_2:[0-9]+]] @f_ulong_2(%[[VALUE19:[0-9]+]] <unnamed>: u64, %[[VALUE20:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_long_1:[0-9]+]] @f_long_1(%[[VALUE21:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_long_2:[0-9]+]] @f_long_2(%[[VALUE22:[0-9]+]] <unnamed>: i64, %[[VALUE23:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ullong_1:[0-9]+]] @f_ullong_1(%[[VALUE24:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ullong_2:[0-9]+]] @f_ullong_2(%[[VALUE25:[0-9]+]] <unnamed>: u64, %[[VALUE26:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_llong_1:[0-9]+]] @f_llong_1(%[[VALUE27:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_llong_2:[0-9]+]] @f_llong_2(%[[VALUE28:[0-9]+]] <unnamed>: i64, %[[VALUE29:[0-9]+]] <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_size_1:[0-9]+]] @f_size_1(%[[VALUE30:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_size_2:[0-9]+]] @f_size_2(%[[VALUE31:[0-9]+]] <unnamed>: u64, %[[VALUE32:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_random_unsigned_value:[0-9]+]] @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unsigned_range:[0-9]+]] @unsigned_range(%[[VALUE_min:[0-9]+]] min: u64, %[[VALUE_max:[0-9]+]] max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]]);
// DEFAULT-NEXT:         if logical_or<bool>(lt<u64>(read<u64>(%[[VALUE_val]]), read<u64>(%[[VALUE_min]])), lt<u64>(read<u64>(%[[VALUE_max]]), read<u64>(%[[VALUE_val]])))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_val]], read<u64>(%[[VALUE_min]]));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_val]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_random_signed_value:[0-9]+]] @random_signed_value() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_signed_range:[0-9]+]] @signed_range(%[[VALUE_min_2:[0-9]+]] min: i64, %[[VALUE_max_2:[0-9]+]] max: i64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val_2:[0-9]+]] val: i64 [storage=automatic] = call<i64, signature=fn() -> i64>(%[[VALUE_random_signed_value]]);
// DEFAULT-NEXT:         if logical_or<bool>(lt<i64>(read<i64>(%[[VALUE_val_2]]), read<i64>(%[[VALUE_min_2]])), lt<i64>(read<i64>(%[[VALUE_max_2]]), read<i64>(%[[VALUE_val_2]])))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_val_2]], read<i64>(%[[VALUE_min_2]]));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_val_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_unsigned_anti_range:[0-9]+]] @unsigned_anti_range(%[[VALUE_min_3:[0-9]+]] min: u64, %[[VALUE_max_3:[0-9]+]] max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val_3:[0-9]+]] val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%[[VALUE_random_unsigned_value]]);
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(read<u64>(%[[VALUE_min_3]]), read<u64>(%[[VALUE_val_3]])), le<u64>(read<u64>(%[[VALUE_val_3]]), read<u64>(%[[VALUE_max_3]])))
// DEFAULT-NEXT:             write<u64>(%[[VALUE_val_3]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_min_3]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_val_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE33:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_uchar_cst:[0-9]+]] @test_uchar_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_4:[0-9]+]] max: u8 [storage=automatic] [const] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], read<u8>(%[[VALUE_max_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), read<u8>(%[[VALUE_max_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_max_4]]), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_max_4]]), read<u8>(%[[VALUE_max_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_uchar_range:[0-9]+]] @test_uchar_range(%[[VALUE_n:[0-9]+]] n: u8, %[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_5:[0-9]+]] max: u8 [storage=automatic] [const] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u8>(%[[VALUE_max_5]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_max_5]]))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_anti_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_anti_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_max_5]]))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_anti_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_max_5]]))), const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_max_5]]))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_n]]), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]]))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_n]]), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]]))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_max_5]]), read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_max_5]]), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_n]]), read<u8>(%[[VALUE_max_5]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]]))), read<u8>(%[[VALUE_max_5]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_n]]), read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]]))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u8>(%[[VALUE_max_5]])))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u8>(%[[VALUE_max_5]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_schar_cst:[0-9]+]] @test_schar_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_4:[0-9]+]] min: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_max_6:[0-9]+]] max: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(127));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], read<i8>(%[[VALUE_min_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], read<i8>(%[[VALUE_max_6]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), read<i8>(%[[VALUE_min_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_min_4]]), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_schar_range:[0-9]+]] @test_schar_range(%[[VALUE_n_2:[0-9]+]] n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_5:[0-9]+]] min: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_max_7:[0-9]+]] max: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(127));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_max_7]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i8>(%[[VALUE_max_7]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_max_7]])), const<i32>(1))), widen<i64, reason=arg>(read<i8>(%[[VALUE_max_7]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_n_2]]), read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1))))), read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_n_2]]), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1))))), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1))))), read<i8>(%[[VALUE_min_5]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_min_5]]), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i8>(%[[VALUE_min_5]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_min_5]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))), truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(1)), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))), read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_n_2]]), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_max_7]]), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i8>(%[[VALUE_max_7]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i8>(%[[VALUE_max_7]])))), read<i8>(%[[VALUE_max_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ushrt_cst:[0-9]+]] @test_ushrt_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_8:[0-9]+]] max: u16 [storage=automatic] [const] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], read<u16>(%[[VALUE_max_8]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), read<u16>(%[[VALUE_max_8]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], read<u16>(%[[VALUE_max_8]]), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if logical_and<bool>(lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_max_8]]))))), const<u64>(18446744073709551615)), lt<u64>(mul<u64, overflow=wrap>(widen<u64, reason=explicit>(read<u16>(%[[VALUE_max_8]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_max_8]])))))), div<u64, by_zero=ub>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], read<u16>(%[[VALUE_max_8]]), read<u16>(%[[VALUE_max_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ushrt_range:[0-9]+]] @test_ushrt_range(%[[VALUE_n_3:[0-9]+]] n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_9:[0-9]+]] max: u16 [storage=automatic] [const] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], read<u16>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_max_9]]))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u16>(%[[VALUE_max_9]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_max_9]]))), const<i32>(1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_shrt_cst:[0-9]+]] @test_shrt_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_6:[0-9]+]] min: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_max_10:[0-9]+]] max: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=always>(const<i32>(32767));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], read<i16>(%[[VALUE_min_6]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], read<i16>(%[[VALUE_max_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_shrt_range:[0-9]+]] @test_shrt_range(%[[VALUE_n_4:[0-9]+]] n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_7:[0-9]+]] min: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_max_11:[0-9]+]] max: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=always>(const<i32>(32767));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], read<i16>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i16>(%[[VALUE_min_7]])), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_min_7]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i16>(%[[VALUE_min_7]])), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_max_11]])), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i16>(%[[VALUE_max_11]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_max_11]])), const<i32>(1))), widen<i64, reason=arg>(read<i16>(%[[VALUE_max_11]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_uint_cst:[0-9]+]] @test_uint_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_12:[0-9]+]] max: u32 [storage=automatic] [const] = add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_max_12]])), const<u64>(18446744073709551615))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], sub<u32, overflow=wrap>(read<u32>(%[[VALUE_max_12]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], read<u32>(%[[VALUE_max_12]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_uint_range:[0-9]+]] @test_uint_range(%[[VALUE_n_5:[0-9]+]] n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_13:[0-9]+]] max: u32 [storage=automatic] [const] = add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], read<u32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), widen<u64, reason=arg>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_max_13]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_max_13]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u32>(%[[VALUE_max_13]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_int_cst:[0-9]+]] @test_int_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_8:[0-9]+]] min: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_max_14:[0-9]+]] max: i32 [storage=automatic] [const] = const<i32>(2147483647);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], read<i32>(%[[VALUE_min_8]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], read<i32>(%[[VALUE_max_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_int_range:[0-9]+]] @test_int_range(%[[VALUE_n_6:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_9:[0-9]+]] min: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_max_15:[0-9]+]] max: i32 [storage=automatic] [const] = const<i32>(2147483647);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i32>(%[[VALUE_min_9]])), widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%[[VALUE_min_9]]), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(read<i32>(%[[VALUE_min_9]])), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_max_15]]), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i32>(%[[VALUE_max_15]]))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_max_15]]), const<i32>(1))), widen<i64, reason=arg>(read<i32>(%[[VALUE_max_15]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ulong_cst:[0-9]+]] @test_ulong_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_16:[0-9]+]] max: u64 [storage=automatic] [const] = add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         addr_of<ptr<const u64>>(%[[VALUE_max_16]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ulong_range:[0-9]+]] @test_ulong_range(%[[VALUE_n_7:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_17:[0-9]+]] max: u64 [storage=automatic] [const] = add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], read<u64>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_17]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_17]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%[[VALUE_max_17]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long_cst:[0-9]+]] @test_long_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_10:[0-9]+]] min: i64 [storage=automatic] [const] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1));
// DEFAULT-NEXT:         let %[[VALUE_max_18:[0-9]+]] max: i64 [storage=automatic] [const] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], read<i64>(%[[VALUE_min_10]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], read<i64>(%[[VALUE_max_18]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long_range:[0-9]+]] @test_long_range(%[[VALUE_n_8:[0-9]+]] n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_min_11:[0-9]+]] min: i64 [storage=automatic] [const] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1));
// DEFAULT-NEXT:         let %[[VALUE_max_19:[0-9]+]] max: i64 [storage=automatic] [const] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], read<i64>(%[[VALUE_n_8]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], read<i64>(%[[VALUE_min_11]]), add<i64, overflow=ub>(read<i64>(%[[VALUE_min_11]]), widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], read<i64>(%[[VALUE_min_11]]), widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(0)), sub<i64, overflow=ub>(read<i64>(%[[VALUE_max_19]]), widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(const<i32>(1)), read<i64>(%[[VALUE_max_19]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], sub<i64, overflow=ub>(read<i64>(%[[VALUE_max_19]]), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64>(%[[VALUE_max_19]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_size_cst:[0-9]+]] @test_size_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_max_20:[0-9]+]] max: u64 [storage=automatic] [const] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], read<u64>(%[[VALUE_max_20]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%[[VALUE_max_20]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_max_20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_size_range:[0-9]+]] @test_size_range(%[[VALUE_ui:[0-9]+]] ui: u64, %[[VALUE_si:[0-9]+]] si: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_smin:[0-9]+]] smin: i64 [storage=automatic] [const] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_smax:[0-9]+]] smax: i64 [storage=automatic] [const] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:         let %[[VALUE_umax:[0-9]+]] umax: u64 [storage=automatic] [const] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], read<u64>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_si]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%[[VALUE_umax]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_anti_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_anti_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_anti_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], read<u64>(%[[VALUE_ui]]), read<u64>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_si]])), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_si]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], read<u64>(%[[VALUE_ui]]), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_si]])), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_si]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), read<u64>(%[[VALUE_umax]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), read<u64>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], read<u64>(%[[VALUE_ui]]), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_umax]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_unsigned_range]], add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%[[VALUE_umax]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], read<i64>(%[[VALUE_smin]]), widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], read<i64>(%[[VALUE_smin]]), widen<i64, reason=arg>(const<i32>(1)))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), read<i64>(%[[VALUE_smax]]))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), read<i64>(%[[VALUE_smax]]))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(9)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_signed_range]], widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(9)))), div<u64, by_zero=ub>(read<u64>(%[[VALUE_umax]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
