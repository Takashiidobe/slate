// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /Gv /std:c11
int WinMain(void *, void *, char *, int);
static int wWinMain(void *a, void *b, unsigned short *c, int d) { return d; }
void scope(void) {
  int DllMain(void *, unsigned long, void *);
  _Static_assert(_Generic(&DllMain, int(__stdcall *)(void *, unsigned long, void *): 1, default: 0), "DllMain");
}
void parameter(int WinMain(void)) {
  _Static_assert(_Generic(WinMain, int(__vectorcall *)(void): 1, default: 0), "parameter");
}
void alias(void) {
  typedef int WinMain(void);
  WinMain *p = 0;
  _Static_assert(_Generic(p, int(__stdcall *)(void): 1, default: 0), "typedef");
}
_Static_assert(_Generic(&WinMain, int(__stdcall *)(void *, void *, char *, int): 1, default: 0), "WinMain");
_Static_assert(_Generic(&wWinMain, int(__stdcall *)(void *, void *, unsigned short *, int): 1, default: 0), "wWinMain");

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type[[TYPE_WinMain:[0-9]+]] WinMain = fn stdcall() -> i32;
// DEFAULT-NEXT:     fn %[[VALUE_WinMain:[0-9]+]] @WinMain(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [abi=x86_win32 stdcall(scalar, scalar, scalar, scalar) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_wWinMain:[0-9]+]] @wWinMain(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: ptr<void>, %[[VALUE_c:[0-9]+]] c: ptr<u16>, %[[VALUE_d:[0-9]+]] d: i32) -> i32 [linkage=internal] [abi=x86_win32 stdcall(scalar, scalar, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_DllMain:[0-9]+]] @DllMain(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE5:[0-9]+]] <unnamed>: u32, %[[VALUE6:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external] [abi=x86_win32 stdcall(scalar, scalar, scalar) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_scope:[0-9]+]] @scope() -> void [linkage=external] [abi=x86_win32 vectorcall() -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter:[0-9]+]] @parameter(%[[VALUE_WinMain_2:[0-9]+]] WinMain: ptr<fn vectorcall() -> i32>) -> void [linkage=external] [abi=x86_win32 vectorcall(scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alias:[0-9]+]] @alias() -> void [linkage=external] [abi=x86_win32 vectorcall() -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<fn stdcall() -> i32> [storage=automatic] = null<ptr<fn stdcall() -> i32>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
