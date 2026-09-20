_BitInt(8) global;
struct S {
  _BitInt(17) field;
};
_BitInt(33) function(_BitInt(9) parameter) {
  _BitInt(11) local;
  return local + parameter;
}

// SLATE-FILECHECK-ARGS -pedantic
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C17

// SLATE-FILECHECK-BEGIN C17
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/sema/bitint_extension_warnings.c:1:1]
// C17: 1 │ _BitInt(8) global;
// C17: · ──────────────────
// C17: 2 │ struct S {
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/sema/bitint_extension_warnings.c:3:3]
// C17: 2 │ struct S {
// C17: 3 │   _BitInt(17) field;
// C17: ·   ──────────────────
// C17: 4 │ };
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/sema/bitint_extension_warnings.c:5:1]
// C17: 4 │     };
// C17: 5 │ ╭─▶ _BitInt(33) function(_BitInt(9) parameter) {
// C17: 6 │ │     _BitInt(11) local;
// C17: 7 │ │     return local + parameter;
// C17: 8 │ ╰─▶ }
// C17: 9 │
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/sema/bitint_extension_warnings.c:5:1]
// C17: 4 │     };
// C17: 5 │ ╭─▶ _BitInt(33) function(_BitInt(9) parameter) {
// C17: 6 │ │     _BitInt(11) local;
// C17: 7 │ │     return local + parameter;
// C17: 8 │ ╰─▶ }
// C17: 9 │
// C17: ╰────
// C17: -Wbit-int-extension
// C17: ⚠ '_BitInt' is an extension before C23
// C17: ╭─[tests/fixtures/sema/bitint_extension_warnings.c:6:3]
// C17: 5 │ _BitInt(33) function(_BitInt(9) parameter) {
// C17: 6 │   _BitInt(11) local;
// C17: ·   ──────────────────
// C17: 7 │   return local + parameter;
// C17: ╰────
// SLATE-FILECHECK-END C17
