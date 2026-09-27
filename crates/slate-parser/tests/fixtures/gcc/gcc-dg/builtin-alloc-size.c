/* PR c/78668 - aligned_alloc, realloc, et al. missing attribute alloc_size
   Test to verify that memory allocation built-ins are decorated with
   attribute alloc_size that __builtin_object_size can make use of (or
   are treated as if they were for that purpose)..
   { dg-do compile }
   { dg-additional-options "-O2 -fdump-tree-optimized" } */

void sink (void*);

static unsigned size (unsigned n)
{
  return n;
}

void test_aligned_alloc (unsigned a)
{
  unsigned n = size (7);

  void *p = __builtin_aligned_alloc (a, n);
  if (__builtin_object_size (p, 0) != n)
    __builtin_abort ();
  sink (p);
}

void test_alloca (void)
{
  unsigned n = size (13);

  void *p = __builtin_alloca (n);

  /* Also verify that alloca is declared with attribute returns_nonnull
     (or treated as it were as the case may be).  */
  if (!p)
    __builtin_abort ();

  if (__builtin_object_size (p, 0) != n)
    __builtin_abort ();
  sink (p);
}

void test_calloc (void)
{
  unsigned m = size (19);
  unsigned n = size (23);

  void *p = __builtin_calloc (m, n);
  if (__builtin_object_size (p, 0) != m * n)
    __builtin_abort ();
  sink (p);
}

void test_malloc (void)
{
  unsigned n = size (17);

  void *p = __builtin_malloc (n);
  if (__builtin_object_size (p, 0) != n)
    __builtin_abort ();
  sink (p);
}

void test_realloc (void *p)
{
  unsigned n = size (31);

  p = __builtin_realloc (p, n);
  if (__builtin_object_size (p, 0) != n)
    __builtin_abort ();
  sink (p);
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
// DEFAULT-NEXT:     fn %0 @sink(%20 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @size(%2 n: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @aligned_alloc(%21 <unnamed>: u64, %22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %26 @__builtin_object_size(%24 <unnamed>: ptr<const void>, %25 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @test_aligned_alloc(%4 a: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %6 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%23, widen<u64, reason=arg>(read<u32>(%4)), widen<u64, reason=arg>(read<u32>(%5)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%6)), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @__builtin_alloca(%28 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @test_alloca() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(13)));
// DEFAULT-NEXT:         let %9 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%29, widen<u64, reason=arg>(read<u32>(%8)));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%9), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%9)), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @__builtin_calloc(%30 <unnamed>: u64, %31 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @test_calloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 m: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(19)));
// DEFAULT-NEXT:         let %12 n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(23)));
// DEFAULT-NEXT:         let %13 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%32, widen<u64, reason=arg>(read<u32>(%11)), widen<u64, reason=arg>(read<u32>(%12)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%13)), const<i32>(0)), widen<u64, reason=usual_arith>(mul<u32, overflow=wrap>(read<u32>(%11), read<u32>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @__builtin_malloc(%33 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %14 @test_malloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         let %16 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%34, widen<u64, reason=arg>(read<u32>(%15)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%16)), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @__builtin_realloc(%35 <unnamed>: ptr<void>, %36 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %17 @test_realloc(%18 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%1, reinterpret<u32, reason=arg, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<ptr<void>>(%18, call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%37, read<ptr<void>>(%18), widen<u64, reason=arg>(read<u32>(%19))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%37, read<ptr<void>>(%18), widen<u64, reason=arg>(read<u32>(%19)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%26, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%18)), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%19)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, read<ptr<void>>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
