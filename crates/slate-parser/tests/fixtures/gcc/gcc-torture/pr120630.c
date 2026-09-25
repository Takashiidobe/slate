/* PR middle-end/120630 */

__attribute__((noipa)) int foo(const char *x, ...) { return *x; }

int      a, b, c;
unsigned d = 1;

int main() {
  if (a)
    foo("0");
  int e = -1;
  if (a < 1) {
    e = c;
    if (c)
      while (1)
        ;
  }
  b = (~e + 0UL) / -1;
  if (d > b)
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
// DEFAULT-NEXT:     global %2 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 d: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<const i8>, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(read<ptr<const i8>>(%1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%8)));
// DEFAULT-NEXT:         let %7 e: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%4));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                     while %9 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%3, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(not<i32>(read<i32>(%7)))), const<u64>(0)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))))));
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
