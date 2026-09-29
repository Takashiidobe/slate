/* PR c/78284 - warn on malloc with very large arguments
   Test to exercise the interaction of the -Walloca-larger-than,
   -Wvla-larger-than, and -Walloc-size-larger-than options.  The former
   two more specific options override the more general latter option.  */
/* { dg-do compile } */
/* { dg-options "-O -Walloc-size-larger-than=123 -Walloca-larger-than=234 -Wvla-larger-than=345" } */

typedef __SIZE_TYPE__ size_t;

void sink (void*);

static size_t alloc_size_limit (void)
{
  return 123;
}

static size_t alloca_limit (void)
{
  return 234;
}

static size_t vla_limit (void)
{
  return 345;
}

void test_alloca (void)
{
  void *p;

  /* No warning should be issued for the following call because the more
     permissive alloca limit overrides the stricter alloc_size limit.  */
  p = __builtin_alloca (alloca_limit ());
  sink (p);

  p = __builtin_alloca (alloca_limit () + 1);   /* { dg-warning "argument to .alloca. is too large" } */
  sink (p);
}

void test_vla (void)
{
  /* Same as above, no warning should be issued here because the more
     permissive VLA limit overrides the stricter alloc_size limit.  */
  char vla1 [vla_limit ()];
  sink (vla1);

  char vla2 [vla_limit () + 1];   /* { dg-warning "argument to variable-length array is too large" } */
  sink (vla2);
}

void test_malloc (void)
{
  void *p;
  p = __builtin_malloc (alloc_size_limit ());
  sink (p);

  p = __builtin_malloc (alloc_size_limit () + 1);   /* { dg-warning "argument 1 value .124\[lu\]*. exceeds maximum object size 123" } */
  sink (p);
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
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_alloc_size_limit:[0-9]+]] @alloc_size_limit() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(123)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alloca_limit:[0-9]+]] @alloca_limit() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(234)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vla_limit:[0-9]+]] @vla_limit() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(345)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_alloca:[0-9]+]] @test_alloca() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], call<u64, signature=fn() -> u64>(%[[VALUE_alloca_limit]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], call<u64, signature=fn() -> u64>(%[[VALUE_alloca_limit]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], add<u64, overflow=wrap>(call<u64, signature=fn() -> u64>(%[[VALUE_alloca_limit]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], add<u64, overflow=wrap>(call<u64, signature=fn() -> u64>(%[[VALUE_alloca_limit]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_vla:[0-9]+]] @test_vla() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = call<u64, signature=fn() -> u64>(%[[VALUE_vla_limit]]);
// DEFAULT-NEXT:         let %[[VALUE_vla1:[0-9]+]] vla1: vla<i8, %[[VALUE2]]> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_vla1]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(call<u64, signature=fn() -> u64>(%[[VALUE_vla_limit]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_vla2:[0-9]+]] vla2: vla<i8, %[[VALUE3]]> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=None>(%[[VALUE_vla2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE4:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_malloc:[0-9]+]] @test_malloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p_2]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], call<u64, signature=fn() -> u64>(%[[VALUE_alloc_size_limit]])));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], call<u64, signature=fn() -> u64>(%[[VALUE_alloc_size_limit]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p_2]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], add<u64, overflow=wrap>(call<u64, signature=fn() -> u64>(%[[VALUE_alloc_size_limit]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], add<u64, overflow=wrap>(call<u64, signature=fn() -> u64>(%[[VALUE_alloc_size_limit]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
