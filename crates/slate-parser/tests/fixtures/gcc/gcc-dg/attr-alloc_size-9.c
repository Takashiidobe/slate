/* PR c/78284 - warn on malloc with very large arguments
   Test verifying that the built-in allocation functions are declared
   with attribute malloc.  This means that the pointer they return
   can be assumed not to alias any other valid pointer.  */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

void sink (void*);

extern int x;

#define TEST(call)				\
  do {						\
    p = call;					\
    x = 123;					\
    *(int*)p = 456;				\
    (x == 123) ? (void)0 : __builtin_abort ();	\
    sink (p);					\
  } while (0)

void test (void *p, unsigned n)
{
  TEST (__builtin_aligned_alloc (8, n));
  TEST (__builtin_alloca (n));
  TEST (__builtin_calloc (4, n));
  TEST (__builtin_malloc (n));
  TEST (__builtin_realloc (p, n + 1));
}

/* { dg-final { scan-tree-dump-not "abort" "optimized" } } */

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
// DEFAULT-NEXT:     extern %1 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @sink(%5 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @aligned_alloc(%7 <unnamed>: u64, %8 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @__builtin_alloca(%12 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_calloc(%15 <unnamed>: u64, %16 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %20 @__builtin_malloc(%19 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %24 @__builtin_realloc(%22 <unnamed>: ptr<void>, %23 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @test(%3 p: ptr<void>, %4 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %6
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%3, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%9, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), widen<u64, reason=arg>(read<u32>(%4))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%9, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), widen<u64, reason=arg>(read<u32>(%4)));
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%3))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%1), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %11
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%3, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%13, widen<u64, reason=arg>(read<u32>(%4))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64) -> ptr<void>>(%13, widen<u64, reason=arg>(read<u32>(%4)));
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%3))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%1), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %14
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%3, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), widen<u64, reason=arg>(read<u32>(%4))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), widen<u64, reason=arg>(read<u32>(%4)));
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%3))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%1), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %18
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%3, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%20, widen<u64, reason=arg>(read<u32>(%4))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64) -> ptr<void>>(%20, widen<u64, reason=arg>(read<u32>(%4)));
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%3))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%1), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %21
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%3, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%24, read<ptr<void>>(%3), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%24, read<ptr<void>>(%3), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%1, const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%3))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%1), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
