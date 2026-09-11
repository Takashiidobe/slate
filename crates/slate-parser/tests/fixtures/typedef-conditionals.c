#ifdef USE_INT
typedef int Value;
#else
typedef char Value;
#endif

Value value;

#ifdef ONLY_LEFT
typedef int LeftOnly;
LeftOnly left_value;
#else
typedef int RightOnly;
RightOnly right_value;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES INT USE_INT
// SLATE-FILECHECK-DEFINES LEFT ONLY_LEFT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: conditional
// DEFAULT-NEXT:   branch[0]: when=defined(USE_INT)
// DEFAULT-NEXT:     decl[0]: typedef name=Value type=int
// DEFAULT-NEXT:   branch[1]: when=not(defined(USE_INT))
// DEFAULT-NEXT:     decl[0]: typedef name=Value type=char
// DEFAULT-NEXT: decl[1]: declaration type=Value declarator=name=value
// DEFAULT-NEXT: decl[2]: conditional
// DEFAULT-NEXT:   branch[0]: when=defined(ONLY_LEFT)
// DEFAULT-NEXT:     decl[0]: typedef name=LeftOnly type=int
// DEFAULT-NEXT:     decl[1]: declaration type=LeftOnly declarator=name=left_value
// DEFAULT-NEXT:   branch[1]: when=not(defined(ONLY_LEFT))
// DEFAULT-NEXT:     decl[0]: typedef name=RightOnly type=int
// DEFAULT-NEXT:     decl[1]: declaration type=RightOnly declarator=name=right_value
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: typedef name=Value type=char
// DEFAULT-NEXT: decl[1]: declaration type=Value declarator=name=value
// DEFAULT-NEXT: decl[2]: typedef name=RightOnly type=int
// DEFAULT-NEXT: decl[3]: declaration type=RightOnly declarator=name=right_value
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INT
// INT: polyvariant:
// INT-NEXT: decl[0]: conditional
// INT-NEXT:   branch[0]: when=defined(USE_INT)
// INT-NEXT:     decl[0]: typedef name=Value type=int
// INT-NEXT:   branch[1]: when=not(defined(USE_INT))
// INT-NEXT:     decl[0]: typedef name=Value type=char
// INT-NEXT: decl[1]: declaration type=Value declarator=name=value
// INT-NEXT: decl[2]: conditional
// INT-NEXT:   branch[0]: when=defined(ONLY_LEFT)
// INT-NEXT:     decl[0]: typedef name=LeftOnly type=int
// INT-NEXT:     decl[1]: declaration type=LeftOnly declarator=name=left_value
// INT-NEXT:   branch[1]: when=not(defined(ONLY_LEFT))
// INT-NEXT:     decl[0]: typedef name=RightOnly type=int
// INT-NEXT:     decl[1]: declaration type=RightOnly declarator=name=right_value
// INT-NEXT: concrete:
// INT-NEXT: decl[0]: typedef name=Value type=int
// INT-NEXT: decl[1]: declaration type=Value declarator=name=value
// INT-NEXT: decl[2]: typedef name=RightOnly type=int
// INT-NEXT: decl[3]: declaration type=RightOnly declarator=name=right_value
// SLATE-FILECHECK-END INT
// SLATE-FILECHECK-BEGIN LEFT
// LEFT: polyvariant:
// LEFT-NEXT: decl[0]: conditional
// LEFT-NEXT:   branch[0]: when=defined(USE_INT)
// LEFT-NEXT:     decl[0]: typedef name=Value type=int
// LEFT-NEXT:   branch[1]: when=not(defined(USE_INT))
// LEFT-NEXT:     decl[0]: typedef name=Value type=char
// LEFT-NEXT: decl[1]: declaration type=Value declarator=name=value
// LEFT-NEXT: decl[2]: conditional
// LEFT-NEXT:   branch[0]: when=defined(ONLY_LEFT)
// LEFT-NEXT:     decl[0]: typedef name=LeftOnly type=int
// LEFT-NEXT:     decl[1]: declaration type=LeftOnly declarator=name=left_value
// LEFT-NEXT:   branch[1]: when=not(defined(ONLY_LEFT))
// LEFT-NEXT:     decl[0]: typedef name=RightOnly type=int
// LEFT-NEXT:     decl[1]: declaration type=RightOnly declarator=name=right_value
// LEFT-NEXT: concrete:
// LEFT-NEXT: decl[0]: typedef name=Value type=char
// LEFT-NEXT: decl[1]: declaration type=Value declarator=name=value
// LEFT-NEXT: decl[2]: typedef name=LeftOnly type=int
// LEFT-NEXT: decl[3]: declaration type=LeftOnly declarator=name=left_value
// SLATE-FILECHECK-END LEFT
