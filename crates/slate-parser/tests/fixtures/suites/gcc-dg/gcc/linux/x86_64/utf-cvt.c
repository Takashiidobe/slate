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
// DEFAULT-NEXT:     type @type0 char16_t = u16;
// DEFAULT-NEXT:     type @type1 char32_t = u32;
// DEFAULT-NEXT:     fn %2 @f_c(%20 <unnamed>: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @fsc(%21 <unnamed>: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @fuc(%22 <unnamed>: u8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @f_s(%23 <unnamed>: i16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @fss(%24 <unnamed>: i16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @fus(%25 <unnamed>: u16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @f_i(%26 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @fsi(%27 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @fui(%28 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @f_l(%29 <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @fsl(%30 <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @ful(%31 <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @f_ll(%32 <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @fsll(%33 <unnamed>: i64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @full(%34 <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @m(%18 c0: u16, %19 c1: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%2, reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%3, reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%4, truncate<u8, reason=arg, fits=unknown>(read<u16>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%5, reinterpret<i16, reason=arg, fits=unknown>(read<u16>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%6, reinterpret<i16, reason=arg, fits=unknown>(read<u16>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%7, read<u16>(%18));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%8, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%9, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%10, widen<u32, reason=arg>(read<u16>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%11, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%12, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%13, widen<u64, reason=arg>(read<u16>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%14, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%15, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u16>(%18))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%16, widen<u64, reason=arg>(read<u16>(%18)));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%2, reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%3, reinterpret<i8, reason=arg, fits=unknown>(truncate<u8, reason=arg, fits=unknown>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%4, truncate<u8, reason=arg, fits=unknown>(read<u32>(%19)));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%5, reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%6, reinterpret<i16, reason=arg, fits=unknown>(truncate<u16, reason=arg, fits=unknown>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%7, truncate<u16, reason=arg, fits=unknown>(read<u32>(%19)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%8, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%19)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%9, reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%19)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%10, read<u32>(%19));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%11, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%12, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%13, widen<u64, reason=arg>(read<u32>(%19)));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%14, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%15, reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(read<u32>(%19))));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%16, widen<u64, reason=arg>(read<u32>(%19)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
