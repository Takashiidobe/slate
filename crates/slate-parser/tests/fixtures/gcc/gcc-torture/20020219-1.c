/* PR c/4308
   This testcase failed because 0x8000000000000000 >> 0
   was incorrectly folded into 0xffffffff00000000.  */

extern void abort(void);
extern void exit(int);

long long foo(void) {
  long long C = 1ULL << 63, X;
  int       Y = 32;
  X           = C >> (Y & 31);
  return X;
}

int main(void) {
  if (foo() != 1ULL << 63)
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
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 C: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)));
// DEFAULT-NEXT:         let %4 X: i64 [storage=automatic];
// DEFAULT-NEXT:         let %5 Y: i32 [storage=automatic] = const<i32>(32);
// DEFAULT-NEXT:         write<i64>(%4, shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%3), and<i32>(read<i32>(%5), const<i32>(31))));
// DEFAULT-NEXT:         return read<i64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(call<i64, signature=fn() -> i64>(%2)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(63)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
