int foo(unsigned long long aLL) {
  switch (aLL) {
  case 1000000000000000000ULL ... 9999999999999999999ULL:
    return 19;
  default:
    return 20;
  };
};
extern void abort(void);
int         main() {
  unsigned long long aLL = 1000000000000000000ULL;
  if (foo(aLL) != 19)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 aLL: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %5 read<u64>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %5 const<u64>(1000000000000000000) ... const<u64>(9999999999999999999):
// DEFAULT-NEXT:                     return const<i32>(19);
// DEFAULT-NEXT:                 default %5:
// DEFAULT-NEXT:                     return const<i32>(20);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 aLL: u64 [storage=automatic] = const<u64>(1000000000000000000);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u64) -> i32>(%0, read<u64>(%4)), const<i32>(19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
