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
// DEFAULT-NEXT:     extern %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_aligned_alloc:[0-9]+]] @aligned_alloc(%[[VALUE1:[0-9]+]] <unnamed>: u64, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_calloc:[0-9]+]] @__builtin_calloc(%[[VALUE4:[0-9]+]] <unnamed>: u64, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE6:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_realloc:[0-9]+]] @__builtin_realloc(%[[VALUE7:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_n:[0-9]+]] n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), widen<u64, reason=arg>(read<u32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))), widen<u64, reason=arg>(read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], widen<u64, reason=arg>(read<u32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], widen<u64, reason=arg>(read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), widen<u64, reason=arg>(read<u32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), widen<u64, reason=arg>(read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], widen<u64, reason=arg>(read<u32>(%[[VALUE_n]]))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], widen<u64, reason=arg>(read<u32>(%[[VALUE_n]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p]]), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_n]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p]]), widen<u64, reason=arg>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_n]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], const<i32>(123));
// DEFAULT-NEXT:                 write<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))), const<i32>(456));
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(123))
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
