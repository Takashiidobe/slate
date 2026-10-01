typedef float v4sf __attribute__((vector_size(16)));

float sum(v4sf v) {
  return __builtin_reduce_add(v);
}

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × reduction builtin operand type
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/reduce-builtin-operand.c:4:10]
// DEFAULT: 3 │ float sum(v4sf v) {
// DEFAULT: 4 │   return __builtin_reduce_add(v);
// DEFAULT: ·          ───────────────────────
// DEFAULT: 5 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
