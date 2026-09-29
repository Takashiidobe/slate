/* { dg-do compile } */
#pragma GCC diagnostic error "-Wnoexcept" /* { dg-warning "is valid for C../ObjC.. but not for C" } */
#pragma GCC diagnostic error "-fstrict-aliasing" /* { dg-warning "not an option that controls warnings" } */
#pragma GCC diagnostic error "-Werror" /* { dg-warning "not an option that controls warnings" } */
#pragma GCC diagnostic error "-Wvla2" /* { dg-warning "unknown option after '#pragma GCC diagnostic' kind" } */
/* { dg-message "did you mean '-Wvla'" "" { target *-*-* } .-1 } */
#pragma GCC diagnostic error "-Walla" /* { dg-warning "unknown option after '#pragma GCC diagnostic' kind" } */
/* { dg-message "did you mean '-Wall'" "" { target *-*-* } .-1 } */
#pragma GCC diagnostic warning "-Walla" /* { dg-warning "unknown option after '#pragma GCC diagnostic' kind" } */
/* { dg-message "did you mean '-Wall'" "" { target *-*-* } .-1 } */
int i;

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
