int global_array[0];
struct S { int size; int data[1 - 1]; };
void prototype(int data[0]);

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × zero-size array is not allowed with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_zero_size_array.c:1:1]
// PARSE:  1 │ int global_array[0];
// PARSE:    · ────────────────────
// PARSE:  2 │ struct S { int size; int data[1 - 1]; };
// PARSE:    ╰────
// PARSE: Error:
// PARSE:   × zero-size array is not allowed with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_zero_size_array.c:2:1]
// PARSE:  1 │ int global_array[0];
// PARSE:  2 │ struct S { int size; int data[1 - 1]; };
// PARSE:    · ────────────────────────────────────────
// PARSE:  3 │ void prototype(int data[0]);
// PARSE:    ╰────
// PARSE: Error:
// PARSE:   × zero-size array is not allowed with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_zero_size_array.c:3:1]
// PARSE:  2 │ struct S { int size; int data[1 - 1]; };
// PARSE:  3 │ void prototype(int data[0]);
// PARSE:    · ────────────────────────────
// PARSE:  4 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
