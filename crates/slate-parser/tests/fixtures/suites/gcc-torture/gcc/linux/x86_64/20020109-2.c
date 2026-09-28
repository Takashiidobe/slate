// SLATE-FILECHECK-DEFINES DEFAULT

typedef union
{
  unsigned char member3;
  signed short member4;
  unsigned int member5;
}
UNI02;

struct srt_dat_t
{
  UNI02 un2;
  unsigned long member1;
  signed short member2;
};

struct srt_dat_t exsrt1;
void
extern_test (struct srt_dat_t arg1)
{
  arg1.un2.member3++;
  arg1.member1++;
  arg1.member2++;
}

int
main (void)
{
  extern_test (exsrt1);
  return (0);
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 member3: u8;
// DEFAULT-NEXT:         field1 member4: i16;
// DEFAULT-NEXT:         field2 member5: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type1 UNI02 = @type0;
// DEFAULT-NEXT:     type @type2 srt_dat_t = struct {
// DEFAULT-NEXT:         field0 un2: @type0;
// DEFAULT-NEXT:         field1 member1: u64;
// DEFAULT-NEXT:         field2 member2: i16;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %3 exsrt1: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @extern_test(%5 arg1: @type2) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7: u8 [synthetic] = read<u8>(field0(field0(%5)));
// DEFAULT-NEXT:         let %8: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(field0(field0(%5)), read<u8>(%8));
// DEFAULT-NEXT:         let %9: u64 [synthetic] = read<u64>(field1(%5));
// DEFAULT-NEXT:         let %10: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%9), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(field1(%5), read<u64>(%10));
// DEFAULT-NEXT:         let %11: i16 [synthetic] = read<i16>(field2(%5));
// DEFAULT-NEXT:         let %12: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%11)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(field2(%5), read<i16>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(@type2) -> void, abi=sysv64(native_c) -> void>(%4, copy<@type2, reason=arg>(read<@type2>(%3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
