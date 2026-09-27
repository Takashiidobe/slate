/* { dg-require-visibility "" } */
/* { dg-require-dll "" } */

extern void __attribute__((dllimport, visibility("hidden"))) 
  f1(); /* { dg-error "visibility" } */
extern void __attribute__((visibility("hidden"), dllimport)) 
  f2(); /* { dg-error "visibility" } */
extern void __attribute__((dllexport, visibility("hidden"))) 
  f3(); /* { dg-error "visibility" } */
extern void __attribute__((visibility("hidden"), dllexport)) 
  f4(); /* { dg-error "visibility" } */
extern void __attribute__((visibility("default"), dllimport))
  f5();
extern void __attribute__((dllimport, visibility("default")))
  f6();
extern void __attribute__((visibility("default"), dllexport))
  f7();
extern void __attribute__((dllexport, visibility("default")))
  f8();

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
// DEFAULT-NEXT:     fn %0 @f1(unprototyped) -> void [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     fn %1 @f2(unprototyped) -> void [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     fn %2 @f3(unprototyped) -> void [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     fn %3 @f4(unprototyped) -> void [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     fn %4 @f5(unprototyped) -> void [linkage=external] [visibility=default];
// DEFAULT-NEXT:     fn %5 @f6(unprototyped) -> void [linkage=external] [visibility=default];
// DEFAULT-NEXT:     fn %6 @f7(unprototyped) -> void [linkage=external] [visibility=default];
// DEFAULT-NEXT:     fn %7 @f8(unprototyped) -> void [linkage=external] [visibility=default];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
