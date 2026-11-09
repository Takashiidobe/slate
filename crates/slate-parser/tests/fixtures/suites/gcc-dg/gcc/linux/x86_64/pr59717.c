/* PR c/59717 */
/* { dg-do compile } */
/* { dg-options "-std=gnu11 -Wno-implicit-function-declaration" } */

void
math (double d, int *ex, double *dp)
{
  acos (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  acosh (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  asin (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  asinh (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  atan (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  atanh (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  atan2 (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  cbrt (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  ceil (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  copysign (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  cos (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  cosh (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  erf (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  erfc (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  exp (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  exp2 (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  expm1 (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  fabs (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  fdim (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  floor (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  fma (d, d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  fmax (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  fmin (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  fmod (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  frexp (d, ex); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  hypot (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  /* We don't generate the warning for ilogb.  */
  ilogb (d);
  ldexp (d, *ex); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  lgamma (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  llrint (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  llround (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  log (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  log10 (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  log1p (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  log2 (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  logb (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  lrint (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  lround (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  modf (d, dp); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  nan (""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  nearbyint (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  nextafter (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  nexttoward (d, 20.0L); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  pow (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  remainder (d, d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  remquo (d, d, ex); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  rint (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  round (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  scalbln (d, 100L); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  scalbn (d, 100); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  sin (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  sinh (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  sincos (d, dp, dp); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  sqrt (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  tan (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  tanh (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  tgamma (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
  trunc (d); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..math.h.." "" { target *-*-* } .-1 } */
}

void
cmplx (double _Complex z)
{
  cabs (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  cacos (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  cacosh (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  carg (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  casin (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  casinh (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  catan (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  catanh (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  ccos (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  ccosh (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  cexp (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  cimag (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  clog (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  conj (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  cpow (z, z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  cproj (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  creal (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  csin (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  csinh (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  csqrt (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  ctan (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
  ctanh (z); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..complex.h.." "" { target *-*-* } .-1 } */
}

void
string (void *p, void *q, __SIZE_TYPE__ sz)
{
  memchr (p, 2, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  /* We don't generate the warning for memcmp.  */
  memcmp (p, q, sz);
  memcpy (p, q, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  memmove (p, q, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  memset (p, 0, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strcat (p, q); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strchr (p, 'a'); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  /* We don't generate the warning for strcmp.  */
  strcmp (p, q);
  strcpy (p, q); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strcspn (p, q); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strlen (p); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strncat (p, q, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  /* We don't generate the warning for strncmp.  */
  strncmp (p, q, sz);
  strncpy (p, q, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strpbrk (p, q); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strrchr (p, 'q'); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strspn (p, q); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
  strstr (p, q); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..string.h.." "" { target *-*-* } .-1 } */
}

/* Fake FILE.  */
typedef struct { int i; } FILE;

void
stdio (FILE *fp, void *p, __SIZE_TYPE__ sz)
{
  fprintf (fp, ""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  fscanf (fp, ""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  fwrite (p, sz, sz, fp); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  printf (""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  scanf (""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  snprintf ("", sz, ""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  sprintf ("", ""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
  sscanf ("", ""); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdio.h.." "" { target *-*-* } .-1 } */
}

void
stdlib (void *p, void *q, __SIZE_TYPE__ sz)
{
  abort (); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  void *p1 = calloc (sz, 1); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  exit (1); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  free (p); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  labs (1L); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  llabs (1LL); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  void *p2 = malloc (sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  void *p3 = realloc (p, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
  void *p4 = aligned_alloc (sz, sz); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..stdlib.h.." "" { target *-*-* } .-1 } */
}

void
inttypes (__INTMAX_TYPE__ j)
{
  imaxabs (j); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..inttypes.h.." "" { target *-*-* } .-1 } */
}

struct tm;

void
timeh (char *s, __SIZE_TYPE__ sz, struct tm *tm)
{
  strftime (s, sz, "", tm); /* { dg-warning "incompatible implicit" } */
  /* { dg-message "include ..time.h.." "" { target *-*-* } .-1 } */
}

// SLATE-FILECHECK-STD DEFAULT gnu11
// SLATE-FILECHECK-ARGS -Wno-implicit-function-declaration
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_tm:[0-9]+]] tm = struct incomplete;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_acos:[0-9]+]] @acos(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acosh:[0-9]+]] @acosh(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asin:[0-9]+]] @asin(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinh:[0-9]+]] @asinh(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan:[0-9]+]] @atan(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanh:[0-9]+]] @atanh(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2:[0-9]+]] @atan2(%[[VALUE6:[0-9]+]] <unnamed>: f64, %[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cbrt:[0-9]+]] @cbrt(%[[VALUE8:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceil:[0-9]+]] @ceil(%[[VALUE9:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_copysign:[0-9]+]] @copysign(%[[VALUE10:[0-9]+]] <unnamed>: f64, %[[VALUE11:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cos:[0-9]+]] @cos(%[[VALUE12:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosh:[0-9]+]] @cosh(%[[VALUE13:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erf:[0-9]+]] @erf(%[[VALUE14:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erfc:[0-9]+]] @erfc(%[[VALUE15:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE16:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2:[0-9]+]] @exp2(%[[VALUE17:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1:[0-9]+]] @expm1(%[[VALUE18:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE19:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fdim:[0-9]+]] @fdim(%[[VALUE20:[0-9]+]] <unnamed>: f64, %[[VALUE21:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_floor:[0-9]+]] @floor(%[[VALUE22:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fma:[0-9]+]] @fma(%[[VALUE23:[0-9]+]] <unnamed>: f64, %[[VALUE24:[0-9]+]] <unnamed>: f64, %[[VALUE25:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmax:[0-9]+]] @fmax(%[[VALUE26:[0-9]+]] <unnamed>: f64, %[[VALUE27:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmin:[0-9]+]] @fmin(%[[VALUE28:[0-9]+]] <unnamed>: f64, %[[VALUE29:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmod:[0-9]+]] @fmod(%[[VALUE30:[0-9]+]] <unnamed>: f64, %[[VALUE31:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frexp:[0-9]+]] @frexp(%[[VALUE32:[0-9]+]] <unnamed>: f64, %[[VALUE33:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hypot:[0-9]+]] @hypot(%[[VALUE34:[0-9]+]] <unnamed>: f64, %[[VALUE35:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogb:[0-9]+]] @ilogb(%[[VALUE36:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexp:[0-9]+]] @ldexp(%[[VALUE37:[0-9]+]] <unnamed>: f64, %[[VALUE38:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgamma:[0-9]+]] @lgamma(%[[VALUE39:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrint:[0-9]+]] @llrint(%[[VALUE40:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llround:[0-9]+]] @llround(%[[VALUE41:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log:[0-9]+]] @log(%[[VALUE42:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log10:[0-9]+]] @log10(%[[VALUE43:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log1p:[0-9]+]] @log1p(%[[VALUE44:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log2:[0-9]+]] @log2(%[[VALUE45:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logb:[0-9]+]] @logb(%[[VALUE46:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrint:[0-9]+]] @lrint(%[[VALUE47:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lround:[0-9]+]] @lround(%[[VALUE48:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_modf:[0-9]+]] @modf(%[[VALUE49:[0-9]+]] <unnamed>: f64, %[[VALUE50:[0-9]+]] <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyint:[0-9]+]] @nearbyint(%[[VALUE51:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nextafter:[0-9]+]] @nextafter(%[[VALUE52:[0-9]+]] <unnamed>: f64, %[[VALUE53:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nexttoward:[0-9]+]] @nexttoward(%[[VALUE54:[0-9]+]] <unnamed>: f64, %[[VALUE55:[0-9]+]] <unnamed>: f80) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE56:[0-9]+]] <unnamed>: f64, %[[VALUE57:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainder:[0-9]+]] @remainder(%[[VALUE58:[0-9]+]] <unnamed>: f64, %[[VALUE59:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remquo:[0-9]+]] @remquo(%[[VALUE60:[0-9]+]] <unnamed>: f64, %[[VALUE61:[0-9]+]] <unnamed>: f64, %[[VALUE62:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rint:[0-9]+]] @rint(%[[VALUE63:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_round:[0-9]+]] @round(%[[VALUE64:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_scalbln:[0-9]+]] @scalbln(%[[VALUE65:[0-9]+]] <unnamed>: f64, %[[VALUE66:[0-9]+]] <unnamed>: i64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbn:[0-9]+]] @scalbn(%[[VALUE67:[0-9]+]] <unnamed>: f64, %[[VALUE68:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE69:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinh:[0-9]+]] @sinh(%[[VALUE70:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincos:[0-9]+]] @sincos(%[[VALUE71:[0-9]+]] <unnamed>: f64, %[[VALUE72:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE73:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrt:[0-9]+]] @sqrt(%[[VALUE74:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tan:[0-9]+]] @tan(%[[VALUE75:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanh:[0-9]+]] @tanh(%[[VALUE76:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tgamma:[0-9]+]] @tgamma(%[[VALUE77:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_trunc:[0-9]+]] @trunc(%[[VALUE78:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_math:[0-9]+]] @math(%[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_ex:[0-9]+]] ex: ptr<i32>, %[[VALUE_dp:[0-9]+]] dp: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_acos]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_acosh]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_asin]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_asinh]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_atan]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_atanh]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_atan2]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cbrt]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cosh]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_erf]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_erfc]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp2]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_expm1]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fdim]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_fma]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmax]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmin]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE_frexp]], read<f64>(%[[VALUE_d]]), read<ptr<i32>>(%[[VALUE_ex]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_hypot]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_ilogb]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_ldexp]], read<f64>(%[[VALUE_d]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_ex]]))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_lgamma]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_llrint]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_llround]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log10]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log1p]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log2]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_logb]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_lrint]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_lround]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%[[VALUE_modf]], read<f64>(%[[VALUE_d]]), read<ptr<f64>>(%[[VALUE_dp]]));
// DEFAULT-NEXT:         const<f64>(bits=0x7ff8000000000000);
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_nearbyint]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_nextafter]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f80) -> f64>(%[[VALUE_nexttoward]], read<f64>(%[[VALUE_d]]), const<f80>(20));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_remainder]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%[[VALUE_remquo]], read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]), read<ptr<i32>>(%[[VALUE_ex]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_rint]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_round]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i64) -> f64>(%[[VALUE_scalbln]], read<f64>(%[[VALUE_d]]), const<i64>(100));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_scalbn]], read<f64>(%[[VALUE_d]]), const<i32>(100));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sinh]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE_sincos]], read<f64>(%[[VALUE_d]]), read<ptr<f64>>(%[[VALUE_dp]]), read<ptr<f64>>(%[[VALUE_dp]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tan]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tanh]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tgamma]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cabs:[0-9]+]] @cabs(%[[VALUE79:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_cacos:[0-9]+]] @cacos(%[[VALUE80:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cacosh:[0-9]+]] @cacosh(%[[VALUE81:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_carg:[0-9]+]] @carg(%[[VALUE82:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_casin:[0-9]+]] @casin(%[[VALUE83:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_casinh:[0-9]+]] @casinh(%[[VALUE84:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_catan:[0-9]+]] @catan(%[[VALUE85:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_catanh:[0-9]+]] @catanh(%[[VALUE86:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_ccos:[0-9]+]] @ccos(%[[VALUE87:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_ccosh:[0-9]+]] @ccosh(%[[VALUE88:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cexp:[0-9]+]] @cexp(%[[VALUE89:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cimag:[0-9]+]] @cimag(%[[VALUE90:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_clog:[0-9]+]] @clog(%[[VALUE91:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_conj:[0-9]+]] @conj(%[[VALUE92:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [memory=none] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cpow:[0-9]+]] @cpow(%[[VALUE93:[0-9]+]] <unnamed>: complex<f64>, %[[VALUE94:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cproj:[0-9]+]] @cproj(%[[VALUE95:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [memory=none] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_creal:[0-9]+]] @creal(%[[VALUE96:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_csin:[0-9]+]] @csin(%[[VALUE97:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_csinh:[0-9]+]] @csinh(%[[VALUE98:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_csqrt:[0-9]+]] @csqrt(%[[VALUE99:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_ctan:[0-9]+]] @ctan(%[[VALUE100:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_ctanh:[0-9]+]] @ctanh(%[[VALUE101:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cmplx:[0-9]+]] @cmplx(%[[VALUE_z:[0-9]+]] z: complex<f64>) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_cacos]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_cacosh]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_carg]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_casin]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_casinh]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_catan]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_catanh]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ccos]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ccosh]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_cexp]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cimag]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_clog]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_conj]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>, complex<f64>) -> complex<f64>, abi=sysv64(native_c, native_c) -> native_c>(%[[VALUE_cpow]], read<complex<f64>>(%[[VALUE_z]]), read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_cproj]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_creal]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_csin]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_csinh]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_csqrt]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ctan]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_ctanh]], read<complex<f64>>(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memchr:[0-9]+]] @memchr(%[[VALUE102:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE103:[0-9]+]] <unnamed>: i32, %[[VALUE104:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE105:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE106:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE107:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE108:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE109:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE110:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memmove:[0-9]+]] @memmove(%[[VALUE111:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE112:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE113:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE114:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE115:[0-9]+]] <unnamed>: i32, %[[VALUE116:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcat:[0-9]+]] @strcat(%[[VALUE117:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE118:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE119:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE120:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE121:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE122:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE123:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE124:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcspn:[0-9]+]] @strcspn(%[[VALUE125:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE126:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE127:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncat:[0-9]+]] @strncat(%[[VALUE128:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE129:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE130:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncmp:[0-9]+]] @strncmp(%[[VALUE131:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE132:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE133:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE134:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE135:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE136:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strpbrk:[0-9]+]] @strpbrk(%[[VALUE137:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE138:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strrchr:[0-9]+]] @strrchr(%[[VALUE139:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE140:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strspn:[0-9]+]] @strspn(%[[VALUE141:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE142:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strstr:[0-9]+]] @strstr(%[[VALUE143:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE144:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_string:[0-9]+]] @string(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_q:[0-9]+]] q: ptr<void>, %[[VALUE_sz:[0-9]+]] sz: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(2), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memmove]], read<ptr<void>>(%[[VALUE_p]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], read<ptr<void>>(%[[VALUE_p]]), const<i32>(0), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], pointer_cast<ptr<i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(97));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], pointer_cast<ptr<i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_strcspn]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]]))) [c="__size_t"];
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]]))) [c="__size_t"];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], pointer_cast<ptr<i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_strncmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], pointer_cast<ptr<i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz]])) [c="__size_t"]);
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strpbrk]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strrchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(113));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_strspn]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]]))) [c="__size_t"];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<void>>(%[[VALUE_q]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fprintf:[0-9]+]] @fprintf(%[[VALUE145:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE146:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fscanf:[0-9]+]] @fscanf(%[[VALUE147:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE148:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fwrite:[0-9]+]] @fwrite(%[[VALUE149:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE150:[0-9]+]] <unnamed>: u64, %[[VALUE151:[0-9]+]] <unnamed>: u64, %[[VALUE152:[0-9]+]] <unnamed>: ptr<void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE153:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scanf:[0-9]+]] @scanf(%[[VALUE154:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_snprintf:[0-9]+]] @snprintf(%[[VALUE155:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE156:[0-9]+]] <unnamed>: u64, %[[VALUE157:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE158:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE159:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sscanf:[0-9]+]] @sscanf(%[[VALUE160:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE161:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_stdio:[0-9]+]] @stdio(%[[VALUE_fp:[0-9]+]] fp: ptr<@type[[TYPE0]]> [c="FILE *"], %[[VALUE_p_2:[0-9]+]] p: ptr<void>, %[[VALUE_sz_2:[0-9]+]] sz: u64) -> void [linkage=external] [fallthrough=ret_void] [c="void(FILE *, void *, unsigned long)"] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, ptr<const i8>, ...) -> i32>(%[[VALUE_fprintf]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_fp]]) [c="FILE *"]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, ptr<const i8>, ...) -> i32>(%[[VALUE_fscanf]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_fp]]) [c="FILE *"]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fwrite]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_2]])), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_2]])) [c="__size_t"], reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_2]])) [c="__size_t"], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_fp]]) [c="FILE *"])) [c="__size_t"];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_scanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ...) -> i32>(%[[VALUE_snprintf]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]]), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_2]])) [c="__size_t"], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_7]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_8]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sscanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_9]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE162:[0-9]+]] <unnamed>: u64, %[[VALUE163:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE164:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE165:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_labs:[0-9]+]] @labs(%[[VALUE166:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_llabs:[0-9]+]] @llabs(%[[VALUE167:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE168:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE169:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE170:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_aligned_alloc:[0-9]+]] @aligned_alloc(%[[VALUE171:[0-9]+]] <unnamed>: u64, %[[VALUE172:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_stdlib:[0-9]+]] @stdlib(%[[VALUE_p_3:[0-9]+]] p: ptr<void>, %[[VALUE_q_2:[0-9]+]] q: ptr<void>, %[[VALUE_sz_3:[0-9]+]] sz: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_3]])) [c="__size_t"], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))) [c="__size_t"]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         call<i64, signature=fn(i64) -> i64>(%[[VALUE_labs]], const<i64>(1));
// DEFAULT-NEXT:         call<i64, signature=fn(i64) -> i64>(%[[VALUE_llabs]], const<i64>(1));
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_3]])) [c="__size_t"]);
// DEFAULT-NEXT:         let %[[VALUE_p3:[0-9]+]] p3: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_p_3]]), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_3]])) [c="__size_t"]);
// DEFAULT-NEXT:         let %[[VALUE_p4:[0-9]+]] p4: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_3]])) [c="__size_t"], reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_3]])) [c="__size_t"]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_imaxabs:[0-9]+]] @imaxabs(%[[VALUE173:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_inttypes:[0-9]+]] @inttypes(%[[VALUE_j:[0-9]+]] j: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i64, signature=fn(i64) -> i64>(%[[VALUE_imaxabs]], read<i64>(%[[VALUE_j]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strftime:[0-9]+]] @strftime(%[[VALUE174:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE175:[0-9]+]] <unnamed>: u64, %[[VALUE176:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE177:[0-9]+]] <unnamed>: ptr<const void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_timeh:[0-9]+]] @timeh(%[[VALUE_s:[0-9]+]] s: ptr<i8>, %[[VALUE_sz_4:[0-9]+]] sz: u64, %[[VALUE_tm:[0-9]+]] tm: ptr<@type[[TYPE_tm]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<u64, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<const void>) -> u64>(%[[VALUE_strftime]], read<ptr<i8>>(%[[VALUE_s]]), reinterpret<u64, reason=arg>(read<u64>(%[[VALUE_sz_4]])) [c="__size_t"], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_11]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_tm]]>>(%[[VALUE_tm]]))) [c="__size_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
