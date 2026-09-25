void abort(void);
void exit(int);

int f(unsigned bitcount, int mant) {
  int mask = -1 << bitcount;
  {
    if (!(mant & -mask))
      goto ab;
    if (mant & ~mask)
      goto auf;
  }
ab:
  return 0;
auf:
  return 1;
}

int main(void) {
  if (f(0, -1))
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
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%5 bitcount: u32, %6 mant: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 mask: i32 [storage=automatic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=ub>(const<i32>(1)), read<u32>(%5));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if not<bool>(ne<i32>(and<i32>(read<i32>(%6), neg<i32, overflow=ub>(read<i32>(%7))), const<i32>(0)))
// DEFAULT-NEXT:                 goto %3;
// DEFAULT-NEXT:             if ne<i32>(and<i32>(read<i32>(%6), not<i32>(read<i32>(%7))), const<i32>(0))
// DEFAULT-NEXT:                 goto %4;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         label %3 ab:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         label %4 auf:
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%2, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
