long long file_scope;
unsigned long long unsigned_file_scope;
typedef long long ll_t;
struct S {
  long long field;
};
long long returns_long_long(long long parameter);
void body(void) { long long local; }
ll_t typedef_use_does_not_warn;

// SLATE-FILECHECK-ARGS -pedantic
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-WARNING C89

// SLATE-FILECHECK-BEGIN C89
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:1:1]
// C89: 1 │ long long file_scope;
// C89: · ─────────────────────
// C89: 2 │ unsigned long long unsigned_file_scope;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:2:1]
// C89: 1 │ long long file_scope;
// C89: 2 │ unsigned long long unsigned_file_scope;
// C89: · ───────────────────────────────────────
// C89: 3 │ typedef long long ll_t;
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:3:1]
// C89: 2 │ unsigned long long unsigned_file_scope;
// C89: 3 │ typedef long long ll_t;
// C89: · ───────────────────────
// C89: 4 │ struct S {
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:5:3]
// C89: 4 │ struct S {
// C89: 5 │   long long field;
// C89: ·   ────────────────
// C89: 6 │ };
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:7:1]
// C89: 6 │ };
// C89: 7 │ long long returns_long_long(long long parameter);
// C89: · ─────────────────────────────────────────────────
// C89: 8 │ void body(void) { long long local; }
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:7:1]
// C89: 6 │ };
// C89: 7 │ long long returns_long_long(long long parameter);
// C89: · ─────────────────────────────────────────────────
// C89: 8 │ void body(void) { long long local; }
// C89: ╰────
// C89: -Wlong-long
// C89: ⚠ 'long long' is an extension when C99 mode is not enabled
// C89: ╭─[tests/fixtures/sema/long_long_declaration_warnings.c:8:19]
// C89: 7 │ long long returns_long_long(long long parameter);
// C89: 8 │ void body(void) { long long local; }
// C89: ·                   ────────────────
// C89: 9 │ ll_t typedef_use_does_not_warn;
// C89: ╰────
// SLATE-FILECHECK-END C89
