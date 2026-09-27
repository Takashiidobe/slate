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
// DEFAULT-NEXT:     type @type0 ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     fn %2 @f_uchar_1(%96 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @f_uchar_2(%97 <unnamed>: u8, %98 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @f_schar_1(%99 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @f_schar_2(%100 <unnamed>: i8, %101 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @f_ushrt_1(%102 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @f_ushrt_2(%103 <unnamed>: u16, %104 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @f_shrt_1(%105 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @f_shrt_2(%106 <unnamed>: i16, %107 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @f_uint_1(%108 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %11 @f_uint_2(%109 <unnamed>: u32, %110 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @f_int_1(%111 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @f_int_2(%112 <unnamed>: i32, %113 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @f_ulong_1(%114 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @f_ulong_2(%115 <unnamed>: u64, %116 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %16 @f_long_1(%117 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %17 @f_long_2(%118 <unnamed>: i64, %119 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @f_ullong_1(%120 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %19 @f_ullong_2(%121 <unnamed>: u64, %122 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %20 @f_llong_1(%123 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %21 @f_llong_2(%124 <unnamed>: i64, %125 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %22 @f_size_1(%126 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %23 @f_size_2(%127 <unnamed>: u64, %128 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %27 @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %24 @unsigned_range(%25 min: u64, %26 max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%27);
// DEFAULT-NEXT:         if logical_or<bool>(lt<u64>(read<u64>(%28), read<u64>(%25)), lt<u64>(read<u64>(%26), read<u64>(%28)))
// DEFAULT-NEXT:             write<u64>(%28, read<u64>(%25));
// DEFAULT-NEXT:         return read<u64>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @random_signed_value() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %29 @signed_range(%30 min: i64, %31 max: i64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 val: i64 [storage=automatic] = call<i64, signature=fn() -> i64>(%32);
// DEFAULT-NEXT:         if logical_or<bool>(lt<i64>(read<i64>(%33), read<i64>(%30)), lt<i64>(read<i64>(%31), read<i64>(%33)))
// DEFAULT-NEXT:             write<i64>(%33, read<i64>(%30));
// DEFAULT-NEXT:         return read<i64>(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @unsigned_anti_range(%35 min: u64, %36 max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37 val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%27);
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(read<u64>(%35), read<u64>(%37)), le<u64>(read<u64>(%37), read<u64>(%36)))
// DEFAULT-NEXT:             write<u64>(%37, sub<u64, overflow=wrap>(read<u64>(%35), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%37);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @sink(%129 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %39 @test_uchar_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %40 max: u8 [storage=automatic] [const] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, read<u8>(%40)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), read<u8>(%40)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%40), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%40), read<u8>(%40)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test_uchar_range(%42 n: u8, %43 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %44 max: u8 [storage=automatic] [const] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(127), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, read<u8>(%42)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u8>(%44))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%44))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%34, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%34, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%44))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%2, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%34, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%44))), const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%44))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), read<u8>(%42)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%42), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), read<u8>(%42)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%42), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%44), read<u8>(%42)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%44), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%42), read<u8>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43))), read<u8>(%44)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, read<u8>(%42), read<u8>(%42)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%43)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u8>(%44)))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%3, truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))), truncate<u8, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u8>(%44))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @test_schar_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %46 min: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)));
// DEFAULT-NEXT:         let %47 max: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(127));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, read<i8>(%46)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, read<i8>(%47)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=always>(const<i32>(0)), read<i8>(%46)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, read<i8>(%46), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @test_schar_range(%49 n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %50 min: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1)));
// DEFAULT-NEXT:         let %51 max: i8 [storage=automatic] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(127));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, read<i8>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%51)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i8>(%51))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%51)), const<i32>(1))), widen<i64, reason=arg>(read<i8>(%51))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, read<i8>(%49), read<i8>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1))))), read<i8>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, read<i8>(%49), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1))))), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1))))), read<i8>(%50)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, read<i8>(%50), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i8>(%50)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))), truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=always>(const<i32>(1)), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))), read<i8>(%49)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, read<i8>(%49), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, read<i8>(%51), truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i8>(%51))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%5, truncate<i8, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i8>(%51)))), read<i8>(%51)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @test_ushrt_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53 max: u16 [storage=automatic] [const] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, read<u16>(%53)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), read<u16>(%53)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, read<u16>(%53), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if logical_and<bool>(lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%53))))), const<u64>(18446744073709551615)), lt<u64>(mul<u64, overflow=wrap>(widen<u64, reason=explicit>(read<u16>(%53)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%53)))))), div<u64, by_zero=ub>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%7, read<u16>(%53), read<u16>(%53)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @test_ushrt_range(%55 n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %56 max: u16 [storage=automatic] [const] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32767)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, read<u16>(%55)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%56))), const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u16>(%56))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%6, truncate<u16, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%56))), const<i32>(1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @test_shrt_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %58 min: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)));
// DEFAULT-NEXT:         let %59 max: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=always>(const<i32>(32767));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, read<i16>(%58)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, read<i16>(%59)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @test_shrt_range(%61 n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %62 min: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)));
// DEFAULT-NEXT:         let %63 max: i16 [storage=automatic] [const] = truncate<i16, reason=assign, fits=always>(const<i32>(32767));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, read<i16>(%61)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i16>(%62)), widen<i64, reason=arg>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%62)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i16>(%62)), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%63)), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i16>(%63))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%63)), const<i32>(1))), widen<i64, reason=arg>(read<i16>(%63))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @test_uint_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %65 max: u32 [storage=automatic] [const] = add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%65)), const<u64>(18446744073709551615))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, sub<u32, overflow=wrap>(read<u32>(%65), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, read<u32>(%65)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @test_uint_range(%67 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %68 max: u32 [storage=automatic] [const] = add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, read<u32>(%67)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), widen<u64, reason=arg>(sub<u32, overflow=wrap>(read<u32>(%68), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(sub<u32, overflow=wrap>(read<u32>(%68), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%10, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u32>(%68))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @test_int_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %70 min: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %71 max: i32 [storage=automatic] [const] = const<i32>(2147483647);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, read<i32>(%70)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, read<i32>(%71)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @test_int_range(%73 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %74 min: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %75 max: i32 [storage=automatic] [const] = const<i32>(2147483647);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, read<i32>(%73)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i32>(%74)), widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%74), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(read<i32>(%74)), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(sub<i32, overflow=ub>(read<i32>(%75), const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(read<i32>(%75))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%12, truncate<i32, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(sub<i32, overflow=ub>(read<i32>(%75), const<i32>(1))), widen<i64, reason=arg>(read<i32>(%75))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @test_ulong_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %77 max: u64 [storage=automatic] [const] = add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         addr_of<ptr<const u64>>(%77);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @test_ulong_range(%79 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %80 max: u64 [storage=automatic] [const] = add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(9223372036854775807)), const<u64>(2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, read<u64>(%79)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%80), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%80), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%80))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @test_long_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %82 min: i64 [storage=automatic] [const] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1));
// DEFAULT-NEXT:         let %83 max: i64 [storage=automatic] [const] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, read<i64>(%82)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, read<i64>(%83)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @test_long_range(%85 n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %86 min: i64 [storage=automatic] [const] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), const<i64>(1));
// DEFAULT-NEXT:         let %87 max: i64 [storage=automatic] [const] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, read<i64>(%85)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, read<i64>(%86), add<i64, overflow=ub>(read<i64>(%86), widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, read<i64>(%86), widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(0)), sub<i64, overflow=ub>(read<i64>(%87), widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(const<i32>(1)), read<i64>(%87))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%16, call<i64, signature=fn(i64, i64) -> i64>(%29, sub<i64, overflow=ub>(read<i64>(%87), widen<i64, reason=usual_arith>(const<i32>(1))), read<i64>(%87))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @test_size_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %89 max: u64 [storage=automatic] [const] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, read<u64>(%89)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), sub<u64, overflow=wrap>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%89)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, read<u64>(%89), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, div<u64, by_zero=ub>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, div<u64, by_zero=ub>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @test_size_range(%91 ui: u64, %92 si: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %93 smin: i64 [storage=automatic] [const] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         let %94 smax: i64 [storage=automatic] [const] = const<i64>(9223372036854775807);
// DEFAULT-NEXT:         let %95 umax: u64 [storage=automatic] [const] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, read<u64>(%91)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%92))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%95))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%34, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%34, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%22, call<u64, signature=fn(u64, u64) -> u64>(%34, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, read<u64>(%91), read<u64>(%91)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%92)), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%92))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, read<u64>(%91), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%92)), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%91)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%92))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), read<u64>(%95)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, call<u64, signature=fn(u64, u64) -> u64>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, call<u64, signature=fn(u64, u64) -> u64>(%24, add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), read<u64>(%91)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, read<u64>(%91), call<u64, signature=fn(u64, u64) -> u64>(%24, add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, call<u64, signature=fn(u64, u64) -> u64>(%24, add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%95)), call<u64, signature=fn(u64, u64) -> u64>(%24, add<u64, overflow=wrap>(div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), read<u64>(%95))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, read<i64>(%93), widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, read<i64>(%93), widen<i64, reason=arg>(const<i32>(1)))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), read<i64>(%94))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), read<i64>(%94))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))), widen<i64, reason=arg>(const<i32>(1)))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(9)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%38, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(call<i64, signature=fn(i64, i64) -> i64>(%29, widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(9))), widen<i64, reason=arg>(const<i32>(9)))), div<u64, by_zero=ub>(read<u64>(%95), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
