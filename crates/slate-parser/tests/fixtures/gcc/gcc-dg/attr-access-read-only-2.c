/* PR c/93132 - bogus 'attribute((access))' warning when size-index
   is specified
   { dg-do compile }
   { dg-options "-Wall" } */

void __attribute__ ((access (read_only, 1, 5)))
f (void*, int, int, int, int);   // { dg-message "previous declaration" }

void __attribute__ ((access (read_only, 1, 3)))
f (void*, int, int, int, int);   // { dg-warning "attribute 'access\\\(read_only, 1, 3\\\)' mismatched positional argument values 3 and 5" }

void __attribute__ ((access (read_only, 1, 4)))
f (void*, int, int, int, int);   // { dg-warning "attribute 'access\\\(read_only, 1, 4\\\)' mismatched positional argument values 4 and 5" }

void __attribute__ ((access (read_only, 1, 5)))
f (void*, int, int, int, int);

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @f(%1 <unnamed>: ptr<void>, %2 <unnamed>: i32, %3 <unnamed>: i32, %4 <unnamed>: i32, %5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
