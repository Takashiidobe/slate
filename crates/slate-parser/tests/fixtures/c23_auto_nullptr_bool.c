void f(void) {
#if __STDC_VERSION__ >= 202311L
  auto z = 1.0;
#else
  auto int z = 1;
#endif
  int *p = nullptr;
  _Bool t = true;
  _Bool u = false;
}

// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-IR-ERROR C17

// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × semantic analysis failed
// C17: Error:
// C17: × unresolved ordinary name `nullptr`
// C17: ╭─[tests/fixtures/c23_auto_nullptr_bool.c:7:12]
// C17: 6 │ #endif
// C17: 7 │   int *p = nullptr;
// C17: ·            ───────
// C17: 8 │   _Bool t = true;
// C17: ╰────
// C17: Error:
// C17: × unresolved ordinary name `true`
// C17: ╭─[tests/fixtures/c23_auto_nullptr_bool.c:8:13]
// C17: 7 │   int *p = nullptr;
// C17: 8 │   _Bool t = true;
// C17: ·             ────
// C17: 9 │   _Bool u = false;
// C17: ╰────
// C17: Error:
// C17: × unresolved ordinary name `false`
// C17: ╭─[tests/fixtures/c23_auto_nullptr_bool.c:9:13]
// C17: 8 │   _Bool t = true;
// C17: 9 │   _Bool u = false;
// C17: ·             ─────
// C17: 10 │ }
// C17: ╰────
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     fn %0 @f() -> void [linkage=external] [fallthrough=ret_void] {
// C23-NEXT:         let %1 z: f64 [storage=automatic] = const<f64>(1.0);
// C23-NEXT:         let %2 p: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// C23-NEXT:         let %3 t: bool [storage=automatic] = const<bool>(true);
// C23-NEXT:         let %4 u: bool [storage=automatic] = const<bool>(false);
// C23-NEXT:     }
// C23-NEXT: }
// SLATE-FILECHECK-END C23
