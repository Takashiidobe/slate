/* PR tree-optimization/112941 */
/* { dg-do compile { target bitint575 } } */
/* { dg-options "-O2 -std=c23" } */

unsigned _BitInt(495) f1 (signed _BitInt(381) x) { unsigned _BitInt(539) y = x; return y; }
unsigned _BitInt(495) f2 (unsigned _BitInt(381) x) { unsigned _BitInt(539) y = x; return y; }
unsigned _BitInt(495) f3 (signed _BitInt(381) x) { _BitInt(539) y = x; return y; }
unsigned _BitInt(495) f4 (unsigned _BitInt(381) x) { _BitInt(539) y = x; return y; }
_BitInt(495) f5 (signed _BitInt(381) x) { unsigned _BitInt(539) y = x; return y; }
_BitInt(495) f6 (unsigned _BitInt(381) x) { unsigned _BitInt(539) y = x; return y; }
_BitInt(495) f7 (signed _BitInt(381) x) { _BitInt(539) y = x; return y; }
_BitInt(495) f8 (unsigned _BitInt(381) x) { _BitInt(539) y = x; return y; }
unsigned _BitInt(495) f9 (signed _BitInt(381) x) { return (unsigned _BitInt(539)) x; }
unsigned _BitInt(495) f10 (unsigned _BitInt(381) x) { return (unsigned _BitInt(539)) x; }
unsigned _BitInt(495) f11 (signed _BitInt(381) x) { return (_BitInt(539)) x; }
unsigned _BitInt(495) f12 (unsigned _BitInt(381) x) { return (_BitInt(539)) x; }
_BitInt(495) f13 (signed _BitInt(381) x) { return (unsigned _BitInt(539)) x; }
_BitInt(495) f14 (unsigned _BitInt(381) x) { return (unsigned _BitInt(539)) x; }
_BitInt(495) f15 (signed _BitInt(381) x) { return (_BitInt(539)) x; }
_BitInt(495) f16 (unsigned _BitInt(381) x) { return (_BitInt(539)) x; }

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: u539b [storage=automatic] = reinterpret<u539b, reason=assign, fits=unknown>(widen<i539b, reason=assign>(read<i381b>(%[[VALUE_x]])));
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(read<u539b>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: u539b [storage=automatic] = widen<u539b, reason=assign>(read<u381b>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(read<u539b>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: i539b [storage=automatic] = widen<i539b, reason=assign>(read<i381b>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(read<i539b>(%[[VALUE_y_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_4:[0-9]+]] x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: i539b [storage=automatic] = reinterpret<i539b, reason=assign, fits=unknown>(widen<u539b, reason=assign>(read<u381b>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(read<i539b>(%[[VALUE_y_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_x_5:[0-9]+]] x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_5:[0-9]+]] y: u539b [storage=automatic] = reinterpret<u539b, reason=assign, fits=unknown>(widen<i539b, reason=assign>(read<i381b>(%[[VALUE_x_5]])));
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(read<u539b>(%[[VALUE_y_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_x_6:[0-9]+]] x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_6:[0-9]+]] y: u539b [storage=automatic] = widen<u539b, reason=assign>(read<u381b>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(read<u539b>(%[[VALUE_y_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_x_7:[0-9]+]] x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_7:[0-9]+]] y: i539b [storage=automatic] = widen<i539b, reason=assign>(read<i381b>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(read<i539b>(%[[VALUE_y_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_x_8:[0-9]+]] x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_8:[0-9]+]] y: i539b [storage=automatic] = reinterpret<i539b, reason=assign, fits=unknown>(widen<u539b, reason=assign>(read<u381b>(%[[VALUE_x_8]])));
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(read<i539b>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_x_9:[0-9]+]] x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(reinterpret<u539b, reason=explicit, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%[[VALUE_x_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_x_10:[0-9]+]] x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%[[VALUE_x_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_x_11:[0-9]+]] x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%[[VALUE_x_11]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_x_12:[0-9]+]] x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(reinterpret<i539b, reason=explicit, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%[[VALUE_x_12]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_x_13:[0-9]+]] x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(reinterpret<u539b, reason=explicit, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%[[VALUE_x_13]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_x_14:[0-9]+]] x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%[[VALUE_x_14]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f15:[0-9]+]] @f15(%[[VALUE_x_15:[0-9]+]] x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%[[VALUE_x_15]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f16:[0-9]+]] @f16(%[[VALUE_x_16:[0-9]+]] x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(reinterpret<i539b, reason=explicit, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%[[VALUE_x_16]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
