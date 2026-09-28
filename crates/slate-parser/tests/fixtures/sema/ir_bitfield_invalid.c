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
// SIZEOF: ╭─[tests/fixtures/sema/ir_bitfield_invalid.c:4:1]
// SIZEOF: 3 │
// SIZEOF: 4 │ ╭─▶ unsigned long invalid(struct Flags *f) {
// SIZEOF: 5 │ │   #ifdef SIZEOF
// SIZEOF: 6 │ │       return sizeof(f->low);
// SIZEOF: 7 │ │   #endif
// SIZEOF: 8 │ │   #ifdef ALIGNOF
// SIZEOF: 9 │ │       return _Alignof(f->low);
// SIZEOF: 10 │ │   #endif
// SIZEOF: 11 │ │   #ifdef ADDRESS
// SIZEOF: 12 │ │       return (unsigned long)&f->low;
// SIZEOF: 13 │ │   #endif
// SIZEOF: 14 │ ╰─▶ }
// SIZEOF: 15 │
// SIZEOF: ╰────
// SLATE-FILECHECK-END SIZEOF
// SLATE-FILECHECK-BEGIN ALIGNOF
// ALIGNOF: Error:   × semantic analysis failed
// ALIGNOF: Error:
// ALIGNOF: × invalid in this context: application of sizeof or alignof to a bit-field
// ALIGNOF: ╭─[tests/fixtures/sema/ir_bitfield_invalid.c:4:1]
// ALIGNOF: 3 │
// ALIGNOF: 4 │ ╭─▶ unsigned long invalid(struct Flags *f) {
// ALIGNOF: 5 │ │   #ifdef SIZEOF
// ALIGNOF: 6 │ │       return sizeof(f->low);
// ALIGNOF: 7 │ │   #endif
// ALIGNOF: 8 │ │   #ifdef ALIGNOF
// ALIGNOF: 9 │ │       return _Alignof(f->low);
// ALIGNOF: 10 │ │   #endif
// ALIGNOF: 11 │ │   #ifdef ADDRESS
// ALIGNOF: 12 │ │       return (unsigned long)&f->low;
// ALIGNOF: 13 │ │   #endif
// ALIGNOF: 14 │ ╰─▶ }
// ALIGNOF: 15 │
// ALIGNOF: ╰────
// SLATE-FILECHECK-END ALIGNOF
// SLATE-FILECHECK-BEGIN ADDRESS
// ADDRESS: Error:   × semantic analysis failed
// ADDRESS: Error:
// ADDRESS: × invalid in this context: address of a bit-field
// ADDRESS: ╭─[tests/fixtures/sema/ir_bitfield_invalid.c:4:1]
// ADDRESS: 3 │
// ADDRESS: 4 │ ╭─▶ unsigned long invalid(struct Flags *f) {
// ADDRESS: 5 │ │   #ifdef SIZEOF
// ADDRESS: 6 │ │       return sizeof(f->low);
// ADDRESS: 7 │ │   #endif
// ADDRESS: 8 │ │   #ifdef ALIGNOF
// ADDRESS: 9 │ │       return _Alignof(f->low);
// ADDRESS: 10 │ │   #endif
// ADDRESS: 11 │ │   #ifdef ADDRESS
// ADDRESS: 12 │ │       return (unsigned long)&f->low;
// ADDRESS: 13 │ │   #endif
// ADDRESS: 14 │ ╰─▶ }
// ADDRESS: 15 │
// ADDRESS: ╰────
// SLATE-FILECHECK-END ADDRESS
