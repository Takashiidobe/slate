// SLATE-FILECHECK-DEFINES FIELD -DFIELD
// SLATE-FILECHECK-DEFINES INCOMPLETE -DINCOMPLETE
// SLATE-FILECHECK-DEFINES BITFIELD -DBITFIELD
// SLATE-FILECHECK-DEFINES WIDTH -DWIDTH
// SLATE-FILECHECK-DEFINES ANON_BITFIELD -DANON_BITFIELD
// SLATE-FILECHECK-DEFINES FLOAT_INDEX -DFLOAT_INDEX
// SLATE-FILECHECK-ERROR FIELD
// SLATE-FILECHECK-ERROR INCOMPLETE
// SLATE-FILECHECK-ERROR BITFIELD
// SLATE-FILECHECK-ERROR WIDTH
// SLATE-FILECHECK-ERROR ANON_BITFIELD
// SLATE-FILECHECK-ERROR FLOAT_INDEX
// SLATE-FILECHECK-ARGS --dump-ir
struct S { int field; unsigned bits : 2; struct { unsigned flag : 1; }; };
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
#ifdef ANON_BITFIELD
unsigned long bad(void) { return __builtin_offsetof(struct S, flag); }
#endif
#ifdef FLOAT_INDEX
struct Array { long items[4]; };
unsigned long bad(double index) { return __builtin_offsetof(struct Array, items[index]); }
#endif

// SLATE-FILECHECK-BEGIN FIELD
// FIELD: Error:   × semantic analysis failed
// FIELD: Error:
// FIELD: × unknown offsetof member
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
// INCOMPLETE: × sizeof of incomplete type
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
// BITFIELD: × offsetof bit-field
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
// WIDTH: × invalid _BitInt width
// WIDTH: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:13:18]
// WIDTH: 12 │ #ifdef WIDTH
// WIDTH: 13 │ void bad(void) { (_BitInt(sizeof(int) - 4))1; }
// WIDTH: ·                  ───────────────────────────
// WIDTH: 14 │ #endif
// WIDTH: ╰────
// SLATE-FILECHECK-END WIDTH
// SLATE-FILECHECK-BEGIN ANON_BITFIELD
// ANON_BITFIELD: Error:   × semantic analysis failed
// ANON_BITFIELD: Error:
// ANON_BITFIELD: × offsetof bit-field
// ANON_BITFIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:16:34]
// ANON_BITFIELD: 15 │ #ifdef ANON_BITFIELD
// ANON_BITFIELD: 16 │ unsigned long bad(void) { return __builtin_offsetof(struct S, flag); }
// ANON_BITFIELD: ·                                  ──────────────────────────────────
// ANON_BITFIELD: 17 │ #endif
// ANON_BITFIELD: ╰────
// SLATE-FILECHECK-END ANON_BITFIELD
// SLATE-FILECHECK-BEGIN FLOAT_INDEX
// FLOAT_INDEX: Error:   × semantic analysis failed
// FLOAT_INDEX: Error:
// FLOAT_INDEX: × offsetof index is not an integer
// FLOAT_INDEX: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_layout_constants.c:20:42]
// FLOAT_INDEX: 19 │ struct Array { long items[4]; };
// FLOAT_INDEX: 20 │ unsigned long bad(double index) { return __builtin_offsetof(struct Array, items[index]); }
// FLOAT_INDEX: ·                                          ──────────────────────────────────────────────
// FLOAT_INDEX: 21 │ #endif
// FLOAT_INDEX: ╰────
// SLATE-FILECHECK-END FLOAT_INDEX
