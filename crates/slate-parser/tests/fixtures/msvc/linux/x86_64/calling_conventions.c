void __cdecl caller(void);
void __stdcall callee(void);
void __fastcall fast(void);
void __vectorcall vector(void);
void __thiscall method(void *);
typedef void (__cdecl *callback)(void);
void (__stdcall *pointer)(void);
void (__fastcall *fast_pointer)(void);
void (__vectorcall *vector_pointer)(void);
void (__thiscall *method_pointer)(void *);
void accepts(void (__stdcall *callback)(int));
__declspec(dllimport) int imported;
__declspec(dllexport) void exported(void);
__declspec(align(16)) int aligned;
__declspec(dllimport align(32)) int combined;
struct callbacks { void (__stdcall *callback)(int); };
void __cdecl definition(void) { __declspec(align(16)) int local; }
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
// DEFAULT-NEXT:     type @type[[TYPE_callback:[0-9]+]] callback = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_callbacks:[0-9]+]] callbacks = struct {
// DEFAULT-NEXT:         field0 callback: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_pointer:[0-9]+]] pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fast_pointer:[0-9]+]] fast_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vector_pointer:[0-9]+]] vector_pointer: ptr<fn vectorcall() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_method_pointer:[0-9]+]] method_pointer: ptr<fn(ptr<void>) -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_imported:[0-9]+]] imported: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_aligned:[0-9]+]] aligned: i32 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_combined:[0-9]+]] combined: i32 [storage=static] [align=32] [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_caller:[0-9]+]] @caller() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_callee:[0-9]+]] @callee() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_vector:[0-9]+]] @vector() -> void [linkage=external] [abi=sysv64 vectorcall() -> void];
// DEFAULT-NEXT:     fn %[[VALUE_method:[0-9]+]] @method(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_accepts:[0-9]+]] @accepts(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exported:[0-9]+]] @exported() -> void [linkage=external] [dllexport];
// DEFAULT-NEXT:     fn %[[VALUE_definition:[0-9]+]] @definition() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic] [align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
