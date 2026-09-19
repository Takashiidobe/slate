// SLATE-FILECHECK-DEFINES ASSIGN ASSIGN
// SLATE-FILECHECK-DEFINES COMPOUND COMPOUND
// SLATE-FILECHECK-DEFINES INCREMENT INCREMENT
// SLATE-FILECHECK-DEFINES MEMBER MEMBER
// SLATE-FILECHECK-DEFINES RECORD RECORD
// SLATE-FILECHECK-DEFINES ARRAY ARRAY
// SLATE-FILECHECK-ERROR ASSIGN
// SLATE-FILECHECK-ERROR COMPOUND
// SLATE-FILECHECK-ERROR INCREMENT
// SLATE-FILECHECK-ERROR MEMBER
// SLATE-FILECHECK-ERROR RECORD
// SLATE-FILECHECK-ERROR ARRAY
// SLATE-FILECHECK-ARGS --dump-ir

struct Frozen {
    const int member;
    int thawed;
};

void invalid(struct Frozen *frozen, struct Frozen *other) {
    const int constant = 1;
    int array[3];
#ifdef ASSIGN
    constant = 2;
#endif
#ifdef COMPOUND
    constant += 1;
#endif
#ifdef INCREMENT
    constant++;
#endif
#ifdef MEMBER
    frozen->member = 3;
#endif
#ifdef RECORD
    *frozen = *other;
#endif
#ifdef ARRAY
    array = 0;
#endif
    (void)array;
}

// SLATE-FILECHECK-BEGIN ASSIGN
// ASSIGN: Error:   × invalid in this context: cannot assign to a const-qualified lvalue
// SLATE-FILECHECK-END ASSIGN
// SLATE-FILECHECK-BEGIN COMPOUND
// COMPOUND: Error:   × invalid in this context: cannot assign to a const-qualified lvalue
// SLATE-FILECHECK-END COMPOUND
// SLATE-FILECHECK-BEGIN INCREMENT
// INCREMENT: Error:   × invalid in this context: cannot assign to a const-qualified lvalue
// SLATE-FILECHECK-END INCREMENT
// SLATE-FILECHECK-BEGIN MEMBER
// MEMBER: Error:   × invalid in this context: cannot assign to a const-qualified lvalue
// SLATE-FILECHECK-END MEMBER
// SLATE-FILECHECK-BEGIN RECORD
// RECORD: Error:   × invalid in this context: cannot assign to a variable with a const-
// SLATE-FILECHECK-END RECORD
// SLATE-FILECHECK-BEGIN ARRAY
// ARRAY: Error:   × invalid in this context: cannot assign to an array type
// SLATE-FILECHECK-END ARRAY
