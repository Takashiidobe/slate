/* { dg-xfail-if "ABI specifies bitfields cannot exceed 32 bits" { mcore-*-* } }
 */

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

struct tmp sub(struct tmp tmp) {
  tmp.field |= 0x0008765412345678LL;
  return tmp;
}

struct tmp2 sub2(struct tmp2 tmp2) {
  tmp2.field |= 0x0008765412345678LL;
  return tmp2;
}

int main(void) {
  struct tmp  tmp  = {0x123, 0xFFF000FFF000FLL};
  struct tmp2 tmp2 = {0xFFF000FFF000FLL, 0x123};

  tmp  = sub(tmp);
  tmp2 = sub2(tmp2);

  if (tmp.pad != 0x123 || tmp.field != 0xFFFFFF541FFF567FLL)
    abort();
  if (tmp2.pad != 0x123 || tmp2.field != 0xFFFFFF541FFF567FLL)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_sub:[0-9]+]] @sub(%[[VALUE_tmp:[0-9]+]] tmp: @type[[TYPE_tmp]]) -> @type[[TYPE_tmp]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%[[VALUE_tmp]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = or<i64>(read<i64>(%[[VALUE1]]), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%[[VALUE_tmp]]), read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_tmp]], reason=return>(read<@type[[TYPE_tmp]]>(%[[VALUE_tmp]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sub2:[0-9]+]] @sub2(%[[VALUE_tmp2:[0-9]+]] tmp2: @type[[TYPE_tmp2]]) -> @type[[TYPE_tmp2]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%[[VALUE_tmp2]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = or<i64>(read<i64>(%[[VALUE3]]), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%[[VALUE_tmp2]]), read<i64>(%[[VALUE4]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_tmp2]], reason=return>(read<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_tmp_2:[0-9]+]] tmp: @type[[TYPE_tmp]] [storage=automatic] = aggregate<@type[[TYPE_tmp]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(291)), field1 = const<i64>(4502500384112655));
// DEFAULT-NEXT:         let %[[VALUE_tmp2_2:[0-9]+]] tmp2: @type[[TYPE_tmp2]] [storage=automatic] = aggregate<@type[[TYPE_tmp2]], zero_fill=false>(field0 = const<i64>(4502500384112655), field1 = widen<i64, reason=assign>(const<i32>(291)));
// DEFAULT-NEXT:         write<@type[[TYPE_tmp]]>(%[[VALUE_tmp_2]], copy<@type[[TYPE_tmp]], reason=assign>(call<@type[[TYPE_tmp]], signature=fn(@type[[TYPE_tmp]]) -> @type[[TYPE_tmp]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub]], copy<@type[[TYPE_tmp]], reason=arg>(read<@type[[TYPE_tmp]]>(%[[VALUE_tmp_2]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE_tmp]], reason=assign>(call<@type[[TYPE_tmp]], signature=fn(@type[[TYPE_tmp]]) -> @type[[TYPE_tmp]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub]], copy<@type[[TYPE_tmp]], reason=arg>(read<@type[[TYPE_tmp]]>(%[[VALUE_tmp_2]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2_2]], copy<@type[[TYPE_tmp2]], reason=assign>(call<@type[[TYPE_tmp2]], signature=fn(@type[[TYPE_tmp2]]) -> @type[[TYPE_tmp2]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub2]], copy<@type[[TYPE_tmp2]], reason=arg>(read<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2_2]])))));
// DEFAULT-NEXT:         copy<@type[[TYPE_tmp2]], reason=assign>(call<@type[[TYPE_tmp2]], signature=fn(@type[[TYPE_tmp2]]) -> @type[[TYPE_tmp2]], abi=sysv64(native_c) -> native_c>(%[[VALUE_sub2]], copy<@type[[TYPE_tmp2]], reason=arg>(read<@type[[TYPE_tmp2]]>(%[[VALUE_tmp2_2]]))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_tmp_2]]))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%[[VALUE_tmp_2]]))), const<u64>(18446743335512004223)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=52..64>(%[[VALUE_tmp2_2]]))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%[[VALUE_tmp2_2]]))), const<u64>(18446743335512004223)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
