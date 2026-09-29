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
// DEFAULT-NEXT:     global %[[VALUE_wv1:[0-9]+]] wv1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv6:[0-9]+]] wv6: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv9:[0-9]+]] wv9: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv10:[0-9]+]] wv10: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv11:[0-9]+]] wv11: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv12:[0-9]+]] wv12: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv13:[0-9]+]] wv13: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wv14:[0-9]+]] wv14: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wf1:[0-9]+]] @wf1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf6:[0-9]+]] @wf6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf9:[0-9]+]] @wf9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf10:[0-9]+]] @wf10() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf11:[0-9]+]] @wf11() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf12:[0-9]+]] @wf12() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf13:[0-9]+]] @wf13() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf14:[0-9]+]] @wf14() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
