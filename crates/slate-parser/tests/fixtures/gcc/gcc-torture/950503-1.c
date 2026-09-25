void abort(void);
void exit(int);

int main(void) {
  int                tmp;
  unsigned long long utmp1, utmp2;

  tmp = 16;

  utmp1 = (~((unsigned long long)0)) >> tmp;
  utmp2 = (~((unsigned long long)0)) >> 16;

  if (utmp1 != utmp2)
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
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 tmp: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 utmp1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %5 utmp2: u64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(16));
// DEFAULT-NEXT:         write<u64>(%4, shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), read<i32>(%3)));
// DEFAULT-NEXT:         write<u64>(%5, shr<u64, amount_out_of_range=ub, fill=zero_extend>(not<u64>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0)))), const<i32>(16)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%4), read<u64>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
