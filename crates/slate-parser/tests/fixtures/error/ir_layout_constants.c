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
// FIELD: Error:   × unsupported in numeric IR lowering: unknown offsetof member
// SLATE-FILECHECK-END FIELD
// SLATE-FILECHECK-BEGIN INCOMPLETE
// INCOMPLETE: Error:   × unresolved tag name `Incomplete`
// SLATE-FILECHECK-END INCOMPLETE
// SLATE-FILECHECK-BEGIN BITFIELD
// BITFIELD: Error:   × unsupported in numeric IR lowering: offsetof bit-field
// SLATE-FILECHECK-END BITFIELD
// SLATE-FILECHECK-BEGIN WIDTH
// WIDTH: Error:   × unsupported in numeric IR lowering: invalid _BitInt width
// SLATE-FILECHECK-END WIDTH
