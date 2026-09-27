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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @f1(%1 x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 y: u539b [storage=automatic] = reinterpret<u539b, reason=assign, fits=unknown>(widen<i539b, reason=assign>(read<i381b>(%1)));
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(read<u539b>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 y: u539b [storage=automatic] = widen<u539b, reason=assign>(read<u381b>(%4));
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(read<u539b>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3(%7 x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 y: i539b [storage=automatic] = widen<i539b, reason=assign>(read<i381b>(%7));
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(read<i539b>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f4(%10 x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 y: i539b [storage=automatic] = reinterpret<i539b, reason=assign, fits=unknown>(widen<u539b, reason=assign>(read<u381b>(%10)));
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(read<i539b>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f5(%13 x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 y: u539b [storage=automatic] = reinterpret<u539b, reason=assign, fits=unknown>(widen<i539b, reason=assign>(read<i381b>(%13)));
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(read<u539b>(%14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f6(%16 x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 y: u539b [storage=automatic] = widen<u539b, reason=assign>(read<u381b>(%16));
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(read<u539b>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f7(%19 x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 y: i539b [storage=automatic] = widen<i539b, reason=assign>(read<i381b>(%19));
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(read<i539b>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @f8(%22 x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 y: i539b [storage=automatic] = reinterpret<i539b, reason=assign, fits=unknown>(widen<u539b, reason=assign>(read<u381b>(%22)));
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(read<i539b>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f9(%25 x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(reinterpret<u539b, reason=explicit, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%25))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f10(%27 x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<u495b, reason=return, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%27)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f11(%29 x: i381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%29))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f12(%31 x: u381b) -> u495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u495b, reason=return, fits=unknown>(truncate<i495b, reason=return, fits=unknown>(reinterpret<i539b, reason=explicit, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%31)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f13(%33 x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(reinterpret<u539b, reason=explicit, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%33)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @f14(%35 x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i495b, reason=return, fits=unknown>(truncate<u495b, reason=return, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%35))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @f15(%37 x: i381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(widen<i539b, reason=explicit>(read<i381b>(%37)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f16(%39 x: u381b) -> i495b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i495b, reason=return, fits=unknown>(reinterpret<i539b, reason=explicit, fits=unknown>(widen<u539b, reason=explicit>(read<u381b>(%39))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
