


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
// DEFAULT-NEXT:     extern %0 ExternGlobalDecl: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %1 GlobalDef: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %2 GlobalInit: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %3 GlobalDeclInit: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %4 GlobalRedecl1: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %5 GlobalRedecl2: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %11 PrecedenceGlobal1A: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %12 PrecedenceGlobal1B: i32 [storage=static] [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %13 PrecedenceGlobal2A: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %14 PrecedenceGlobal2B: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %15 PrecedenceGlobalRedecl1: i32 [storage=static] = const<i32>(0) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %16 PrecedenceGlobalRedecl2: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %17 PrecedenceGlobalMixed1: i32 [storage=static] = const<i32>(1) [linkage=external] [dllexport];
// DEFAULT-NEXT:     global %18 PrecedenceGlobalMixed2: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %6 @def() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @inlineFunc() -> void [linkage=external] [dllexport] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @externInlineFunc() -> void [linkage=external] [dllexport] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @redecl1() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @redecl2() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @precedence1A() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @precedence1B() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @precedence2A() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @precedence2B() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @precedenceRedecl1() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @precedenceRedecl2() -> void [linkage=external] [dllexport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
