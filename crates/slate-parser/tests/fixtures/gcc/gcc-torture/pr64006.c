/* PR tree-optimization/64006 */

int v;

long __attribute__((noinline, noclone)) test(long *x, int y) {
  int  i;
  long s = 1;
  for (i = 0; i < y; i++)
    if (__builtin_mul_overflow(s, x[i], &s))
      v++;
  return s;
}

int main() {
  long d[7] = {975, 975, 975, 975, 975, 975, 975};
  long r    = test(d, 7);
  if (sizeof(long) * __CHAR_BIT__ == 64 && v != 1)
    __builtin_abort();
  else if (sizeof(long) * __CHAR_BIT__ == 32 && v != 4)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @test(%2 x: ptr<i64>, %3 y: i32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 s: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), read<i32>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if overflow_mul<bool>(read<i64>(%5), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%2), read<i32>(%4)))), deref(addr_of<ptr<i64>>(%5)))
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                     let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%0, read<i32>(%13));
// DEFAULT-NEXT:         return read<i64>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 d: array<i64, 7> [storage=automatic] = aggregate<array<i64, 7>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(975)), index1 = widen<i64, reason=assign>(const<i32>(975)), index2 = widen<i64, reason=assign>(const<i32>(975)), index3 = widen<i64, reason=assign>(const<i32>(975)), index4 = widen<i64, reason=assign>(const<i32>(975)), index5 = widen<i64, reason=assign>(const<i32>(975)), index6 = widen<i64, reason=assign>(const<i32>(975)));
// DEFAULT-NEXT:         let %8 r: i64 [storage=automatic] = call<i64, signature=fn(ptr<i64>, i32) -> i64>(%1, array_decay<ptr<i64>, length=Some(7)>(%7), const<i32>(7));
// DEFAULT-NEXT:         if logical_and<bool>(eq<u64>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64)))), ne<i32>(read<i32>(%0), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<u64>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32)))), ne<i32>(read<i32>(%0), const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
