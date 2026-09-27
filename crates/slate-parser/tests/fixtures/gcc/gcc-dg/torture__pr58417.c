/* { dg-do run } */

long long                               arr[6] = {0, 1, 2, 3, 4, 5};
extern void                             abort(void);
void __attribute__((noinline, noclone)) foo(long long sum) { asm(""); }
int                                     main() {
  int       i, n = 5;
  long long sum = 0, prevsum = 0;

  for (i = 1; i <= n; i++) {
    foo(sum);
    sum      = (i - 1) * arr[i] - prevsum;
    prevsum += arr[i];
  }

  if (sum != 10)
    abort();
  return 0;
}



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
// DEFAULT-NEXT:     global %0 arr: array<i64, 6> [storage=static] [align=16] = aggregate<array<i64, 6>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(0)), index1 = widen<i64, reason=assign>(const<i32>(1)), index2 = widen<i64, reason=assign>(const<i32>(2)), index3 = widen<i64, reason=assign>(const<i32>(3)), index4 = widen<i64, reason=assign>(const<i32>(4)), index5 = widen<i64, reason=assign>(const<i32>(5))) [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 sum: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "" [dialect=att];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 n: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %7 sum: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %8 prevsum: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%5), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(i64) -> void>(%2, read<i64>(%7));
// DEFAULT-NEXT:                     write<i64>(%7, sub<i64, overflow=ub>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(read<i32>(%5), const<i32>(1))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(6)>(%0), read<i32>(%5))))), read<i64>(%8)));
// DEFAULT-NEXT:                     let %12: i64 [synthetic] = read<i64>(%8);
// DEFAULT-NEXT:                     let %13: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%12), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(6)>(%0), read<i32>(%5)))));
// DEFAULT-NEXT:                     write<i64>(%8, read<i64>(%13));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
