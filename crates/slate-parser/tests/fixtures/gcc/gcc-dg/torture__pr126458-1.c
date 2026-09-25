/* { dg-do run { target bitint } } */
/* PR tree-optimization/126458 */

typedef unsigned _BitInt(17) u17;
typedef unsigned _BitInt(16) u16;
typedef unsigned _BitInt(15) u15;
typedef signed _BitInt(16) s16;

__attribute__((noipa)) int neg(u16 a) { return ((s16)a) < 0; }

__attribute__((noipa)) u17 fref(u16 a) {
  return neg(a) ? (u17)a : (u17)(u16)-1u;
}

__attribute__((noipa)) u17 f(u16 a) {
  return ((s16)a) < 0 ? (u17)a : (u17)(u16)-1u;
}

int main(void) {
  static const u16 v[] = {((u16)1u) << 15, (u16)-2u, (u16)-1u, (u16)(u15)-1u,
                          0u};

  for (unsigned i = 0; i < sizeof v / sizeof v[0]; i++)
    if (f(v[i]) != fref(v[i]))
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
// DEFAULT-NEXT:     type @type0 u17 = u17b;
// DEFAULT-NEXT:     type @type1 u16 = u16b;
// DEFAULT-NEXT:     type @type2 u15 = u15b;
// DEFAULT-NEXT:     type @type3 s16 = i16b;
// DEFAULT-NEXT:     global %11 v: array<u16b, 5> [storage=static] [const] = aggregate<array<u16b, 5>, zero_fill=false>(index0 = shl<u16b, overflow=wrap, amount_out_of_range=ub>(truncate<u16b, reason=explicit, fits=always>(const<u32>(1)), const<i32>(15)), index1 = truncate<u16b, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(2))), index2 = truncate<u16b, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(1))), index3 = widen<u16b, reason=explicit>(truncate<u15b, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(1)))), index4 = truncate<u16b, reason=assign, fits=always>(const<u32>(0))) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @neg(%5 a: u16b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(widen<i32, reason=usual_arith>(reinterpret<i16b, reason=explicit, fits=unknown>(read<u16b>(%5))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fref(%7 a: u16b) -> u17b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u17b>(ne<i32>(call<i32, signature=fn(u16b) -> i32>(%4, read<u16b>(%7)), const<i32>(0)), widen<u17b, reason=explicit>(read<u16b>(%7)), widen<u17b, reason=explicit>(truncate<u16b, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f(%9 a: u16b) -> u17b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u17b>(lt<i32>(widen<i32, reason=usual_arith>(reinterpret<i16b, reason=explicit, fits=unknown>(read<u16b>(%9))), const<i32>(0)), widen<u17b, reason=explicit>(read<u16b>(%9)), widen<u17b, reason=explicit>(truncate<u16b, reason=explicit, fits=unknown>(neg<u32, overflow=wrap>(const<u32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %12 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%12)), div<u64, by_zero=ub>(const<u64>(10), const<u64>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: u32 [synthetic] = read<u32>(%12);
// DEFAULT-NEXT:                 let %15: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%14), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%12, read<u32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u17b>(call<u17b, signature=fn(u16b) -> u17b>(%8, read<u16b>(deref(ptr_offset<ptr<const u16b>, subtract=false, element=u16b, overflow=ub>(array_decay<ptr<const u16b>, length=Some(5)>(%11), read<u32>(%12))))), call<u17b, signature=fn(u16b) -> u17b>(%6, read<u16b>(deref(ptr_offset<ptr<const u16b>, subtract=false, element=u16b, overflow=ub>(array_decay<ptr<const u16b>, length=Some(5)>(%11), read<u32>(%12))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
