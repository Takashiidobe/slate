enum Values { VALUE = 340282366920938463463374607431768211456 };
struct Bits { unsigned field : 340282366920938463463374607431768211456; };
_Static_assert(340282366920938463463374607431768211456, "value");

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × static assertion requires an integer constant expression: integer literal
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/literal_diagnostics_all_contexts.c:3:16]
// DEFAULT: 2 │ struct Bits { unsigned field : 340282366920938463463374607431768211456; };
// DEFAULT: 3 │ _Static_assert(340282366920938463463374607431768211456, "value");
// DEFAULT: ·                ───────────────────────────────────────
// DEFAULT: 4 │
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × integer literal `340282366920938463463374607431768211456` is too large to
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/literal_diagnostics_all_contexts.c:1:23]
// DEFAULT: 1 │ enum Values { VALUE = 340282366920938463463374607431768211456 };
// DEFAULT: ·                       ───────────────────────────────────────
// DEFAULT: 2 │ struct Bits { unsigned field : 340282366920938463463374607431768211456; };
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × integer literal `340282366920938463463374607431768211456` is too large to
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/literal_diagnostics_all_contexts.c:2:32]
// DEFAULT: 1 │ enum Values { VALUE = 340282366920938463463374607431768211456 };
// DEFAULT: 2 │ struct Bits { unsigned field : 340282366920938463463374607431768211456; };
// DEFAULT: ·                                ───────────────────────────────────────
// DEFAULT: 3 │ _Static_assert(340282366920938463463374607431768211456, "value");
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × integer literal `340282366920938463463374607431768211456` is too large to
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/literal_diagnostics_all_contexts.c:3:16]
// DEFAULT: 2 │ struct Bits { unsigned field : 340282366920938463463374607431768211456; };
// DEFAULT: 3 │ _Static_assert(340282366920938463463374607431768211456, "value");
// DEFAULT: ·                ───────────────────────────────────────
// DEFAULT: 4 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
