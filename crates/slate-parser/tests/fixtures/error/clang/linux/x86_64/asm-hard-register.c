void f(int x) {
  asm("" : "={rax}"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid output constraint '={rax}' in asm
// PARSE: ╰─▶ invalid output constraint '={rax}' in asm
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-hard-register.c:2:12]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("" : "={rax}"(x));
// PARSE: ·            ────────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
