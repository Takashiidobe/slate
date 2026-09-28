/* { dg-require-effective-target int32plus } */
#define FIELDS2 long long l;
#include "20040629-1.c"

#if 0
// SLATE-FILECHECK-DEFINES DEFAULT
#endif

#if 0
#endif

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
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 15;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 2, 8], bit_offsets=[Some(0), Some(6), Some(17), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 i: u32 : 5;
// DEFAULT-NEXT:         field1 j: u32 : 1;
// DEFAULT-NEXT:         field2 k: u32 : 26;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(5), Some(6), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 i: u32 : 16;
// DEFAULT-NEXT:         field1 j: u32 : 8;
// DEFAULT-NEXT:         field2 k: u32 : 8;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 2, 3, 8], bit_offsets=[Some(0), Some(16), Some(24), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     global %2 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 d: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @ret1() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @ret2() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ret3() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @ret4() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @ret5() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @ret6() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @ret7() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @ret8() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @ret9() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @fn1_1(%17 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %341: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %342: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%341))), read<u32>(%17));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%342));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @fn2_1(%19 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %343: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %344: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%343))), read<u32>(%19));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%344));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @fn3_1(%21 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %345: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %346: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%345))), read<u32>(%21));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%346));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @fn4_1(%23 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %347: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %348: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%347))), read<u32>(%23));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%348));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @fn5_1(%25 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %349: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %350: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%349))), read<u32>(%25));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%350));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @fn6_1(%27 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %351: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %352: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%351))), read<u32>(%27));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%352));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @fn7_1(%29 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %353: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %354: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%353))), read<u32>(%29));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%354));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @fn8_1(%31 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %355: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %356: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%355))), read<u32>(%31));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%356));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @fn9_1(%33 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %357: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %358: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%357))), read<u32>(%33));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%358));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @fn1_2(%35 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %359: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %360: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%359)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%360));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @fn2_2(%37 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %361: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %362: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%361)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%362));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @fn3_2(%39 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %363: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %364: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%363)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%364));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @fn4_2(%41 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %365: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %366: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%365)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%366));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @fn5_2(%43 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %367: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %368: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%367)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%368));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @fn6_2(%45 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %369: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %370: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%369)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%370));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @fn7_2(%47 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %371: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %372: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%371)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%372));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @fn8_2(%49 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %373: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %374: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%373)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%374));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @fn9_2(%51 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %375: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %376: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%375)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%376));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @fn1_3(%53 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %377: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %378: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%377)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%378));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @fn2_3(%55 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %379: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %380: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%379)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%380));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @fn3_3(%57 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %381: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %382: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%381)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%382));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @fn4_3(%59 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %383: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %384: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%383)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%384));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @fn5_3(%61 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %385: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %386: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%385)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%386));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @fn6_3(%63 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %387: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %388: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%387)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%388));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @fn7_3(%65 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %389: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %390: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%389)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%390));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @fn8_3(%67 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %391: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %392: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%391)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%392));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @fn9_3(%69 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %393: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %394: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%393)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%394));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @fn1_4(%71 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %395: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %396: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%395))), read<u32>(%71));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%396));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @fn2_4(%73 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %397: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %398: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%397))), read<u32>(%73));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%398));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @fn3_4(%75 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %399: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %400: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%399))), read<u32>(%75));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%400));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @fn4_4(%77 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %401: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %402: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%401))), read<u32>(%77));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%402));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @fn5_4(%79 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %403: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %404: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%403))), read<u32>(%79));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%404));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @fn6_4(%81 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %405: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %406: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%405))), read<u32>(%81));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%406));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %82 @fn7_4(%83 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %407: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %408: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%407))), read<u32>(%83));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%408));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @fn8_4(%85 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %409: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %410: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%409))), read<u32>(%85));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%410));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @fn9_4(%87 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %411: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %412: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%411))), read<u32>(%87));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%412));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @fn1_5(%89 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %413: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %414: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%413)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%414));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @fn2_5(%91 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %415: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %416: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%415)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%416));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @fn3_5(%93 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %417: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %418: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%417)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%418));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @fn4_5(%95 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %419: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %420: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%419)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%420));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @fn5_5(%97 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %421: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %422: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%421)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%422));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @fn6_5(%99 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %423: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %424: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%423)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%424));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @fn7_5(%101 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %425: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %426: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%425)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%426));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @fn8_5(%103 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %427: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %428: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%427)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%428));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @fn9_5(%105 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %429: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %430: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%429)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%430));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @fn1_6(%107 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %431: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %432: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%431)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%432));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %108 @fn2_6(%109 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %433: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %434: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%433)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%434));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @fn3_6(%111 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %435: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %436: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%435)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%436));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @fn4_6(%113 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %437: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %438: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%437)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%438));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @fn5_6(%115 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %439: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %440: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%439)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%440));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @fn6_6(%117 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %441: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %442: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%441)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%442));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @fn7_6(%119 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %443: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %444: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%443)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%444));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @fn8_6(%121 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %445: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %446: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%445)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%446));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @fn9_6(%123 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %447: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %448: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%447)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%448));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @fn1_7(%125 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %449: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %450: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%449))), read<u32>(%125));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%450));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @fn2_7(%127 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %451: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %452: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%451))), read<u32>(%127));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%452));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @fn3_7(%129 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %453: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %454: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%453))), read<u32>(%129));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%454));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @fn4_7(%131 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %455: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %456: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%455))), read<u32>(%131));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%456));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %132 @fn5_7(%133 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %457: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %458: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%457))), read<u32>(%133));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%458));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @fn6_7(%135 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %459: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %460: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%459))), read<u32>(%135));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%460));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %136 @fn7_7(%137 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %461: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %462: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%461))), read<u32>(%137));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%462));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %138 @fn8_7(%139 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %463: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %464: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%463))), read<u32>(%139));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%464));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @fn9_7(%141 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %465: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %466: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%465))), read<u32>(%141));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%466));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %142 @fn1_8(%143 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %467: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %468: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%467))), read<u32>(%143));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%468));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %144 @fn2_8(%145 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %469: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %470: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%469))), read<u32>(%145));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%470));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @fn3_8(%147 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %471: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %472: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%471))), read<u32>(%147));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%472));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %148 @fn4_8(%149 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %473: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %474: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%473))), read<u32>(%149));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%474));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @fn5_8(%151 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %475: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %476: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%475))), read<u32>(%151));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%476));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @fn6_8(%153 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %477: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %478: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%477))), read<u32>(%153));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%478));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %154 @fn7_8(%155 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %479: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %480: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%479))), read<u32>(%155));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%480));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %156 @fn8_8(%157 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %481: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %482: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%481))), read<u32>(%157));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%482));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @fn9_8(%159 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %483: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %484: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%483))), read<u32>(%159));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%484));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @fn1_9(%161 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %485: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %486: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%485))), read<u32>(%161));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%486));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @fn2_9(%163 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %487: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %488: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%487))), read<u32>(%163));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%488));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @fn3_9(%165 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %489: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %490: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%489))), read<u32>(%165));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%490));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %166 @fn4_9(%167 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %491: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %492: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%491))), read<u32>(%167));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%492));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %168 @fn5_9(%169 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %493: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %494: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%493))), read<u32>(%169));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%494));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @fn6_9(%171 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %495: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %496: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%495))), read<u32>(%171));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%496));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %172 @fn7_9(%173 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %497: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %498: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%497))), read<u32>(%173));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%498));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %174 @fn8_9(%175 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %499: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %500: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%499))), read<u32>(%175));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%500));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @fn9_9(%177 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %501: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %502: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%501))), read<u32>(%177));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%502));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %178 @fn1_a(%179 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %503: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %504: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%503))), read<u32>(%179));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%504));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %180 @fn2_a(%181 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %505: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %506: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%505))), read<u32>(%181));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%506));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @fn3_a(%183 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %507: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %508: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%507))), read<u32>(%183));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%508));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %184 @fn4_a(%185 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %509: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %510: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%509))), read<u32>(%185));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%510));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %186 @fn5_a(%187 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %511: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %512: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%511))), read<u32>(%187));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%512));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %188 @fn6_a(%189 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %513: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %514: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%513))), read<u32>(%189));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%514));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %190 @fn7_a(%191 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %515: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %516: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%515))), read<u32>(%191));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%516));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %192 @fn8_a(%193 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %517: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %518: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%517))), read<u32>(%193));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%518));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %194 @fn9_a(%195 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %519: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %520: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%519))), read<u32>(%195));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%520));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %196 @fn1_b(%197 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %521: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %522: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%521))), read<u32>(%197));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%522));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %198 @fn2_b(%199 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %523: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %524: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%523))), read<u32>(%199));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%524));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %200 @fn3_b(%201 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %525: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %526: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%525))), read<u32>(%201));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%526));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %202 @fn4_b(%203 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %527: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %528: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%527))), read<u32>(%203));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%528));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %204 @fn5_b(%205 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %529: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %530: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%529))), read<u32>(%205));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%530));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %206 @fn6_b(%207 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %531: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %532: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%531))), read<u32>(%207));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%532));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %208 @fn7_b(%209 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %533: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %534: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%533))), read<u32>(%209));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%534));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %210 @fn8_b(%211 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %535: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %536: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%535))), read<u32>(%211));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%536));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %212 @fn9_b(%213 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %537: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %538: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%537))), read<u32>(%213));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%538));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %214 @fn1_c(%215 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %539: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %540: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%539)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%540));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %216 @fn2_c(%217 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %541: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %542: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%541)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%542));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %218 @fn3_c(%219 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %543: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %544: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%543)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%544));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %220 @fn4_c(%221 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %545: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %546: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%545)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%546));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %222 @fn5_c(%223 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %547: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %548: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%547)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%548));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %224 @fn6_c(%225 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %549: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %550: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%549)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%550));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %226 @fn7_c(%227 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %551: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %552: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%551)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%552));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %228 @fn8_c(%229 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %553: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %554: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%553)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%554));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %230 @fn9_c(%231 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %555: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %556: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%555)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%556));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %232 @fn1_d(%233 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %557: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %558: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%557)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%558));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %234 @fn2_d(%235 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %559: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %560: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%559)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%560));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %236 @fn3_d(%237 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %561: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %562: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%561)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%562));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %238 @fn4_d(%239 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %563: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %564: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%563)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%564));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %240 @fn5_d(%241 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %565: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %566: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%565)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%566));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %242 @fn6_d(%243 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %567: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %568: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%567)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%568));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %244 @fn7_d(%245 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %569: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %570: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%569)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%570));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %246 @fn8_d(%247 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %571: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %572: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%571)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%572));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %248 @fn9_d(%249 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %573: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %574: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%573)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%574));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %250 @fn1_e(%251 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %575: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %576: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%575)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%576));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %252 @fn2_e(%253 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %577: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %578: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%577)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%578));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %254 @fn3_e(%255 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %579: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %580: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%579)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%580));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %256 @fn4_e(%257 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %581: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %582: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%581)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%582));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %258 @fn5_e(%259 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %583: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %584: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%583)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%584));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %260 @fn6_e(%261 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %585: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %586: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%585)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%586));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %262 @fn7_e(%263 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %587: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %588: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%587)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%588));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %264 @fn8_e(%265 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %589: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %590: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%589)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%590));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %266 @fn9_e(%267 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %591: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %592: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%591)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%592));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %268 @fn1_f(%269 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %593: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %594: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%593)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%594));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %270 @fn2_f(%271 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %595: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %596: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%595)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%596));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %272 @fn3_f(%273 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %597: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %598: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%597)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%598));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %274 @fn4_f(%275 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %599: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %600: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%599)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%600));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %276 @fn5_f(%277 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %601: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %602: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%601)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%602));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %278 @fn6_f(%279 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %603: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %604: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%603)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%604));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %280 @fn7_f(%281 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %605: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %606: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%605)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%606));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %282 @fn8_f(%283 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %607: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %608: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%607)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%608));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %284 @fn9_f(%285 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %609: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %610: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%609)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%610));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %286 @fn1_g(%287 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %611: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %612: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%611)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%612));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %288 @fn2_g(%289 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %613: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %614: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%613)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%614));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %290 @fn3_g(%291 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %615: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %616: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%615)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%616));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %292 @fn4_g(%293 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %617: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %618: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%617)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%618));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %294 @fn5_g(%295 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %619: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %620: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%619)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%620));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %296 @fn6_g(%297 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %621: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %622: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%621)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%622));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %298 @fn7_g(%299 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %623: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %624: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%623)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%624));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %300 @fn8_g(%301 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %625: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %626: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%625)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%626));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %302 @fn9_g(%303 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %627: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %628: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%627)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%628));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %304 @fn1_h(%305 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %629: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %630: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%629)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%630));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %306 @fn2_h(%307 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %631: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %632: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%631)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%632));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %308 @fn3_h(%309 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %633: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %634: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%633)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%634));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %310 @fn4_h(%311 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %635: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %636: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%635)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%636));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %312 @fn5_h(%313 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %637: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %638: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%637)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%638));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %314 @fn6_h(%315 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %639: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %640: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%639)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%640));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %316 @fn7_h(%317 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %641: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %642: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%641)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%642));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %318 @fn8_h(%319 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %643: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %644: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%643)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%644));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %320 @fn9_h(%321 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %645: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %646: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%645)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%646));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %322 @fn1_i(%323 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %647: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2));
// DEFAULT-NEXT:         let %648: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%647)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), read<u32>(%648));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %324 @fn2_i(%325 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %649: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2));
// DEFAULT-NEXT:         let %650: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%649)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), read<u32>(%650));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %326 @fn3_i(%327 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %651: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2));
// DEFAULT-NEXT:         let %652: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%651)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), read<u32>(%652));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %328 @fn4_i(%329 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %653: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4));
// DEFAULT-NEXT:         let %654: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%653)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), read<u32>(%654));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %330 @fn5_i(%331 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %655: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4));
// DEFAULT-NEXT:         let %656: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%655)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), read<u32>(%656));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %332 @fn6_i(%333 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %657: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4));
// DEFAULT-NEXT:         let %658: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%657)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), read<u32>(%658));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %334 @fn7_i(%335 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %659: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6));
// DEFAULT-NEXT:         let %660: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%659)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), read<u32>(%660));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %336 @fn8_i(%337 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %661: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6));
// DEFAULT-NEXT:         let %662: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%661)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), read<u32>(%662));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %338 @fn9_i(%339 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %663: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6));
// DEFAULT-NEXT:         let %664: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%663)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), read<u32>(%664));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %340 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%16, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%20, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%22, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%26, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%28, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%30, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%32, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%34, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%36, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%38, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%40, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%42, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%44, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%46, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%48, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%50, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%52, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%54, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%56, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%58, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%60, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%62, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%64, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%66, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%68, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%70, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%72, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%74, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%76, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%78, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%80, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%82, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%84, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%86, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%88, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%90, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%92, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%94, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%96, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%98, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%100, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%102, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%104, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%106, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%108, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%110, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%112, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%114, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%116, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%118, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%120, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%122, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%124, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%126, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%128, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%130, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%132, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%134, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%136, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%138, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%140, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%142, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%144, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%146, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%148, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%150, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%152, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%154, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%156, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%158, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%160, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%162, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%164, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%166, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%168, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%170, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%172, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%174, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%176, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%178, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%180, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%182, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%184, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%186, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%188, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%190, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%192, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%194, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%196, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%198, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%200, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%202, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%204, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%206, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%208, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%210, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%212, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%214, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%216, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%218, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%220, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%222, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%224, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%226, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%228, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%230, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%232, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%234, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%236, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%238, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%240, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%242, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%244, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%246, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%248, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%250, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(51), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%252, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(636), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%254, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(31278), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%256, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(21), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%258, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(1), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%260, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(33554432), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%262, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(26812), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%264, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(156), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%266, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(187), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%268, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(51), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%270, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(636), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%272, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(31278), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%274, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(21), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%276, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(1), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%278, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(33554432), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%280, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(26812), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%282, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(156), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%284, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(187), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%286, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(51), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%288, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(636), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%290, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(31278), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%292, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(21), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%294, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(1), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%296, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(33554432), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%298, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(26812), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%300, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(156), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%302, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(187), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%304, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%306, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%308, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%310, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%312, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%314, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%316, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%318, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%320, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%322, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%7), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%324, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%326, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%2), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%328, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%330, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%332, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%334, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%336, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%338, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
