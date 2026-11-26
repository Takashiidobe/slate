int known(void);
int defined(void) { return 0; }
typedef int Function(void);

void first(void) {
    int local(void);
    extern int external(void);
    int local(void);
    int known(void);
    int defined(void);
    Function aliased;
}

void repeat(void) {
    extern int external(void);
    Function aliased;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT /W4
// SLATE-FILECHECK-WARNING DEFAULT
// SLATE-FILECHECK-DEFINES STRICT
// SLATE-FILECHECK-PREFIX-ARGS STRICT /Za /W4
// SLATE-FILECHECK-WARNING STRICT
// SLATE-FILECHECK-DEFINES LOW
// SLATE-FILECHECK-PREFIX-ARGS LOW /W3
// SLATE-FILECHECK-WARNING LOW

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wblock-scope-function
// DEFAULT:   ⚠ nonstandard extension used: function given file scope
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/za_block_scope_function_warning.c:6:9]
// DEFAULT:  5 │ void first(void) {
// DEFAULT:  6 │     int local(void);
// DEFAULT:    ·         ───────────
// DEFAULT:  7 │     extern int external(void);
// DEFAULT:    ╰────
// DEFAULT: -Wblock-scope-function
// DEFAULT:   ⚠ nonstandard extension used: function given file scope
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/za_block_scope_function_warning.c:7:16]
// DEFAULT:  6 │     int local(void);
// DEFAULT:  7 │     extern int external(void);
// DEFAULT:    ·                ──────────────
// DEFAULT:  8 │     int local(void);
// DEFAULT:    ╰────
// DEFAULT: -Wblock-scope-function
// DEFAULT:   ⚠ nonstandard extension used: function given file scope
// DEFAULT:     ╭─[tests/fixtures/msvc/windows/x86_64/za_block_scope_function_warning.c:11:14]
// DEFAULT:  10 │     int defined(void);
// DEFAULT:  11 │     Function aliased;
// DEFAULT:     ·              ───────
// DEFAULT:  12 │ }
// DEFAULT:     ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN STRICT
// SLATE-FILECHECK-END STRICT
// SLATE-FILECHECK-BEGIN LOW
// SLATE-FILECHECK-END LOW
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f64;
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
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     type @type[[TYPE_Function:[0-9]+]] Function = fn() -> i32;
// IR-DEFAULT-NEXT:     fn %[[VALUE_known:[0-9]+]] @known() -> i32 [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_defined:[0-9]+]] @defined() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return const<i32>(0);
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> i32 [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_external:[0-9]+]] @external() -> i32 [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_aliased:[0-9]+]] @aliased() -> i32 [linkage=external] [c="Function"];
// IR-DEFAULT-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> void [linkage=external] [fallthrough=ret_void] {
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_aliased_2:[0-9]+]] @aliased() -> i32 [linkage=external] [c="Function"];
// IR-DEFAULT-NEXT:     fn %[[VALUE_repeat:[0-9]+]] @repeat() -> void [linkage=external] [fallthrough=ret_void] {
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
// SLATE-FILECHECK-BEGIN IR-STRICT
// IR-STRICT: module {
// IR-STRICT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-STRICT-NEXT:         endian = little;
// IR-STRICT-NEXT:         pointer [size=8, align=8];
// IR-STRICT-NEXT:         stack_alignment = 16;
// IR-STRICT-NEXT:         long_double = f64;
// IR-STRICT-NEXT:         storage bool [size=1, align=1];
// IR-STRICT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-STRICT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-STRICT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-STRICT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-STRICT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-STRICT-NEXT:         storage bf16 [size=2, align=2];
// IR-STRICT-NEXT:         storage f16 [size=2, align=2];
// IR-STRICT-NEXT:         storage f32 [size=4, align=4];
// IR-STRICT-NEXT:         storage f64 [size=8, align=8];
// IR-STRICT-NEXT:         storage f128 [size=16, align=16];
// IR-STRICT-NEXT:         storage d32 [size=4, align=4];
// IR-STRICT-NEXT:         storage d64 [size=8, align=8];
// IR-STRICT-NEXT:         storage d128 [size=16, align=16];
// IR-STRICT-NEXT:     }
// IR-STRICT-NEXT:     type @type[[TYPE_Function:[0-9]+]] Function = fn() -> i32;
// IR-STRICT-NEXT:     fn %[[VALUE_known:[0-9]+]] @known() -> i32 [linkage=external];
// IR-STRICT-NEXT:     fn %[[VALUE_defined:[0-9]+]] @defined() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-STRICT-NEXT:         return const<i32>(0);
// IR-STRICT-NEXT:     }
// IR-STRICT-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> i32 [linkage=external];
// IR-STRICT-NEXT:     fn %[[VALUE_external:[0-9]+]] @external() -> i32 [linkage=external];
// IR-STRICT-NEXT:     fn %[[VALUE_aliased:[0-9]+]] @aliased() -> i32 [linkage=external] [c="Function"];
// IR-STRICT-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> void [linkage=external] [fallthrough=ret_void] {
// IR-STRICT-NEXT:     }
// IR-STRICT-NEXT:     fn %[[VALUE_aliased_2:[0-9]+]] @aliased() -> i32 [linkage=external] [c="Function"];
// IR-STRICT-NEXT:     fn %[[VALUE_repeat:[0-9]+]] @repeat() -> void [linkage=external] [fallthrough=ret_void] {
// IR-STRICT-NEXT:     }
// IR-STRICT-NEXT: }
// SLATE-FILECHECK-END IR-STRICT
// SLATE-FILECHECK-BEGIN IR-LOW
// IR-LOW: module {
// IR-LOW-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-LOW-NEXT:         endian = little;
// IR-LOW-NEXT:         pointer [size=8, align=8];
// IR-LOW-NEXT:         stack_alignment = 16;
// IR-LOW-NEXT:         long_double = f64;
// IR-LOW-NEXT:         storage bool [size=1, align=1];
// IR-LOW-NEXT:         storage i8, u8 [size=1, align=1];
// IR-LOW-NEXT:         storage i16, u16 [size=2, align=2];
// IR-LOW-NEXT:         storage i32, u32 [size=4, align=4];
// IR-LOW-NEXT:         storage i64, u64 [size=8, align=8];
// IR-LOW-NEXT:         storage i128, u128 [size=16, align=16];
// IR-LOW-NEXT:         storage bf16 [size=2, align=2];
// IR-LOW-NEXT:         storage f16 [size=2, align=2];
// IR-LOW-NEXT:         storage f32 [size=4, align=4];
// IR-LOW-NEXT:         storage f64 [size=8, align=8];
// IR-LOW-NEXT:         storage f128 [size=16, align=16];
// IR-LOW-NEXT:         storage d32 [size=4, align=4];
// IR-LOW-NEXT:         storage d64 [size=8, align=8];
// IR-LOW-NEXT:         storage d128 [size=16, align=16];
// IR-LOW-NEXT:     }
// IR-LOW-NEXT:     type @type[[TYPE_Function:[0-9]+]] Function = fn() -> i32;
// IR-LOW-NEXT:     fn %[[VALUE_known:[0-9]+]] @known() -> i32 [linkage=external];
// IR-LOW-NEXT:     fn %[[VALUE_defined:[0-9]+]] @defined() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-LOW-NEXT:         return const<i32>(0);
// IR-LOW-NEXT:     }
// IR-LOW-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> i32 [linkage=external];
// IR-LOW-NEXT:     fn %[[VALUE_external:[0-9]+]] @external() -> i32 [linkage=external];
// IR-LOW-NEXT:     fn %[[VALUE_aliased:[0-9]+]] @aliased() -> i32 [linkage=external] [c="Function"];
// IR-LOW-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> void [linkage=external] [fallthrough=ret_void] {
// IR-LOW-NEXT:     }
// IR-LOW-NEXT:     fn %[[VALUE_aliased_2:[0-9]+]] @aliased() -> i32 [linkage=external] [c="Function"];
// IR-LOW-NEXT:     fn %[[VALUE_repeat:[0-9]+]] @repeat() -> void [linkage=external] [fallthrough=ret_void] {
// IR-LOW-NEXT:     }
// IR-LOW-NEXT: }
// SLATE-FILECHECK-END IR-LOW
