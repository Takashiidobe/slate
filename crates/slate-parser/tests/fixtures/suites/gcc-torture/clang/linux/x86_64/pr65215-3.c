/* PR tree-optimization/65215 */

struct S {
  unsigned long long l1 : 24, l2 : 8, l3 : 32;
};

static inline unsigned int foo(unsigned int x) {
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

__attribute__((noinline, noclone)) unsigned long long bar(struct S *x) {
  unsigned long long x1 = foo(((unsigned int)x->l1 << 8) | x->l2);
  unsigned long long x2 = foo(x->l3);
  return (x2 << 32) | x1;
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(unsigned int) != 4 ||
      sizeof(unsigned long long) != 8)
    return 0;
  struct S           s = {0xdeadbeU, 0xefU, 0xfeedbea8U};
  unsigned long long l = bar(&s);
  if (foo(l >> 32) != s.l3 || (foo(l) >> 8) != s.l1 || (foo(l) & 0xff) != s.l2)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 l1: u64 : 24;
// DEFAULT-NEXT:         field1 l2: u64 : 8;
// DEFAULT-NEXT:         field2 l3: u64 : 32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 3, 4], bit_offsets=[Some(0), Some(24), Some(32)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<u32>(or<u32>(or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%2), const<i32>(24)), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%2), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280)))), and<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%2), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680)))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%2), const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: ptr<@type0>) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 x1: u64 [storage=automatic] = widen<u64, reason=assign>(call<u32, signature=fn(u32) -> u32>(%1, or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(reinterpret<u32, reason=explicit, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..24>(deref(read<ptr<@type0>>(%4))))))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=24..32>(deref(read<ptr<@type0>>(%4))))))))));
// DEFAULT-NEXT:         let %6 x2: u64 [storage=automatic] = widen<u64, reason=assign>(call<u32, signature=fn(u32) -> u32>(%1, truncate<u32, reason=arg, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=32..64>(deref(read<ptr<@type0>>(%4)))))));
// DEFAULT-NEXT:         return or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%6), const<i32>(32)), read<u64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %8 s: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = widen<u64, reason=assign>(const<u32>(14593470)), field1 = widen<u64, reason=assign>(const<u32>(239)), field2 = widen<u64, reason=assign>(const<u32>(4276993704)));
// DEFAULT-NEXT:         let %9 l: u64 [storage=automatic] = call<u64, signature=fn(ptr<@type0>) -> u64>(%3, addr_of<ptr<@type0>>(%8));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%1, truncate<u32, reason=arg, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%9), const<i32>(32))))), read<u64>(bitfield2<unit=0, bytes=0..8, bits=32..64>(%8)))
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(call<u32, signature=fn(u32) -> u32>(%1, truncate<u32, reason=arg, fits=unknown>(read<u64>(%9))), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..24>(%8)))))));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<u32>(and<u32>(call<u32, signature=fn(u32) -> u32>(%1, truncate<u32, reason=arg, fits=unknown>(read<u64>(%9))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=24..32>(%8)))))));
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
