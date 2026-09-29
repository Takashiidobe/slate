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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 15;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 2, 8], bit_offsets=[Some(0), Some(6), Some(17), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 i: u32 : 5;
// DEFAULT-NEXT:         field1 j: u32 : 1;
// DEFAULT-NEXT:         field2 k: u32 : 26;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(5), Some(6), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 i: u32 : 16;
// DEFAULT-NEXT:         field1 j: u32 : 8;
// DEFAULT-NEXT:         field2 k: u32 : 8;
// DEFAULT-NEXT:         field3 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 2, 3, 8], bit_offsets=[Some(0), Some(16), Some(24), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: @type[[TYPE1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: @type[[TYPE2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_ret1:[0-9]+]] @ret1() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret2:[0-9]+]] @ret2() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret3:[0-9]+]] @ret3() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret4:[0-9]+]] @ret4() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret5:[0-9]+]] @ret5() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret6:[0-9]+]] @ret6() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret7:[0-9]+]] @ret7() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret8:[0-9]+]] @ret8() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ret9:[0-9]+]] @ret9() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_1:[0-9]+]] @fn1_1(%[[VALUE_x:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE0]]))), read<u32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_1:[0-9]+]] @fn2_1(%[[VALUE_x_2:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE2]]))), read<u32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_1:[0-9]+]] @fn3_1(%[[VALUE_x_3:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE4]]))), read<u32>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_1:[0-9]+]] @fn4_1(%[[VALUE_x_4:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE6]]))), read<u32>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_1:[0-9]+]] @fn5_1(%[[VALUE_x_5:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE8]]))), read<u32>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_1:[0-9]+]] @fn6_1(%[[VALUE_x_6:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE10]]))), read<u32>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_1:[0-9]+]] @fn7_1(%[[VALUE_x_7:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE12]]))), read<u32>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_1:[0-9]+]] @fn8_1(%[[VALUE_x_8:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE14]]))), read<u32>(%[[VALUE_x_8]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_1:[0-9]+]] @fn9_1(%[[VALUE_x_9:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE16]]))), read<u32>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_2:[0-9]+]] @fn1_2(%[[VALUE_x_10:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE18]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_2:[0-9]+]] @fn2_2(%[[VALUE_x_11:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE20]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_2:[0-9]+]] @fn3_2(%[[VALUE_x_12:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE22]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_2:[0-9]+]] @fn4_2(%[[VALUE_x_13:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE24]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE25]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_2:[0-9]+]] @fn5_2(%[[VALUE_x_14:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE26]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_2:[0-9]+]] @fn6_2(%[[VALUE_x_15:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE28]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE29]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_2:[0-9]+]] @fn7_2(%[[VALUE_x_16:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE30]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE31]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_2:[0-9]+]] @fn8_2(%[[VALUE_x_17:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE32]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE33]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_2:[0-9]+]] @fn9_2(%[[VALUE_x_18:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE34]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE35]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_3:[0-9]+]] @fn1_3(%[[VALUE_x_19:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE36]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE37]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_3:[0-9]+]] @fn2_3(%[[VALUE_x_20:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE38]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE39]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_3:[0-9]+]] @fn3_3(%[[VALUE_x_21:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE40]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE41]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_3:[0-9]+]] @fn4_3(%[[VALUE_x_22:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE42]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE43]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_3:[0-9]+]] @fn5_3(%[[VALUE_x_23:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE44]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE45]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_3:[0-9]+]] @fn6_3(%[[VALUE_x_24:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE46]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE47]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_3:[0-9]+]] @fn7_3(%[[VALUE_x_25:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE48]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE49]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_3:[0-9]+]] @fn8_3(%[[VALUE_x_26:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE50]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE51]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_3:[0-9]+]] @fn9_3(%[[VALUE_x_27:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE52]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE53]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_4:[0-9]+]] @fn1_4(%[[VALUE_x_28:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE54]]))), read<u32>(%[[VALUE_x_28]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE55]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_4:[0-9]+]] @fn2_4(%[[VALUE_x_29:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE56]]))), read<u32>(%[[VALUE_x_29]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE57]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_4:[0-9]+]] @fn3_4(%[[VALUE_x_30:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE58]]))), read<u32>(%[[VALUE_x_30]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE59]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_4:[0-9]+]] @fn4_4(%[[VALUE_x_31:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE60]]))), read<u32>(%[[VALUE_x_31]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE61]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_4:[0-9]+]] @fn5_4(%[[VALUE_x_32:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE62]]))), read<u32>(%[[VALUE_x_32]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE63]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_4:[0-9]+]] @fn6_4(%[[VALUE_x_33:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE64]]))), read<u32>(%[[VALUE_x_33]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE65]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_4:[0-9]+]] @fn7_4(%[[VALUE_x_34:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE66]]))), read<u32>(%[[VALUE_x_34]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE67]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_4:[0-9]+]] @fn8_4(%[[VALUE_x_35:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE68]]))), read<u32>(%[[VALUE_x_35]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE69]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_4:[0-9]+]] @fn9_4(%[[VALUE_x_36:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE70]]))), read<u32>(%[[VALUE_x_36]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE71]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_5:[0-9]+]] @fn1_5(%[[VALUE_x_37:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE72]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE73]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_5:[0-9]+]] @fn2_5(%[[VALUE_x_38:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE74]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE75]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_5:[0-9]+]] @fn3_5(%[[VALUE_x_39:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE76]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE77]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_5:[0-9]+]] @fn4_5(%[[VALUE_x_40:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE78]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE79]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_5:[0-9]+]] @fn5_5(%[[VALUE_x_41:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE80]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE81]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_5:[0-9]+]] @fn6_5(%[[VALUE_x_42:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE82]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE83]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_5:[0-9]+]] @fn7_5(%[[VALUE_x_43:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE84]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE85]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_5:[0-9]+]] @fn8_5(%[[VALUE_x_44:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE86]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE87]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_5:[0-9]+]] @fn9_5(%[[VALUE_x_45:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE88]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE89]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_6:[0-9]+]] @fn1_6(%[[VALUE_x_46:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE90]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE91]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_6:[0-9]+]] @fn2_6(%[[VALUE_x_47:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE92]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE93]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_6:[0-9]+]] @fn3_6(%[[VALUE_x_48:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE94]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE95]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_6:[0-9]+]] @fn4_6(%[[VALUE_x_49:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE96]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE97]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_6:[0-9]+]] @fn5_6(%[[VALUE_x_50:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE98]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE99]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_6:[0-9]+]] @fn6_6(%[[VALUE_x_51:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE100]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE101]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_6:[0-9]+]] @fn7_6(%[[VALUE_x_52:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE102]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE103]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_6:[0-9]+]] @fn8_6(%[[VALUE_x_53:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE104]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE105]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_6:[0-9]+]] @fn9_6(%[[VALUE_x_54:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE106]])), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE107]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_7:[0-9]+]] @fn1_7(%[[VALUE_x_55:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE108]]))), read<u32>(%[[VALUE_x_55]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE109]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_7:[0-9]+]] @fn2_7(%[[VALUE_x_56:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE110]]))), read<u32>(%[[VALUE_x_56]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE111]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_7:[0-9]+]] @fn3_7(%[[VALUE_x_57:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE112]]))), read<u32>(%[[VALUE_x_57]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE113]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_7:[0-9]+]] @fn4_7(%[[VALUE_x_58:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE114]]))), read<u32>(%[[VALUE_x_58]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE115]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_7:[0-9]+]] @fn5_7(%[[VALUE_x_59:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE116]]))), read<u32>(%[[VALUE_x_59]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE117]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_7:[0-9]+]] @fn6_7(%[[VALUE_x_60:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE118]]))), read<u32>(%[[VALUE_x_60]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE119]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_7:[0-9]+]] @fn7_7(%[[VALUE_x_61:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE121:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE120]]))), read<u32>(%[[VALUE_x_61]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE121]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_7:[0-9]+]] @fn8_7(%[[VALUE_x_62:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE122]]))), read<u32>(%[[VALUE_x_62]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE123]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_7:[0-9]+]] @fn9_7(%[[VALUE_x_63:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE124]]))), read<u32>(%[[VALUE_x_63]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE125]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_8:[0-9]+]] @fn1_8(%[[VALUE_x_64:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE126]]))), read<u32>(%[[VALUE_x_64]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE127]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_8:[0-9]+]] @fn2_8(%[[VALUE_x_65:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE128]]))), read<u32>(%[[VALUE_x_65]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE129]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_8:[0-9]+]] @fn3_8(%[[VALUE_x_66:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE130]]))), read<u32>(%[[VALUE_x_66]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE131]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_8:[0-9]+]] @fn4_8(%[[VALUE_x_67:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE132]]))), read<u32>(%[[VALUE_x_67]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE133]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_8:[0-9]+]] @fn5_8(%[[VALUE_x_68:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE134]]))), read<u32>(%[[VALUE_x_68]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE135]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_8:[0-9]+]] @fn6_8(%[[VALUE_x_69:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE136]]))), read<u32>(%[[VALUE_x_69]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE137]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_8:[0-9]+]] @fn7_8(%[[VALUE_x_70:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE138]]))), read<u32>(%[[VALUE_x_70]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE139]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_8:[0-9]+]] @fn8_8(%[[VALUE_x_71:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE140]]))), read<u32>(%[[VALUE_x_71]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE141]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_8:[0-9]+]] @fn9_8(%[[VALUE_x_72:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE142]]))), read<u32>(%[[VALUE_x_72]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE143]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_9:[0-9]+]] @fn1_9(%[[VALUE_x_73:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE144]]))), read<u32>(%[[VALUE_x_73]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE145]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_9:[0-9]+]] @fn2_9(%[[VALUE_x_74:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE146]]))), read<u32>(%[[VALUE_x_74]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE147]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_9:[0-9]+]] @fn3_9(%[[VALUE_x_75:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE148]]))), read<u32>(%[[VALUE_x_75]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE149]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_9:[0-9]+]] @fn4_9(%[[VALUE_x_76:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE150]]))), read<u32>(%[[VALUE_x_76]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE151]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_9:[0-9]+]] @fn5_9(%[[VALUE_x_77:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE152]]))), read<u32>(%[[VALUE_x_77]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE153]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_9:[0-9]+]] @fn6_9(%[[VALUE_x_78:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE154]]))), read<u32>(%[[VALUE_x_78]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE155]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_9:[0-9]+]] @fn7_9(%[[VALUE_x_79:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE156]]))), read<u32>(%[[VALUE_x_79]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE157]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_9:[0-9]+]] @fn8_9(%[[VALUE_x_80:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE158]]))), read<u32>(%[[VALUE_x_80]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE159]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_9:[0-9]+]] @fn9_9(%[[VALUE_x_81:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE160]]))), read<u32>(%[[VALUE_x_81]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE161]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_a:[0-9]+]] @fn1_a(%[[VALUE_x_82:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE162]]))), read<u32>(%[[VALUE_x_82]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE163]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_a:[0-9]+]] @fn2_a(%[[VALUE_x_83:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE164]]))), read<u32>(%[[VALUE_x_83]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE165]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_a:[0-9]+]] @fn3_a(%[[VALUE_x_84:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE166]]))), read<u32>(%[[VALUE_x_84]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE167]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_a:[0-9]+]] @fn4_a(%[[VALUE_x_85:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE168]]))), read<u32>(%[[VALUE_x_85]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE169]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_a:[0-9]+]] @fn5_a(%[[VALUE_x_86:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE170:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE170]]))), read<u32>(%[[VALUE_x_86]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE171]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_a:[0-9]+]] @fn6_a(%[[VALUE_x_87:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE172:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE172]]))), read<u32>(%[[VALUE_x_87]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE173]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_a:[0-9]+]] @fn7_a(%[[VALUE_x_88:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE174:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE175:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE174]]))), read<u32>(%[[VALUE_x_88]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE175]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_a:[0-9]+]] @fn8_a(%[[VALUE_x_89:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE176:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE176]]))), read<u32>(%[[VALUE_x_89]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE177]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_a:[0-9]+]] @fn9_a(%[[VALUE_x_90:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE178:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE179:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE178]]))), read<u32>(%[[VALUE_x_90]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE179]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_b:[0-9]+]] @fn1_b(%[[VALUE_x_91:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE180:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE181:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE180]]))), read<u32>(%[[VALUE_x_91]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE181]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_b:[0-9]+]] @fn2_b(%[[VALUE_x_92:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE182:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE183:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE182]]))), read<u32>(%[[VALUE_x_92]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE183]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_b:[0-9]+]] @fn3_b(%[[VALUE_x_93:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE184:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE185:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE184]]))), read<u32>(%[[VALUE_x_93]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE185]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_b:[0-9]+]] @fn4_b(%[[VALUE_x_94:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE186:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE187:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE186]]))), read<u32>(%[[VALUE_x_94]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE187]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_b:[0-9]+]] @fn5_b(%[[VALUE_x_95:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE188:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE189:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE188]]))), read<u32>(%[[VALUE_x_95]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE189]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_b:[0-9]+]] @fn6_b(%[[VALUE_x_96:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE190:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE191:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE190]]))), read<u32>(%[[VALUE_x_96]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE191]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_b:[0-9]+]] @fn7_b(%[[VALUE_x_97:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE192:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE193:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE192]]))), read<u32>(%[[VALUE_x_97]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE193]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_b:[0-9]+]] @fn8_b(%[[VALUE_x_98:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE194:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE195:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE194]]))), read<u32>(%[[VALUE_x_98]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE195]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_b:[0-9]+]] @fn9_b(%[[VALUE_x_99:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE196:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE197:[0-9]+]]: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE196]]))), read<u32>(%[[VALUE_x_99]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE197]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_c:[0-9]+]] @fn1_c(%[[VALUE_x_100:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE198:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE199:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE198]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE199]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_c:[0-9]+]] @fn2_c(%[[VALUE_x_101:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE200:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE201:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE200]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE201]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_c:[0-9]+]] @fn3_c(%[[VALUE_x_102:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE202:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE203:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE202]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE203]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_c:[0-9]+]] @fn4_c(%[[VALUE_x_103:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE204:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE205:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE204]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE205]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_c:[0-9]+]] @fn5_c(%[[VALUE_x_104:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE206:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE207:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE206]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE207]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_c:[0-9]+]] @fn6_c(%[[VALUE_x_105:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE208:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE209:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE208]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE209]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_c:[0-9]+]] @fn7_c(%[[VALUE_x_106:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE210:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE211:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE210]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE211]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_c:[0-9]+]] @fn8_c(%[[VALUE_x_107:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE212:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE213:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE212]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE213]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_c:[0-9]+]] @fn9_c(%[[VALUE_x_108:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE214:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE215:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE214]])), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE215]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_d:[0-9]+]] @fn1_d(%[[VALUE_x_109:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE216:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE217:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE216]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE217]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_d:[0-9]+]] @fn2_d(%[[VALUE_x_110:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE218:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE219:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE218]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE219]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_d:[0-9]+]] @fn3_d(%[[VALUE_x_111:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE220:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE221:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE220]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE221]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_d:[0-9]+]] @fn4_d(%[[VALUE_x_112:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE222:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE223:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE222]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE223]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_d:[0-9]+]] @fn5_d(%[[VALUE_x_113:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE224:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE225:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE224]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE225]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_d:[0-9]+]] @fn6_d(%[[VALUE_x_114:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE226:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE227:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE226]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE227]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_d:[0-9]+]] @fn7_d(%[[VALUE_x_115:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE228:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE229:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE228]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE229]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_d:[0-9]+]] @fn8_d(%[[VALUE_x_116:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE230:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE231:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE230]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE231]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_d:[0-9]+]] @fn9_d(%[[VALUE_x_117:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE232:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE233:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE232]])), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE233]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_e:[0-9]+]] @fn1_e(%[[VALUE_x_118:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE234:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE235:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE234]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE235]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_e:[0-9]+]] @fn2_e(%[[VALUE_x_119:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE236:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE237:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE236]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE237]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_e:[0-9]+]] @fn3_e(%[[VALUE_x_120:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE238:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE239:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE238]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE239]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_e:[0-9]+]] @fn4_e(%[[VALUE_x_121:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE240:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE241:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE240]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE241]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_e:[0-9]+]] @fn5_e(%[[VALUE_x_122:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE242:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE243:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE242]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE243]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_e:[0-9]+]] @fn6_e(%[[VALUE_x_123:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE244:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE245:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE244]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE245]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_e:[0-9]+]] @fn7_e(%[[VALUE_x_124:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE246:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE247:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE246]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE247]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_e:[0-9]+]] @fn8_e(%[[VALUE_x_125:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE248:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE249:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE248]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE249]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_e:[0-9]+]] @fn9_e(%[[VALUE_x_126:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE250:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE251:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE250]])), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE251]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_f:[0-9]+]] @fn1_f(%[[VALUE_x_127:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE252:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE253:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE252]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE253]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_f:[0-9]+]] @fn2_f(%[[VALUE_x_128:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE254:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE255:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE254]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE255]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_f:[0-9]+]] @fn3_f(%[[VALUE_x_129:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE256:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE257:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE256]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE257]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_f:[0-9]+]] @fn4_f(%[[VALUE_x_130:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE258:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE259:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE258]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE259]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_f:[0-9]+]] @fn5_f(%[[VALUE_x_131:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE260:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE261:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE260]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE261]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_f:[0-9]+]] @fn6_f(%[[VALUE_x_132:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE262:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE263:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE262]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE263]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_f:[0-9]+]] @fn7_f(%[[VALUE_x_133:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE264:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE265:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE264]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE265]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_f:[0-9]+]] @fn8_f(%[[VALUE_x_134:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE266:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE267:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE266]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE267]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_f:[0-9]+]] @fn9_f(%[[VALUE_x_135:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE268:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE269:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE268]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE269]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_g:[0-9]+]] @fn1_g(%[[VALUE_x_136:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE270:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE271:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE270]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE271]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_g:[0-9]+]] @fn2_g(%[[VALUE_x_137:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE272:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE273:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE272]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE273]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_g:[0-9]+]] @fn3_g(%[[VALUE_x_138:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE274:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE275:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE274]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE275]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_g:[0-9]+]] @fn4_g(%[[VALUE_x_139:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE276:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE277:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE276]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE277]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_g:[0-9]+]] @fn5_g(%[[VALUE_x_140:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE278:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE279:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE278]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE279]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_g:[0-9]+]] @fn6_g(%[[VALUE_x_141:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE280:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE281:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE280]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE281]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_g:[0-9]+]] @fn7_g(%[[VALUE_x_142:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE282:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE283:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE282]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE283]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_g:[0-9]+]] @fn8_g(%[[VALUE_x_143:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE284:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE285:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE284]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE285]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_g:[0-9]+]] @fn9_g(%[[VALUE_x_144:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE286:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE287:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE286]])), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE287]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_h:[0-9]+]] @fn1_h(%[[VALUE_x_145:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE288:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE289:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE288]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE289]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_h:[0-9]+]] @fn2_h(%[[VALUE_x_146:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE290:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE291:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE290]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE291]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_h:[0-9]+]] @fn3_h(%[[VALUE_x_147:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE292:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE293:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE292]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE293]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_h:[0-9]+]] @fn4_h(%[[VALUE_x_148:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE294:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE295:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE294]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE295]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_h:[0-9]+]] @fn5_h(%[[VALUE_x_149:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE296:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE297:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE296]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE297]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_h:[0-9]+]] @fn6_h(%[[VALUE_x_150:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE298:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE299:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE298]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE299]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_h:[0-9]+]] @fn7_h(%[[VALUE_x_151:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE300:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE301:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE300]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE301]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_h:[0-9]+]] @fn8_h(%[[VALUE_x_152:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE302:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE303:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE302]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE303]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_h:[0-9]+]] @fn9_h(%[[VALUE_x_153:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE304:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE305:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE304]])), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE305]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1_i:[0-9]+]] @fn1_i(%[[VALUE_x_154:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE306:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE307:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE306]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), read<u32>(%[[VALUE307]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2_i:[0-9]+]] @fn2_i(%[[VALUE_x_155:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE308:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE309:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE308]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), read<u32>(%[[VALUE309]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3_i:[0-9]+]] @fn3_i(%[[VALUE_x_156:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE310:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE311:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE310]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), read<u32>(%[[VALUE311]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4_i:[0-9]+]] @fn4_i(%[[VALUE_x_157:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE312:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE313:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE312]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), read<u32>(%[[VALUE313]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5_i:[0-9]+]] @fn5_i(%[[VALUE_x_158:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE314:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE315:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE314]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), read<u32>(%[[VALUE315]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6_i:[0-9]+]] @fn6_i(%[[VALUE_x_159:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE316:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE317:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE316]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), read<u32>(%[[VALUE317]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7_i:[0-9]+]] @fn7_i(%[[VALUE_x_160:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE318:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE319:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE318]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), read<u32>(%[[VALUE319]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn8_i:[0-9]+]] @fn8_i(%[[VALUE_x_161:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE320:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE321:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE320]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), read<u32>(%[[VALUE321]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn9_i:[0-9]+]] @fn9_i(%[[VALUE_x_162:[0-9]+]] x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE322:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE323:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE322]])), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), read<u32>(%[[VALUE323]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_2]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_4]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_5]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_6]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_7]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_9]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_a]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_b]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_c]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_d]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(51), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(636), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(31278), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(21), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(1), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(33554432), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(26812), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(156), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_e]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(187), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(51), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(636), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(31278), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(21), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(1), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(33554432), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(26812), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(156), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(187), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(51), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(636), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(31278), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(21), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(1), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(33554432), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(26812), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(156), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_g]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(187), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_h]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn1_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret1]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn2_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn3_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret3]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_b]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn4_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret4]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn5_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret5]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn6_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret6]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%[[VALUE_c]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn7_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret7]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn8_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret8]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fn9_i]], reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_ret9]]), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_d]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
