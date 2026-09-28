/* On Darwin, you have to have a definition of the function to link,
   even if later on it won't be present in some dylib.  (That is,
   you have to link with the latest version of the dylib.)  */
void wf1(void) { }
void wf6(void) { }
void wf9(void) { }
void wf10(void) { }
void wf11(void) { }
void wf12(void) { }
void wf13(void) { }
void wf14(void) { }

int wv1;
int wv6;
int wv9;
int wv10;
int wv11;
int wv12;
int wv13;
int wv14;

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
// DEFAULT-NEXT:     global %8 wv1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 wv6: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 wv9: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 wv10: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 wv11: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 wv12: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 wv13: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 wv14: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @wf1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @wf6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @wf9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @wf10() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @wf11() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @wf12() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @wf13() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @wf14() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
