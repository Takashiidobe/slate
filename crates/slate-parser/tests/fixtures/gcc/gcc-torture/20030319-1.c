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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 digits: array<u16, 4>;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type1 INT_64 = @type0;
// DEFAULT-NEXT:     fn %2 @int_64_com(%3 a: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%3)), const<i32>(0));
// DEFAULT-NEXT:         let %5: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%4)));
// DEFAULT-NEXT:         let %6: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%4)), read<u16>(%6));
// DEFAULT-NEXT:         let %7: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%3)), const<i32>(1));
// DEFAULT-NEXT:         let %8: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%7)));
// DEFAULT-NEXT:         let %9: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%8))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%7)), read<u16>(%9));
// DEFAULT-NEXT:         let %10: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%3)), const<i32>(2));
// DEFAULT-NEXT:         let %11: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%10)));
// DEFAULT-NEXT:         let %12: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%11))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%10)), read<u16>(%12));
// DEFAULT-NEXT:         let %13: ptr<u16> [synthetic] = ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(4)>(field0(%3)), const<i32>(3));
// DEFAULT-NEXT:         let %14: u16 [synthetic] = read<u16>(deref(read<ptr<u16>>(%13)));
// DEFAULT-NEXT:         let %15: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%14))), const<i32>(65535))));
// DEFAULT-NEXT:         write<u16>(deref(read<ptr<u16>>(%13)), read<u16>(%15));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
