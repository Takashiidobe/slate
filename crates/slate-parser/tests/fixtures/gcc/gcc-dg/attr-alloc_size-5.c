/* PR c/78284 - warn on malloc with very large arguments
   Test exercising the ability to detect and diagnose calls to allocation
   functions decorated with attribute alloc_size that attempt to allocate
   zero bytes.  For standard allocation functions the return value is
   implementation-defined and so relying on it may be a source of bugs.  */
/* { dg-do compile } */
/* { dg-options "-O1 -Wall -Walloc-zero" } */

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

#define SIZE_MAX   __SIZE_MAX__

typedef __SIZE_TYPE__ size_t;


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

void* f_size_1_nonnull (size_t)
     ALLOC_SIZE (1)  __attribute__ ((returns_nonnull));
void* f_size_2_nonnull (size_t, size_t)
     ALLOC_SIZE (1, 2) __attribute__ ((returns_nonnull));

void sink (void*);

void
test_uchar (unsigned char n)
{
  sink (f_uchar_1 (0));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_uchar_2 (0, 1));  /* { dg-warning "argument 1 value is zero" } */
  sink (f_uchar_2 (1, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_uchar_2 (n, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_uchar_2 (0, n));  /* { dg-warning "argument 1 value is zero" } */

  sink (f_uchar_1 (n));
  n = 0;
  sink (f_uchar_1 (n));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_uchar_2 (1, n));  /* { dg-warning "argument 2 value is zero" } */
}

void
test_schar (signed char n)
{
  sink (f_schar_1 (0));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_schar_2 (0, 1));  /* { dg-warning "argument 1 value is zero" } */
  sink (f_schar_2 (1, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_schar_2 (n, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_schar_2 (0, n));  /* { dg-warning "argument 1 value is zero" } */

  sink (f_schar_1 (n));
  n = 0;
  sink (f_schar_1 (n));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_schar_2 (1, n));  /* { dg-warning "argument 2 value is zero" } */
}

void
test_ushrt (unsigned short n)
{
  sink (f_ushrt_1 (0));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_ushrt_2 (0, 1));  /* { dg-warning "argument 1 value is zero" } */
  sink (f_ushrt_2 (1, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_ushrt_2 (n, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_ushrt_2 (0, n));  /* { dg-warning "argument 1 value is zero" } */

  sink (f_ushrt_1 (n));
  n = 0;
  sink (f_ushrt_1 (n));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_ushrt_2 (1, n));  /* { dg-warning "argument 2 value is zero" } */
}

void
test_shrt (short n)
{
  sink (f_shrt_1 (0));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_shrt_2 (0, 1));   /* { dg-warning "argument 1 value is zero" } */
  sink (f_shrt_2 (1, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_shrt_2 (n, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_shrt_2 (0, n));   /* { dg-warning "argument 1 value is zero" } */

  sink (f_shrt_1 (n));
  n = 0;
  sink (f_shrt_1 (n));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_shrt_2 (1, n));   /* { dg-warning "argument 2 value is zero" } */
}

void
test_uint (unsigned n)
{
  sink (f_uint_1 (0));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_uint_2 (0, 1));   /* { dg-warning "argument 1 value is zero" } */
  sink (f_uint_2 (1, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_uint_2 (n, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_uint_2 (0, n));   /* { dg-warning "argument 1 value is zero" } */

  sink (f_uint_1 (n));
  n = 0;
  sink (f_uint_1 (n));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_uint_2 (1, n));   /* { dg-warning "argument 2 value is zero" } */
}

void
test_int (int n)
{
  sink (f_int_1 (0));       /* { dg-warning "argument 1 value is zero" } */
  sink (f_int_2 (0, 1));    /* { dg-warning "argument 1 value is zero" } */
  sink (f_int_2 (1, 0));    /* { dg-warning "argument 2 value is zero" } */
  sink (f_int_2 (n, 0));    /* { dg-warning "argument 2 value is zero" } */
  sink (f_int_2 (0, n));    /* { dg-warning "argument 1 value is zero" } */

  sink (f_int_1 (n));
  n = 0;
  sink (f_int_1 (n));       /* { dg-warning "argument 1 value is zero" } */
  sink (f_int_2 (1, n));    /* { dg-warning "argument 2 value is zero" } */
}

void
test_ulong (unsigned long n)
{
  sink (f_ulong_1 (0));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_ulong_2 (0, 1));  /* { dg-warning "argument 1 value is zero" } */
  sink (f_ulong_2 (1, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_ulong_2 (n, 0));  /* { dg-warning "argument 2 value is zero" } */
  sink (f_ulong_2 (0, n));  /* { dg-warning "argument 1 value is zero" } */

  sink (f_ulong_1 (n));
  n = 0;
  sink (f_ulong_1 (n));     /* { dg-warning "argument 1 value is zero" } */
  sink (f_ulong_2 (1, n));  /* { dg-warning "argument 2 value is zero" } */
}

void
test_long (long n)
{
  sink (f_long_1 (0));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_long_2 (0, 1));   /* { dg-warning "argument 1 value is zero" } */
  sink (f_long_2 (1, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_long_2 (n, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_long_2 (0, n));   /* { dg-warning "argument 1 value is zero" } */

  sink (f_long_1 (n));
  n = 0;
  sink (f_long_1 (n));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_long_2 (1, n));   /* { dg-warning "argument 2 value is zero" } */
}

void
test_size (size_t n)
{
  sink (f_size_1 (0));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_size_2 (0, 1));   /* { dg-warning "argument 1 value is zero" } */
  sink (f_size_2 (1, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_size_2 (n, 0));   /* { dg-warning "argument 2 value is zero" } */
  sink (f_size_2 (0, n));   /* { dg-warning "argument 1 value is zero" } */

  sink (f_size_1 (n));
  n = 0;
  sink (f_size_1 (n));      /* { dg-warning "argument 1 value is zero" } */
  sink (f_size_2 (1, n));   /* { dg-warning "argument 2 value is zero" } */
}

/* Verify that calls to allocation function decorated with attribute
   returns_nonnull don't cause warnings (unlike functions like malloc
   that can return null in this case there's nothing to warn about
   because a returns_nonnull function guarantees success).  */

void
test_size_nonnull (size_t n)
{
  sink (f_size_1_nonnull (0));
  sink (f_size_2_nonnull (0, 1));
  sink (f_size_2_nonnull (1, 0));
  sink (f_size_2_nonnull (n, 0));
  sink (f_size_2_nonnull (0, n));

  sink (f_size_1_nonnull (n));
  n = 0;
  sink (f_size_1_nonnull (n));
  sink (f_size_2_nonnull (1, n));
}

/* Verify that call to plain alloca(0) is not diagnosed.  */

void
test_alloca (size_t n)
{
  extern void* alloca (size_t);

  alloca (0); /* { dg-warning "ignoring return value of '.*' declared with attribute 'warn_unused_result'" } */
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
// DEFAULT-NEXT:     fn %1 @f_uchar_1(%49 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @f_uchar_2(%50 <unnamed>: u8, %51 <unnamed>: u8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @f_schar_1(%52 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @f_schar_2(%53 <unnamed>: i8, %54 <unnamed>: i8) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @f_ushrt_1(%55 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @f_ushrt_2(%56 <unnamed>: u16, %57 <unnamed>: u16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @f_shrt_1(%58 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @f_shrt_2(%59 <unnamed>: i16, %60 <unnamed>: i16) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @f_uint_1(%61 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @f_uint_2(%62 <unnamed>: u32, %63 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %11 @f_int_1(%64 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @f_int_2(%65 <unnamed>: i32, %66 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %13 @f_ulong_1(%67 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @f_ulong_2(%68 <unnamed>: u64, %69 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @f_long_1(%70 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %16 @f_long_2(%71 <unnamed>: i64, %72 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %17 @f_ullong_1(%73 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @f_ullong_2(%74 <unnamed>: u64, %75 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %19 @f_llong_1(%76 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %20 @f_llong_2(%77 <unnamed>: i64, %78 <unnamed>: i64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %21 @f_size_1(%79 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %22 @f_size_2(%80 <unnamed>: u64, %81 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %23 @f_size_1_nonnull(%82 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %24 @f_size_2_nonnull(%83 <unnamed>: u64, %84 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %25 @sink(%85 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %26 @test_uchar(%27 n: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%1, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%2, read<u8>(%27), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), read<u8>(%27)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%1, read<u8>(%27)));
// DEFAULT-NEXT:         write<u8>(%27, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8) -> ptr<void>>(%1, read<u8>(%27)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%2, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), read<u8>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_schar(%29 n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%3, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(1)), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%4, read<i8>(%29), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(0)), read<i8>(%29)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%3, read<i8>(%29)));
// DEFAULT-NEXT:         write<i8>(%29, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8) -> ptr<void>>(%3, read<i8>(%29)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%4, truncate<i8, reason=arg, fits=always>(const<i32>(1)), read<i8>(%29)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test_ushrt(%31 n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%5, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%6, read<u16>(%31), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), read<u16>(%31)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%5, read<u16>(%31)));
// DEFAULT-NEXT:         write<u16>(%31, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16) -> ptr<void>>(%5, read<u16>(%31)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), read<u16>(%31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @test_shrt(%33 n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%7, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=always>(const<i32>(0)), truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=always>(const<i32>(1)), truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%8, read<i16>(%33), truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=always>(const<i32>(0)), read<i16>(%33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%7, read<i16>(%33)));
// DEFAULT-NEXT:         write<i16>(%33, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16) -> ptr<void>>(%7, read<i16>(%33)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%8, truncate<i16, reason=arg, fits=always>(const<i32>(1)), read<i16>(%33)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test_uint(%35 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%9, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%10, read<u32>(%35), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), read<u32>(%35)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%9, read<u32>(%35)));
// DEFAULT-NEXT:         write<u32>(%35, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%9, read<u32>(%35)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), read<u32>(%35)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @test_int(%37 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%11, const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%12, const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%12, const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%12, read<i32>(%37), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%12, const<i32>(0), read<i32>(%37)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%11, read<i32>(%37)));
// DEFAULT-NEXT:         write<i32>(%37, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%11, read<i32>(%37)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%12, const<i32>(1), read<i32>(%37)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @test_ulong(%39 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%14, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%14, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%14, read<u64>(%39), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%14, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%39)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%13, read<u64>(%39)));
// DEFAULT-NEXT:         write<u64>(%39, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%13, read<u64>(%39)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%14, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%39)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test_long(%41 n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%15, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%16, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%16, widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%16, read<i64>(%41), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%16, widen<i64, reason=arg>(const<i32>(0)), read<i64>(%41)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%15, read<i64>(%41)));
// DEFAULT-NEXT:         write<i64>(%41, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64) -> ptr<void>>(%15, read<i64>(%41)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%16, widen<i64, reason=arg>(const<i32>(1)), read<i64>(%41)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @test_size(%43 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%21, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, read<u64>(%43), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%43)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%21, read<u64>(%43)));
// DEFAULT-NEXT:         write<u64>(%43, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%21, read<u64>(%43)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%43)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @test_size_nonnull(%45 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%24, read<u64>(%45), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%45)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%23, read<u64>(%45)));
// DEFAULT-NEXT:         write<u64>(%45, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%23, read<u64>(%45)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%45)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @alloca(%86 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %46 @test_alloca(%47 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%48, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
