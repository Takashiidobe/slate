// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /Gv /std:c11
int f(void);
int WinMain(void *, void *, char *, int);
int wWinMain(void *, void *, unsigned short *, int);
int DllMain(void *, unsigned long, void *);
_Static_assert(_Generic(&f, int(__vectorcall *)(void): 1, default: 0), "f");
_Static_assert(_Generic(&WinMain, int(__cdecl *)(void *, void *, char *, int): 1, default: 0), "WinMain");
_Static_assert(_Generic(&wWinMain, int(__cdecl *)(void *, void *, unsigned short *, int): 1, default: 0), "wWinMain");
_Static_assert(_Generic(&DllMain, int(__cdecl *)(void *, unsigned long, void *): 1, default: 0), "DllMain");

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [abi=win64 vectorcall() -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_WinMain:[0-9]+]] @WinMain(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wWinMain:[0-9]+]] @wWinMain(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE5:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<u16>, %[[VALUE7:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_DllMain:[0-9]+]] @DllMain(%[[VALUE8:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE9:[0-9]+]] <unnamed>: u32, %[[VALUE10:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
