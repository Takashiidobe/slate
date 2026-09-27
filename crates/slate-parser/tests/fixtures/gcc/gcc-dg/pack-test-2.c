/* Tests for syntax checking of #pragma pack.
   Contributed by Mike Coleman <mcoleman2@kc.rr.com> */

/* { dg-do compile } */

#pragma pack(pop)               /* { dg-warning "without matching" } */

#pragma pack(push)
#pragma pack(pop)               /* reset */

#pragma pack(push, foo, 1)
#pragma pack(pop, foo, 1)       /* { dg-warning "malformed" } (/
#pragma pack(pop)               /* reset */

#pragma pack(push, foo, 1)
#pragma pack(pop, bar)          /* { dg-warning "without matching" } */
#pragma pack(pop)               /* reset */

#pragma pack(push, foo, 1)
#pragma pack(pop)
#pragma pack(pop, foo)          /* { dg-warning "without matching" } */

#pragma pack(push, foo, 3)      /* { dg-warning "small power of two" } */

extern int blah;	/* prevent "ISO C forbids an empty translation unit" */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     extern %0 blah: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
