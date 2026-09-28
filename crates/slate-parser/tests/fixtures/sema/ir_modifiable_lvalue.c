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
// ASSIGN: Error:   × semantic analysis failed
// ASSIGN: Error:
// ASSIGN: × invalid in this context: cannot assign to a const-qualified lvalue
// ASSIGN: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:11:5]
// ASSIGN: 10 │ #ifdef ASSIGN
// ASSIGN: 11 │     constant = 2;
// ASSIGN: ·     ────────────
// ASSIGN: 12 │ #endif
// ASSIGN: ╰────
// SLATE-FILECHECK-END ASSIGN
// SLATE-FILECHECK-BEGIN COMPOUND
// COMPOUND: Error:   × semantic analysis failed
// COMPOUND: Error:
// COMPOUND: × invalid in this context: cannot assign to a const-qualified lvalue
// COMPOUND: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:14:5]
// COMPOUND: 13 │ #ifdef COMPOUND
// COMPOUND: 14 │     constant += 1;
// COMPOUND: ·     ─────────────
// COMPOUND: 15 │ #endif
// COMPOUND: ╰────
// SLATE-FILECHECK-END COMPOUND
// SLATE-FILECHECK-BEGIN INCREMENT
// INCREMENT: Error:   × semantic analysis failed
// INCREMENT: Error:
// INCREMENT: × invalid in this context: cannot assign to a const-qualified lvalue
// INCREMENT: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:17:5]
// INCREMENT: 16 │ #ifdef INCREMENT
// INCREMENT: 17 │     constant++;
// INCREMENT: ·     ──────────
// INCREMENT: 18 │ #endif
// INCREMENT: ╰────
// SLATE-FILECHECK-END INCREMENT
// SLATE-FILECHECK-BEGIN MEMBER
// MEMBER: Error:   × semantic analysis failed
// MEMBER: Error:
// MEMBER: × invalid in this context: cannot assign to a const-qualified lvalue
// MEMBER: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:20:5]
// MEMBER: 19 │ #ifdef MEMBER
// MEMBER: 20 │     frozen->member = 3;
// MEMBER: ·     ──────────────────
// MEMBER: 21 │ #endif
// MEMBER: ╰────
// SLATE-FILECHECK-END MEMBER
// SLATE-FILECHECK-BEGIN RECORD
// RECORD: Error:   × semantic analysis failed
// RECORD: Error:
// RECORD: × invalid in this context: cannot assign to a variable with a const-
// RECORD: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:23:5]
// RECORD: 22 │ #ifdef RECORD
// RECORD: 23 │     *frozen = *other;
// RECORD: ·     ────────────────
// RECORD: 24 │ #endif
// RECORD: ╰────
// SLATE-FILECHECK-END RECORD
// SLATE-FILECHECK-BEGIN ARRAY
// ARRAY: Error:   × semantic analysis failed
// ARRAY: Error:
// ARRAY: × invalid in this context: cannot assign to an array type
// ARRAY: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:26:5]
// ARRAY: 25 │ #ifdef ARRAY
// ARRAY: 26 │     array = 0;
// ARRAY: ·     ─────────
// ARRAY: 27 │ #endif
// ARRAY: ╰────
// SLATE-FILECHECK-END ARRAY
