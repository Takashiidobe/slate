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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 u: array<u32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_out:[0-9]+]] out: ptr<@type[[TYPE_S]]>, %[[VALUE_in:[0-9]+]] in: ptr<const @type[[TYPE_S]]>, %[[VALUE_shift:[0-9]+]] shift: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_th:[0-9]+]] th: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_tl:[0-9]+]] tl: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_oh:[0-9]+]] oh: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ol:[0-9]+]] ol: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_th]], or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in]])))), const<i32>(3))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in]])))), const<i32>(2)))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_tl]], or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in]])))), const<i32>(1))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in]])))), const<i32>(0)))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_oh]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_th]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_shift]]), const<i32>(8))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ol]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_tl]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_shift]]), const<i32>(8))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_ol]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE0]]), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_th]]), sub<i32, overflow=ub>(const<i32>(64), mul<i32, overflow=ub>(read<i32>(%[[VALUE_shift]]), const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ol]], read<u64>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out]])))), const<i32>(1))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ol]]), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out]])))), const<i32>(0))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_ol]])));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out]])))), const<i32>(3))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_oh]]), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out]])))), const<i32>(2))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_oh]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_out_2:[0-9]+]] out: ptr<@type[[TYPE_S]]>, %[[VALUE_in_2:[0-9]+]] in: ptr<const @type[[TYPE_S]]>, %[[VALUE_shift_2:[0-9]+]] shift: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_th_2:[0-9]+]] th: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_tl_2:[0-9]+]] tl: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_oh_2:[0-9]+]] oh: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ol_2:[0-9]+]] ol: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_th_2]], or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in_2]])))), const<i32>(3))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in_2]])))), const<i32>(2)))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_tl_2]], or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in_2]])))), const<i32>(1))))), const<i32>(32)), widen<u64, reason=usual_arith>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(4)>(field0(deref(read<ptr<const @type[[TYPE_S]]>>(%[[VALUE_in_2]])))), const<i32>(0)))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_oh_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_th_2]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_shift_2]]), const<i32>(8))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ol_2]], shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_tl_2]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_shift_2]]), const<i32>(8))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_oh_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = or<u64>(read<u64>(%[[VALUE2]]), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_tl_2]]), sub<i32, overflow=ub>(const<i32>(64), mul<i32, overflow=ub>(read<i32>(%[[VALUE_shift_2]]), const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_oh_2]], read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out_2]])))), const<i32>(1))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ol_2]]), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out_2]])))), const<i32>(0))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_ol_2]])));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out_2]])))), const<i32>(3))), truncate<u32, reason=assign, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_oh_2]]), const<i32>(32))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_out_2]])))), const<i32>(2))), truncate<u32, reason=assign, fits=unknown>(read<u64>(%[[VALUE_oh_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_r:[0-9]+]] r: ptr<@type[[TYPE_S]]>, %[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_S]]>, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_S]]>, %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_S]]>, %[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_S]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, ptr<const @type[[TYPE_S]]>, i32) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]]), pointer_cast<ptr<const @type[[TYPE_S]]>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]])), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, ptr<const @type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_y]]), pointer_cast<ptr<const @type[[TYPE_S]]>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_c]])), const<i32>(1));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_r]])))), const<i32>(0))),
// DEFAULT-SAME: xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]])))), const<i32>(0)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_x]])), const<i32>(0))))), and<u32>(shr<u32, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]])))), const<i32>(0)))), const<i32>(11)), const<u32>(3758096367))),
// DEFAULT-SAME: read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_y]])), const<i32>(0))))),
// DEFAULT-SAME: shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_d]])))), const<i32>(0)))), const<i32>(18))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_r]])))), const<i32>(1))),
// DEFAULT-SAME: xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]])))), const<i32>(1)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_x]])), const<i32>(1))))), and<u32>(shr<u32, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]])))), const<i32>(1)))), const<i32>(11)), const<u32>(3724462975))),
// DEFAULT-SAME: read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_y]])), const<i32>(1))))),
// DEFAULT-SAME: shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_d]])))), const<i32>(1)))), const<i32>(18))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_r]])))), const<i32>(2))),
// DEFAULT-SAME: xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]])))), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_x]])), const<i32>(2))))), and<u32>(shr<u32, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]])))), const<i32>(2)))), const<i32>(11)), const<u32>(3220897791))),
// DEFAULT-SAME: read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_y]])), const<i32>(2))))),
// DEFAULT-SAME: shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_d]])))), const<i32>(2)))), const<i32>(18))));
// DEFAULT-NEXT:         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_r]])))), const<i32>(3))),
// DEFAULT-SAME: xor<u32>(xor<u32>(xor<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]])))), const<i32>(3)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_x]])), const<i32>(3))))), and<u32>(shr<u32, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]])))), const<i32>(3)))), const<i32>(11)), const<u32>(3221225462))),
// DEFAULT-SAME: read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(%[[VALUE_y]])), const<i32>(3))))),
// DEFAULT-SAME: shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_d]])))), const<i32>(3)))), const<i32>(18))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a:
// DEFAULT-SAME: array<@type[[TYPE_S]], 6> [storage=automatic] [align=16] =
// DEFAULT-SAME: aggregate<array<@type[[TYPE_S]], 6>, zero_fill=false>(index0 =
// DEFAULT-SAME: aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign,
// DEFAULT-SAME: fits=always>(const<i32>(1235)), index1 = const<u32>(3159640283), index2 = const<u32>(4062961311), index3 = const<u32>(3954462607))), index1 =
// DEFAULT-SAME: aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign,
// DEFAULT-SAME: fits=always>(const<i32>(61024153)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1623097926)), index2 = const<u32>(2731697901), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2057405750)))), index2 =
// DEFAULT-SAME: aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign,
// DEFAULT-SAME: fits=always>(const<i32>(363037976)), index1 = const<u32>(3479526625), index2 = const<u32>(4091582868), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1357867518)))), index3 =
// DEFAULT-SAME: aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = const<u32>(2284276487), index1 = const<u32>(3888728166),
// DEFAULT-SAME: index2 = const<u32>(3236601671), index3 = const<u32>(3064478787))), index4 = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<array<u32, 4>,
// DEFAULT-SAME: zero_fill=false>(index0 = const<u32>(3440181298), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1564997079)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1510669302)), index3 = const<u32>(2930277156))), index5 =
// DEFAULT-SAME: aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = aggregate<array<u32, 4>, zero_fill=false>(index0 = reinterpret<u32, reason=assign,
// DEFAULT-SAME: fits=always>(const<i32>(0)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), index3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: ptr<@type[[TYPE_S]]>) ->
// DEFAULT-SAME: void>(%[[VALUE_baz]],
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(5)))),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(0)))),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(1)))),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(2)))),
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(3)))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>,
// DEFAULT-SAME: length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(4))))), const<i32>(0)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(5))))), const<i32>(0))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(4))))), const<i32>(1)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(5))))), const<i32>(1)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(4))))), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(5))))), const<i32>(2)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(4))))), const<i32>(3)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<u32>, length=Some(4)>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_S]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>,
// DEFAULT-SAME: length=Some(6)>(%[[VALUE_a_2]]), const<i32>(5))))), const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
