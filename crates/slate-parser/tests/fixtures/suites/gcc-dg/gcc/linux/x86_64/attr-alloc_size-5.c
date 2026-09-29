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
// DEFAULT-NEXT:     fn %[[VALUE_f_size_1_nonnull:[0-9]+]] @f_size_1_nonnull(%[[VALUE33:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_size_2_nonnull:[0-9]+]] @f_size_2_nonnull(%[[VALUE34:[0-9]+]] <unnamed>: u64, %[[VALUE35:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE36:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_uchar:[0-9]+]] @test_uchar(%[[VALUE_n:[0-9]+]] n: u8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], read<u8>(%[[VALUE_n]]), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_n]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8) -> ptr<void>>(%[[VALUE_f_uchar_1]], read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u8, u8) -> ptr<void>>(%[[VALUE_f_uchar_2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), read<u8>(%[[VALUE_n]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_schar:[0-9]+]] @test_schar(%[[VALUE_n_2:[0-9]+]] n: i8) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(1)), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], read<i8>(%[[VALUE_n_2]]), truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_n_2]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8) -> ptr<void>>(%[[VALUE_f_schar_1]], read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i8, i8) -> ptr<void>>(%[[VALUE_f_schar_2]], truncate<i8, reason=arg, fits=always>(const<i32>(1)), read<i8>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ushrt:[0-9]+]] @test_ushrt(%[[VALUE_n_3:[0-9]+]] n: u16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], read<u16>(%[[VALUE_n_3]]), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), read<u16>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], read<u16>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_n_3]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16) -> ptr<void>>(%[[VALUE_f_ushrt_1]], read<u16>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u16, u16) -> ptr<void>>(%[[VALUE_f_ushrt_2]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), read<u16>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_shrt:[0-9]+]] @test_shrt(%[[VALUE_n_4:[0-9]+]] n: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%[[VALUE_f_shrt_2]], truncate<i16, reason=arg, fits=always>(const<i32>(0)), truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%[[VALUE_f_shrt_2]], truncate<i16, reason=arg, fits=always>(const<i32>(1)), truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%[[VALUE_f_shrt_2]], read<i16>(%[[VALUE_n_4]]), truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%[[VALUE_f_shrt_2]], truncate<i16, reason=arg, fits=always>(const<i32>(0)), read<i16>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], read<i16>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_n_4]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16) -> ptr<void>>(%[[VALUE_f_shrt_1]], read<i16>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i16, i16) -> ptr<void>>(%[[VALUE_f_shrt_2]], truncate<i16, reason=arg, fits=always>(const<i32>(1)), read<i16>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_uint:[0-9]+]] @test_uint(%[[VALUE_n_5:[0-9]+]] n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%[[VALUE_f_uint_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%[[VALUE_f_uint_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%[[VALUE_f_uint_2]], read<u32>(%[[VALUE_n_5]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%[[VALUE_f_uint_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), read<u32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], read<u32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_n_5]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_f_uint_1]], read<u32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%[[VALUE_f_uint_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), read<u32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_int:[0-9]+]] @test_int(%[[VALUE_n_6:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%[[VALUE_f_int_2]], const<i32>(0), const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%[[VALUE_f_int_2]], const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%[[VALUE_f_int_2]], read<i32>(%[[VALUE_n_6]]), const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%[[VALUE_f_int_2]], const<i32>(0), read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n_6]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32) -> ptr<void>>(%[[VALUE_f_int_1]], read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i32, i32) -> ptr<void>>(%[[VALUE_f_int_2]], const<i32>(1), read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_ulong:[0-9]+]] @test_ulong(%[[VALUE_n_7:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_ulong_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_ulong_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_ulong_2]], read<u64>(%[[VALUE_n_7]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_ulong_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], read<u64>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n_7]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_ulong_1]], read<u64>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_ulong_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long:[0-9]+]] @test_long(%[[VALUE_n_8:[0-9]+]] n: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%[[VALUE_f_long_2]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%[[VALUE_f_long_2]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%[[VALUE_f_long_2]], read<i64>(%[[VALUE_n_8]]), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%[[VALUE_f_long_2]], widen<i64, reason=arg>(const<i32>(0)), read<i64>(%[[VALUE_n_8]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], read<i64>(%[[VALUE_n_8]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_n_8]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64) -> ptr<void>>(%[[VALUE_f_long_1]], read<i64>(%[[VALUE_n_8]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(i64, i64) -> ptr<void>>(%[[VALUE_f_long_2]], widen<i64, reason=arg>(const<i32>(1)), read<i64>(%[[VALUE_n_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_size:[0-9]+]] @test_size(%[[VALUE_n_9:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], read<u64>(%[[VALUE_n_9]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%[[VALUE_n_9]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], read<u64>(%[[VALUE_n_9]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n_9]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1]], read<u64>(%[[VALUE_n_9]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%[[VALUE_n_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_size_nonnull:[0-9]+]] @test_size_nonnull(%[[VALUE_n_10:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1_nonnull]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2_nonnull]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2_nonnull]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2_nonnull]], read<u64>(%[[VALUE_n_10]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2_nonnull]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), read<u64>(%[[VALUE_n_10]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1_nonnull]], read<u64>(%[[VALUE_n_10]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_n_10]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_f_size_1_nonnull]], read<u64>(%[[VALUE_n_10]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_f_size_2_nonnull]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%[[VALUE_n_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloca:[0-9]+]] @alloca(%[[VALUE37:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_alloca:[0-9]+]] @test_alloca(%[[VALUE_n_11:[0-9]+]] n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_alloca]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
