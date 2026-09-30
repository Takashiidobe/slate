// SLATE-FILECHECK-DEFINES THREAD_FUNCTION THREAD_FUNCTION
// SLATE-FILECHECK-ERROR THREAD_FUNCTION
// SLATE-FILECHECK-STD THREAD_FUNCTION c23
// SLATE-FILECHECK-DEFINES BLOCK_STATIC_FUNCTION BLOCK_STATIC_FUNCTION
// SLATE-FILECHECK-ERROR BLOCK_STATIC_FUNCTION
// SLATE-FILECHECK-STD BLOCK_STATIC_FUNCTION c23
// SLATE-FILECHECK-DEFINES THREAD_AUTO THREAD_AUTO
// SLATE-FILECHECK-ERROR THREAD_AUTO
// SLATE-FILECHECK-STD THREAD_AUTO c23
// SLATE-FILECHECK-DEFINES BLOCK_EXTERN_INIT BLOCK_EXTERN_INIT
// SLATE-FILECHECK-ERROR BLOCK_EXTERN_INIT
// SLATE-FILECHECK-STD BLOCK_EXTERN_INIT c23
// SLATE-FILECHECK-DEFINES MULTIPLE_INIT MULTIPLE_INIT
// SLATE-FILECHECK-ERROR MULTIPLE_INIT
// SLATE-FILECHECK-STD MULTIPLE_INIT c23
// SLATE-FILECHECK-DEFINES CONSTEXPR CONSTEXPR
// SLATE-FILECHECK-ERROR CONSTEXPR
// SLATE-FILECHECK-STD CONSTEXPR c23
// SLATE-FILECHECK-DEFINES VLA_INIT VLA_INIT
// SLATE-FILECHECK-ERROR VLA_INIT
// SLATE-FILECHECK-STD VLA_INIT c23
// SLATE-FILECHECK-DEFINES INCOMPLETE_ENUM INCOMPLETE_ENUM
// SLATE-FILECHECK-ERROR INCOMPLETE_ENUM
// SLATE-FILECHECK-STD INCOMPLETE_ENUM c23
// SLATE-FILECHECK-DEFINES VOID_TYPEDEF VOID_TYPEDEF
// SLATE-FILECHECK-ERROR VOID_TYPEDEF
// SLATE-FILECHECK-STD VOID_TYPEDEF c23
#if defined(THREAD_FUNCTION)
_Thread_local void f(void);
#elif defined(BLOCK_STATIC_FUNCTION)
void g(void) { static void h(void); }
#elif defined(THREAD_AUTO)
void g(void) { _Thread_local int x; }
#elif defined(BLOCK_EXTERN_INIT)
void g(void) { extern int x = 1; }
#elif defined(MULTIPLE_INIT)
int x = 1;
int x = 2;
#elif defined(CONSTEXPR)
constexpr int c;
#elif defined(VLA_INIT)
void g(int n) { int a[n] = {1}; }
#elif defined(INCOMPLETE_ENUM)
enum E;
void g(int x) { (enum E)x; }
#elif defined(VOID_TYPEDEF)
typedef void alias;
void g(void) { alias bad; }
#endif

// SLATE-FILECHECK-BEGIN THREAD_FUNCTION
// THREAD_FUNCTION: Error:   × semantic analysis failed
// THREAD_FUNCTION: Error:
// THREAD_FUNCTION: × thread-local function
// THREAD_FUNCTION: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:2:20]
// THREAD_FUNCTION: 1 │ #if defined(THREAD_FUNCTION)
// THREAD_FUNCTION: 2 │ _Thread_local void f(void);
// THREAD_FUNCTION: ·                    ───────
// THREAD_FUNCTION: 3 │ #elif defined(BLOCK_STATIC_FUNCTION)
// THREAD_FUNCTION: ╰────
// SLATE-FILECHECK-END THREAD_FUNCTION
// SLATE-FILECHECK-BEGIN BLOCK_STATIC_FUNCTION
// BLOCK_STATIC_FUNCTION: Error:   × semantic analysis failed
// BLOCK_STATIC_FUNCTION: Error:
// BLOCK_STATIC_FUNCTION: × block scope static function
// BLOCK_STATIC_FUNCTION: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:4:28]
// BLOCK_STATIC_FUNCTION: 3 │ #elif defined(BLOCK_STATIC_FUNCTION)
// BLOCK_STATIC_FUNCTION: 4 │ void g(void) { static void h(void); }
// BLOCK_STATIC_FUNCTION: ·                            ───────
// BLOCK_STATIC_FUNCTION: 5 │ #elif defined(THREAD_AUTO)
// BLOCK_STATIC_FUNCTION: ╰────
// SLATE-FILECHECK-END BLOCK_STATIC_FUNCTION
// SLATE-FILECHECK-BEGIN THREAD_AUTO
// THREAD_AUTO: Error:   × semantic analysis failed
// THREAD_AUTO: Error:
// THREAD_AUTO: × thread-local automatic variable
// THREAD_AUTO: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:6:34]
// THREAD_AUTO: 5 │ #elif defined(THREAD_AUTO)
// THREAD_AUTO: 6 │ void g(void) { _Thread_local int x; }
// THREAD_AUTO: ·                                  ─
// THREAD_AUTO: 7 │ #elif defined(BLOCK_EXTERN_INIT)
// THREAD_AUTO: ╰────
// SLATE-FILECHECK-END THREAD_AUTO
// SLATE-FILECHECK-BEGIN BLOCK_EXTERN_INIT
// BLOCK_EXTERN_INIT: Error:   × semantic analysis failed
// BLOCK_EXTERN_INIT: Error:
// BLOCK_EXTERN_INIT: × block scope extern initializer
// BLOCK_EXTERN_INIT: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:8:27]
// BLOCK_EXTERN_INIT: 7 │ #elif defined(BLOCK_EXTERN_INIT)
// BLOCK_EXTERN_INIT: 8 │ void g(void) { extern int x = 1; }
// BLOCK_EXTERN_INIT: ·                           ─────
// BLOCK_EXTERN_INIT: 9 │ #elif defined(MULTIPLE_INIT)
// BLOCK_EXTERN_INIT: ╰────
// SLATE-FILECHECK-END BLOCK_EXTERN_INIT
// SLATE-FILECHECK-BEGIN MULTIPLE_INIT
// MULTIPLE_INIT: Error:   × semantic analysis failed
// MULTIPLE_INIT: Error:
// MULTIPLE_INIT: × multiple global initializers
// MULTIPLE_INIT: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:11:5]
// MULTIPLE_INIT: 10 │ int x = 1;
// MULTIPLE_INIT: 11 │ int x = 2;
// MULTIPLE_INIT: ·     ─────
// MULTIPLE_INIT: 12 │ #elif defined(CONSTEXPR)
// MULTIPLE_INIT: ╰────
// SLATE-FILECHECK-END MULTIPLE_INIT
// SLATE-FILECHECK-BEGIN CONSTEXPR
// CONSTEXPR: Error:   × semantic analysis failed
// CONSTEXPR: Error:
// CONSTEXPR: × constexpr object requires an initializer
// CONSTEXPR: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:13:15]
// CONSTEXPR: 12 │ #elif defined(CONSTEXPR)
// CONSTEXPR: 13 │ constexpr int c;
// CONSTEXPR: ·               ─
// CONSTEXPR: 14 │ #elif defined(VLA_INIT)
// CONSTEXPR: ╰────
// SLATE-FILECHECK-END CONSTEXPR
// SLATE-FILECHECK-BEGIN VLA_INIT
// VLA_INIT: Error:   × semantic analysis failed
// VLA_INIT: Error:
// VLA_INIT: × variable length array initializer
// VLA_INIT: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:15:21]
// VLA_INIT: 14 │ #elif defined(VLA_INIT)
// VLA_INIT: 15 │ void g(int n) { int a[n] = {1}; }
// VLA_INIT: ·                     ──────────
// VLA_INIT: 16 │ #elif defined(INCOMPLETE_ENUM)
// VLA_INIT: ╰────
// SLATE-FILECHECK-END VLA_INIT
// SLATE-FILECHECK-BEGIN INCOMPLETE_ENUM
// INCOMPLETE_ENUM: Error:   × semantic analysis failed
// INCOMPLETE_ENUM: Error:
// INCOMPLETE_ENUM: × conversion involving an incomplete enum type
// INCOMPLETE_ENUM: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:18:17]
// INCOMPLETE_ENUM: 17 │ enum E;
// INCOMPLETE_ENUM: 18 │ void g(int x) { (enum E)x; }
// INCOMPLETE_ENUM: ·                 ─────────
// INCOMPLETE_ENUM: 19 │ #elif defined(VOID_TYPEDEF)
// INCOMPLETE_ENUM: ╰────
// SLATE-FILECHECK-END INCOMPLETE_ENUM
// SLATE-FILECHECK-BEGIN VOID_TYPEDEF
// VOID_TYPEDEF: Error:   × semantic analysis failed
// VOID_TYPEDEF: Error:
// VOID_TYPEDEF: × object cannot have type void
// VOID_TYPEDEF: ╭─[tests/fixtures/error/clang/linux/x86_64/declaration_rules.c:21:22]
// VOID_TYPEDEF: 20 │ typedef void alias;
// VOID_TYPEDEF: 21 │ void g(void) { alias bad; }
// VOID_TYPEDEF: ·                      ───
// VOID_TYPEDEF: 22 │ #endif
// VOID_TYPEDEF: ╰────
// SLATE-FILECHECK-END VOID_TYPEDEF
