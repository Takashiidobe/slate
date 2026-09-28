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
// ASSIGN: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:7:1]
// ASSIGN: 6 │
// ASSIGN: 7 │ ╭─▶ void invalid(struct Frozen *frozen, struct Frozen *other) {
// ASSIGN: 8 │ │       const int constant = 1;
// ASSIGN: 9 │ │       int array[3];
// ASSIGN: 10 │ │   #ifdef ASSIGN
// ASSIGN: 11 │ │       constant = 2;
// ASSIGN: 12 │ │   #endif
// ASSIGN: 13 │ │   #ifdef COMPOUND
// ASSIGN: 14 │ │       constant += 1;
// ASSIGN: 15 │ │   #endif
// ASSIGN: 16 │ │   #ifdef INCREMENT
// ASSIGN: 17 │ │       constant++;
// ASSIGN: 18 │ │   #endif
// ASSIGN: 19 │ │   #ifdef MEMBER
// ASSIGN: 20 │ │       frozen->member = 3;
// ASSIGN: 21 │ │   #endif
// ASSIGN: 22 │ │   #ifdef RECORD
// ASSIGN: 23 │ │       *frozen = *other;
// ASSIGN: 24 │ │   #endif
// ASSIGN: 25 │ │   #ifdef ARRAY
// ASSIGN: 26 │ │       array = 0;
// ASSIGN: 27 │ │   #endif
// ASSIGN: 28 │ │       (void)array;
// ASSIGN: 29 │ ╰─▶ }
// ASSIGN: 30 │
// ASSIGN: ╰────
// SLATE-FILECHECK-END ASSIGN
// SLATE-FILECHECK-BEGIN COMPOUND
// COMPOUND: Error:   × semantic analysis failed
// COMPOUND: Error:
// COMPOUND: × invalid in this context: cannot assign to a const-qualified lvalue
// COMPOUND: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:7:1]
// COMPOUND: 6 │
// COMPOUND: 7 │ ╭─▶ void invalid(struct Frozen *frozen, struct Frozen *other) {
// COMPOUND: 8 │ │       const int constant = 1;
// COMPOUND: 9 │ │       int array[3];
// COMPOUND: 10 │ │   #ifdef ASSIGN
// COMPOUND: 11 │ │       constant = 2;
// COMPOUND: 12 │ │   #endif
// COMPOUND: 13 │ │   #ifdef COMPOUND
// COMPOUND: 14 │ │       constant += 1;
// COMPOUND: 15 │ │   #endif
// COMPOUND: 16 │ │   #ifdef INCREMENT
// COMPOUND: 17 │ │       constant++;
// COMPOUND: 18 │ │   #endif
// COMPOUND: 19 │ │   #ifdef MEMBER
// COMPOUND: 20 │ │       frozen->member = 3;
// COMPOUND: 21 │ │   #endif
// COMPOUND: 22 │ │   #ifdef RECORD
// COMPOUND: 23 │ │       *frozen = *other;
// COMPOUND: 24 │ │   #endif
// COMPOUND: 25 │ │   #ifdef ARRAY
// COMPOUND: 26 │ │       array = 0;
// COMPOUND: 27 │ │   #endif
// COMPOUND: 28 │ │       (void)array;
// COMPOUND: 29 │ ╰─▶ }
// COMPOUND: 30 │
// COMPOUND: ╰────
// SLATE-FILECHECK-END COMPOUND
// SLATE-FILECHECK-BEGIN INCREMENT
// INCREMENT: Error:   × semantic analysis failed
// INCREMENT: Error:
// INCREMENT: × invalid in this context: cannot assign to a const-qualified lvalue
// INCREMENT: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:7:1]
// INCREMENT: 6 │
// INCREMENT: 7 │ ╭─▶ void invalid(struct Frozen *frozen, struct Frozen *other) {
// INCREMENT: 8 │ │       const int constant = 1;
// INCREMENT: 9 │ │       int array[3];
// INCREMENT: 10 │ │   #ifdef ASSIGN
// INCREMENT: 11 │ │       constant = 2;
// INCREMENT: 12 │ │   #endif
// INCREMENT: 13 │ │   #ifdef COMPOUND
// INCREMENT: 14 │ │       constant += 1;
// INCREMENT: 15 │ │   #endif
// INCREMENT: 16 │ │   #ifdef INCREMENT
// INCREMENT: 17 │ │       constant++;
// INCREMENT: 18 │ │   #endif
// INCREMENT: 19 │ │   #ifdef MEMBER
// INCREMENT: 20 │ │       frozen->member = 3;
// INCREMENT: 21 │ │   #endif
// INCREMENT: 22 │ │   #ifdef RECORD
// INCREMENT: 23 │ │       *frozen = *other;
// INCREMENT: 24 │ │   #endif
// INCREMENT: 25 │ │   #ifdef ARRAY
// INCREMENT: 26 │ │       array = 0;
// INCREMENT: 27 │ │   #endif
// INCREMENT: 28 │ │       (void)array;
// INCREMENT: 29 │ ╰─▶ }
// INCREMENT: 30 │
// INCREMENT: ╰────
// SLATE-FILECHECK-END INCREMENT
// SLATE-FILECHECK-BEGIN MEMBER
// MEMBER: Error:   × semantic analysis failed
// MEMBER: Error:
// MEMBER: × invalid in this context: cannot assign to a const-qualified lvalue
// MEMBER: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:7:1]
// MEMBER: 6 │
// MEMBER: 7 │ ╭─▶ void invalid(struct Frozen *frozen, struct Frozen *other) {
// MEMBER: 8 │ │       const int constant = 1;
// MEMBER: 9 │ │       int array[3];
// MEMBER: 10 │ │   #ifdef ASSIGN
// MEMBER: 11 │ │       constant = 2;
// MEMBER: 12 │ │   #endif
// MEMBER: 13 │ │   #ifdef COMPOUND
// MEMBER: 14 │ │       constant += 1;
// MEMBER: 15 │ │   #endif
// MEMBER: 16 │ │   #ifdef INCREMENT
// MEMBER: 17 │ │       constant++;
// MEMBER: 18 │ │   #endif
// MEMBER: 19 │ │   #ifdef MEMBER
// MEMBER: 20 │ │       frozen->member = 3;
// MEMBER: 21 │ │   #endif
// MEMBER: 22 │ │   #ifdef RECORD
// MEMBER: 23 │ │       *frozen = *other;
// MEMBER: 24 │ │   #endif
// MEMBER: 25 │ │   #ifdef ARRAY
// MEMBER: 26 │ │       array = 0;
// MEMBER: 27 │ │   #endif
// MEMBER: 28 │ │       (void)array;
// MEMBER: 29 │ ╰─▶ }
// MEMBER: 30 │
// MEMBER: ╰────
// SLATE-FILECHECK-END MEMBER
// SLATE-FILECHECK-BEGIN RECORD
// RECORD: Error:   × semantic analysis failed
// RECORD: Error:
// RECORD: × invalid in this context: cannot assign to a variable with a const-
// RECORD: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:7:1]
// RECORD: 6 │
// RECORD: 7 │ ╭─▶ void invalid(struct Frozen *frozen, struct Frozen *other) {
// RECORD: 8 │ │       const int constant = 1;
// RECORD: 9 │ │       int array[3];
// RECORD: 10 │ │   #ifdef ASSIGN
// RECORD: 11 │ │       constant = 2;
// RECORD: 12 │ │   #endif
// RECORD: 13 │ │   #ifdef COMPOUND
// RECORD: 14 │ │       constant += 1;
// RECORD: 15 │ │   #endif
// RECORD: 16 │ │   #ifdef INCREMENT
// RECORD: 17 │ │       constant++;
// RECORD: 18 │ │   #endif
// RECORD: 19 │ │   #ifdef MEMBER
// RECORD: 20 │ │       frozen->member = 3;
// RECORD: 21 │ │   #endif
// RECORD: 22 │ │   #ifdef RECORD
// RECORD: 23 │ │       *frozen = *other;
// RECORD: 24 │ │   #endif
// RECORD: 25 │ │   #ifdef ARRAY
// RECORD: 26 │ │       array = 0;
// RECORD: 27 │ │   #endif
// RECORD: 28 │ │       (void)array;
// RECORD: 29 │ ╰─▶ }
// RECORD: 30 │
// RECORD: ╰────
// SLATE-FILECHECK-END RECORD
// SLATE-FILECHECK-BEGIN ARRAY
// ARRAY: Error:   × semantic analysis failed
// ARRAY: Error:
// ARRAY: × invalid in this context: cannot assign to an array type
// ARRAY: ╭─[tests/fixtures/sema/ir_modifiable_lvalue.c:7:1]
// ARRAY: 6 │
// ARRAY: 7 │ ╭─▶ void invalid(struct Frozen *frozen, struct Frozen *other) {
// ARRAY: 8 │ │       const int constant = 1;
// ARRAY: 9 │ │       int array[3];
// ARRAY: 10 │ │   #ifdef ASSIGN
// ARRAY: 11 │ │       constant = 2;
// ARRAY: 12 │ │   #endif
// ARRAY: 13 │ │   #ifdef COMPOUND
// ARRAY: 14 │ │       constant += 1;
// ARRAY: 15 │ │   #endif
// ARRAY: 16 │ │   #ifdef INCREMENT
// ARRAY: 17 │ │       constant++;
// ARRAY: 18 │ │   #endif
// ARRAY: 19 │ │   #ifdef MEMBER
// ARRAY: 20 │ │       frozen->member = 3;
// ARRAY: 21 │ │   #endif
// ARRAY: 22 │ │   #ifdef RECORD
// ARRAY: 23 │ │       *frozen = *other;
// ARRAY: 24 │ │   #endif
// ARRAY: 25 │ │   #ifdef ARRAY
// ARRAY: 26 │ │       array = 0;
// ARRAY: 27 │ │   #endif
// ARRAY: 28 │ │       (void)array;
// ARRAY: 29 │ ╰─▶ }
// ARRAY: 30 │
// ARRAY: ╰────
// SLATE-FILECHECK-END ARRAY
