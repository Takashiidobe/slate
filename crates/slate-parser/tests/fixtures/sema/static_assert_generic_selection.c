// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES DEFAULT_BRANCH DEFAULT_BRANCH
// SLATE-FILECHECK-ERROR DEFAULT_BRANCH
// SLATE-FILECHECK-ARGS -std=c23

#if defined(DEFAULT_BRANCH)
void pointer(char *p) { static_assert(_Generic(p, int: 1, default: 0), "pointer selects default"); }
#else
typedef unsigned char byte;
static_assert(_Generic((int)0, int: 1, default: 0));
static_assert(_Generic(0, long: 0, default: 1));
static_assert(_Generic((byte)0, unsigned char: 1, default: 0));
static_assert(_Generic(1.0, double: 1, default: 0));
extern int table[4];
extern void routine(void);
static_assert(_Generic(table, int *: 1, default: 0));
static_assert(_Generic(routine, void (*)(void): 1, default: 0));
static_assert(_Generic(int, int: 1, default: 0));
static_assert(_Generic(0, int: _Generic(0.0f, float: 1, default: 0), default: 0));
void selected(int n) {
    unsigned int a;
    static_assert(_Generic(__builtin_add_overflow(1, 1, &a), bool: 1, default: 0));
    static_assert(_Generic(__builtin_mul_overflow(1, 1, &a), bool: 1, default: 0));
    static_assert(_Generic(n, int: sizeof(int), default: 0) == sizeof(int));
}
#endif

// SLATE-FILECHECK-BEGIN DEFAULT_BRANCH
// DEFAULT_BRANCH: Error:   × semantic analysis failed
// DEFAULT_BRANCH: Error:
// DEFAULT_BRANCH: × static assertion failed: pointer selects default
// DEFAULT_BRANCH: ╭─[tests/fixtures/sema/static_assert_generic_selection.c:3:39]
// DEFAULT_BRANCH: 2 │ #if defined(DEFAULT_BRANCH)
// DEFAULT_BRANCH: 3 │ void pointer(char *p) { static_assert(_Generic(p, int: 1, default: 0), "pointer selects default"); }
// DEFAULT_BRANCH: ·                                       ───────────────────────────────
// DEFAULT_BRANCH: 4 │ #else
// DEFAULT_BRANCH: ╰────
// SLATE-FILECHECK-END DEFAULT_BRANCH
