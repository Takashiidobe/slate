
#define JOIN2(x, y) x##y
#define JOIN(x, y) JOIN2(x, y)
#define USEVAR(var) int JOIN(use, __LINE__)(void) { return var; }
#define USE(func) void JOIN(use, __LINE__)(void) { func(); }



//===----------------------------------------------------------------------===//
// Globals
//===----------------------------------------------------------------------===//

// Import declaration.
__declspec(dllimport) extern int ExternGlobalDecl;
USEVAR(ExternGlobalDecl)

// dllimport implies a declaration.
__declspec(dllimport) int GlobalDecl;
USEVAR(GlobalDecl)

// Redeclarations
__declspec(dllimport) extern int GlobalRedecl1;
__declspec(dllimport) extern int GlobalRedecl1;
USEVAR(GlobalRedecl1)

__declspec(dllimport) int GlobalRedecl2;
__declspec(dllimport) int GlobalRedecl2;
USEVAR(GlobalRedecl2)

// NB: MSVC issues a warning and makes GlobalRedecl3 dllexport. We follow GCC
// and drop the dllimport with a warning.
__declspec(dllimport) extern int GlobalRedecl3;
                      extern int GlobalRedecl3; // dllimport ignored
USEVAR(GlobalRedecl3)

// Make sure this works even if the decl has been used before it's defined (PR20792).
__declspec(dllimport) extern int GlobalRedecl4;
USEVAR(GlobalRedecl4)
                      int GlobalRedecl4; // dllimport ignored

// FIXME: dllimport is dropped in the AST; this should be reflected in codegen (PR02803).
__declspec(dllimport) extern int GlobalRedecl5;
USEVAR(GlobalRedecl5)
                      extern int GlobalRedecl5; // dllimport ignored

// Redeclaration in local context.
__declspec(dllimport) int GlobalRedecl6;
int functionScope(void) {
  extern int GlobalRedecl6; // still dllimport
  return GlobalRedecl6;
}



//===----------------------------------------------------------------------===//
// Functions
//===----------------------------------------------------------------------===//

// Import function declaration.
__declspec(dllimport) void decl(void);

// Initialize use_decl with the address of the thunk.
void (*use_decl)(void) = &decl;

// Import inline function.
__declspec(dllimport) inline void inlineFunc(void) {}
USE(inlineFunc)

// inline attributes
__declspec(dllimport) __attribute__((noinline)) inline void noinline(void) {}
__declspec(dllimport) __attribute__((always_inline)) inline void alwaysInline(void) {}
USE(noinline)
USE(alwaysInline)

// Redeclarations
__declspec(dllimport) void redecl1(void);
__declspec(dllimport) void redecl1(void);
USE(redecl1)

// NB: MSVC issues a warning and makes redecl2/redecl3 dllexport. We follow GCC
// and drop the dllimport with a warning.
__declspec(dllimport) void redecl2(void);
                      void redecl2(void);
USE(redecl2)

__declspec(dllimport) void redecl3(void);
                      void redecl3(void) {} // dllimport ignored
USE(redecl3)

// Make sure this works even if the decl is used before it's defined (PR20792).
__declspec(dllimport) void redecl4(void);
USE(redecl4)
                      void redecl4(void) {} // dllimport ignored

// FIXME: dllimport is dropped in the AST; this should be reflected in codegen (PR20803).
__declspec(dllimport) void redecl5(void);
USE(redecl5)
                      void redecl5(void); // dllimport ignored

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c11

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
// DEFAULT-NEXT:     extern %[[VALUE_ExternGlobalDecl:[0-9]+]] ExternGlobalDecl: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalDecl:[0-9]+]] GlobalDecl: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     extern %[[VALUE_GlobalRedecl1:[0-9]+]] GlobalRedecl1: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalRedecl2:[0-9]+]] GlobalRedecl2: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     extern %[[VALUE_GlobalRedecl3:[0-9]+]] GlobalRedecl3: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalRedecl4:[0-9]+]] GlobalRedecl4: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     extern %[[VALUE_GlobalRedecl5:[0-9]+]] GlobalRedecl5: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_GlobalRedecl6:[0-9]+]] GlobalRedecl6: i32 [storage=static] [linkage=external] [dllimport];
// DEFAULT-NEXT:     global %[[VALUE_use_decl:[0-9]+]] use_decl: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_decl:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_use15:[0-9]+]] @use15() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_ExternGlobalDecl]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use19:[0-9]+]] @use19() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalDecl]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use24:[0-9]+]] @use24() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalRedecl1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use28:[0-9]+]] @use28() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalRedecl2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use34:[0-9]+]] @use34() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalRedecl3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use38:[0-9]+]] @use38() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalRedecl4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use43:[0-9]+]] @use43() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalRedecl5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_functionScope:[0-9]+]] @functionScope() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_GlobalRedecl6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_decl]] @decl() -> void [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_inlineFunc:[0-9]+]] @inlineFunc() -> void [linkage=external] [dllimport] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use67:[0-9]+]] @use67() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_inlineFunc]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_noinline:[0-9]+]] @noinline() -> void [linkage=external] [dllimport] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_alwaysInline:[0-9]+]] @alwaysInline() -> void [linkage=external] [dllimport] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use72:[0-9]+]] @use72() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_noinline]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use73:[0-9]+]] @use73() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_alwaysInline]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl1:[0-9]+]] @redecl1() -> void [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_use78:[0-9]+]] @use78() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_redecl1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl2:[0-9]+]] @redecl2() -> void [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_use84:[0-9]+]] @use84() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_redecl2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl3:[0-9]+]] @redecl3() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use88:[0-9]+]] @use88() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_redecl3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl4:[0-9]+]] @redecl4() -> void [linkage=external] [dllimport] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use92:[0-9]+]] @use92() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_redecl4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_redecl5:[0-9]+]] @redecl5() -> void [linkage=external] [dllimport];
// DEFAULT-NEXT:     fn %[[VALUE_use97:[0-9]+]] @use97() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_redecl5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
