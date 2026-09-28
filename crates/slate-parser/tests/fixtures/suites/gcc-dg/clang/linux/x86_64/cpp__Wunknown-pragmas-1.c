/* Copyright 2003 Free Software Foundation, Inc.  */

/* { dg-do compile } */
/* { dg-options "-Wunknown-pragmas" } */

/* Make sure we get warnings in the expected lines.  */

#pragma unknown1 /* { dg-warning "-:unknown1" "unknown1" } */

#define COMMA ,
#define FOO(x) x /* { dg-note "in definition of macro 'FOO'" } */

#define BAR1(x) _Pragma("unknown_before1") x  /* { dg-warning "unknown_before1" } */
#define BAR2(x) _Pragma("unknown_before2") x  /* { dg-warning "unknown_before2" } */

#define BAZ1(x) x _Pragma("unknown_after1")  /* { dg-warning "unknown_after1" } */
#define BAZ2(x) x _Pragma("unknown_after2")  /* { dg-warning "unknown_after2" } */

int _Pragma("unknown2") bar1; /* { dg-warning "unknown2" "unknown2" } */

FOO(int _Pragma("unknown3") bar2); /* { dg-warning "unknown3" "unknown3" } */

int BAR1(bar3); /* { dg-note "in expansion of macro 'BAR1'" } */

BAR2(int bar4); /* { dg-note "in expansion of macro 'BAR2'" } */

int BAZ1(bar5); /* { dg-note "in expansion of macro 'BAZ1'" } */

int BAZ2(bar6;) /* { dg-note "in expansion of macro 'BAZ2'" } */

FOO(int bar7; _Pragma("unknown4")) /* { dg-warning "-:unknown4" "unknown4" } */

#pragma unknown5 /* { dg-warning "-:unknown5" "unknown5" } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %0 bar1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 bar2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 bar3: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 bar4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 bar5: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 bar6: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 bar7: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
