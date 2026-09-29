/* Contributed by Kris Van Hees <kris.van.hees@oracle.com> */
/* Test the char16_t and char32_t promotion rules. */
/* { dg-do compile } */
/* { dg-require-effective-target int32plus } */
/* { dg-options "-std=gnu99 -Wall -Wconversion -Wsign-conversion" } */

typedef __CHAR16_TYPE__ char16_t;
typedef __CHAR32_TYPE__ char32_t;

extern void f_c (char);
extern void fsc (signed char);
extern void fuc (unsigned char);
extern void f_s (short);
extern void fss (signed short);
extern void fus (unsigned short);
extern void f_i (int);
extern void fsi (signed int);
extern void fui (unsigned int);
extern void f_l (long);
extern void fsl (signed long);
extern void ful (unsigned long);
extern void f_ll (long long);
extern void fsll (signed long long);
extern void full (unsigned long long);

void m (char16_t c0, char32_t c1)
{
    f_c (c0);	/* { dg-warning "conversion from .char16_t\[^\n\r\]*. to .char. may change value" } */
    fsc (c0);	/* { dg-warning "may change value" } */
    fuc (c0);	/* { dg-warning "may change value" } */
    f_s (c0);	/* { dg-warning "change the sign" } */
    fss (c0);	/* { dg-warning "change the sign" } */
    fus (c0);
    f_i (c0);
    fsi (c0);
    fui (c0);
    f_l (c0);
    fsl (c0);
    ful (c0);
    f_ll (c0);
    fsll (c0);
    full (c0);

    f_c (c1);	/* { dg-warning "may change value" } */
    fsc (c1);	/* { dg-warning "may change value" } */
    fuc (c1);	/* { dg-warning "may change value" } */
    f_s (c1);	/* { dg-warning "may change value" } */
    fss (c1);	/* { dg-warning "may change value" } */
    fus (c1);	/* { dg-warning "may change value" } */
    f_i (c1);	/* { dg-warning "change the sign" "" { target { ! int16 } } } */
    fsi (c1);	/* { dg-warning "change the sign" "" { target { ! int16 } } } */
    fui (c1);
    f_l (c1);	/* { dg-warning "change the sign" "" { target { llp64 || ilp32 } } } */
    fsl (c1);	/* { dg-warning "change the sign" "" { target { llp64 || ilp32 } } } */
    ful (c1);
    f_ll (c1);
    fsll (c1);
    full (c1);
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type[[TYPE_char16_t:[0-9]+]] char16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     fn %[[VALUE_f_c:[0-9]+]] @f_c(%[[VALUE0:[0-9]+]] <unnamed>: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsc:[0-9]+]] @fsc(%[[VALUE1:[0-9]+]] <unnamed>: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fuc:[0-9]+]] @fuc(%[[VALUE2:[0-9]+]] <unnamed>: u8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_s:[0-9]+]] @f_s(%[[VALUE3:[0-9]+]] <unnamed>: i16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fss:[0-9]+]] @fss(%[[VALUE4:[0-9]+]] <unnamed>: i16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fus:[0-9]+]] @fus(%[[VALUE5:[0-9]+]] <unnamed>: u16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_i:[0-9]+]] @f_i(%[[VALUE6:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsi:[0-9]+]] @fsi(%[[VALUE7:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fui:[0-9]+]] @fui(%[[VALUE8:[0-9]+]] <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_l:[0-9]+]] @f_l(%[[VALUE9:[0-9]+]] <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsl:[0-9]+]] @fsl(%[[VALUE10:[0-9]+]] <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ful:[0-9]+]] @ful(%[[VALUE11:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f_ll:[0-9]+]] @f_ll(%[[VALUE12:[0-9]+]] <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsll:[0-9]+]] @fsll(%[[VALUE13:[0-9]+]] <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_full:[0-9]+]] @full(%[[VALUE14:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_m:[0-9]+]] @m(%[[VALUE_c0:[0-9]+]] c0: u16, %[[VALUE_c1:[0-9]+]] c1: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%[[VALUE_f_c]], reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%[[VALUE_fsc]], reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_fuc]], truncate<u8, reason=arg, fits=unknown>(read<u16>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%[[VALUE_f_s]], reinterpret<i16, reason=arg, fits=unknown>(read<u16>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%[[VALUE_fss]], reinterpret<i16, reason=arg, fits=unknown>(read<u16>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%[[VALUE_fus]], read<u16>(%[[VALUE_c0]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f_i]], reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], widen<u32, reason=arg>(read<u16>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f_l]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_fsl]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_ful]], widen<u64, reason=arg>(read<u16>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f_ll]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_fsll]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%[[VALUE_c0]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_full]], widen<u64, reason=arg>(read<u16>(%[[VALUE_c0]])));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%[[VALUE_f_c]], reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%[[VALUE_fsc]], reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%[[VALUE_fuc]], truncate<u8, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]])));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%[[VALUE_f_s]], reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%[[VALUE_fss]], reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%[[VALUE_fus]], truncate<u16, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_f_i]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_c1]])));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], read<u32>(%[[VALUE_c1]]));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f_l]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_fsl]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_ful]], widen<u64, reason=arg>(read<u32>(%[[VALUE_c1]])));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_f_ll]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_fsll]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%[[VALUE_c1]]))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_full]], widen<u64, reason=arg>(read<u32>(%[[VALUE_c1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
