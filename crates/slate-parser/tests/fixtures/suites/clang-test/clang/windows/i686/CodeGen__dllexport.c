


//===----------------------------------------------------------------------===//
// Globals
//===----------------------------------------------------------------------===//

// Declarations are not exported.
__declspec(dllexport) extern int ExternGlobalDecl;

// dllexport implies a definition.
__declspec(dllexport) int GlobalDef;

// Export definition.
__declspec(dllexport) int GlobalInit = 1;

// Declare, then export definition.
__declspec(dllexport) extern int GlobalDeclInit;
int GlobalDeclInit = 1;

// Redeclarations
__declspec(dllexport) extern int GlobalRedecl1;
__declspec(dllexport)        int GlobalRedecl1;

__declspec(dllexport) extern int GlobalRedecl2;
                             int GlobalRedecl2;



//===----------------------------------------------------------------------===//
// Functions
//===----------------------------------------------------------------------===//

// Declarations are not exported.

// Export function definition.
__declspec(dllexport) void def(void) {}

// Export inline function.
__declspec(dllexport) inline void inlineFunc(void) {}
__declspec(dllexport) inline void externInlineFunc(void) {}
extern void externInlineFunc(void);

// Redeclarations
__declspec(dllexport) void redecl1(void);
__declspec(dllexport) void redecl1(void) {}

__declspec(dllexport) void redecl2(void);
                      void redecl2(void) {}



//===----------------------------------------------------------------------===//
// Precedence
//===----------------------------------------------------------------------===//

// dllexport takes precedence over the dllimport if both are specified.
__attribute__((dllimport, dllexport))       int PrecedenceGlobal1A;
__declspec(dllimport) __declspec(dllexport) int PrecedenceGlobal1B;

__attribute__((dllexport, dllimport))       int PrecedenceGlobal2A;
__declspec(dllexport) __declspec(dllimport) int PrecedenceGlobal2B;

__declspec(dllexport) extern int PrecedenceGlobalRedecl1;
__declspec(dllimport)        int PrecedenceGlobalRedecl1 = 0;

__declspec(dllimport) extern int PrecedenceGlobalRedecl2;
__declspec(dllexport)        int PrecedenceGlobalRedecl2;

__attribute__((dllexport)) extern int PrecedenceGlobalMixed1;
__declspec(dllimport)             int PrecedenceGlobalMixed1 = 1;

__attribute__((dllimport)) extern int PrecedenceGlobalMixed2;
__declspec(dllexport)             int PrecedenceGlobalMixed2;

void __attribute__((dllimport, dllexport))       precedence1A(void) {}
void __declspec(dllimport) __declspec(dllexport) precedence1B(void) {}

void __attribute__((dllexport, dllimport))       precedence2A(void) {}
void __declspec(dllexport) __declspec(dllimport) precedence2B(void) {}

void __declspec(dllimport) precedenceRedecl1(void);
void __declspec(dllexport) precedenceRedecl1(void) {}

void __declspec(dllexport) precedenceRedecl2(void);
void __declspec(dllimport) precedenceRedecl2(void) {}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c11

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
// DEFAULT-NEXT:     extern %[[VALUE_ExternGlobalDecl:[0-9]+]] ExternGlobalDecl: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalDef:[0-9]+]] GlobalDef: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalInit:[0-9]+]] GlobalInit: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalDeclInit:[0-9]+]] GlobalDeclInit: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalRedecl1:[0-9]+]] GlobalRedecl1: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalRedecl2:[0-9]+]] GlobalRedecl2: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobal1A:[0-9]+]] PrecedenceGlobal1A: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobal1B:[0-9]+]] PrecedenceGlobal1B: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobal2A:[0-9]+]] PrecedenceGlobal2A: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobal2B:[0-9]+]] PrecedenceGlobal2B: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobalRedecl1:[0-9]+]] PrecedenceGlobalRedecl1: i32 [storage=static] = const<i32>(0) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobalRedecl2:[0-9]+]] PrecedenceGlobalRedecl2: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobalMixed1:[0-9]+]] PrecedenceGlobalMixed1: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %[[VALUE_PrecedenceGlobalMixed2:[0-9]+]] PrecedenceGlobalMixed2: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_def:[0-9]+]] @def() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_inlineFunc:[0-9]+]] @inlineFunc() -> void [linkage=external] [dllexport] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_externInlineFunc:[0-9]+]] @externInlineFunc() -> void [linkage=external] [dllexport] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl1:[0-9]+]] @redecl1() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl2:[0-9]+]] @redecl2() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_precedence1A:[0-9]+]] @precedence1A() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_precedence1B:[0-9]+]] @precedence1B() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_precedence2A:[0-9]+]] @precedence2A() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_precedence2B:[0-9]+]] @precedence2B() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_precedenceRedecl1:[0-9]+]] @precedenceRedecl1() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_precedenceRedecl2:[0-9]+]] @precedenceRedecl2() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
