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
// DEFAULT-NEXT:     type @type0 tmp = struct {
// DEFAULT-NEXT:         field0 pad: i64 : 12;
// DEFAULT-NEXT:         field1 field: i64 : 52;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[Some(0), Some(12)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 tmp2 = struct {
// DEFAULT-NEXT:         field0 field: i64 : 52;
// DEFAULT-NEXT:         field1 pad: i64 : 12;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 6], bit_offsets=[Some(0), Some(52)], bit_units=[(0, 8)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @sub(%5 tmp: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12: i64 [synthetic] = read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%5));
// DEFAULT-NEXT:         let %13: i64 [synthetic] = or<i64>(read<i64>(%12), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%5), read<i64>(%13));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @sub2(%7 tmp2: @type1) -> @type1 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14: i64 [synthetic] = read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%7));
// DEFAULT-NEXT:         let %15: i64 [synthetic] = or<i64>(read<i64>(%14), const<i64>(2381903268435576));
// DEFAULT-NEXT:         write<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%7), read<i64>(%15));
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 tmp: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(291)), field1 = const<i64>(4502500384112655));
// DEFAULT-NEXT:         let %10 tmp2: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i64>(4502500384112655), field1 = widen<i64, reason=assign>(const<i32>(291)));
// DEFAULT-NEXT:         write<@type0>(%9, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%4, copy<@type0, reason=arg>(read<@type0>(%9)))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%4, copy<@type0, reason=arg>(read<@type0>(%9))));
// DEFAULT-NEXT:         write<@type1>(%10, copy<@type1, reason=assign>(call<@type1, signature=fn(@type1) -> @type1, abi=sysv64(native_c) -> native_c>(%6, copy<@type1, reason=arg>(read<@type1>(%10)))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(@type1) -> @type1, abi=sysv64(native_c) -> native_c>(%6, copy<@type1, reason=arg>(read<@type1>(%10))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%9))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=12..64>(%9))), const<u64>(18446743335512004223)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(truncate<i32, reason=promotion, fits=unknown>(read<i64>(bitfield1<unit=0, bytes=0..8, bits=52..64>(%10))), const<i32>(291)), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(bitfield0<unit=0, bytes=0..8, bits=0..52>(%10))), const<u64>(18446743335512004223)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
