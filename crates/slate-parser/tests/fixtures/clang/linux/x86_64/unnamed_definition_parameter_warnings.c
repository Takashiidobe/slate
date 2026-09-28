int first(int, int b) { return b; }
void pointer(char *, long) {}
int named(int a) { return a; }
int declaration_only(int, int);

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C89
// SLATE-FILECHECK-WARNING C17

// SLATE-FILECHECK-BEGIN C89
// C89: -Wc23-extensions
// C89: ⚠ omitting the parameter name in a function definition is a C23 extension
// C89: ╭─[tests/fixtures/clang/linux/x86_64/unnamed_definition_parameter_warnings.c:1:11]
// C89: 1 │ int first(int, int b) { return b; }
// C89: ·           ───
// C89: 2 │ void pointer(char *, long) {}
// C89: ╰────
// C89: -Wc23-extensions
// C89: ⚠ omitting the parameter name in a function definition is a C23 extension
// C89: ╭─[tests/fixtures/clang/linux/x86_64/unnamed_definition_parameter_warnings.c:2:14]
// C89: 1 │ int first(int, int b) { return b; }
// C89: 2 │ void pointer(char *, long) {}
// C89: ·              ──────
// C89: 3 │ int named(int a) { return a; }
// C89: ╰────
// C89: -Wc23-extensions
// C89: ⚠ omitting the parameter name in a function definition is a C23 extension
// C89: ╭─[tests/fixtures/clang/linux/x86_64/unnamed_definition_parameter_warnings.c:2:22]
// C89: 1 │ int first(int, int b) { return b; }
// C89: 2 │ void pointer(char *, long) {}
// C89: ·                      ────
// C89: 3 │ int named(int a) { return a; }
// C89: ╰────
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C17
// C17: -Wc23-extensions
// C17: ⚠ omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/clang/linux/x86_64/unnamed_definition_parameter_warnings.c:1:11]
// C17: 1 │ int first(int, int b) { return b; }
// C17: ·           ───
// C17: 2 │ void pointer(char *, long) {}
// C17: ╰────
// C17: -Wc23-extensions
// C17: ⚠ omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/clang/linux/x86_64/unnamed_definition_parameter_warnings.c:2:14]
// C17: 1 │ int first(int, int b) { return b; }
// C17: 2 │ void pointer(char *, long) {}
// C17: ·              ──────
// C17: 3 │ int named(int a) { return a; }
// C17: ╰────
// C17: -Wc23-extensions
// C17: ⚠ omitting the parameter name in a function definition is a C23 extension
// C17: ╭─[tests/fixtures/clang/linux/x86_64/unnamed_definition_parameter_warnings.c:2:22]
// C17: 1 │ int first(int, int b) { return b; }
// C17: 2 │ void pointer(char *, long) {}
// C17: ·                      ────
// C17: 3 │ int named(int a) { return a; }
// C17: ╰────
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN IR-C89
// IR-C89: module {
// IR-C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C89-NEXT:         endian = little;
// IR-C89-NEXT:         pointer [size=8, align=8];
// IR-C89-NEXT:         stack_alignment = 16;
// IR-C89-NEXT:         long_double = f80;
// IR-C89-NEXT:         storage bool [size=1, align=1];
// IR-C89-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C89-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C89-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C89-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C89-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C89-NEXT:         storage bf16 [size=2, align=2];
// IR-C89-NEXT:         storage f16 [size=2, align=2];
// IR-C89-NEXT:         storage f32 [size=4, align=4];
// IR-C89-NEXT:         storage f64 [size=8, align=8];
// IR-C89-NEXT:         storage f80 [size=16, align=16];
// IR-C89-NEXT:         storage f128 [size=16, align=16];
// IR-C89-NEXT:         storage d32 [size=4, align=4];
// IR-C89-NEXT:         storage d64 [size=8, align=8];
// IR-C89-NEXT:         storage d128 [size=16, align=16];
// IR-C89-NEXT:     }
// IR-C89-NEXT:     fn %0 @first(%6 <unnamed>: i32, %1 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-C89-NEXT:         return read<i32>(%1);
// IR-C89-NEXT:     }
// IR-C89-NEXT:     fn %2 @pointer(%7 <unnamed>: ptr<i8>, %8 <unnamed>: i64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-C89-NEXT:     }
// IR-C89-NEXT:     fn %3 @named(%4 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-C89-NEXT:         return read<i32>(%4);
// IR-C89-NEXT:     }
// IR-C89-NEXT:     fn %5 @declaration_only(%9 <unnamed>: i32, %10 <unnamed>: i32) -> i32 [linkage=external];
// IR-C89-NEXT: }
// SLATE-FILECHECK-END IR-C89
// SLATE-FILECHECK-BEGIN IR-C17
// IR-C17: module {
// IR-C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C17-NEXT:         endian = little;
// IR-C17-NEXT:         pointer [size=8, align=8];
// IR-C17-NEXT:         stack_alignment = 16;
// IR-C17-NEXT:         long_double = f80;
// IR-C17-NEXT:         storage bool [size=1, align=1];
// IR-C17-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C17-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C17-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C17-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C17-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C17-NEXT:         storage bf16 [size=2, align=2];
// IR-C17-NEXT:         storage f16 [size=2, align=2];
// IR-C17-NEXT:         storage f32 [size=4, align=4];
// IR-C17-NEXT:         storage f64 [size=8, align=8];
// IR-C17-NEXT:         storage f80 [size=16, align=16];
// IR-C17-NEXT:         storage f128 [size=16, align=16];
// IR-C17-NEXT:         storage d32 [size=4, align=4];
// IR-C17-NEXT:         storage d64 [size=8, align=8];
// IR-C17-NEXT:         storage d128 [size=16, align=16];
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %0 @first(%6 <unnamed>: i32, %1 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-C17-NEXT:         return read<i32>(%1);
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %2 @pointer(%7 <unnamed>: ptr<i8>, %8 <unnamed>: i64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %3 @named(%4 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-C17-NEXT:         return read<i32>(%4);
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %5 @declaration_only(%9 <unnamed>: i32, %10 <unnamed>: i32) -> i32 [linkage=external];
// IR-C17-NEXT: }
// SLATE-FILECHECK-END IR-C17
