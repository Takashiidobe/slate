extern void abort(void);

static unsigned int my_add(unsigned int si1, unsigned int si2) {
  return (si1 > (50 - si2)) ? si1 : (si1 + si2);
}

static unsigned int my_shift(unsigned int left, unsigned int right) {
  return (right > 100) ? left : (left >> right);
}

static int func_4(unsigned int p_6) {
  int count = 0;
  for (p_6 = 1; p_6 < 3; p_6 = my_add(p_6, 1)) {
    if (count++ > 1)
      abort();

    if (my_shift(p_6, p_6))
      return 0;
  }
  return 0;
}

int main(void) {
  func_4(0);
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
// DEFAULT-NEXT:     fn %1 @my_add(%2 si1: u32, %3 si2: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(gt<u32>(read<u32>(%2), sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(50)), read<u32>(%3))), read<u32>(%2), add<u32, overflow=wrap>(read<u32>(%2), read<u32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @my_shift(%5 left: u32, %6 right: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(gt<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(100))), read<u32>(%5), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%5), read<u32>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @func_4(%8 p_6: u32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%8, reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<u32>(%8, call<u32, signature=fn(u32, u32) -> u32>(%1, read<u32>(%8), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:                 call<u32, signature=fn(u32, u32) -> u32>(%1, read<u32>(%8), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                     let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%9, read<i32>(%13));
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(%12), const<i32>(1))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     if ne<u32>(call<u32, signature=fn(u32, u32) -> u32>(%4, read<u32>(%8), read<u32>(%8)), const<u32>(0))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(u32) -> i32>(%7, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
