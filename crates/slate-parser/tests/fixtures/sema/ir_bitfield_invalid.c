// SLATE-FILECHECK-DEFINES SIZEOF SIZEOF
// SLATE-FILECHECK-DEFINES ALIGNOF ALIGNOF
// SLATE-FILECHECK-DEFINES ADDRESS ADDRESS
// SLATE-FILECHECK-ERROR SIZEOF
// SLATE-FILECHECK-ERROR ALIGNOF
// SLATE-FILECHECK-ERROR ADDRESS
// SLATE-FILECHECK-ARGS --dump-ir

struct Flags { unsigned low : 3; int plain; };

unsigned long invalid(struct Flags *f) {
#ifdef SIZEOF
    return sizeof(f->low);
#endif
#ifdef ALIGNOF
    return _Alignof(f->low);
#endif
#ifdef ADDRESS
    return (unsigned long)&f->low;
#endif
}

// SLATE-FILECHECK-BEGIN SIZEOF
// SIZEOF: Error:   × semantic analysis failed
// SIZEOF: Error:
// SIZEOF: × invalid in this context: application of sizeof or alignof to a bit-field
// SIZEOF: ╭─[tests/fixtures/sema/ir_bitfield_invalid.c:6:12]
// SIZEOF: 5 │ #ifdef SIZEOF
// SIZEOF: 6 │     return sizeof(f->low);
// SIZEOF: ·            ──────────────
// SIZEOF: 7 │ #endif
// SIZEOF: ╰────
// SLATE-FILECHECK-END SIZEOF
// SLATE-FILECHECK-BEGIN ALIGNOF
// ALIGNOF: Error:   × semantic analysis failed
// ALIGNOF: Error:
// ALIGNOF: × invalid in this context: application of sizeof or alignof to a bit-field
// ALIGNOF: ╭─[tests/fixtures/sema/ir_bitfield_invalid.c:9:12]
// ALIGNOF: 8 │ #ifdef ALIGNOF
// ALIGNOF: 9 │     return _Alignof(f->low);
// ALIGNOF: ·            ────────────────
// ALIGNOF: 10 │ #endif
// ALIGNOF: ╰────
// SLATE-FILECHECK-END ALIGNOF
// SLATE-FILECHECK-BEGIN ADDRESS
// ADDRESS: Error:   × semantic analysis failed
// ADDRESS: Error:
// ADDRESS: × invalid in this context: address of a bit-field
// ADDRESS: ╭─[tests/fixtures/sema/ir_bitfield_invalid.c:12:27]
// ADDRESS: 11 │ #ifdef ADDRESS
// ADDRESS: 12 │     return (unsigned long)&f->low;
// ADDRESS: ·                           ───────
// ADDRESS: 13 │ #endif
// ADDRESS: ╰────
// SLATE-FILECHECK-END ADDRESS
