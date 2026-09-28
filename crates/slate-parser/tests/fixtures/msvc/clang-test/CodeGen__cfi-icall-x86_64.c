
// Tests that we assign appropriate identifiers to unprototyped functions.

void f() {
}

void xf();

void g(int b) {
  void (*fp)() = b ? f : xf;
  fp();
}

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

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
// DEFAULT-NEXT:     fn %0 @f(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @xf(unprototyped) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @g(%3 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 fp: ptr<fn(unprototyped) -> void> [storage=automatic] = conditional<ptr<fn(unprototyped) -> void>>(ne<i32>(read<i32>(%3), const<i32>(0)), function_decay<ptr<fn(unprototyped) -> void>>(%0), function_decay<ptr<fn(unprototyped) -> void>>(%1));
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(read<ptr<fn(unprototyped) -> void>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
