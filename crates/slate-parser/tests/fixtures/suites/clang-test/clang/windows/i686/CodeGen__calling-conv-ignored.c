
void foo_default(const char *lpString1, const char *lpString2);
void __stdcall foo_std(const char *lpString1, const char *lpString2);
void __fastcall foo_fast(const char *lpString1, const char *lpString2);
void __vectorcall foo_vector(const char *lpString1, const char *lpString2);

void __cdecl bar(void) {
  foo_default(0, 0);
  foo_std(0, 0);
  foo_fast(0, 0);
  foo_vector(0, 0);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

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
// DEFAULT-NEXT:     fn %0 @foo_default(%5 lpString1: ptr<const i8>, %6 lpString2: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo_std(%7 lpString1: ptr<const i8>, %8 lpString2: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo_fast(%9 lpString1: ptr<const i8>, %10 lpString2: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo_vector(%11 lpString1: ptr<const i8>, %12 lpString2: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>) -> void>(%0, null<ptr<const i8>>, null<ptr<const i8>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>) -> void>(%1, null<ptr<const i8>>, null<ptr<const i8>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>) -> void>(%2, null<ptr<const i8>>, null<ptr<const i8>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<const i8>) -> void>(%3, null<ptr<const i8>>, null<ptr<const i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
