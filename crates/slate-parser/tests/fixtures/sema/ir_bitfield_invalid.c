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
// SIZEOF: Error:   × invalid in this context: application of sizeof or alignof to a bit-field
// SLATE-FILECHECK-END SIZEOF
// SLATE-FILECHECK-BEGIN ALIGNOF
// ALIGNOF: Error:   × invalid in this context: application of sizeof or alignof to a bit-field
// SLATE-FILECHECK-END ALIGNOF
// SLATE-FILECHECK-BEGIN ADDRESS
// ADDRESS: Error:   × invalid in this context: address of a bit-field
// SLATE-FILECHECK-END ADDRESS
