// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES FALSE_FILE FALSE_FILE
// SLATE-FILECHECK-DEFINES FALSE_BLOCK FALSE_BLOCK
// SLATE-FILECHECK-DEFINES NONCONSTANT NONCONSTANT
// SLATE-FILECHECK-DEFINES DIVZERO DIVZERO
// SLATE-FILECHECK-DEFINES FLOAT FLOAT
// SLATE-FILECHECK-DEFINES SHADOW SHADOW
// SLATE-FILECHECK-DEFINES SHADOW_TAG SHADOW_TAG
// SLATE-FILECHECK-DEFINES CALL CALL
// SLATE-FILECHECK-DEFINES CALL_POINTER CALL_POINTER
// SLATE-FILECHECK-DEFINES CALL_COMMA CALL_COMMA
// SLATE-FILECHECK-DEFINES CALL_LIBRARY CALL_LIBRARY
// SLATE-FILECHECK-DEFINES CONCAT CONCAT
// SLATE-FILECHECK-ERROR FALSE_FILE
// SLATE-FILECHECK-ERROR FALSE_BLOCK
// SLATE-FILECHECK-ERROR NONCONSTANT
// SLATE-FILECHECK-ERROR DIVZERO
// SLATE-FILECHECK-ERROR FLOAT
// SLATE-FILECHECK-ERROR SHADOW
// SLATE-FILECHECK-ERROR SHADOW_TAG
// SLATE-FILECHECK-ERROR CALL
// SLATE-FILECHECK-ERROR CALL_POINTER
// SLATE-FILECHECK-ERROR CALL_COMMA
// SLATE-FILECHECK-ERROR CALL_LIBRARY
// SLATE-FILECHECK-ERROR CONCAT
// SLATE-FILECHECK-ARGS -std=c23

#if defined(FALSE_FILE)
_Static_assert(2 + 2 == 5, "file assertion failed");
#elif defined(FALSE_BLOCK)
void block(void) { if (0) { static_assert(0, "unreachable still checked"); } }
#elif defined(NONCONSTANT)
void variable(int n) { static_assert(n, "not constant"); }
#elif defined(DIVZERO)
static_assert(1 / 0, "undefined expression");
#elif defined(FLOAT)
static_assert(1.0, "integer required");
#elif defined(SHADOW)
enum { VALUE = 1 };
void shadow(void) { int VALUE = 1; static_assert(VALUE, "object shadows enum"); }
#elif defined(SHADOW_TAG)
struct Tag { int a; };
void tag(void) { struct Tag { long long a, b; }; static_assert(sizeof(struct Tag) == 4, "inner tag"); }
#elif defined(CALL)
int f(void);
static_assert(1 && f(), "call is never constant");
#elif defined(CALL_POINTER)
int (*fp)(void);
static_assert((fp)(), "call through pointer");
#elif defined(CALL_COMMA)
int f(void);
static_assert((1, f()), "comma operand call");
#elif defined(CALL_LIBRARY)
int abs(int);
static_assert(abs(-2) == 2, "abs does not fold");
#elif defined(CONCAT)
#define NAMED(x) #x " named"
static_assert(1, "file " "scope");
void concat(void) { static_assert(1, "block " u8"scope"); }
static_assert(0, "pieces " NAMED(macro) L" joined");
#else
typedef unsigned char byte;
enum { FIRST = 2, SECOND = FIRST + 1 };
static_assert(SECOND == 3);
static_assert((byte)256 == 0);
static_assert(sizeof(byte) == 1);
static_assert(_Alignof(int) > 0);
static_assert((int)3.5 == 3);
static_assert(1 || (1 / 0));
static_assert(1 ? 7 : (1 / 0));
static_assert(-1 < 0 && ~(unsigned)0 > 0);
int opaque(void);
unsigned long strlen(const char *);
static_assert(1 || opaque());
static_assert(!(0 && opaque()));
static_assert(1 ? 1 : opaque());
static_assert(sizeof(opaque()) == sizeof(int));
static_assert(_Generic(1, int: 1, default: opaque()));
static_assert(__builtin_clz(8) == 28);
static_assert(strlen("ab") == 2);
static_assert(__builtin_choose_expr(1, 1, opaque()));
struct Outer { int a; };
void valid(int n) {
    static_assert(sizeof(n) == sizeof(int));
    { typedef short byte; static_assert(sizeof(byte) == sizeof(short)); }
    static_assert(sizeof(byte) == 1);
    { struct Outer { long long a, b; }; static_assert(sizeof(struct Outer) == 16); }
    static_assert(sizeof(struct Outer) == sizeof(int));
    { int SECOND = 1; (void)SECOND; }
    static_assert(SECOND == 3);
}
#endif

// SLATE-FILECHECK-IR-ERROR VALID

// SLATE-FILECHECK-BEGIN FALSE_FILE
// FALSE_FILE: Error:   × semantic analysis failed
// FALSE_FILE: Error:
// FALSE_FILE: × static assertion failed: file assertion failed
// FALSE_FILE: ╭─[tests/fixtures/sema/static_assert_validation.c:3:16]
// FALSE_FILE: 2 │ #if defined(FALSE_FILE)
// FALSE_FILE: 3 │ _Static_assert(2 + 2 == 5, "file assertion failed");
// FALSE_FILE: ·                ──────────
// FALSE_FILE: 4 │ #elif defined(FALSE_BLOCK)
// FALSE_FILE: ╰────
// SLATE-FILECHECK-END FALSE_FILE
// SLATE-FILECHECK-BEGIN FALSE_BLOCK
// FALSE_BLOCK: Error:   × semantic analysis failed
// FALSE_BLOCK: Error:
// FALSE_BLOCK: × static assertion failed: unreachable still checked
// FALSE_BLOCK: ╭─[tests/fixtures/sema/static_assert_validation.c:5:43]
// FALSE_BLOCK: 4 │ #elif defined(FALSE_BLOCK)
// FALSE_BLOCK: 5 │ void block(void) { if (0) { static_assert(0, "unreachable still checked"); } }
// FALSE_BLOCK: ·                                           ─
// FALSE_BLOCK: 6 │ #elif defined(NONCONSTANT)
// FALSE_BLOCK: ╰────
// SLATE-FILECHECK-END FALSE_BLOCK
// SLATE-FILECHECK-BEGIN NONCONSTANT
// NONCONSTANT: Error:   × semantic analysis failed
// NONCONSTANT: Error:
// NONCONSTANT: × static assertion requires an integer constant expression: nonconstant or
// NONCONSTANT: ╭─[tests/fixtures/sema/static_assert_validation.c:7:38]
// NONCONSTANT: 6 │ #elif defined(NONCONSTANT)
// NONCONSTANT: 7 │ void variable(int n) { static_assert(n, "not constant"); }
// NONCONSTANT: ·                                      ─
// NONCONSTANT: 8 │ #elif defined(DIVZERO)
// NONCONSTANT: ╰────
// SLATE-FILECHECK-END NONCONSTANT
// SLATE-FILECHECK-BEGIN DIVZERO
// DIVZERO: Error:   × semantic analysis failed
// DIVZERO: Error:
// DIVZERO: × static assertion requires an integer constant expression: nonconstant or
// DIVZERO: ╭─[tests/fixtures/sema/static_assert_validation.c:9:15]
// DIVZERO: 8 │ #elif defined(DIVZERO)
// DIVZERO: 9 │ static_assert(1 / 0, "undefined expression");
// DIVZERO: ·               ─────
// DIVZERO: 10 │ #elif defined(FLOAT)
// DIVZERO: ╰────
// SLATE-FILECHECK-END DIVZERO
// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × semantic analysis failed
// FLOAT: Error:
// FLOAT: × static assertion requires an integer constant expression: non-integer
// FLOAT: ╭─[tests/fixtures/sema/static_assert_validation.c:11:15]
// FLOAT: 10 │ #elif defined(FLOAT)
// FLOAT: 11 │ static_assert(1.0, "integer required");
// FLOAT: ·               ───
// FLOAT: 12 │ #elif defined(SHADOW)
// FLOAT: ╰────
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN SHADOW
// SHADOW: Error:   × semantic analysis failed
// SHADOW: Error:
// SHADOW: × static assertion requires an integer constant expression: nonconstant or
// SHADOW: ╭─[tests/fixtures/sema/static_assert_validation.c:14:50]
// SHADOW: 13 │ enum { VALUE = 1 };
// SHADOW: 14 │ void shadow(void) { int VALUE = 1; static_assert(VALUE, "object shadows enum"); }
// SHADOW: ·                                                  ─────
// SHADOW: 15 │ #elif defined(SHADOW_TAG)
// SHADOW: ╰────
// SLATE-FILECHECK-END SHADOW
// SLATE-FILECHECK-BEGIN SHADOW_TAG
// SHADOW_TAG: Error:   × semantic analysis failed
// SHADOW_TAG: Error:
// SHADOW_TAG: × static assertion failed: inner tag
// SHADOW_TAG: ╭─[tests/fixtures/sema/static_assert_validation.c:17:64]
// SHADOW_TAG: 16 │ struct Tag { int a; };
// SHADOW_TAG: 17 │ void tag(void) { struct Tag { long long a, b; }; static_assert(sizeof(struct Tag) == 4, "inner tag"); }
// SHADOW_TAG: ·                                                                ───────────────────────
// SHADOW_TAG: 18 │ #elif defined(CALL)
// SHADOW_TAG: ╰────
// SLATE-FILECHECK-END SHADOW_TAG
// SLATE-FILECHECK-BEGIN CALL
// CALL: Error:   × semantic analysis failed
// CALL: Error:
// CALL: × static assertion requires an integer constant expression: call to a
// CALL: ╭─[tests/fixtures/sema/static_assert_validation.c:20:20]
// CALL: 19 │ int f(void);
// CALL: 20 │ static_assert(1 && f(), "call is never constant");
// CALL: ·                    ───
// CALL: 21 │ #elif defined(CALL_POINTER)
// CALL: ╰────
// SLATE-FILECHECK-END CALL
// SLATE-FILECHECK-BEGIN CALL_POINTER
// CALL_POINTER: Error:   × semantic analysis failed
// CALL_POINTER: Error:
// CALL_POINTER: × static assertion requires an integer constant expression: call to a
// CALL_POINTER: ╭─[tests/fixtures/sema/static_assert_validation.c:23:15]
// CALL_POINTER: 22 │ int (*fp)(void);
// CALL_POINTER: 23 │ static_assert((fp)(), "call through pointer");
// CALL_POINTER: ·               ──────
// CALL_POINTER: 24 │ #elif defined(CALL_COMMA)
// CALL_POINTER: ╰────
// SLATE-FILECHECK-END CALL_POINTER
// SLATE-FILECHECK-BEGIN CALL_COMMA
// CALL_COMMA: Error:   × semantic analysis failed
// CALL_COMMA: Error:
// CALL_COMMA: × static assertion requires an integer constant expression: call to a
// CALL_COMMA: ╭─[tests/fixtures/sema/static_assert_validation.c:26:19]
// CALL_COMMA: 25 │ int f(void);
// CALL_COMMA: 26 │ static_assert((1, f()), "comma operand call");
// CALL_COMMA: ·                   ───
// CALL_COMMA: 27 │ #elif defined(CALL_LIBRARY)
// CALL_COMMA: ╰────
// SLATE-FILECHECK-END CALL_COMMA
// SLATE-FILECHECK-BEGIN CALL_LIBRARY
// CALL_LIBRARY: Error:   × semantic analysis failed
// CALL_LIBRARY: Error:
// CALL_LIBRARY: × static assertion requires an integer constant expression: call to a
// CALL_LIBRARY: ╭─[tests/fixtures/sema/static_assert_validation.c:29:15]
// CALL_LIBRARY: 28 │ int abs(int);
// CALL_LIBRARY: 29 │ static_assert(abs(-2) == 2, "abs does not fold");
// CALL_LIBRARY: ·               ───────
// CALL_LIBRARY: 30 │ #elif defined(CONCAT)
// CALL_LIBRARY: ╰────
// SLATE-FILECHECK-END CALL_LIBRARY
// SLATE-FILECHECK-BEGIN CONCAT
// CONCAT: Error:   × semantic analysis failed
// CONCAT: Error:
// CONCAT: × static assertion failed: pieces macro named joined
// CONCAT: ╭─[tests/fixtures/sema/static_assert_validation.c:34:15]
// CONCAT: 33 │ void concat(void) { static_assert(1, "block " u8"scope"); }
// CONCAT: 34 │ static_assert(0, "pieces " NAMED(macro) L" joined");
// CONCAT: ·               ─
// CONCAT: 35 │ #else
// CONCAT: ╰────
// SLATE-FILECHECK-END CONCAT
// SLATE-FILECHECK-BEGIN VALID
// VALID: Error:   × semantic analysis failed
// VALID: Error:
// VALID: × static assertion requires an integer constant expression: nonconstant call
// VALID: ╭─[tests/fixtures/sema/static_assert_validation.c:51:15]
// VALID: 50 │ static_assert(1 ? 1 : opaque());
// VALID: 51 │ static_assert(sizeof(opaque()) == sizeof(int));
// VALID: ·               ───────────────────────────────
// VALID: 52 │ static_assert(_Generic(1, int: 1, default: opaque()));
// VALID: ╰────
// SLATE-FILECHECK-END VALID
