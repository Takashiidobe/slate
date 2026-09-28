/* PR c/78284 - warn on malloc with very large arguments
   Test exercising the ability of the built-in allocation functions
   to detect and diagnose, without optimization, calls that attemnpt
   to allocate objects in excess of the number of bytes specified by
   -Walloc-larger-than=maximum.  */
/* { dg-do compile } */
/* { dg-options "-O0 -Wall -Walloc-size-larger-than=12345 -Wno-use-after-free" } */

#define MAXOBJSZ  12345

typedef __SIZE_TYPE__ size_t;

void sink (void*);


void test_lit (char *p, char *q)
{
  sink (__builtin_aligned_alloc (1, MAXOBJSZ));
  sink (__builtin_aligned_alloc (1, MAXOBJSZ + 1));   /* { dg-warning "argument 2 value .12346\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_alloca (MAXOBJSZ));
  sink (__builtin_alloca (MAXOBJSZ + 2));   /* { dg-warning "argument 1 value .12347\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_calloc (MAXOBJSZ, 1));
  sink (__builtin_calloc (1, MAXOBJSZ));

  /* Verify that the signed to unsigned conversion below doesn't cause
     a warning.  */
  sink (__builtin_calloc (p - q, 1));
  sink (__builtin_calloc (1, q - p));
  sink (__builtin_calloc (p - q, MAXOBJSZ));
  sink (__builtin_calloc (MAXOBJSZ, q - p));

  sink (__builtin_calloc (MAXOBJSZ / 2, 3));   /* { dg-warning "product .6172\[lu\]* \\* 3\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */
  sink (__builtin_calloc (4, MAXOBJSZ / 3));   /* { dg-warning "product .4\[lu\]* \\* 4115\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */

  sink (__builtin_malloc (MAXOBJSZ));
  sink (__builtin_malloc (MAXOBJSZ + 3));   /* { dg-warning "argument 1 value .12348\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_realloc (p, MAXOBJSZ));
  sink (__builtin_realloc (p, MAXOBJSZ + 4));  /* { dg-warning "argument 2 value .12349\[lu\]*. exceeds maximum object size 12345" } */
}


enum { max = MAXOBJSZ };

void test_cst (char *p, char *q)
{
  sink (__builtin_aligned_alloc (1, max));
  sink (__builtin_aligned_alloc (1, max + 1));   /* { dg-warning "argument 2 value .12346\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_alloca (max));
  sink (__builtin_alloca (max + 2));   /* { dg-warning "argument 1 value .12347\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_calloc (max, 1));
  sink (__builtin_calloc (1, max));

  /* Verify that the signed to unsigned conversion below doesn't cause
     a warning.  */
  sink (__builtin_calloc (p - q, 1));
  sink (__builtin_calloc (1, q - p));
  sink (__builtin_calloc (p - q, max));
  sink (__builtin_calloc (max, q - p));

  sink (__builtin_calloc (max / 2, 3));   /* { dg-warning "product .6172\[lu\]* \\* 3\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */
  sink (__builtin_calloc (4, max / 3));   /* { dg-warning "product .4\[lu\]* \\* 4115\[lu\]*. of arguments 1 and 2 exceeds maximum object size 12345" } */

  sink (__builtin_malloc (max));
  sink (__builtin_malloc (max + 3));   /* { dg-warning "argument 1 value .12348\[lu\]*. exceeds maximum object size 12345" } */

  sink (__builtin_realloc (p, max));
  sink (__builtin_realloc (p, max + 4));  /* { dg-warning "argument 2 value .12349\[lu\]*. exceeds maximum object size 12345" } */
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = enum : u32 {
// DEFAULT-NEXT:         %0 max = const<i32>(12345);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %1 @sink(%10 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @aligned_alloc(%11 <unnamed>: u64, %12 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_alloca(%14 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %18 @__builtin_calloc(%16 <unnamed>: u64, %17 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %20 @__builtin_malloc(%19 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %23 @__builtin_realloc(%21 <unnamed>: ptr<void>, %22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_lit(%3 p: ptr<i8>, %4 q: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%15, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%15, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%3), read<ptr<i8>>(%4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%4), read<ptr<i8>>(%3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%3), read<ptr<i8>>(%4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345))), reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%4), read<ptr<i8>>(%3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(12345), const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(12345), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%20, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%20, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_cst(%8 p: ptr<i8>, %9 q: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%15, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%15, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(2))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%8), read<ptr<i8>>(%9))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%9), read<ptr<i8>>(%8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%8), read<ptr<i8>>(%9))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345))), reinterpret<u64, reason=arg, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(%9), read<ptr<i8>>(%8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(12345), const<i32>(2)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%18, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(12345), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%20, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%20, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(3))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%8)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%1, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%8)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(const<i32>(12345), const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
