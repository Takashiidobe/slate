/* { dg-require-effective-target int32plus } */

#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#else
typedef __UINT32_TYPE__ unsigned;
#endif

struct bitfield {
  unsigned char f0 : 7;
  unsigned char    : 1;
  unsigned char f1 : 7;
  unsigned char    : 1;
  unsigned char f2 : 7;
  unsigned char    : 1;
  unsigned char f3 : 7;
};

struct ok {
  unsigned char f0;
  unsigned char f1;
  unsigned char f2;
  unsigned char f3;
};

union bf_or_uint32 {
  struct ok       inval;
  struct bitfield bfval;
};

__attribute__((noinline, noclone)) uint32_t
partial_read_le32(union bf_or_uint32 in) {
  return in.bfval.f0 | (in.bfval.f1 << 8) | (in.bfval.f2 << 16) |
         (in.bfval.f3 << 24);
}

__attribute__((noinline, noclone)) uint32_t
partial_read_be32(union bf_or_uint32 in) {
  return in.bfval.f3 | (in.bfval.f2 << 8) | (in.bfval.f1 << 16) |
         (in.bfval.f0 << 24);
}

__attribute__((noinline, noclone)) uint32_t fake_read_le32(char *x, char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  *y = 1;
  c2 = x[2];
  c3 = x[3];
  return c0 | c1 << 8 | c2 << 16 | c3 << 24;
}

__attribute__((noinline, noclone)) uint32_t fake_read_be32(char *x, char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  *y = 1;
  c2 = x[2];
  c3 = x[3];
  return c3 | c2 << 8 | c1 << 16 | c0 << 24;
}

__attribute__((noinline, noclone)) uint32_t incorrect_read_le32(char *x,
                                                                char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  c2 = x[2];
  c3 = x[3];
  *y = 1;
  return c0 | c1 << 8 | c2 << 16 | c3 << 24;
}

__attribute__((noinline, noclone)) uint32_t incorrect_read_be32(char *x,
                                                                char *y) {
  unsigned char c0, c1, c2, c3;

  c0 = x[0];
  c1 = x[1];
  c2 = x[2];
  c3 = x[3];
  *y = 1;
  return c3 | c2 << 8 | c1 << 16 | c0 << 24;
}

int main() {
  union bf_or_uint32 bfin;
  uint32_t           out;
  char               cin[] = {0x83, 0x85, 0x87, 0x89};

  if (sizeof(uint32_t) * __CHAR_BIT__ != 32)
    return 0;
  bfin.inval = (struct ok){0x83, 0x85, 0x87, 0x89};
  out        = partial_read_le32(bfin);
  /* Test what bswap would do if its check are not strict enough instead of
     what is the expected result as there is too many possible results with
     bitfields.  */
  if (out == 0x89878583)
    __builtin_abort();
  bfin.inval = (struct ok){0x83, 0x85, 0x87, 0x89};
  out        = partial_read_be32(bfin);
  /* Test what bswap would do if its check are not strict enough instead of
     what is the expected result as there is too many possible results with
     bitfields.  */
  if (out == 0x83858789)
    __builtin_abort();
  out = fake_read_le32(cin, &cin[2]);
  if (out != 0x89018583)
    __builtin_abort();
  cin[2] = 0x87;
  out    = fake_read_be32(cin, &cin[2]);
  if (out != 0x83850189)
    __builtin_abort();
  cin[2] = 0x87;
  out    = incorrect_read_le32(cin, &cin[2]);
  if (out != 0x89878583)
    __builtin_abort();
  cin[2] = 0x87;
  out    = incorrect_read_be32(cin, &cin[2]);
  if (out != 0x83858789)
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
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_bitfield:[0-9]+]] bitfield = struct {
// DEFAULT-NEXT:         field0 f0: u8 : 7;
// DEFAULT-NEXT:         field1 <anonymous>: u8 : 1;
// DEFAULT-NEXT:         field2 f1: u8 : 7;
// DEFAULT-NEXT:         field3 <anonymous>: u8 : 1;
// DEFAULT-NEXT:         field4 f2: u8 : 7;
// DEFAULT-NEXT:         field5 <anonymous>: u8 : 1;
// DEFAULT-NEXT:         field6 f3: u8 : 7;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 0, 1, 1, 2, 2, 3], bit_offsets=[Some(0), Some(7), Some(8), Some(15), Some(16), Some(23), Some(24)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_ok:[0-9]+]] ok = struct {
// DEFAULT-NEXT:         field0 f0: u8;
// DEFAULT-NEXT:         field1 f1: u8;
// DEFAULT-NEXT:         field2 f2: u8;
// DEFAULT-NEXT:         field3 f3: u8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_bf_or_uint32:[0-9]+]] bf_or_uint32 = union {
// DEFAULT-NEXT:         field0 inval: @type[[TYPE_ok]];
// DEFAULT-NEXT:         field1 bfval: @type[[TYPE_bitfield]];
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_partial_read_le32:[0-9]+]] @partial_read_le32(%[[VALUE_in:[0-9]+]] in: @type[[TYPE_bf_or_uint32]]) -> u32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..4, bits=0..7>(field1(%[[VALUE_in]]))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield2<unit=0, bytes=0..4, bits=8..15>(field1(%[[VALUE_in]]))))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield4<unit=0, bytes=0..4, bits=16..23>(field1(%[[VALUE_in]]))))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield6<unit=0, bytes=0..4, bits=24..31>(field1(%[[VALUE_in]]))))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_partial_read_be32:[0-9]+]] @partial_read_be32(%[[VALUE_in_2:[0-9]+]] in: @type[[TYPE_bf_or_uint32]]) -> u32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield6<unit=0, bytes=0..4, bits=24..31>(field1(%[[VALUE_in_2]]))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield4<unit=0, bytes=0..4, bits=16..23>(field1(%[[VALUE_in_2]]))))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield2<unit=0, bytes=0..4, bits=8..15>(field1(%[[VALUE_in_2]]))))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..4, bits=0..7>(field1(%[[VALUE_in_2]]))))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fake_read_le32:[0-9]+]] @fake_read_le32(%[[VALUE_x:[0-9]+]] x: ptr<i8>, %[[VALUE_y:[0-9]+]] y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c0:[0-9]+]] c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c1:[0-9]+]] c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c2:[0-9]+]] c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c3:[0-9]+]] c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c0]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c1]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_y]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c2]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c3]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x]]), const<i32>(3))))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c0]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c1]]))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c2]]))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c3]]))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fake_read_be32:[0-9]+]] @fake_read_be32(%[[VALUE_x_2:[0-9]+]] x: ptr<i8>, %[[VALUE_y_2:[0-9]+]] y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c0_2:[0-9]+]] c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c1_2:[0-9]+]] c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c2_2:[0-9]+]] c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c3_2:[0-9]+]] c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c0_2]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c1_2]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_y_2]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c2_2]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c3_2]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_2]]), const<i32>(3))))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c3_2]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c2_2]]))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c1_2]]))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c0_2]]))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_incorrect_read_le32:[0-9]+]] @incorrect_read_le32(%[[VALUE_x_3:[0-9]+]] x: ptr<i8>, %[[VALUE_y_3:[0-9]+]] y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c0_3:[0-9]+]] c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c1_3:[0-9]+]] c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c2_3:[0-9]+]] c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c3_3:[0-9]+]] c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c0_3]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_3]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c1_3]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_3]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c2_3]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_3]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c3_3]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_3]]), const<i32>(3))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_y_3]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c0_3]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c1_3]]))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c2_3]]))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c3_3]]))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_incorrect_read_be32:[0-9]+]] @incorrect_read_be32(%[[VALUE_x_4:[0-9]+]] x: ptr<i8>, %[[VALUE_y_4:[0-9]+]] y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c0_4:[0-9]+]] c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c1_4:[0-9]+]] c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c2_4:[0-9]+]] c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c3_4:[0-9]+]] c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c0_4]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_4]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c1_4]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_4]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c2_4]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_4]]), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_c3_4]], reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_x_4]]), const<i32>(3))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_y_4]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c3_4]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c2_4]]))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c1_4]]))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c0_4]]))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_bfin:[0-9]+]] bfin: @type[[TYPE_bf_or_uint32]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_out:[0-9]+]] out: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cin:[0-9]+]] cin: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=unknown>(const<i32>(131)), index1 = truncate<i8, reason=assign, fits=unknown>(const<i32>(133)), index2 = truncate<i8, reason=assign, fits=unknown>(const<i32>(135)), index3 = truncate<i8, reason=assign, fits=unknown>(const<i32>(137)));
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<@type[[TYPE_ok]]>(field0(%[[VALUE_bfin]]), copy<@type[[TYPE_ok]], reason=assign>(read<@type[[TYPE_ok]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_ok]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(133))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(135))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(137)))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_out]], call<u32, signature=fn(@type[[TYPE_bf_or_uint32]]) -> u32, abi=sysv64(native_c) -> scalar>(%[[VALUE_partial_read_le32]], copy<@type[[TYPE_bf_or_uint32]], reason=arg>(read<@type[[TYPE_bf_or_uint32]]>(%[[VALUE_bfin]]))));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_out]]), const<u32>(2307360131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_ok]]>(field0(%[[VALUE_bfin]]), copy<@type[[TYPE_ok]], reason=assign>(read<@type[[TYPE_ok]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_ok]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(133))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(135))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(137)))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_out]], call<u32, signature=fn(@type[[TYPE_bf_or_uint32]]) -> u32, abi=sysv64(native_c) -> scalar>(%[[VALUE_partial_read_be32]], copy<@type[[TYPE_bf_or_uint32]], reason=arg>(read<@type[[TYPE_bf_or_uint32]]>(%[[VALUE_bfin]]))));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_out]]), const<u32>(2206566281))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_out]], call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%[[VALUE_fake_read_le32]], array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_out]]), const<u32>(2298578307))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))), truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_out]], call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%[[VALUE_fake_read_be32]], array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_out]]), const<u32>(2206531977))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))), truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_out]], call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%[[VALUE_incorrect_read_le32]], array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_out]]), const<u32>(2307360131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))), truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_out]], call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%[[VALUE_incorrect_read_be32]], array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_cin]]), const<i32>(2))))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_out]]), const<u32>(2206566281))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
