#ifdef UNKNOWN
int unknown[] = {
#embed "pp-embed-parameter-errors.bin" clang::offset(1)
};
#endif
#ifdef NO_CLAUSE
#if __has_embed("pp-embed-parameter-errors.bin" prefix)
#endif
#endif
#ifdef NEGATIVE
#if __has_embed("pp-embed-parameter-errors.bin" limit(-1))
#endif
#endif
int value;

// SLATE-FILECHECK-DEFINES UNKNOWN UNKNOWN
// SLATE-FILECHECK-ERROR UNKNOWN
// SLATE-FILECHECK-DEFINES NO_CLAUSE NO_CLAUSE
// SLATE-FILECHECK-ERROR NO_CLAUSE
// SLATE-FILECHECK-DEFINES NEGATIVE NEGATIVE
// SLATE-FILECHECK-ERROR NEGATIVE
// SLATE-FILECHECK-STD UNKNOWN c23
// SLATE-FILECHECK-STD NO_CLAUSE c23
// SLATE-FILECHECK-STD NEGATIVE c23

// SLATE-FILECHECK-BEGIN UNKNOWN
// UNKNOWN: Error:   × unknown #embed parameter `clang::offset`
// UNKNOWN: ╰─▶ unknown #embed parameter `clang::offset`
// UNKNOWN: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-embed-parameter-errors.c:3:40]
// UNKNOWN: 2 │ int unknown[] = {
// UNKNOWN: 3 │ #embed "pp-embed-parameter-errors.bin" clang::offset(1)
// UNKNOWN: ·                                        ─────
// UNKNOWN: 4 │ };
// UNKNOWN: ╰────
// SLATE-FILECHECK-END UNKNOWN
// SLATE-FILECHECK-BEGIN NO_CLAUSE
// NO_CLAUSE: Error:   × invalid #embed parameter
// NO_CLAUSE: ╰─▶ invalid #embed parameter
// NO_CLAUSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-embed-parameter-errors.c:7:49]
// NO_CLAUSE: 6 │ #ifdef NO_CLAUSE
// NO_CLAUSE: 7 │ #if __has_embed("pp-embed-parameter-errors.bin" prefix)
// NO_CLAUSE: ·                                                 ──────
// NO_CLAUSE: 8 │ #endif
// NO_CLAUSE: ╰────
// SLATE-FILECHECK-END NO_CLAUSE
// SLATE-FILECHECK-BEGIN NEGATIVE
// NEGATIVE: Error:   × negative #embed parameter operand
// NEGATIVE: ╰─▶ negative #embed parameter operand
// NEGATIVE: ╭─[tests/fixtures/error/gcc/linux/x86_64/pp-embed-parameter-errors.c:11:49]
// NEGATIVE: 10 │ #ifdef NEGATIVE
// NEGATIVE: 11 │ #if __has_embed("pp-embed-parameter-errors.bin" limit(-1))
// NEGATIVE: ·                                                 ─────
// NEGATIVE: 12 │ #endif
// NEGATIVE: ╰────
// SLATE-FILECHECK-END NEGATIVE
