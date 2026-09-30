// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES RETURN RETURN
// SLATE-FILECHECK-DEFINES FALLTHROUGH FALLTHROUGH
// SLATE-FILECHECK-DEFINES NESTED NESTED
// SLATE-FILECHECK-IR-ERROR RETURN
// SLATE-FILECHECK-IR-ERROR FALLTHROUGH
// SLATE-FILECHECK-IR-ERROR NESTED
// SLATE-FILECHECK-STD VALID gnu89
// SLATE-FILECHECK-STD RETURN gnu89
// SLATE-FILECHECK-STD FALLTHROUGH gnu23
// SLATE-FILECHECK-STD NESTED gnu89
// SLATE-FILECHECK-ARGS --dump-ir

int g(int);

int attributed(int x) {
  switch (x) {
  case 1:
    x++;
    __attribute__((fallthrough));
  default:
    break;
  }
done:
  __attribute__((unused));
  if (x)
    goto done;
  return x;
}

#ifdef RETURN
int valueless(int x) {
  if (x)
    return;
  return g(x);
}
#endif

#ifdef FALLTHROUGH
int misplaced(int x) {
  switch (x) {
  case 1:
    [[fallthrough]] x++;
  default:
    break;
  }
  return x;
}
#endif

#ifdef NESTED
int outer(int x) {
  int inner(int y) { return y + x; }
  return inner(x);
}
#endif

// SLATE-FILECHECK-BEGIN RETURN
// RETURN: Error:   × semantic analysis failed
// RETURN: Error:
// RETURN: × non-void function should return a value
// RETURN: ╭─[tests/fixtures/clang/linux/x86_64/statement_flavor_rules.c:22:5]
// RETURN: 21 │   if (x)
// RETURN: 22 │     return;
// RETURN: ·     ───────
// RETURN: 23 │   return g(x);
// RETURN: ╰────
// SLATE-FILECHECK-END RETURN
// SLATE-FILECHECK-BEGIN FALLTHROUGH
// FALLTHROUGH: Error:   × semantic analysis failed
// FALLTHROUGH: Error:
// FALLTHROUGH: × fallthrough attribute on a non-empty statement
// FALLTHROUGH: ╭─[tests/fixtures/clang/linux/x86_64/statement_flavor_rules.c:31:5]
// FALLTHROUGH: 30 │   case 1:
// FALLTHROUGH: 31 │     {{\[\[}}fallthrough]] x++;
// FALLTHROUGH: ·     ────────────────────
// FALLTHROUGH: 32 │   default:
// FALLTHROUGH: ╰────
// SLATE-FILECHECK-END FALLTHROUGH
// SLATE-FILECHECK-BEGIN NESTED
// NESTED: Error:   × semantic analysis failed
// NESTED: Error:
// NESTED: × function definition is not allowed here
// NESTED: ╭─[tests/fixtures/clang/linux/x86_64/statement_flavor_rules.c:41:3]
// NESTED: 40 │ int outer(int x) {
// NESTED: 41 │   int inner(int y) { return y + x; }
// NESTED: ·   ──────────────────────────────────
// NESTED: 42 │   return inner(x);
// NESTED: ╰────
// SLATE-FILECHECK-END NESTED
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// VALID-NEXT:     fn %[[VALUE_attributed:[0-9]+]] @attributed(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// VALID-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_x]])
// VALID-NEXT:             {
// VALID-NEXT:                 case %[[VALUE1]] const<i32>(1):
// VALID-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// VALID-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// VALID-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// VALID-NEXT:                 ;
// VALID-NEXT:                 default %[[VALUE1]]:
// VALID-NEXT:                     break %[[VALUE1]];
// VALID-NEXT:             }
// VALID-NEXT:         label %[[VALUE_done:[0-9]+]] done:
// VALID-NEXT:             ;
// VALID-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// VALID-NEXT:             goto %[[VALUE_done]];
// VALID-NEXT:         return read<i32>(%[[VALUE_x]]);
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
