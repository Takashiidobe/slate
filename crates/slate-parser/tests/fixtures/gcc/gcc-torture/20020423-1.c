/* PR c/5430 */
/* Verify that the multiplicative folding code is not fooled
   by the mix between signed variables and unsigned constants. */

extern void abort(void);
extern void exit(int);

int main(void) {
  int          my_int = 924;
  unsigned int result;

  result = ((my_int * 2 + 4) - 8U) / 2;
  if (result != 922U)
    abort();

  result = ((my_int * 2 - 4U) + 2) / 2;
  if (result != 923U)
    abort();

  result = (((my_int + 2) * 2) - 8U - 4) / 2;
  if (result != 920U)
    abort();
  result = (((my_int + 2) * 2) - (8U + 4)) / 2;
  if (result != 920U)
    abort();

  result = ((my_int * 4 + 2U) - 4U) / 2;
  if (result != 1847U)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 my_int: i32 [storage=automatic] = const<i32>(924);
// DEFAULT-NEXT:         let %4 result: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%4, div<u32, by_zero=ub>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)), const<i32>(4))), const<u32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), const<u32>(922))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%4, div<u32, by_zero=ub>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2))), const<u32>(4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), const<u32>(923))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%4, div<u32, by_zero=ub>(sub<u32, overflow=wrap>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%3), const<i32>(2)), const<i32>(2))), const<u32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), const<u32>(920))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%4, div<u32, by_zero=ub>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%3), const<i32>(2)), const<i32>(2))), add<u32, overflow=wrap>(const<u32>(8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), const<u32>(920))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%4, div<u32, by_zero=ub>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%3), const<i32>(4))), const<u32>(2)), const<u32>(4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%4), const<u32>(1847))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
