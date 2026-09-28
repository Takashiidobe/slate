// SLATE-FILECHECK-DEFINES FIELD -DFIELD
// SLATE-FILECHECK-DEFINES INCOMPLETE -DINCOMPLETE
// SLATE-FILECHECK-DEFINES BITFIELD -DBITFIELD
// SLATE-FILECHECK-DEFINES WIDTH -DWIDTH
// SLATE-FILECHECK-ERROR FIELD
// SLATE-FILECHECK-ERROR INCOMPLETE
// SLATE-FILECHECK-ERROR BITFIELD
// SLATE-FILECHECK-ERROR WIDTH
// SLATE-FILECHECK-ARGS --dump-ir
struct S { int field; unsigned bits : 2; };
#ifdef FIELD
unsigned long bad(void) { return __builtin_offsetof(struct S, missing); }
#endif
#ifdef INCOMPLETE
struct Incomplete;
unsigned long bad(void) { return sizeof(struct Incomplete); }
#endif
#ifdef BITFIELD
unsigned long bad(void) { return __builtin_offsetof(struct S, bits); }
#endif
#ifdef WIDTH
void bad(void) { (_BitInt(sizeof(int) - 4))1; }
#endif

// SLATE-FILECHECK-BEGIN FIELD
// FIELD: Error:   × semantic analysis failed
// FIELD: Error:
// FIELD: × unsupported in numeric IR lowering: unknown offsetof member
// FIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:3:34]
// FIELD: 2 │ #ifdef FIELD
// FIELD: 3 │ unsigned long bad(void) { return __builtin_offsetof(struct S, missing); }
// FIELD: ·                                  ─────────────────────────────────────
// FIELD: 4 │ #endif
// FIELD: ╰────
// SLATE-FILECHECK-END FIELD
// SLATE-FILECHECK-BEGIN INCOMPLETE
// INCOMPLETE: Error:   × semantic analysis failed
// INCOMPLETE: Error:
// INCOMPLETE: × unsupported in numeric IR lowering: sizeof of incomplete type
// INCOMPLETE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:7:34]
// INCOMPLETE: 6 │ struct Incomplete;
// INCOMPLETE: 7 │ unsigned long bad(void) { return sizeof(struct Incomplete); }
// INCOMPLETE: ·                                  ─────────────────────────
// INCOMPLETE: 8 │ #endif
// INCOMPLETE: ╰────
// SLATE-FILECHECK-END INCOMPLETE
// SLATE-FILECHECK-BEGIN BITFIELD
// BITFIELD: Error:   × semantic analysis failed
// BITFIELD: Error:
// BITFIELD: × unsupported in numeric IR lowering: offsetof bit-field
// BITFIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:10:34]
// BITFIELD: 9 │ #ifdef BITFIELD
// BITFIELD: 10 │ unsigned long bad(void) { return __builtin_offsetof(struct S, bits); }
// BITFIELD: ·                                  ──────────────────────────────────
// BITFIELD: 11 │ #endif
// BITFIELD: ╰────
// SLATE-FILECHECK-END BITFIELD
// SLATE-FILECHECK-BEGIN WIDTH
// WIDTH: Error:   × semantic analysis failed
// WIDTH: Error:
// WIDTH: × unsupported in numeric IR lowering: invalid _BitInt width
// WIDTH: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:13:18]
// WIDTH: 12 │ #ifdef WIDTH
// WIDTH: 13 │ void bad(void) { (_BitInt(sizeof(int) - 4))1; }
// WIDTH: ·                  ───────────────────────────
// WIDTH: 14 │ #endif
// WIDTH: ╰────
// SLATE-FILECHECK-END WIDTH
