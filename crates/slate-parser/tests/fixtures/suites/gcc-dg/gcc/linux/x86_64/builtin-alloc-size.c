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
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_size:[0-9]+]] @size(%[[VALUE_n:[0-9]+]] n: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_n]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_aligned_alloc:[0-9]+]] @aligned_alloc(%[[VALUE1:[0-9]+]] <unnamed>: u64, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE4:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_aligned_alloc:[0-9]+]] @test_aligned_alloc(%[[VALUE_a:[0-9]+]] a: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_size]], reinterpret<u32, reason=arg, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], widen<u64, reason=arg>(read<u32>(%[[VALUE_a]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p]])), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_alloca:[0-9]+]] @test_alloca() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_size]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13)));
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], widen<u64, reason=arg>(read<u32>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p_2]]), null<ptr<void>>))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_2]])), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_calloc:[0-9]+]] @__builtin_calloc(%[[VALUE6:[0-9]+]] <unnamed>: u64, %[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_calloc:[0-9]+]] @test_calloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_size]], reinterpret<u32, reason=arg, fits=always>(const<i32>(19)));
// DEFAULT-NEXT:         let %[[VALUE_n_4:[0-9]+]] n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_size]], reinterpret<u32, reason=arg, fits=always>(const<i32>(23)));
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE___builtin_calloc]], widen<u64, reason=arg>(read<u32>(%[[VALUE_m]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), const<i32>(0)), widen<u64, reason=usual_arith>(mul<u32, overflow=wrap>(read<u32>(%[[VALUE_m]]), read<u32>(%[[VALUE_n_4]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE8:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_malloc:[0-9]+]] @test_malloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_5:[0-9]+]] n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_size]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         let %[[VALUE_p_4:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], widen<u64, reason=arg>(read<u32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_4]])), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_realloc:[0-9]+]] @__builtin_realloc(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_realloc:[0-9]+]] @test_realloc(%[[VALUE_p_5:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n_6:[0-9]+]] n: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_size]], reinterpret<u32, reason=arg, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p_5]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_5]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_6]]))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_5]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_5]])), const<i32>(0)), widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_n_6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
