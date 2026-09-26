/* PR target/89369 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 && __CHAR_BIT__ == 8
struct S {
  unsigned int u[4];
};

static void foo(struct S *out, struct S const *in, int shift) {
  unsigned long long th, tl, oh, ol;
  th         = ((unsigned long long)in->u[3] << 32) | in->u[2];
  tl         = ((unsigned long long)in->u[1] << 32) | in->u[0];
  oh         = th >> (shift * 8);
  ol         = tl >> (shift * 8);
  ol        |= th << (64 - shift * 8);
  out->u[1]  = ol >> 32;
  out->u[0]  = ol;
  out->u[3]  = oh >> 32;
  out->u[2]  = oh;
}

static void bar(struct S *out, struct S const *in, int shift) {
  unsigned long long th, tl, oh, ol;
  th         = ((unsigned long long)in->u[3] << 32) | in->u[2];
  tl         = ((unsigned long long)in->u[1] << 32) | in->u[0];
  oh         = th << (shift * 8);
  ol         = tl << (shift * 8);
  oh        |= tl >> (64 - shift * 8);
  out->u[1]  = ol >> 32;
  out->u[0]  = ol;
  out->u[3]  = oh >> 32;
  out->u[2]  = oh;
}

__attribute__((noipa)) static void baz(struct S *r, struct S *a, struct S *b,
                                       struct S *c, struct S *d) {
  struct S x, y;
  bar(&x, a, 1);
  foo(&y, c, 1);
  r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> 11) & 0xdfffffefU) ^ y.u[0] ^
            (d->u[0] << 18);
  r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> 11) & 0xddfecb7fU) ^ y.u[1] ^
            (d->u[1] << 18);
  r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> 11) & 0xbffaffffU) ^ y.u[2] ^
            (d->u[2] << 18);
  r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> 11) & 0xbffffff6U) ^ y.u[3] ^
            (d->u[3] << 18);
}

int main() {
  struct S a[] = {{0x000004d3, 0xbc5448db, 0xf22bde9f, 0xebb44f8f},
                  {0x03a32799, 0x60be8246, 0xa2d266ed, 0x7aa18536},
                  {0x15a38518, 0xcf655ce1, 0xf3e09994, 0x50ef69fe},
                  {0x88274b07, 0xe7c94866, 0xc0ea9f47, 0xb6a83c43},
                  {0xcd0d0032, 0x5d47f5d7, 0x5a0afbf6, 0xaea87b24},
                  {0, 0, 0, 0}};
  baz(&a[5], &a[0], &a[1], &a[2], &a[3]);
  if (a[4].u[0] != a[5].u[0] || a[4].u[1] != a[5].u[1] ||
      a[4].u[2] != a[5].u[2] || a[4].u[3] != a[5].u[3])
    __builtin_abort();
  return 0;
}
#else
int main() { return 0; }
#endif


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
// DEFAULT-NEXT:         field0 u: array<u32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo(%2 out: ptr<@type0>, %3 in: ptr<const @type0>, %4 shift: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 th: u64 [storage=automatic];
// DEFAULT-NEXT:         let %6 tl: u64 [storage=automatic];
// DEFAULT-NEXT:         let %7 oh: u64 [storage=automatic];
// DEFAULT-NEXT:         let %8 ol: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%5, or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%3)))), const<i32>(3))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%3)))), const<i32>(2)))))));
// DEFAULT-NEXT:         write<u64>(%6, or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%3)))), const<i32>(1))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%3)))), const<i32>(0)))))));
// DEFAULT-NEXT:         write<u64>(%7, shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%5), mul<i32, overflow=ub>(read<i32>(%4), const<i32>(8))));
// DEFAULT-NEXT:         write<u64>(%8, shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%6), mul<i32, overflow=ub>(read<i32>(%4), const<i32>(8))));
// DEFAULT-NEXT:         let %28: u64 [synthetic] = read<u64>(%8);
// DEFAULT-NEXT:         let %29: u64 [synthetic] = or<u64>(read<u64>(%28), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%5), sub<i32, overflow=ub>(const<i32>(64), mul<i32, overflow=ub>(read<i32>(%4), const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%8, read<u64>(%29));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(1))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%8), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(0))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%8)));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(3))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%7), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(2))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar(%10 out: ptr<@type0>, %11 in: ptr<const @type0>, %12 shift: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 th: u64 [storage=automatic];
// DEFAULT-NEXT:         let %14 tl: u64 [storage=automatic];
// DEFAULT-NEXT:         let %15 oh: u64 [storage=automatic];
// DEFAULT-NEXT:         let %16 ol: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%13, or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(3))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(2)))))));
// DEFAULT-NEXT:         write<u64>(%14, or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(1))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type0>>(%11)))), const<i32>(0)))))));
// DEFAULT-NEXT:         write<u64>(%15, shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%13), mul<i32, overflow=ub>(read<i32>(%12), const<i32>(8))));
// DEFAULT-NEXT:         write<u64>(%16, shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%14), mul<i32, overflow=ub>(read<i32>(%12), const<i32>(8))));
// DEFAULT-NEXT:         let %30: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:         let %31: u64 [synthetic] = or<u64>(read<u64>(%30), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%14), sub<i32, overflow=ub>(const<i32>(64), mul<i32, overflow=ub>(read<i32>(%12), const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%15, read<u64>(%31));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%10)))), const<i32>(1))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%16), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%10)))), const<i32>(0))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%16)));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%10)))), const<i32>(3))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%15), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%10)))), const<i32>(2))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @baz(%18 r: ptr<@type0>, %19 a: ptr<@type0>, %20 b: ptr<@type0>, %21 c: ptr<@type0>, %22 d: ptr<@type0>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %24 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<const @type0>, i32) -> void>(%9, addr_of<ptr<@type0>>(%23), pointer_cast<ptr<const @type0>, reason=arg>(read<ptr<@type0>>(%19)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<const @type0>, i32) -> void>(%1, addr_of<ptr<@type0>>(%24), pointer_cast<ptr<const @type0>, reason=arg>(read<ptr<@type0>>(%21)), const<i32>(1));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%18)))), const<i32>(0))), xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%19)))), const<i32>(0)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%23)), const<i32>(0))))), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%20)))), const<i32>(0)))), const<i32>(11)), const<u32>(3758096367))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%24)), const<i32>(0))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%22)))), const<i32>(0)))), const<i32>(18))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%18)))), const<i32>(1))), xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%19)))), const<i32>(1)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%23)), const<i32>(1))))), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%20)))), const<i32>(1)))), const<i32>(11)), const<u32>(3724462975))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%24)), const<i32>(1))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%22)))), const<i32>(1)))), const<i32>(18))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%18)))), const<i32>(2))), xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%19)))), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%23)), const<i32>(2))))), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%20)))), const<i32>(2)))), const<i32>(11)), const<u32>(3220897791))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%24)), const<i32>(2))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%22)))), const<i32>(2)))), const<i32>(18))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%18)))), const<i32>(3))), xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%19)))), const<i32>(3)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%23)), const<i32>(3))))), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%20)))), const<i32>(3)))), const<i32>(11)), const<u32>(3221225462))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%24)), const<i32>(3))))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%22)))), const<i32>(3)))), const<i32>(18))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %26 a: array<@type0, 6> [storage=automatic] [align=16] = aggregate<array<@type0, 6>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1235)), index1 = const<u32>(3159640283), index2 = const<u32>(4062961311), index3 = const<u32>(3954462607))), index1 = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(61024153)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1623097926)), index2 = const<u32>(2731697901), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2057405750)))), index2 = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(363037976)), index1 = const<u32>(3479526625), index2 = const<u32>(4091582868), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1357867518)))), index3 = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = const<u32>(2284276487), index1 = const<u32>(3888728166), index2 = const<u32>(3236601671), index3 = const<u32>(3064478787))), index4 = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = const<u32>(3440181298), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1564997079)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1510669302)), index3 = const<u32>(2930277156))), index5 = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type0>, ptr<@type0>, ptr<@type0>, ptr<@type0>) -> void>(%17, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(5)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(0)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(1)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(2)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(3)))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(4))))), const<i32>(0)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(5))))), const<i32>(0))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(4))))), const<i32>(1)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(5))))), const<i32>(1)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(4))))), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(5))))), const<i32>(2)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(4))))), const<i32>(3)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(6)>(%26), const<i32>(5))))), const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
