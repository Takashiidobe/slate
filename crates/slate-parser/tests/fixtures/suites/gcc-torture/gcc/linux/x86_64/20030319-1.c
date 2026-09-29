// SLATE-FILECHECK-DEFINES DEFAULT

/* PR 10073 */
typedef struct
{
  unsigned short digits[4];
} INT_64;

INT_64 int_64_com (INT_64 a)
{
  a.digits[0] ^= 0xFFFF;
  a.digits[1] ^= 0xFFFF;
  a.digits[2] ^= 0xFFFF;
  a.digits[3] ^= 0xFFFF;
  return a;
}

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 digits: array<u16, 4>;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_INT_64:[0-9]+]] INT_64 = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_int_64_com:[0-9]+]] @int_64_com(%[[VALUE_a:[0-9]+]] a: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%[[VALUE_a]])), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE1]]))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%[[VALUE0]])), read<u16>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%[[VALUE_a]])), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%[[VALUE3]])));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE4]]))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%[[VALUE3]])), read<u16>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%[[VALUE_a]])), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%[[VALUE6]])));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE7]]))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%[[VALUE6]])), read<u16>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%[[VALUE_a]])), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%[[VALUE9]])));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE10]]))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%[[VALUE9]])), read<u16>(%[[VALUE11]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
