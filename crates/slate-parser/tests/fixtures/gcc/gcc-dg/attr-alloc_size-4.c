/* PR c/77531 - __attribute__((alloc_size(1,2))) could also warn on
   multiplication overflow
   PR c/78284 - warn on malloc with very large arguments
   Test exercising the ability to detect and diagnose calls to allocation
   functions decorated with attribute alloc_size that either overflow or
   exceed the maximum object size specified by -Walloc-size-larger-than.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Wall -Walloc-size-larger-than=1234" } */

#define INT_MAX    __INT_MAX__
#define INT_MIN    (-INT_MAX - 1)
#define UINT_MAX   (INT_MAX * 2U + 1)

#define SIZE_MAX   __SIZE_MAX__

typedef __SIZE_TYPE__ size_t;

#define ALLOC_SIZE(...) __attribute__ ((alloc_size (__VA_ARGS__)))

void* f_uint_1 (unsigned) ALLOC_SIZE (1);
void* f_uint_2 (unsigned, unsigned) ALLOC_SIZE (1, 2);
void* f_int_1 (int) ALLOC_SIZE (1);
void* f_int_2 (int, int) ALLOC_SIZE (1, 2);

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

static int
signed_range (int min, int max)
{
  extern int random_signed_value (void);
  int val = random_signed_value ();
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

static int
signed_anti_range (int min, int max)
{
  extern int random_signed_value (void);
  int val = random_signed_value ();
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
test_uint_cst (void)
{
  const unsigned max = UINT_MAX;

  sink (f_uint_1 (0));
  sink (f_uint_1 (1));
  sink (f_uint_1 (1233));
  sink (f_uint_1 (1234));
  sink (f_uint_1 (1235));       /* { dg-warning "argument 1 value .1235u?. exceeds maximum object size 1234" } */
  sink (f_uint_1 (max - 1));    /* { dg-warning "argument 1 value .\[0-9\]+u?. exceeds maximum object size 1234" } */
  sink (f_uint_1 (max));        /* { dg-warning "argument 1 value .\[0-9\]+u?. exceeds maximum object size 1234" } */
}

void
test_uint_range (unsigned n)
{
  const unsigned max = UINT_MAX;

  sink (f_uint_1 (n));
  sink (f_uint_1 (UR (0, 1)));
  sink (f_uint_1 (UR (0, 1233)));
  sink (f_uint_1 (UR (0, 1234)));
  sink (f_uint_1 (UR (0, 1235)));
  sink (f_uint_1 (UR (1, 1235)));
  sink (f_uint_1 (UR (1234, 1235)));
  sink (f_uint_1 (UR (1235, 1236)));   /* { dg-warning "argument 1 range \\\[\[0-9\]+u?, \[0-9\]+u?\\\] exceeds maximum object size 1234" } */
  sink (f_uint_1 (UR (1, max - 1)));
  sink (f_uint_1 (UR (1, max)));
}

void
test_int_cst (void)
{
  const int min = INT_MIN;
  const int max = INT_MAX;

  sink (f_int_1 (min));   /* { dg-warning "argument 1 value .-\[0-9\]+. is negative" } */
  sink (f_int_1 (-1));    /* { dg-warning "argument 1 value .-1. is negative" } */
  sink (f_int_1 (0));
  sink (f_int_1 (1));
  sink (f_int_1 (1233));
  sink (f_int_1 (1234));
  sink (f_int_1 (max));   /* { dg-warning "argument 1 value .\[0-9\]+u?. exceeds maximum object size 1234" } */
}

void
test_int_range (int n)
{
  const int min = INT_MIN;
  const int max = INT_MAX;

  sink (f_int_1 (n));

  sink (f_int_1 (SR (min, 1234)));
  sink (f_int_1 (SR (-2, -1)));   /* { dg-warning "argument 1 range \\\[-2, -1\\\] is negative" } */

  sink (f_int_1 (SR (1235, 2345)));  /* { dg-warning "argument 1 range \\\[1235, 2345\\\] exceeds maximum object size 1234" } */
  sink (f_int_1 (SR (max - 1, max)));   /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size 1234" } */

  sink (f_int_1 (SAR (-1, 1)));
  sink (f_int_1 (SAR (-2, 12)));
  sink (f_int_1 (SAR (-3, 123)));
  sink (f_int_1 (SAR (-4, 1234)));   /* { dg-warning "argument 1 range \\\[1235, \[0-9\]+\\\] exceeds maximum object size 1234" } */
  sink (f_int_1 (SAR (min + 1, 1233)));

#if __i386__ || __x86_64__
  /* Avoid failures described in bug 79051.  */
  sink (f_int_1 (SAR (min + 2, 1235)));   /* { dg-warning "argument 1 range \\\[1236, \[0-9\]+\\\] exceeds maximum object size 1234" "" { target { i?86-*-* x86_64-*-* } } } */
#endif

  sink (f_int_1 (SAR (0, max)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]*, -1\\\] is negative" } */
  /* The range below includes zero which would be diagnosed by
     -Walloc-size-zero but since all other values are negative it
     is diagnosed by -Walloc-size-larger-than.  */
  sink (f_int_1 (SAR (1, max)));   /* { dg-warning "argument 1 range \\\[-\[0-9\]*, 0\\\] is negative" } */
  sink (f_int_1 (SAR (2, max)));
}

void
test_size_cst (void)
{
  const size_t max = __SIZE_MAX__;

  sink (f_size_1 (0));
  sink (f_size_1 (1));

  sink (f_size_2 (   0, 1234));
  sink (f_size_2 (   1, 1234));
  sink (f_size_2 (   2, 1234));  /* { dg-warning "product .2 \\* 1234. of arguments 1 and 2 exceeds maximum object size \[0-9\]+" } */
  sink (f_size_2 (1234, 1234));  /* { dg-warning "product .1234 \\* 1234. of arguments 1 and 2 exceeds (.SIZE_MAX.|maximum object size 1234)" } */
  sink (f_size_2 (1235, 1234));  /* { dg-warning "argument 1 value .1235. exceeds maximum object size 1234" } */
  sink (f_size_2 (1234, 1235));  /* { dg-warning "argument 2 value .1235. exceeds maximum object size 1234" } */
  sink (f_size_2 (1234, max));  /* { dg-warning "argument 2 value .\[0-9\]+. exceeds maximum object size 1234" } */
  sink (f_size_2 (max, 1234));  /* { dg-warning "argument 1 value .\[0-9\]+. exceeds maximum object size 1234" } */
}

void
test_size_range (size_t n)
{
  const size_t max = __SIZE_MAX__;

  sink (f_size_1 (n));

  sink (f_size_1 (UR (0, 1)));
  sink (f_size_1 (UR (0, max - 1)));
  sink (f_size_1 (UR (1, max - 1)));
  sink (f_size_1 (UR (1, max)));

  sink (f_size_1 (UAR (1, 1)));
  /* Since the only valid argument in the anti-range below is zero
     a warning is expected even though -Walloc-zero is not specified.  */
  sink (f_size_1 (UAR (1, 1234)));   /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " } */
  /* The only valid argument in this range is 1.  */
  sink (f_size_1 (UAR (2, max / 2)));

  sink (f_size_2 (n, n));
  sink (f_size_2 (n, 1234));
  sink (f_size_2 (1234, n));

  sink (f_size_2 (UR (0, 1), 1234));
  sink (f_size_2 (UR (0, 1), 1235));   /* { dg-warning "argument 2 value .1235. exceeds maximum object size 1234" } */

  sink (f_size_2 (UR (1235, 1236), n));  /* { dg-warning "argument 1 range \\\[1235, 1236\\\] exceeds maximum object size 1234" } */

  sink (f_size_2 (UR (1235, 1236), UR (max / 2, max)));  /* { dg-warning "argument 1 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " } */
/* { dg-warning "argument 2 range \\\[\[0-9\]+, \[0-9\]+\\\] exceeds maximum object size " "argument 2" { target *-*-* } .-1 } */

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
// DEFAULT-NEXT:     fn %1 @f_uint_1(%43 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @f_uint_2(%44 <unnamed>: u32, %45 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @f_int_1(%46 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @f_int_2(%47 <unnamed>: i32, %48 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @f_size_1(%49 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @f_size_2(%50 <unnamed>: u64, %51 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @random_unsigned_value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @unsigned_range(%8 min: u64, %9 max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%10);
// DEFAULT-NEXT:         if logical_or<bool>(lt<u64>(read<u64>(%11), read<u64>(%8)), lt<u64>(read<u64>(%9), read<u64>(%11)))
// DEFAULT-NEXT:             write<u64>(%11, read<u64>(%8));
// DEFAULT-NEXT:         return read<u64>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @random_signed_value() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @signed_range(%13 min: i32, %14 max: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 val: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%15);
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%16), read<i32>(%13)), lt<i32>(read<i32>(%14), read<i32>(%16)))
// DEFAULT-NEXT:             write<i32>(%16, read<i32>(%13));
// DEFAULT-NEXT:         return read<i32>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @unsigned_anti_range(%18 min: u64, %19 max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%10);
// DEFAULT-NEXT:         if logical_and<bool>(le<u64>(read<u64>(%18), read<u64>(%20)), le<u64>(read<u64>(%20), read<u64>(%19)))
// DEFAULT-NEXT:             write<u64>(%20, sub<u64, overflow=wrap>(read<u64>(%18), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @signed_anti_range(%22 min: i32, %23 max: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 val: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%15);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%22), read<i32>(%24)), le<i32>(read<i32>(%24), read<i32>(%23)))
// DEFAULT-NEXT:             write<i32>(%24, sub<i32, overflow=ub>(read<i32>(%22), const<i32>(1)));
// DEFAULT-NEXT:         return read<i32>(%24);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @sink(%52 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %26 @test_uint_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 max: u32 [storage=automatic] [const] = add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(1233))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(1234))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(1235))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, sub<u32, overflow=wrap>(read<u32>(%27), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, read<u32>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test_uint_range(%29 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 max: u32 [storage=automatic] [const] = add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, read<u32>(%29)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1233)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1236)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(sub<u32, overflow=wrap>(read<u32>(%30), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%1, truncate<u32, reason=arg, fits=unknown>(call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), widen<u64, reason=arg>(read<u32>(%30))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test_int_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %32 min: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %33 max: i32 [storage=automatic] [const] = const<i32>(2147483647);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, read<i32>(%32)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, const<i32>(1233)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, const<i32>(1234)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, read<i32>(%33)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test_int_range(%35 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %36 min: i32 [storage=automatic] [const] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %37 max: i32 [storage=automatic] [const] = const<i32>(2147483647);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, read<i32>(%35)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%12, read<i32>(%36), const<i32>(1234))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%12, neg<i32, overflow=ub>(const<i32>(2)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%12, const<i32>(1235), const<i32>(2345))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%12, sub<i32, overflow=ub>(read<i32>(%37), const<i32>(1)), read<i32>(%37))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(2)), const<i32>(12))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(3)), const<i32>(123))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, neg<i32, overflow=ub>(const<i32>(4)), const<i32>(1234))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, add<i32, overflow=ub>(read<i32>(%36), const<i32>(1)), const<i32>(1233))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, add<i32, overflow=ub>(read<i32>(%36), const<i32>(2)), const<i32>(1235))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, const<i32>(0), read<i32>(%37))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, const<i32>(1), read<i32>(%37))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(i32) -> ptr<void>>(%3, call<i32, signature=fn(i32, i32) -> i32>(%21, const<i32>(2), read<i32>(%37))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @test_size_cst() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %39 max: u64 [storage=automatic] [const] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234))), read<u64>(%39)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, read<u64>(%39), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test_size_range(%41 n: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %42 max: u64 [storage=automatic] [const] = const<u64>(18446744073709551615);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, read<u64>(%41)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), sub<u64, overflow=wrap>(read<u64>(%42), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), sub<u64, overflow=wrap>(read<u64>(%42), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%42))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, call<u64, signature=fn(u64, u64) -> u64>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), div<u64, by_zero=ub>(read<u64>(%42), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, read<u64>(%41), read<u64>(%41)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, read<u64>(%41), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234))), read<u64>(%41)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1234)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1236)))), read<u64>(%41)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%6, call<u64, signature=fn(u64, u64) -> u64>(%7, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1235))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1236)))), call<u64, signature=fn(u64, u64) -> u64>(%7, div<u64, by_zero=ub>(read<u64>(%42), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), read<u64>(%42))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
