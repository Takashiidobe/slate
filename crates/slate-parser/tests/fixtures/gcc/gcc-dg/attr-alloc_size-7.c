/* PR c/78284 - warn on malloc with very large arguments
   Test exercising the ability of the built-in allocation functions to
   detect and diagnose calls that attemnpt to allocate objects in excess
   of the maximum specified by -Walloc-size-larger-than=maximum.  */
/* { dg-do compile } */
/* { dg-options "-O1 -Wall -Walloc-size-larger-than=12345 -Wno-use-after-free" } */

#define SIZE_MAX   __SIZE_MAX__
#define MAXOBJSZ   12345

typedef __SIZE_TYPE__ size_t;

void sink (void*);

#pragma GCC push_options
/* Verify that constant evaluation takes place even at -O0.  */
#pragma GCC optimize ("0")

void test_cst (void *p)
{
  enum { max = MAXOBJSZ };

  sink (__builtin_aligned_alloc (1, max));
  sink (__builtin_aligned_alloc (1, max + 1));   /* { dg-warning "argument 2 value .12346\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_alloca (max));
  sink (__builtin_alloca (max + 2));   /* { dg-warning "argument 1 value .12347\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_calloc (1, max));
  sink (__builtin_calloc (max, 1));

  sink (__builtin_calloc (max / 2, 3));   /* { dg-warning "product .6172\[lu\]* \\* 3\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */
  sink (__builtin_calloc (4, max / 3));   /* { dg-warning "product .4\[lu\]* \\* 4115\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */

  sink (__builtin_malloc (max));
  sink (__builtin_malloc (max + 3));   /* { dg-warning "argument 1 value .12348\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_realloc (p, max));
  sink (__builtin_realloc (p, max + 4));  /* { dg-warning "argument 2 value .12349\[lu\]*. exceeds maximum object size 12345" } */
}


/* Variable evaluation needs -O1.  */
#pragma GCC pop_options

__attribute__ ((noipa)) void test_var (void *p)
{
  size_t max = MAXOBJSZ;

  sink (__builtin_aligned_alloc (1, max));
  sink (__builtin_aligned_alloc (1, max + 1));   /* { dg-warning "argument 2 value .12346\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_alloca (max));
  sink (__builtin_alloca (max + 2));   /* { dg-warning "argument 1 value .12347\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_calloc (1, max));
  sink (__builtin_calloc (max, 1));

  sink (__builtin_calloc (max / 2, 3));   /* { dg-warning "product .6172\[lu\]* \\* 3\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */
  sink (__builtin_calloc (4, max / 3));   /* { dg-warning "product .4\[lu\]* \\* 4115\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */

  sink (__builtin_malloc (max));
  sink (__builtin_malloc (max + 3));   /* { dg-warning "argument 1 value .12348\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_realloc (p, max));
  sink (__builtin_realloc (p, max + 4));  /* { dg-warning "argument 2 value .12349\[lu\]*. exceeds maximum object size 12345" } */
}


/* Value range evaluation (apparently) needs -O2 here.  */
#pragma GCC optimize ("2")

static size_t maxobjsize (void)
{
  return MAXOBJSZ;
}

__attribute__ ((noipa)) void test_range (void *p, size_t range)
{
  /* Make sure the variable is at least as large as the maximum object
     size but also make sure that it's guaranteed not to be too big to
     increment (and wrap around).  */
  size_t max = maxobjsize ();

  if (range < max || 2 * max <= range)
    range = maxobjsize ();

  sink (__builtin_aligned_alloc (1, range));
  sink (__builtin_aligned_alloc (1, range + 1));   /* { dg-warning "argument 2 range \\\[12346\[lu\]*, \[0-9\]+\[lu\]*\\\] exceeds maximum object size 12345" } */

  sink (__builtin_alloca (range));
  sink (__builtin_alloca (range + 2));   /* { dg-warning "argument 1 range \\\[12347\[lu\]*, \[0-9\]+\[lu\]*\\\] exceeds maximum object size 12345" } */

  sink (__builtin_calloc (range, 1));
  sink (__builtin_calloc (1, range));

  sink (__builtin_calloc (range / 2, 3));   /* { dg-warning "product .6172\[lu\]* \\* 3\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */
  sink (__builtin_calloc (4, range / 3));   /* { dg-warning "product .4\[lu\]* \\* 4115\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */

  sink (__builtin_malloc (range));
  sink (__builtin_malloc (range + 3));   /* { dg-warning "argument 1 range \\\[12348\[lu\]*, 24692\[lu\]*\\\] exceeds maximum object size 12345" } */

  sink (__builtin_realloc (p, range));
  sink (__builtin_realloc (p, range + 4));  /* { dg-warning "argument 2 range \\\[12349\[lu\]*, 24693\[lu\]*\\\] exceeds maximum object size 12345" } */
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
// DEFAULT-NEXT:     type @type1 = enum : u32 {
// DEFAULT-NEXT:         %0 max = const<i32>(12345);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %1 @sink(%14 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @aligned_alloc(%15 <unnamed>: u64, %16 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_alloca(%18 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %22 @__builtin_calloc(%20 <unnamed>: u64, %21 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %24 @__builtin_malloc(%23 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_realloc(%25 <unnamed>: ptr<void>, %26 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_cst(%3 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%19, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%19, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(12345), const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(12345), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%3), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%3), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_var(%7 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 max: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(12345)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), add<u64, overflow=wrap>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%19, read<u64>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%19, add<u64, overflow=wrap>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, read<u64>(%8), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, div<u64, by_zero=ub>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), div<u64, by_zero=ub>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, read<u64>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, add<u64, overflow=wrap>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%7), read<u64>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%7), add<u64, overflow=wrap>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @maxobjsize() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(12345)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_range(%11 p: ptr<void>, %12 range: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 max: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(lt<u64>(read<u64>(%12), read<u64>(%13)), le<u64>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), read<u64>(%13)), read<u64>(%12)))
// DEFAULT-NEXT:             write<u64>(%12, call<u64, signature=fn() -> u64>(%9));
// DEFAULT-NEXT:             call<u64, signature=fn() -> u64>(%9);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), add<u64, overflow=wrap>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%19, read<u64>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%19, add<u64, overflow=wrap>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, read<u64>(%12), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), read<u64>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, div<u64, by_zero=ub>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%22, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), div<u64, by_zero=ub>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, read<u64>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, add<u64, overflow=wrap>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%11), read<u64>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%27, read<ptr<void>>(%11), add<u64, overflow=wrap>(read<u64>(%12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
