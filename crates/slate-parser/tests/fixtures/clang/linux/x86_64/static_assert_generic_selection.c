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
// DEFAULT_BRANCH: ╭─[tests/fixtures/clang/linux/x86_64/static_assert_generic_selection.c:3:39]
// DEFAULT_BRANCH: 2 │ #if defined(DEFAULT_BRANCH)
// DEFAULT_BRANCH: 3 │ void pointer(char *p) { static_assert(_Generic(p, int: 1, default: 0), "pointer selects default"); }
// DEFAULT_BRANCH: ·                                       ───────────────────────────────
// DEFAULT_BRANCH: 4 │ #else
// DEFAULT_BRANCH: ╰────
// SLATE-FILECHECK-END DEFAULT_BRANCH
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     type @type[[TYPE_byte:[0-9]+]] byte = u8;
// VALID-NEXT:     extern %[[VALUE_table:[0-9]+]] table: array<i32, 4> [storage=static] [align=16] [linkage=external];
// VALID-NEXT:     fn %[[VALUE_routine:[0-9]+]] @routine() -> void [linkage=external];
// VALID-NEXT:     fn %[[VALUE_selected:[0-9]+]] @selected(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// VALID-NEXT:         let %[[VALUE_a:[0-9]+]] a: u32 [storage=automatic];
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
