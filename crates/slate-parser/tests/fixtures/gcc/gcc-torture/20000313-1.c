void abort(void);

unsigned int buggy(unsigned int *param) {
  unsigned int accu, zero = 0, borrow;
  accu    = -*param;
  borrow  = -(accu > zero);
  *param += accu;
  return borrow;
}

int main(void) {
  unsigned int param  = 1;
  unsigned int borrow = buggy(&param);

  if (param != 0)
    abort();
  if (borrow + 1 != 0)
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
// DEFAULT-NEXT:     fn %1 @buggy(%2 param: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 accu: u32 [storage=automatic];
// DEFAULT-NEXT:         let %4 zero: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %5 borrow: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%3, neg<u32, overflow=wrap>(read<u32>(deref(read<ptr<u32>>(%2)))));
// DEFAULT-NEXT:         write<u32>(%5, reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<u32>(read<u32>(%3), read<u32>(%4))))));
// DEFAULT-NEXT:         let %9: ptr<u32> [synthetic] = read<ptr<u32>>(%2);
// DEFAULT-NEXT:         let %10: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%9)));
// DEFAULT-NEXT:         let %11: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%10), read<u32>(%3));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%9)), read<u32>(%11));
// DEFAULT-NEXT:         return read<u32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 param: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         let %8 borrow: u32 [storage=automatic] = call<u32, signature=fn(ptr<u32>) -> u32>(%1, addr_of<ptr<u32>>(%7));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u32>(add<u32, overflow=wrap>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
