/* PR rtl-optimization/64255 */

__attribute__((noinline, noclone)) void bar(long i, unsigned long j) {
  if (i != 1 || j != 1)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void foo(long i) {
  unsigned long j;

  if (!i)
    return;
  j = i >= 0 ? (unsigned long)i : -(unsigned long)i;
  if ((i >= 0 ? (unsigned long)i : -(unsigned long)i) != j)
    __builtin_abort();
  bar(i, j);
}

int main() {
  foo(1);
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
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @bar(%1 i: i64, %2 j: u64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(1))), ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo(%4 i: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 j: u64 [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(ne<i64>(read<i64>(%4), const<i64>(0)))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         write<u64>(%5, conditional<u64>(ge<i64>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(0))), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%4)), neg<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%4)))));
// DEFAULT-NEXT:         if ne<u64>(conditional<u64>(ge<i64>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(0))), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%4)), neg<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%4)))), read<u64>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn(i64, u64) -> void>(%0, read<i64>(%4), read<u64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%3, widen<i64, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
