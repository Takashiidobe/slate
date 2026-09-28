int *returns_pointer(void) { return 0; }
int (*returns_function_pointer(int x))(char) { return 0; }
void *(*returns_pointer_returning_function_pointer(int))(void) { return 0; }
int (parenthesized_name)(int x) { return x; }

void outer(void) {
  int (*nested(int x))(char) { return 0; }
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/function_definition_declarators.c:7:3]
// DEFAULT: 6 │ void outer(void) {
// DEFAULT: 7 │   int (*nested(int x))(char) { return 0; }
// DEFAULT: ·   ────────────────────────────────────────
// DEFAULT: 8 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
