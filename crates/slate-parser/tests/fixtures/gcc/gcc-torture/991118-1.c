void abort(void);
void exit(int);

struct tmp {
  long long int pad   : 12;
  long long int field : 52;
};

struct tmp2 {
  long long int field : 52;
  long long int pad   : 12;
};

struct tmp3 {
  long long int pad   : 11;
  long long int field : 53;
};

struct tmp4 {
  long long int field : 53;
  long long int pad   : 11;
};

struct tmp sub(struct tmp tmp) {
  tmp.field ^= 0x0008765412345678LL;
  return tmp;
}

struct tmp2 sub2(struct tmp2 tmp2) {
  tmp2.field ^= 0x0008765412345678LL;
  return tmp2;
}

struct tmp3 sub3(struct tmp3 tmp3) {
  tmp3.field ^= 0x0018765412345678LL;
  return tmp3;
}

struct tmp4 sub4(struct tmp4 tmp4) {
  tmp4.field ^= 0x0018765412345678LL;
  return tmp4;
}

struct tmp  tmp  = {0x123, 0x123456789ABCDLL};
struct tmp2 tmp2 = {0x123456789ABCDLL, 0x123};
struct tmp3 tmp3 = {0x123, 0x1FFFF00000000LL};
struct tmp4 tmp4 = {0x1FFFF00000000LL, 0x123};

int main(void) {

  if (sizeof(long long) != 8)
    exit(0);

  tmp  = sub(tmp);
  tmp2 = sub2(tmp2);

  if (tmp.pad != 0x123 || tmp.field != 0xFFF9551175BDFDB5LL)
    abort();
  if (tmp2.pad != 0x123 || tmp2.field != 0xFFF9551175BDFDB5LL)
    abort();

  tmp3 = sub3(tmp3);
  tmp4 = sub4(tmp4);
  if (tmp3.pad != 0x123 || tmp3.field != 0xFFF989AB12345678LL)
    abort();
  if (tmp4.pad != 0x123 || tmp4.field != 0xFFF989AB12345678LL)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 tmp = struct {
// DEFAULT-NEXT:         field0 pad: i64 : 12;
// DEFAULT-NEXT:         field1 field: i64 : 52;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[Some(0), Some(12)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 tmp2 = struct {
// DEFAULT-NEXT:         field0 field: i64 : 52;
// DEFAULT-NEXT:         field1 pad: i64 : 12;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 6], bit_offsets=[Some(0), Some(52)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type2 tmp3 = struct {
// DEFAULT-NEXT:         field0 pad: i64 : 11;
// DEFAULT-NEXT:         field1 field: i64 : 53;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[Some(0), Some(11)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type3 tmp4 = struct {
// DEFAULT-NEXT:         field0 field: i64 : 53;
// DEFAULT-NEXT:         field1 pad: i64 : 11;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 6], bit_offsets=[Some(0), Some(53)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %14 tmp: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(291)), field1 = const<i64>(320255973501901)) [linkage=external];
// DEFAULT-NEXT:     global %15 tmp2: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i64>(320255973501901), field1 = widen<i64, reason=assign>(const<i32>(291))) [linkage=external];
// DEFAULT-NEXT:     global %16 tmp3: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(291)), field1 = const<i64>(562945658454016)) [linkage=external];
// DEFAULT-NEXT:     global %17 tmp4: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i64>(562945658454016), field1 = widen<i64, reason=assign>(const<i32>(291))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%19 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @sub(%7 tmp: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20: i64 [synthetic] = read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%7));
// DEFAULT-NEXT:         let %21: i64 [synthetic] = xor<i64>(read<i64>(%20), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%7), read<i64>(%21));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @sub2(%9 tmp2: @type1) -> @type1 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22: i64 [synthetic] = read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%9));
// DEFAULT-NEXT:         let %23: i64 [synthetic] = xor<i64>(read<i64>(%22), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%9), read<i64>(%23));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @sub3(%11 tmp3: @type2) -> @type2 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24: i64 [synthetic] = read<i64>(bitfield1<unit=0, bytes=0..8, bits=11..64>(%11));
// DEFAULT-NEXT:         let %25: i64 [synthetic] = xor<i64>(read<i64>(%24), const<i64>(6885502895806072));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=0, bytes=0..8, bits=11..64>(%11), read<i64>(%25));
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @sub4(%13 tmp4: @type3) -> @type3 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26: i64 [synthetic] = read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..53>(%13));
// DEFAULT-NEXT:         let %27: i64 [synthetic] = xor<i64>(read<i64>(%26), const<i64>(6885502895806072));
// DEFAULT-NEXT:         write<i64>(bitfield0<unit=0, bytes=0..8, bits=0..53>(%13), read<i64>(%27));
// DEFAULT-NEXT:         return copy<@type3, reason=return>(read<@type3>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:         write<@type0>(%14, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%6, copy<@type0, reason=arg>(read<@type0>(%14)))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%6, copy<@type0, reason=arg>(read<@type0>(%14))));
// DEFAULT-NEXT:         write<@type1>(%15, copy<@type1, reason=assign>(call<@type1, signature=fn(@type1) -> @type1, abi=sysv64(native_c) -> native_c>(%8, copy<@type1, reason=arg>(read<@type1>(%15)))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(@type1) -> @type1, abi=sysv64(native_c) -> native_c>(%8, copy<@type1, reason=arg>(read<@type1>(%15))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%14))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%14))), const<u64>(18444867282350767541)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=52..64>(%15))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%15))), const<u64>(18444867282350767541)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<@type2>(%16, copy<@type2, reason=assign>(call<@type2, signature=fn(@type2) -> @type2, abi=sysv64(native_c) -> native_c>(%10, copy<@type2, reason=arg>(read<@type2>(%16)))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(@type2) -> @type2, abi=sysv64(native_c) -> native_c>(%10, copy<@type2, reason=arg>(read<@type2>(%16))));
// DEFAULT-NEXT:         write<@type3>(%17, copy<@type3, reason=assign>(call<@type3, signature=fn(@type3) -> @type3, abi=sysv64(native_c) -> native_c>(%12, copy<@type3, reason=arg>(read<@type3>(%17)))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(@type3) -> @type3, abi=sysv64(native_c) -> native_c>(%12, copy<@type3, reason=arg>(read<@type3>(%17))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..11>(%16))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=11..64>(%16))), const<u64>(18444925116710409848)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=53..64>(%17))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..53>(%17))), const<u64>(18444925116710409848)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
