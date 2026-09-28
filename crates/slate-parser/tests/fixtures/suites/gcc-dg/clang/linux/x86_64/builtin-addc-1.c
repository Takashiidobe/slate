/* { dg-do run } */
/* { dg-options "-O2 -g" } */

int
main ()
{
  unsigned int c;
  unsigned long cl;
  unsigned long long cll;
  if (__builtin_addc (1, 42, 0, &c) != 43 || c != 0)
    __builtin_abort ();
  if (__builtin_addc (1, 42, 15, &c) != 58 || c != 0)
    __builtin_abort ();
  if (__builtin_addc (-2U, -3U, -4U, &c) != -9U || c != 1)
    __builtin_abort ();
  if (__builtin_addc (-2U, 1, 0, &c) != -1U || c != 0)
    __builtin_abort ();
  if (__builtin_addc (-2U, 1, 1, &c) != 0 || c != 1)
    __builtin_abort ();
  if (__builtin_addc (-2U, 2, 0, &c) != 0 || c != 1)
    __builtin_abort ();
  if (__builtin_addc (-2U, 0, 2, &c) != 0 || c != 1)
    __builtin_abort ();
  if (__builtin_addcl (1L, 42L, 0L, &cl) != 43 || cl != 0L)
    __builtin_abort ();
  if (__builtin_addcl (1L, 42L, 15L, &cl) != 58 || cl != 0L)
    __builtin_abort ();
  if (__builtin_addcl (-2UL, -3UL, -4UL, &cl) != -9UL || cl != 1L)
    __builtin_abort ();
  if (__builtin_addcl (-2UL, 1L, 0L, &cl) != -1UL || cl != 0L)
    __builtin_abort ();
  if (__builtin_addcl (-2UL, 1L, 1L, &cl) != 0 || cl != 1L)
    __builtin_abort ();
  if (__builtin_addcl (-2UL, 2L, 0L, &cl) != 0 || cl != 1L)
    __builtin_abort ();
  if (__builtin_addcl (-2UL, 0L, 2L, &cl) != 0 || cl != 1L)
    __builtin_abort ();
  if (__builtin_addcll (1LL, 42LL, 0LL, &cll) != 43 || cll != 0LL)
    __builtin_abort ();
  if (__builtin_addcll (1LL, 42LL, 15LL, &cll) != 58 || cll != 0LL)
    __builtin_abort ();
  if (__builtin_addcll (-2ULL, -3ULL, -4ULL, &cll) != -9ULL || cll != 1LL)
    __builtin_abort ();
  if (__builtin_addcll (-2ULL, 1LL, 0LL, &cll) != -1ULL || cll != 0LL)
    __builtin_abort ();
  if (__builtin_addcll (-2ULL, 1LL, 1LL, &cll) != 0 || cll != 1LL)
    __builtin_abort ();
  if (__builtin_addcll (-2ULL, 2LL, 0LL, &cll) != 0 || cll != 1LL)
    __builtin_abort ();
  if (__builtin_addcll (-2ULL, 0LL, 2LL, &cll) != 0 || cll != 1LL)
    __builtin_abort ();
  if (__builtin_subc (42, 42, 0, &c) != 0 || c != 0)
    __builtin_abort ();
  if (__builtin_subc (42, 42, 1, &c) != -1U || c != 1)
    __builtin_abort ();
  if (__builtin_subc (1, -3U, -4U, &c) != 8 || c != 1)
    __builtin_abort ();
  if (__builtin_subc (-2U, 1, 0, &c) != -3U || c != 0)
    __builtin_abort ();
  if (__builtin_subc (-2U, -1U, 0, &c) != -1U || c != 1)
    __builtin_abort ();
  if (__builtin_subc (-2U, -2U, 0, &c) != 0 || c != 0)
    __builtin_abort ();
  if (__builtin_subc (-2U, -2U, 1, &c) != -1U || c != 1)
    __builtin_abort ();
  if (__builtin_subc (-2U, 1, -2U, &c) != -1U || c != 1)
    __builtin_abort ();
  if (__builtin_subcl (42L, 42L, 0L, &cl) != 0L || cl != 0L)
    __builtin_abort ();
  if (__builtin_subcl (42L, 42L, 1L, &cl) != -1UL || cl != 1L)
    __builtin_abort ();
  if (__builtin_subcl (1L, -3UL, -4UL, &cl) != 8L || cl != 1L)
    __builtin_abort ();
  if (__builtin_subcl (-2UL, 1L, 0L, &cl) != -3UL || cl != 0L)
    __builtin_abort ();
  if (__builtin_subcl (-2UL, -1UL, 0L, &cl) != -1UL || cl != 1L)
    __builtin_abort ();
  if (__builtin_subcl (-2UL, -2UL, 0L, &cl) != 0L || cl != 0L)
    __builtin_abort ();
  if (__builtin_subcl (-2UL, -2UL, 1L, &cl) != -1UL || cl != 1L)
    __builtin_abort ();
  if (__builtin_subcl (-2UL, 1L, -2UL, &cl) != -1UL || cl != 1L)
    __builtin_abort ();
  if (__builtin_subcll (42LL, 42LL, 0LL, &cll) != 0LL || cll != 0LL)
    __builtin_abort ();
  if (__builtin_subcll (42LL, 42LL, 1LL, &cll) != -1ULL || cll != 1LL)
    __builtin_abort ();
  if (__builtin_subcll (1LL, -3ULL, -4ULL, &cll) != 8LL || cll != 1LL)
    __builtin_abort ();
  if (__builtin_subcll (-2ULL, 1LL, 0LL, &cll) != -3ULL || cll != 0LL)
    __builtin_abort ();
  if (__builtin_subcll (-2ULL, -1ULL, 0LL, &cll) != -1ULL || cll != 1LL)
    __builtin_abort ();
  if (__builtin_subcll (-2ULL, -2ULL, 0LL, &cll) != 0LL || cll != 0LL)
    __builtin_abort ();
  if (__builtin_subcll (-2ULL, -2ULL, 1LL, &cll) != -1ULL || cll != 1LL)
    __builtin_abort ();
  if (__builtin_subcll (-2ULL, 1LL, -2ULL, &cll) != -1ULL || cll != 1LL)
    __builtin_abort ();
  return 0;
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
// DEFAULT-NEXT:     fn %8 @__builtin_addc(%4 <unnamed>: u32, %5 <unnamed>: u32, %6 <unnamed>: u32, %7 <unnamed>: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @__builtin_addcl(%10 <unnamed>: u64, %11 <unnamed>: u64, %12 <unnamed>: u64, %13 <unnamed>: ptr<u64>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_addcll(%15 <unnamed>: u64, %16 <unnamed>: u64, %17 <unnamed>: u64, %18 <unnamed>: ptr<u64>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %24 @__builtin_subc(%20 <unnamed>: u32, %21 <unnamed>: u32, %22 <unnamed>: u32, %23 <unnamed>: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %29 @__builtin_subcl(%25 <unnamed>: u64, %26 <unnamed>: u64, %27 <unnamed>: u64, %28 <unnamed>: ptr<u64>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %34 @__builtin_subcll(%30 <unnamed>: u64, %31 <unnamed>: u64, %32 <unnamed>: u64, %33 <unnamed>: ptr<u64>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 c: u32 [storage=automatic];
// DEFAULT-NEXT:         let %2 cl: u64 [storage=automatic];
// DEFAULT-NEXT:         let %3 cll: u64 [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(43))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), reinterpret<u32, reason=arg, fits=always>(const<i32>(15)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(58))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, neg<u32, overflow=wrap>(const<u32>(2)), neg<u32, overflow=wrap>(const<u32>(3)), neg<u32, overflow=wrap>(const<u32>(4)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(9))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(1))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%8, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(43)))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(15)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(58)))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(3)), neg<u64, overflow=wrap>(const<u64>(4)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(9))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%14, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), reinterpret<u64, reason=arg, fits=always>(const<i64>(2)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(43)))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(15)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(58)))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(3)), neg<u64, overflow=wrap>(const<u64>(4)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(9))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%19, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), reinterpret<u64, reason=arg, fits=always>(const<i64>(2)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(1))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), neg<u32, overflow=wrap>(const<u32>(3)), neg<u32, overflow=wrap>(const<u32>(4)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(3))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, neg<u32, overflow=wrap>(const<u32>(2)), neg<u32, overflow=wrap>(const<u32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(1))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, neg<u32, overflow=wrap>(const<u32>(2)), neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), addr_of<ptr<u32>>(%1)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, neg<u32, overflow=wrap>(const<u32>(2)), neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(1))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(call<u32, signature=fn(u32, u32, u32, ptr<u32>) -> u32>(%24, neg<u32, overflow=wrap>(const<u32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), neg<u32, overflow=wrap>(const<u32>(2)), addr_of<ptr<u32>>(%1)), neg<u32, overflow=wrap>(const<u32>(1))), ne<u32>(read<u32>(%1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), neg<u64, overflow=wrap>(const<u64>(3)), neg<u64, overflow=wrap>(const<u64>(4)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(8))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(3))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%2)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%29, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), neg<u64, overflow=wrap>(const<u64>(2)), addr_of<ptr<u64>>(%2)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(42)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), neg<u64, overflow=wrap>(const<u64>(3)), neg<u64, overflow=wrap>(const<u64>(4)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(8))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(3))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(0)), addr_of<ptr<u64>>(%3)), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, neg<u64, overflow=wrap>(const<u64>(2)), neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(call<u64, signature=fn(u64, u64, u64, ptr<u64>) -> u64>(%34, neg<u64, overflow=wrap>(const<u64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), neg<u64, overflow=wrap>(const<u64>(2)), addr_of<ptr<u64>>(%3)), neg<u64, overflow=wrap>(const<u64>(1))), ne<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
