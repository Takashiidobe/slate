struct outer {
  int a;;;
  struct inner { int b; /* gap */ ; } in;
};

union u {
  ;
  int x;
};

int f(void) {
  struct local { int c; ; } l = {0};
  return l.c;
}

// SLATE-FILECHECK-ARGS -pedantic-errors
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ERROR C17

// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × semantic analysis failed
// C17: Error: -Wextra-semi
// C17:   × extra semicolon in struct or union specified
// C17:    ╭─[tests/fixtures/error/gcc/linux/x86_64/struct_extra_semicolon_pedantic_errors.c:2:9]
// C17:  1 │ struct outer {
// C17:  2 │   int a;;;
// C17:    ·         ─
// C17:  3 │   struct inner { int b; /* gap */ ; } in;
// C17:    ╰────
// C17: Error: -Wextra-semi
// C17:   × extra semicolon in struct or union specified
// C17:    ╭─[tests/fixtures/error/gcc/linux/x86_64/struct_extra_semicolon_pedantic_errors.c:2:10]
// C17:  1 │ struct outer {
// C17:  2 │   int a;;;
// C17:    ·          ─
// C17:  3 │   struct inner { int b; /* gap */ ; } in;
// C17:    ╰────
// C17: Error: -Wextra-semi
// C17:   × extra semicolon in struct or union specified
// C17:    ╭─[tests/fixtures/error/gcc/linux/x86_64/struct_extra_semicolon_pedantic_errors.c:3:35]
// C17:  2 │   int a;;;
// C17:  3 │   struct inner { int b; /* gap */ ; } in;
// C17:    ·                                   ─
// C17:  4 │ };
// C17:    ╰────
// C17: Error: -Wextra-semi
// C17:   × extra semicolon in struct or union specified
// C17:    ╭─[tests/fixtures/error/gcc/linux/x86_64/struct_extra_semicolon_pedantic_errors.c:7:3]
// C17:  6 │ union u {
// C17:  7 │   ;
// C17:    ·   ─
// C17:  8 │   int x;
// C17:    ╰────
// C17: Error: -Wextra-semi
// C17:   × extra semicolon in struct or union specified
// C17:     ╭─[tests/fixtures/error/gcc/linux/x86_64/struct_extra_semicolon_pedantic_errors.c:12:25]
// C17:  11 │ int f(void) {
// C17:  12 │   struct local { int c; ; } l = {0};
// C17:     ·                         ─
// C17:  13 │   return l.c;
// C17:     ╰────
// SLATE-FILECHECK-END C17
