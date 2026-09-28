/* { dg-require-effective-target int32plus } */

/* Failed on sparc with -mv8plus because sparc.c:set_extends() thought
   erroneously that SImode ASHIFT chops the upper bits, it does not.  */

typedef unsigned long long uint64;

void g(uint64 x, int y, int z, uint64 *p) {
  unsigned w  = ((x >> y) & 0xffffffffULL) << (z & 0x1f);
  *p         |= (w & 0xffffffffULL) << z;
}

int main(void) {
  uint64 a = 0;
  g(0xdeadbeef01234567ULL, 0, 0, &a);
  return (a == 0x01234567) ? 0 : 1;
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
// DEFAULT-NEXT:     type @type0 uint64 = u64;
// DEFAULT-NEXT:     fn %1 @g(%2 x: u64, %3 y: i32, %4 z: i32, %5 p: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 w: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(shl<u64, overflow=wrap, amount_out_of_range=ub>(and<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%2), read<i32>(%3)), const<u64>(4294967295)), and<i32>(read<i32>(%4), const<i32>(31))));
// DEFAULT-NEXT:         let %9: ptr<u64> [synthetic] = read<ptr<u64>>(%5);
// DEFAULT-NEXT:         let %10: u64 [synthetic] = read<u64>(deref(read<ptr<u64>>(%9)));
// DEFAULT-NEXT:         let %11: u64 [synthetic] = or<u64>(read<u64>(%10), shl<u64, overflow=wrap, amount_out_of_range=ub>(and<u64>(widen<u64, reason=usual_arith>(read<u32>(%6)), const<u64>(4294967295)), read<i32>(%4)));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%9)), read<u64>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(u64, i32, i32, ptr<u64>) -> void>(%1, const<u64>(16045690981116495207), const<i32>(0), const<i32>(0), addr_of<ptr<u64>>(%8));
// DEFAULT-NEXT:         return conditional<i32>(eq<u64>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(19088743)))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
