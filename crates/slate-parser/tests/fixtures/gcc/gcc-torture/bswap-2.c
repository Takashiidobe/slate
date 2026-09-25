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
// DEFAULT-NEXT:     type @type0 uint32_t = u32;
// DEFAULT-NEXT:     type @type1 bitfield = struct {
// DEFAULT-NEXT:         field0 f0: u8 : 7;
// DEFAULT-NEXT:         field1 <anonymous>: u8 : 1;
// DEFAULT-NEXT:         field2 f1: u8 : 7;
// DEFAULT-NEXT:         field3 <anonymous>: u8 : 1;
// DEFAULT-NEXT:         field4 f2: u8 : 7;
// DEFAULT-NEXT:         field5 <anonymous>: u8 : 1;
// DEFAULT-NEXT:         field6 f3: u8 : 7;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 0, 1, 1, 2, 2, 3], bit_offsets=[Some(0), Some(7), Some(8), Some(15), Some(16), Some(23), Some(24)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type2 ok = struct {
// DEFAULT-NEXT:         field0 f0: u8;
// DEFAULT-NEXT:         field1 f1: u8;
// DEFAULT-NEXT:         field2 f2: u8;
// DEFAULT-NEXT:         field3 f3: u8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type3 bf_or_uint32 = union {
// DEFAULT-NEXT:         field0 inval: @type2;
// DEFAULT-NEXT:         field1 bfval: @type1;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %4 @partial_read_le32(%5 in: @type3) -> u32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..4, bits=0..7>(field1(%5))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield2<unit=0, bytes=0..4, bits=8..15>(field1(%5))))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield4<unit=0, bytes=0..4, bits=16..23>(field1(%5))))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield6<unit=0, bytes=0..4, bits=24..31>(field1(%5))))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @partial_read_be32(%7 in: @type3) -> u32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield6<unit=0, bytes=0..4, bits=24..31>(field1(%7))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield4<unit=0, bytes=0..4, bits=16..23>(field1(%7))))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield2<unit=0, bytes=0..4, bits=8..15>(field1(%7))))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..4, bits=0..7>(field1(%7))))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fake_read_le32(%9 x: ptr<i8>, %10 y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %12 c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %13 c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %14 c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%11, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%12, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(1))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%10)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u8>(%13, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%14, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(3))))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%13))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @fake_read_be32(%16 x: ptr<i8>, %17 y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %19 c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %20 c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %21 c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%18, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%19, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(1))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%17)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u8>(%20, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%21, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(3))))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%21))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%20))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%19))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%18))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @incorrect_read_le32(%23 x: ptr<i8>, %24 y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %26 c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %27 c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %28 c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%25, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%26, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), const<i32>(1))))));
// DEFAULT-NEXT:         write<u8>(%27, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%28, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%23), const<i32>(3))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%24)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%25))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%26))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%28))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @incorrect_read_be32(%30 x: ptr<i8>, %31 y: ptr<i8>) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %32 c0: u8 [storage=automatic];
// DEFAULT-NEXT:         let %33 c1: u8 [storage=automatic];
// DEFAULT-NEXT:         let %34 c2: u8 [storage=automatic];
// DEFAULT-NEXT:         let %35 c3: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%32, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%30), const<i32>(0))))));
// DEFAULT-NEXT:         write<u8>(%33, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%30), const<i32>(1))))));
// DEFAULT-NEXT:         write<u8>(%34, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%30), const<i32>(2))))));
// DEFAULT-NEXT:         write<u8>(%35, reinterpret<u8, reason=assign, fits=unknown>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%30), const<i32>(3))))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%31)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(or<i32>(or<i32>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%35))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%34))), const<i32>(8))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%33))), const<i32>(16))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%32))), const<i32>(24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %37 bfin: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %38 out: u32 [storage=automatic];
// DEFAULT-NEXT:         let %39 cin: array<i8, 4> [storage=automatic] = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=unknown>(const<i32>(131)), index1 = truncate<i8, reason=assign, fits=unknown>(const<i32>(133)), index2 = truncate<i8, reason=assign, fits=unknown>(const<i32>(135)), index3 = truncate<i8, reason=assign, fits=unknown>(const<i32>(137)));
// DEFAULT-NEXT:         if ne<u64>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<@type2>(field0(%37), copy<@type2, reason=assign>(read<@type2>(compound_literal %40 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(133))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(135))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(137)))))));
// DEFAULT-NEXT:         write<u32>(%38, call<u32, signature=fn(@type3) -> u32, abi=sysv64(native_c) -> scalar>(%4, copy<@type3, reason=arg>(read<@type3>(%37))));
// DEFAULT-NEXT:         call<u32, signature=fn(@type3) -> u32, abi=sysv64(native_c) -> scalar>(%4, copy<@type3, reason=arg>(read<@type3>(%37)));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%38), const<u32>(2307360131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<@type2>(field0(%37), copy<@type2, reason=assign>(read<@type2>(compound_literal %41 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(133))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(135))), field3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(137)))))));
// DEFAULT-NEXT:         write<u32>(%38, call<u32, signature=fn(@type3) -> u32, abi=sysv64(native_c) -> scalar>(%6, copy<@type3, reason=arg>(read<@type3>(%37))));
// DEFAULT-NEXT:         call<u32, signature=fn(@type3) -> u32, abi=sysv64(native_c) -> scalar>(%6, copy<@type3, reason=arg>(read<@type3>(%37)));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%38), const<u32>(2206566281))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u32>(%38, call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%8, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))))));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%8, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%38), const<u32>(2298578307))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))), truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<u32>(%38, call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%15, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))))));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%15, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%38), const<u32>(2206531977))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))), truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<u32>(%38, call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%22, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))))));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%22, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%38), const<u32>(2307360131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))), truncate<i8, reason=assign, fits=unknown>(const<i32>(135)));
// DEFAULT-NEXT:         write<u32>(%38, call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%29, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2))))));
// DEFAULT-NEXT:         call<u32, signature=fn(ptr<i8>, ptr<i8>) -> u32>(%29, array_decay<ptr<i8>, length=Some(4)>(%39), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%39), const<i32>(2)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%38), const<u32>(2206566281))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
