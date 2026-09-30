// SLATE-FILECHECK-DEFINES UNKNOWN_FIELD UNKNOWN_FIELD
// SLATE-FILECHECK-ERROR UNKNOWN_FIELD
// SLATE-FILECHECK-STD UNKNOWN_FIELD c23
// SLATE-FILECHECK-DEFINES OUT_OF_RANGE OUT_OF_RANGE
// SLATE-FILECHECK-ERROR OUT_OF_RANGE
// SLATE-FILECHECK-STD OUT_OF_RANGE c23
// SLATE-FILECHECK-DEFINES NEGATIVE_INDEX NEGATIVE_INDEX
// SLATE-FILECHECK-ERROR NEGATIVE_INDEX
// SLATE-FILECHECK-STD NEGATIVE_INDEX c23
// SLATE-FILECHECK-DEFINES EMPTY_RANGE EMPTY_RANGE
// SLATE-FILECHECK-ERROR EMPTY_RANGE
// SLATE-FILECHECK-STD EMPTY_RANGE c23
// SLATE-FILECHECK-DEFINES FIELD_ON_ARRAY FIELD_ON_ARRAY
// SLATE-FILECHECK-ERROR FIELD_ON_ARRAY
// SLATE-FILECHECK-STD FIELD_ON_ARRAY c23
// SLATE-FILECHECK-DEFINES INDEX_ON_STRUCT INDEX_ON_STRUCT
// SLATE-FILECHECK-ERROR INDEX_ON_STRUCT
// SLATE-FILECHECK-STD INDEX_ON_STRUCT c23
// SLATE-FILECHECK-DEFINES SCALAR_DESIGNATOR SCALAR_DESIGNATOR
// SLATE-FILECHECK-ERROR SCALAR_DESIGNATOR
// SLATE-FILECHECK-STD SCALAR_DESIGNATOR c23
// SLATE-FILECHECK-DEFINES INCOMPLETE_RECORD INCOMPLETE_RECORD
// SLATE-FILECHECK-ERROR INCOMPLETE_RECORD
// SLATE-FILECHECK-STD INCOMPLETE_RECORD c23
// SLATE-FILECHECK-DEFINES WIDE_STRING WIDE_STRING
// SLATE-FILECHECK-ERROR WIDE_STRING
// SLATE-FILECHECK-STD WIDE_STRING c23
// SLATE-FILECHECK-DEFINES COMPOUND_LITERAL COMPOUND_LITERAL
// SLATE-FILECHECK-ERROR COMPOUND_LITERAL
// SLATE-FILECHECK-STD COMPOUND_LITERAL c23
// SLATE-FILECHECK-DEFINES NESTED_LIST NESTED_LIST
// SLATE-FILECHECK-ERROR NESTED_LIST
// SLATE-FILECHECK-STD NESTED_LIST c23
// SLATE-FILECHECK-DEFINES NESTED_DESIGNATOR NESTED_DESIGNATOR
// SLATE-FILECHECK-ERROR NESTED_DESIGNATOR
// SLATE-FILECHECK-STD NESTED_DESIGNATOR c23
struct S { int a; };
struct O { struct S s; int b[2]; };
#if defined(UNKNOWN_FIELD)
struct S s = {.b = 1};
#elif defined(OUT_OF_RANGE)
int a[2] = {[5] = 1};
#elif defined(NEGATIVE_INDEX)
int a[4] = {[-1] = 1};
#elif defined(EMPTY_RANGE)
int a[4] = {[3 ... 1] = 1};
#elif defined(FIELD_ON_ARRAY)
int a[2] = {.x = 1};
#elif defined(INDEX_ON_STRUCT)
struct S s = {[0] = 1};
#elif defined(SCALAR_DESIGNATOR)
int x = {.a = 1};
#elif defined(INCOMPLETE_RECORD)
struct T;
struct T t = {1};
#elif defined(WIDE_STRING)
char w[4] = L"ab";
#elif defined(COMPOUND_LITERAL)
int f(void) { return (struct S){.b = 1}.a; }
#elif defined(NESTED_LIST)
struct O o = {{1}, {[2] = 1}};
#elif defined(NESTED_DESIGNATOR)
struct O o = {.s.c = 1};
#endif

// SLATE-FILECHECK-BEGIN UNKNOWN_FIELD
// UNKNOWN_FIELD: Error:   × semantic analysis failed
// UNKNOWN_FIELD: Error:
// UNKNOWN_FIELD: × unknown field designator
// UNKNOWN_FIELD: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:4:10]
// UNKNOWN_FIELD: 3 │ #if defined(UNKNOWN_FIELD)
// UNKNOWN_FIELD: 4 │ struct S s = {.b = 1};
// UNKNOWN_FIELD: ·          ────────────
// UNKNOWN_FIELD: 5 │ #elif defined(OUT_OF_RANGE)
// UNKNOWN_FIELD: ╰────
// SLATE-FILECHECK-END UNKNOWN_FIELD
// SLATE-FILECHECK-BEGIN OUT_OF_RANGE
// OUT_OF_RANGE: Error:   × semantic analysis failed
// OUT_OF_RANGE: Error:
// OUT_OF_RANGE: × array designator out of range
// OUT_OF_RANGE: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:6:5]
// OUT_OF_RANGE: 5 │ #elif defined(OUT_OF_RANGE)
// OUT_OF_RANGE: 6 │ int a[2] = {[5] = 1};
// OUT_OF_RANGE: ·     ────────────────
// OUT_OF_RANGE: 7 │ #elif defined(NEGATIVE_INDEX)
// OUT_OF_RANGE: ╰────
// SLATE-FILECHECK-END OUT_OF_RANGE
// SLATE-FILECHECK-BEGIN NEGATIVE_INDEX
// NEGATIVE_INDEX: Error:   × semantic analysis failed
// NEGATIVE_INDEX: Error:
// NEGATIVE_INDEX: × non-constant or negative array designator
// NEGATIVE_INDEX: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:8:5]
// NEGATIVE_INDEX: 7 │ #elif defined(NEGATIVE_INDEX)
// NEGATIVE_INDEX: 8 │ int a[4] = {[-1] = 1};
// NEGATIVE_INDEX: ·     ─────────────────
// NEGATIVE_INDEX: 9 │ #elif defined(EMPTY_RANGE)
// NEGATIVE_INDEX: ╰────
// SLATE-FILECHECK-END NEGATIVE_INDEX
// SLATE-FILECHECK-BEGIN EMPTY_RANGE
// EMPTY_RANGE: Error:   × semantic analysis failed
// EMPTY_RANGE: Error:
// EMPTY_RANGE: × empty designated range
// EMPTY_RANGE: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:10:5]
// EMPTY_RANGE: 9 │ #elif defined(EMPTY_RANGE)
// EMPTY_RANGE: 10 │ int a[4] = {[3 ... 1] = 1};
// EMPTY_RANGE: ·     ──────────────────────
// EMPTY_RANGE: 11 │ #elif defined(FIELD_ON_ARRAY)
// EMPTY_RANGE: ╰────
// SLATE-FILECHECK-END EMPTY_RANGE
// SLATE-FILECHECK-BEGIN FIELD_ON_ARRAY
// FIELD_ON_ARRAY: Error:   × semantic analysis failed
// FIELD_ON_ARRAY: Error:
// FIELD_ON_ARRAY: × designator does not match aggregate type
// FIELD_ON_ARRAY: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:12:5]
// FIELD_ON_ARRAY: 11 │ #elif defined(FIELD_ON_ARRAY)
// FIELD_ON_ARRAY: 12 │ int a[2] = {.x = 1};
// FIELD_ON_ARRAY: ·     ───────────────
// FIELD_ON_ARRAY: 13 │ #elif defined(INDEX_ON_STRUCT)
// FIELD_ON_ARRAY: ╰────
// SLATE-FILECHECK-END FIELD_ON_ARRAY
// SLATE-FILECHECK-BEGIN INDEX_ON_STRUCT
// INDEX_ON_STRUCT: Error:   × semantic analysis failed
// INDEX_ON_STRUCT: Error:
// INDEX_ON_STRUCT: × designator does not match aggregate type
// INDEX_ON_STRUCT: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:14:10]
// INDEX_ON_STRUCT: 13 │ #elif defined(INDEX_ON_STRUCT)
// INDEX_ON_STRUCT: 14 │ struct S s = {[0] = 1};
// INDEX_ON_STRUCT: ·          ─────────────
// INDEX_ON_STRUCT: 15 │ #elif defined(SCALAR_DESIGNATOR)
// INDEX_ON_STRUCT: ╰────
// SLATE-FILECHECK-END INDEX_ON_STRUCT
// SLATE-FILECHECK-BEGIN SCALAR_DESIGNATOR
// SCALAR_DESIGNATOR: Error:   × semantic analysis failed
// SCALAR_DESIGNATOR: Error:
// SCALAR_DESIGNATOR: × designator in initializer for scalar type
// SCALAR_DESIGNATOR: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:16:5]
// SCALAR_DESIGNATOR: 15 │ #elif defined(SCALAR_DESIGNATOR)
// SCALAR_DESIGNATOR: 16 │ int x = {.a = 1};
// SCALAR_DESIGNATOR: ·     ────────────
// SCALAR_DESIGNATOR: 17 │ #elif defined(INCOMPLETE_RECORD)
// SCALAR_DESIGNATOR: ╰────
// SLATE-FILECHECK-END SCALAR_DESIGNATOR
// SLATE-FILECHECK-BEGIN INCOMPLETE_RECORD
// INCOMPLETE_RECORD: Error:   × semantic analysis failed
// INCOMPLETE_RECORD: Error:
// INCOMPLETE_RECORD: × initializer for incomplete record
// INCOMPLETE_RECORD: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:19:10]
// INCOMPLETE_RECORD: 18 │ struct T;
// INCOMPLETE_RECORD: 19 │ struct T t = {1};
// INCOMPLETE_RECORD: ·          ───────
// INCOMPLETE_RECORD: 20 │ #elif defined(WIDE_STRING)
// INCOMPLETE_RECORD: ╰────
// SLATE-FILECHECK-END INCOMPLETE_RECORD
// SLATE-FILECHECK-BEGIN WIDE_STRING
// WIDE_STRING: Error:   × semantic analysis failed
// WIDE_STRING: Error:
// WIDE_STRING: × string literal initializer for incompatible array element
// WIDE_STRING: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:21:6]
// WIDE_STRING: 20 │ #elif defined(WIDE_STRING)
// WIDE_STRING: 21 │ char w[4] = L"ab";
// WIDE_STRING: ·      ────────────
// WIDE_STRING: 22 │ #elif defined(COMPOUND_LITERAL)
// WIDE_STRING: ╰────
// SLATE-FILECHECK-END WIDE_STRING
// SLATE-FILECHECK-BEGIN COMPOUND_LITERAL
// COMPOUND_LITERAL: Error:   × semantic analysis failed
// COMPOUND_LITERAL: Error:
// COMPOUND_LITERAL: × unknown field designator
// COMPOUND_LITERAL: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:23:22]
// COMPOUND_LITERAL: 22 │ #elif defined(COMPOUND_LITERAL)
// COMPOUND_LITERAL: 23 │ int f(void) { return (struct S){.b = 1}.a; }
// COMPOUND_LITERAL: ·                      ──────────────────
// COMPOUND_LITERAL: 24 │ #elif defined(NESTED_LIST)
// COMPOUND_LITERAL: ╰────
// SLATE-FILECHECK-END COMPOUND_LITERAL
// SLATE-FILECHECK-BEGIN NESTED_LIST
// NESTED_LIST: Error:   × semantic analysis failed
// NESTED_LIST: Error:
// NESTED_LIST: × array designator out of range
// NESTED_LIST: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:25:10]
// NESTED_LIST: 24 │ #elif defined(NESTED_LIST)
// NESTED_LIST: 25 │ struct O o = {{\{\{}}1}, {[2] = 1{{[}][}]}};
// NESTED_LIST: ·          ────────────────────
// NESTED_LIST: 26 │ #elif defined(NESTED_DESIGNATOR)
// NESTED_LIST: ╰────
// SLATE-FILECHECK-END NESTED_LIST
// SLATE-FILECHECK-BEGIN NESTED_DESIGNATOR
// NESTED_DESIGNATOR: Error:   × semantic analysis failed
// NESTED_DESIGNATOR: Error:
// NESTED_DESIGNATOR: × unknown field designator
// NESTED_DESIGNATOR: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer_rules.c:27:10]
// NESTED_DESIGNATOR: 26 │ #elif defined(NESTED_DESIGNATOR)
// NESTED_DESIGNATOR: 27 │ struct O o = {.s.c = 1};
// NESTED_DESIGNATOR: ·          ──────────────
// NESTED_DESIGNATOR: 28 │ #endif
// NESTED_DESIGNATOR: ╰────
// SLATE-FILECHECK-END NESTED_DESIGNATOR
