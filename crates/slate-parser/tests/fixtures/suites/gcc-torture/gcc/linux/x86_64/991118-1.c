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
// DEFAULT-NEXT:     type @type[[TYPE_tmp:[0-9]+]] tmp = struct {
// DEFAULT-NEXT:         field0 pad: i64 : 12;
// DEFAULT-NEXT:         field1 field: i64 : 52;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[Some(0), Some(12)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_tmp2:[0-9]+]] tmp2 = struct {
// DEFAULT-NEXT:         field0 field: i64 : 52;
// DEFAULT-NEXT:         field1 pad: i64 : 12;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 6], bit_offsets=[Some(0), Some(52)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_tmp3:[0-9]+]] tmp3 = struct {
// DEFAULT-NEXT:         field0 pad: i64 : 11;
// DEFAULT-NEXT:         field1 field: i64 : 53;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[Some(0), Some(11)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_tmp4:[0-9]+]] tmp4 = struct {
// DEFAULT-NEXT:         field0 field: i64 : 53;
// DEFAULT-NEXT:         field1 pad: i64 : 11;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 6], bit_offsets=[Some(0), Some(53)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_tmp:[0-9]+]] tmp: @type[[TYPE_tmp]] [storage=static] = aggregate<@type[[TYPE_tmp]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(291)), field1 = const<i64>(320255973501901)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tmp2:[0-9]+]] tmp2: @type[[TYPE_tmp2]] [storage=static] = aggregate<@type[[TYPE_tmp2]], zero_fill=false>(field0 = const<i64>(320255973501901), field1 = widen<i64, reason=assign>(const<i32>(291))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tmp3:[0-9]+]] tmp3: @type[[TYPE_tmp3]] [storage=static] = aggregate<@type[[TYPE_tmp3]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(291)), field1 = const<i64>(562945658454016)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tmp4:[0-9]+]] tmp4: @type[[TYPE_tmp4]] [storage=static] = aggregate<@type[[TYPE_tmp4]], zero_fill=false>(field0 = const<i64>(562945658454016), field1 = widen<i64, reason=assign>(const<i32>(291))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_sub:[0-9]+]] @sub(%[[VALUE_tmp_2:[0-9]+]] tmp: @type[[TYPE_tmp]]) -> @type[[TYPE_tmp]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%[[VALUE_tmp_2]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = xor<i64>(read<i64>(%[[VALUE1]]), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%[[VALUE_tmp_2]]), read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_tmp]], reason=return>(read<@type[[TYPE_tmp]]>(%[[VALUE_tmp_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub2:[0-9]+]] @sub2(%[[VALUE_tmp2_2:[0-9]+]] tmp2: @type[[TYPE_tmp2]]) -> @type[[TYPE_tmp2]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%[[VALUE_tmp2_2]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = xor<i64>(read<i64>(%[[VALUE3]]), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%[[VALUE_tmp2_2]]), read<i64>(%[[VALUE4]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_tmp2]], reason=return>(read<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub3:[0-9]+]] @sub3(%[[VALUE_tmp3_2:[0-9]+]] tmp3: @type[[TYPE_tmp3]]) -> @type[[TYPE_tmp3]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i64 [synthetic] = read<i64>(bitfield1<unit=0, bytes=0..8, bits=11..64>(%[[VALUE_tmp3_2]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i64 [synthetic] = xor<i64>(read<i64>(%[[VALUE5]]), const<i64>(6885502895806072));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=0, bytes=0..8, bits=11..64>(%[[VALUE_tmp3_2]]), read<i64>(%[[VALUE6]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_tmp3]], reason=return>(read<@type[[TYPE_tmp3]]>(%[[VALUE_tmp3_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub4:[0-9]+]] @sub4(%[[VALUE_tmp4_2:[0-9]+]] tmp4: @type[[TYPE_tmp4]]) -> @type[[TYPE_tmp4]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i64 [synthetic] = read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..53>(%[[VALUE_tmp4_2]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i64 [synthetic] = xor<i64>(read<i64>(%[[VALUE7]]), const<i64>(6885502895806072));
// DEFAULT-NEXT:         write<i64>(bitfield0<unit=0, bytes=0..8, bits=0..53>(%[[VALUE_tmp4_2]]), read<i64>(%[[VALUE8]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_tmp4]], reason=return>(read<@type[[TYPE_tmp4]]>(%[[VALUE_tmp4_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_tmp]]>(%[[VALUE_tmp]], copy<@type[[TYPE_tmp]], reason=assign>(call<@type[[TYPE_tmp]], signature=fn(@type[[TYPE_tmp]]) -> @type[[TYPE_tmp]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub]], copy<@type[[TYPE_tmp]], reason=arg>(read<@type[[TYPE_tmp]]>(%[[VALUE_tmp]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE_tmp]], reason=assign>(call<@type[[TYPE_tmp]], signature=fn(@type[[TYPE_tmp]]) -> @type[[TYPE_tmp]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub]], copy<@type[[TYPE_tmp]], reason=arg>(read<@type[[TYPE_tmp]]>(%[[VALUE_tmp]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2]], copy<@type[[TYPE_tmp2]], reason=assign>(call<@type[[TYPE_tmp2]], signature=fn(@type[[TYPE_tmp2]]) -> @type[[TYPE_tmp2]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub2]], copy<@type[[TYPE_tmp2]], reason=arg>(read<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE_tmp2]], reason=assign>(call<@type[[TYPE_tmp2]], signature=fn(@type[[TYPE_tmp2]]) -> @type[[TYPE_tmp2]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub2]], copy<@type[[TYPE_tmp2]], reason=arg>(read<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2]]))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_tmp]]))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%[[VALUE_tmp]]))), const<u64>(18444867282350767541)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=52..64>(%[[VALUE_tmp2]]))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%[[VALUE_tmp2]]))), const<u64>(18444867282350767541)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_tmp3]]>(%[[VALUE_tmp3]], copy<@type[[TYPE_tmp3]], reason=assign>(call<@type[[TYPE_tmp3]], signature=fn(@type[[TYPE_tmp3]]) -> @type[[TYPE_tmp3]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub3]], copy<@type[[TYPE_tmp3]], reason=arg>(read<@type[[TYPE_tmp3]]>(%[[VALUE_tmp3]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE_tmp3]], reason=assign>(call<@type[[TYPE_tmp3]], signature=fn(@type[[TYPE_tmp3]]) -> @type[[TYPE_tmp3]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub3]], copy<@type[[TYPE_tmp3]], reason=arg>(read<@type[[TYPE_tmp3]]>(%[[VALUE_tmp3]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_tmp4]]>(%[[VALUE_tmp4]], copy<@type[[TYPE_tmp4]], reason=assign>(call<@type[[TYPE_tmp4]], signature=fn(@type[[TYPE_tmp4]]) -> @type[[TYPE_tmp4]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub4]], copy<@type[[TYPE_tmp4]], reason=arg>(read<@type[[TYPE_tmp4]]>(%[[VALUE_tmp4]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE_tmp4]], reason=assign>(call<@type[[TYPE_tmp4]], signature=fn(@type[[TYPE_tmp4]]) -> @type[[TYPE_tmp4]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub4]], copy<@type[[TYPE_tmp4]], reason=arg>(read<@type[[TYPE_tmp4]]>(%[[VALUE_tmp4]]))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..11>(%[[VALUE_tmp3]]))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=11..64>(%[[VALUE_tmp3]]))), const<u64>(18444925116710409848)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=53..64>(%[[VALUE_tmp4]]))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..53>(%[[VALUE_tmp4]]))), const<u64>(18444925116710409848)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
