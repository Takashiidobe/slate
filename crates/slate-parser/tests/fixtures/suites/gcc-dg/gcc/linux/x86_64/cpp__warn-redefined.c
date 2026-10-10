// { dg-do preprocess }
// { dg-options "-std=gnu99 -fdiagnostics-show-option -Wbuiltin-macro-redefined" }

#ifndef __TIME__
#error "__TIME__ builtin is not defined"
// { dg-bogus "__TIME__ builtin is not defined" "no-time" { target *-*-* } .-1 }
#endif

#define __TIME__ "X"  // { dg-warning "'__TIME__' redefined .-Wbuiltin-macro-redefined." }

#define __TIME__ "Y"  // { dg-bogus "-Wbuiltin-macro-redefined" }
                      // { dg-warning "'__TIME__' redefined" "not-builtin-1" { target *-*-* } .-1 }
                      // { dg-message "previous definition" "previous-1" { target *-*-* } 9 }

#define X "X"
#define X "Y"         // { dg-bogus "-Wbuiltin-macro-redefined" }
                      // { dg-warning "'X' redefined" "not-builtin-2" { target *-*-* } .-1 }
                      // { dg-message "previous definition" "previous-2" { target *-*-* } 15 }
// SLATE-FILECHECK-STD DEFAULT gnu99
// SLATE-FILECHECK-ARGS -Wbuiltin-macro-redefined
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT: ⚠ '__TIME__' redefined
// DEFAULT: ╭─[tests/fixtures/suites/gcc-dg/gcc/linux/x86_64/cpp__warn-redefined.c:9:9]
// DEFAULT: 8 │
// DEFAULT: 9 │ #define __TIME__ "X"  // { dg-warning "'__TIME__' redefined .-Wbuiltin-macro-redefined." }
// DEFAULT: ·         ────────
// DEFAULT: 10 │
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ '__TIME__' redefined
// DEFAULT: ╭─[tests/fixtures/suites/gcc-dg/gcc/linux/x86_64/cpp__warn-redefined.c:11:9]
// DEFAULT: 10 │
// DEFAULT: 11 │ #define __TIME__ "Y"  // { dg-bogus "-Wbuiltin-macro-redefined" }
// DEFAULT: ·         ────────
// DEFAULT: 12 │                       // { dg-warning "'__TIME__' redefined" "not-builtin-1" { target *-*-* } .-1 }
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ 'X' redefined
// DEFAULT: ╭─[tests/fixtures/suites/gcc-dg/gcc/linux/x86_64/cpp__warn-redefined.c:16:9]
// DEFAULT: 15 │ #define X "X"
// DEFAULT: 16 │ #define X "Y"         // { dg-bogus "-Wbuiltin-macro-redefined" }
// DEFAULT: ·         ─
// DEFAULT: 17 │                       // { dg-warning "'X' redefined" "not-builtin-2" { target *-*-* } .-1 }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f80;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f80 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
