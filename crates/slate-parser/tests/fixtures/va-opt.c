#define OPTIONAL(base, ...) base __VA_OPT__(+ __VA_ARGS__)
#define WRAP(...) OPTIONAL(1, __VA_ARGS__)
#define STRINGIFY(value) #value
#define TEXT(...) __VA_OPT__(STRINGIFY(__VA_ARGS__))
#define SELF SELF
#define RESCAN(...) __VA_OPT__(SELF)

int omitted = OPTIONAL(1);
int empty = OPTIONAL(1,);
int one = OPTIONAL(1, 2);
int nested_empty = WRAP();
int nested_value = WRAP(3);
char *text_empty = TEXT() "empty";
char *text_many = TEXT(alpha, beta);
int recursive __attribute__((slate_macro(RESCAN(value))));

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `STRINGIFY`
// DEFAULT: ╭─[tests/fixtures/va-opt.c:14:19]
// DEFAULT: 13 │ char *text_empty = TEXT() "empty";
// DEFAULT: 14 │ char *text_many = TEXT(alpha, beta);
// DEFAULT: ·                   ────
// DEFAULT: 15 │ int recursive __attribute__((slate_macro(RESCAN(value))));
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `alpha`
// DEFAULT: ╭─[tests/fixtures/va-opt.c:14:24]
// DEFAULT: 13 │ char *text_empty = TEXT() "empty";
// DEFAULT: 14 │ char *text_many = TEXT(alpha, beta);
// DEFAULT: ·                        ─────
// DEFAULT: 15 │ int recursive __attribute__((slate_macro(RESCAN(value))));
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `beta`
// DEFAULT: ╭─[tests/fixtures/va-opt.c:14:31]
// DEFAULT: 13 │ char *text_empty = TEXT() "empty";
// DEFAULT: 14 │ char *text_many = TEXT(alpha, beta);
// DEFAULT: ·                               ────
// DEFAULT: 15 │ int recursive __attribute__((slate_macro(RESCAN(value))));
// DEFAULT: ╰────
// DEFAULT: ⚠ unknown attribute 'slate_macro' ignored
// DEFAULT: ╭─[tests/fixtures/va-opt.c:15:30]
// DEFAULT: 14 │ char *text_many = TEXT(alpha, beta);
// DEFAULT: 15 │ int recursive __attribute__((slate_macro(RESCAN(value))));
// DEFAULT: ·                              ───────────
// DEFAULT: 16 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
