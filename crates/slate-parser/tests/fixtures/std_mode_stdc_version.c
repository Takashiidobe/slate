#ifdef __STDC_VERSION__
int stdc_version = __STDC_VERSION__;
#else
int no_stdc_version;
#endif

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES ISO1990
// SLATE-FILECHECK-STD ISO1990 iso9899:1990
// SLATE-FILECHECK-DEFINES ISO199409
// SLATE-FILECHECK-STD ISO199409 iso9899:199409
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES ISO1999
// SLATE-FILECHECK-STD ISO1999 iso9899:1999
// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES ISO2011
// SLATE-FILECHECK-STD ISO2011 iso9899:2011
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES ISO2017
// SLATE-FILECHECK-STD ISO2017 iso9899:2017
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %0 no_stdc_version: i32 [storage=static] [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN ISO1990
// ISO1990: module {
// ISO1990-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO1990-NEXT:         endian = little;
// ISO1990-NEXT:         pointer [size=8, align=8];
// ISO1990-NEXT:         stack_alignment = 16;
// ISO1990-NEXT:         long_double = f80;
// ISO1990-NEXT:         storage bool [size=1, align=1];
// ISO1990-NEXT:         storage i8, u8 [size=1, align=1];
// ISO1990-NEXT:         storage i16, u16 [size=2, align=2];
// ISO1990-NEXT:         storage i32, u32 [size=4, align=4];
// ISO1990-NEXT:         storage i64, u64 [size=8, align=8];
// ISO1990-NEXT:         storage i128, u128 [size=16, align=16];
// ISO1990-NEXT:         storage bf16 [size=2, align=2];
// ISO1990-NEXT:         storage f16 [size=2, align=2];
// ISO1990-NEXT:         storage f32 [size=4, align=4];
// ISO1990-NEXT:         storage f64 [size=8, align=8];
// ISO1990-NEXT:         storage f80 [size=16, align=16];
// ISO1990-NEXT:         storage f128 [size=16, align=16];
// ISO1990-NEXT:         storage d32 [size=4, align=4];
// ISO1990-NEXT:         storage d64 [size=8, align=8];
// ISO1990-NEXT:         storage d128 [size=16, align=16];
// ISO1990-NEXT:     }
// ISO1990-NEXT:     global %0 no_stdc_version: i32 [storage=static] [linkage=external];
// ISO1990-NEXT: }
// SLATE-FILECHECK-END ISO1990
// SLATE-FILECHECK-BEGIN ISO199409
// ISO199409: module {
// ISO199409-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO199409-NEXT:         endian = little;
// ISO199409-NEXT:         pointer [size=8, align=8];
// ISO199409-NEXT:         stack_alignment = 16;
// ISO199409-NEXT:         long_double = f80;
// ISO199409-NEXT:         storage bool [size=1, align=1];
// ISO199409-NEXT:         storage i8, u8 [size=1, align=1];
// ISO199409-NEXT:         storage i16, u16 [size=2, align=2];
// ISO199409-NEXT:         storage i32, u32 [size=4, align=4];
// ISO199409-NEXT:         storage i64, u64 [size=8, align=8];
// ISO199409-NEXT:         storage i128, u128 [size=16, align=16];
// ISO199409-NEXT:         storage bf16 [size=2, align=2];
// ISO199409-NEXT:         storage f16 [size=2, align=2];
// ISO199409-NEXT:         storage f32 [size=4, align=4];
// ISO199409-NEXT:         storage f64 [size=8, align=8];
// ISO199409-NEXT:         storage f80 [size=16, align=16];
// ISO199409-NEXT:         storage f128 [size=16, align=16];
// ISO199409-NEXT:         storage d32 [size=4, align=4];
// ISO199409-NEXT:         storage d64 [size=8, align=8];
// ISO199409-NEXT:         storage d128 [size=16, align=16];
// ISO199409-NEXT:     }
// ISO199409-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(199409)) [linkage=external];
// ISO199409-NEXT: }
// SLATE-FILECHECK-END ISO199409
// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=8, align=8];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=8];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage bf16 [size=2, align=2];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=8];
// C99-NEXT:         storage f80 [size=16, align=16];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:         storage d32 [size=4, align=4];
// C99-NEXT:         storage d64 [size=8, align=8];
// C99-NEXT:         storage d128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(199901)) [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN ISO1999
// ISO1999: module {
// ISO1999-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO1999-NEXT:         endian = little;
// ISO1999-NEXT:         pointer [size=8, align=8];
// ISO1999-NEXT:         stack_alignment = 16;
// ISO1999-NEXT:         long_double = f80;
// ISO1999-NEXT:         storage bool [size=1, align=1];
// ISO1999-NEXT:         storage i8, u8 [size=1, align=1];
// ISO1999-NEXT:         storage i16, u16 [size=2, align=2];
// ISO1999-NEXT:         storage i32, u32 [size=4, align=4];
// ISO1999-NEXT:         storage i64, u64 [size=8, align=8];
// ISO1999-NEXT:         storage i128, u128 [size=16, align=16];
// ISO1999-NEXT:         storage bf16 [size=2, align=2];
// ISO1999-NEXT:         storage f16 [size=2, align=2];
// ISO1999-NEXT:         storage f32 [size=4, align=4];
// ISO1999-NEXT:         storage f64 [size=8, align=8];
// ISO1999-NEXT:         storage f80 [size=16, align=16];
// ISO1999-NEXT:         storage f128 [size=16, align=16];
// ISO1999-NEXT:         storage d32 [size=4, align=4];
// ISO1999-NEXT:         storage d64 [size=8, align=8];
// ISO1999-NEXT:         storage d128 [size=16, align=16];
// ISO1999-NEXT:     }
// ISO1999-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(199901)) [linkage=external];
// ISO1999-NEXT: }
// SLATE-FILECHECK-END ISO1999
// SLATE-FILECHECK-BEGIN C11
// C11: module {
// C11-NEXT:     target "x86_64-unknown-linux-gnu" {
// C11-NEXT:         endian = little;
// C11-NEXT:         pointer [size=8, align=8];
// C11-NEXT:         stack_alignment = 16;
// C11-NEXT:         long_double = f80;
// C11-NEXT:         storage bool [size=1, align=1];
// C11-NEXT:         storage i8, u8 [size=1, align=1];
// C11-NEXT:         storage i16, u16 [size=2, align=2];
// C11-NEXT:         storage i32, u32 [size=4, align=4];
// C11-NEXT:         storage i64, u64 [size=8, align=8];
// C11-NEXT:         storage i128, u128 [size=16, align=16];
// C11-NEXT:         storage bf16 [size=2, align=2];
// C11-NEXT:         storage f16 [size=2, align=2];
// C11-NEXT:         storage f32 [size=4, align=4];
// C11-NEXT:         storage f64 [size=8, align=8];
// C11-NEXT:         storage f80 [size=16, align=16];
// C11-NEXT:         storage f128 [size=16, align=16];
// C11-NEXT:         storage d32 [size=4, align=4];
// C11-NEXT:         storage d64 [size=8, align=8];
// C11-NEXT:         storage d128 [size=16, align=16];
// C11-NEXT:     }
// C11-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(201112)) [linkage=external];
// C11-NEXT: }
// SLATE-FILECHECK-END C11
// SLATE-FILECHECK-BEGIN ISO2011
// ISO2011: module {
// ISO2011-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO2011-NEXT:         endian = little;
// ISO2011-NEXT:         pointer [size=8, align=8];
// ISO2011-NEXT:         stack_alignment = 16;
// ISO2011-NEXT:         long_double = f80;
// ISO2011-NEXT:         storage bool [size=1, align=1];
// ISO2011-NEXT:         storage i8, u8 [size=1, align=1];
// ISO2011-NEXT:         storage i16, u16 [size=2, align=2];
// ISO2011-NEXT:         storage i32, u32 [size=4, align=4];
// ISO2011-NEXT:         storage i64, u64 [size=8, align=8];
// ISO2011-NEXT:         storage i128, u128 [size=16, align=16];
// ISO2011-NEXT:         storage bf16 [size=2, align=2];
// ISO2011-NEXT:         storage f16 [size=2, align=2];
// ISO2011-NEXT:         storage f32 [size=4, align=4];
// ISO2011-NEXT:         storage f64 [size=8, align=8];
// ISO2011-NEXT:         storage f80 [size=16, align=16];
// ISO2011-NEXT:         storage f128 [size=16, align=16];
// ISO2011-NEXT:         storage d32 [size=4, align=4];
// ISO2011-NEXT:         storage d64 [size=8, align=8];
// ISO2011-NEXT:         storage d128 [size=16, align=16];
// ISO2011-NEXT:     }
// ISO2011-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(201112)) [linkage=external];
// ISO2011-NEXT: }
// SLATE-FILECHECK-END ISO2011
// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(201710)) [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN ISO2017
// ISO2017: module {
// ISO2017-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO2017-NEXT:         endian = little;
// ISO2017-NEXT:         pointer [size=8, align=8];
// ISO2017-NEXT:         stack_alignment = 16;
// ISO2017-NEXT:         long_double = f80;
// ISO2017-NEXT:         storage bool [size=1, align=1];
// ISO2017-NEXT:         storage i8, u8 [size=1, align=1];
// ISO2017-NEXT:         storage i16, u16 [size=2, align=2];
// ISO2017-NEXT:         storage i32, u32 [size=4, align=4];
// ISO2017-NEXT:         storage i64, u64 [size=8, align=8];
// ISO2017-NEXT:         storage i128, u128 [size=16, align=16];
// ISO2017-NEXT:         storage bf16 [size=2, align=2];
// ISO2017-NEXT:         storage f16 [size=2, align=2];
// ISO2017-NEXT:         storage f32 [size=4, align=4];
// ISO2017-NEXT:         storage f64 [size=8, align=8];
// ISO2017-NEXT:         storage f80 [size=16, align=16];
// ISO2017-NEXT:         storage f128 [size=16, align=16];
// ISO2017-NEXT:         storage d32 [size=4, align=4];
// ISO2017-NEXT:         storage d64 [size=8, align=8];
// ISO2017-NEXT:         storage d128 [size=16, align=16];
// ISO2017-NEXT:     }
// ISO2017-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(201710)) [linkage=external];
// ISO2017-NEXT: }
// SLATE-FILECHECK-END ISO2017
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %0 stdc_version: i32 [storage=static] = truncate<i32, reason=assign, fits=always>(const<i64>(202311)) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
