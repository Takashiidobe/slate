/* PR tree-optimization/65215 */

struct S {
  unsigned long long l1 : 48;
};

static inline unsigned int foo(unsigned int x) {
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

__attribute__((noinline, noclone)) unsigned int bar(struct S *x) {
  return foo(x->l1);
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(unsigned int) != 4 ||
      sizeof(unsigned long long) != 8)
    return 0;
  struct S s;
  s.l1 = foo(0xdeadbeefU) | (0xfeedULL << 32);
  if (bar(&s) != 0xdeadbeefU)
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
// DEFAULT-NEXT:         field0 l1: u64 : 48;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 6)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<u32>(or<u32>(or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%2), const<i32>(24)), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%2), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280)))), and<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%2), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680)))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%2), const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: ptr<@type0>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%1, truncate<u32, reason=arg, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..6, bits=0..48>(deref(read<ptr<@type0>>(%4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %6 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..6, bits=0..48>(%6), or<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%1, const<u32>(3735928559))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(65261), const<i32>(32))));
// DEFAULT-NEXT:         or<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%1, const<u32>(3735928559))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(65261), const<i32>(32)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(ptr<@type0>) -> u32>(%3, addr_of<ptr<@type0>>(%6)), const<u32>(3735928559))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
