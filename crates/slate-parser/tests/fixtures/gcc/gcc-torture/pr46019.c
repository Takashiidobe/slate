/* PR middle-end/46019 */

extern void abort(void);

int main(void) {
  unsigned long long l = 0x40000000000ULL;
  int                n;
  for (n = 0; n < 8; n++)
    if (l / (0x200000000ULL << n) != (0x200 >> n))
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 l: u64 [storage=automatic] = const<u64>(4398046511104);
// DEFAULT-NEXT:         let %3 n: i32 [storage=automatic];
// DEFAULT-NEXT:         for %4
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %5: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%5), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%6));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u64>(div<u64, by_zero=ub>(read<u64>(%2), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(8589934592), read<i32>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(512), read<i32>(%3)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
