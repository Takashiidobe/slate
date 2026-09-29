// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES RETURN RETURN
// SLATE-FILECHECK-DEFINES C99_RETURN RETURN
// SLATE-FILECHECK-STD C99_RETURN gnu99
// SLATE-FILECHECK-DEFINES FALLTHROUGH FALLTHROUGH
// SLATE-FILECHECK-DEFINES NESTED NESTED
// SLATE-FILECHECK-IR-ERROR C99_RETURN
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

// SLATE-FILECHECK-BEGIN C99_RETURN
// C99_RETURN: Error:   × semantic analysis failed
// C99_RETURN: Error:
// C99_RETURN: × non-void function should return a value
// C99_RETURN: ╭─[tests/fixtures/gcc/linux/x86_64/gcc_statement_flavor_rules.c:22:5]
// C99_RETURN: 21 │   if (x)
// C99_RETURN: 22 │     return;
// C99_RETURN: ·     ───────
// C99_RETURN: 23 │   return g(x);
// C99_RETURN: ╰────
// SLATE-FILECHECK-END C99_RETURN
// SLATE-FILECHECK-BEGIN NESTED
// NESTED: Error:   × semantic analysis failed
// NESTED: Error:
// NESTED: × not implemented: GNU nested function
// NESTED: ╭─[tests/fixtures/gcc/linux/x86_64/gcc_statement_flavor_rules.c:41:3]
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
// SLATE-FILECHECK-BEGIN RETURN
// RETURN: module {
// RETURN-NEXT:     target "x86_64-unknown-linux-gnu" {
// RETURN-NEXT:         endian = little;
// RETURN-NEXT:         pointer [size=8, align=8];
// RETURN-NEXT:         stack_alignment = 16;
// RETURN-NEXT:         long_double = f80;
// RETURN-NEXT:         storage bool [size=1, align=1];
// RETURN-NEXT:         storage i8, u8 [size=1, align=1];
// RETURN-NEXT:         storage i16, u16 [size=2, align=2];
// RETURN-NEXT:         storage i32, u32 [size=4, align=4];
// RETURN-NEXT:         storage i64, u64 [size=8, align=8];
// RETURN-NEXT:         storage i128, u128 [size=16, align=16];
// RETURN-NEXT:         storage bf16 [size=2, align=2];
// RETURN-NEXT:         storage f16 [size=2, align=2];
// RETURN-NEXT:         storage f32 [size=4, align=4];
// RETURN-NEXT:         storage f64 [size=8, align=8];
// RETURN-NEXT:         storage f80 [size=16, align=16];
// RETURN-NEXT:         storage f128 [size=16, align=16];
// RETURN-NEXT:         storage d32 [size=4, align=4];
// RETURN-NEXT:         storage d64 [size=8, align=8];
// RETURN-NEXT:         storage d128 [size=16, align=16];
// RETURN-NEXT:     }
// RETURN-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// RETURN-NEXT:     fn %[[VALUE_attributed:[0-9]+]] @attributed(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// RETURN-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_x]])
// RETURN-NEXT:             {
// RETURN-NEXT:                 case %[[VALUE1]] const<i32>(1):
// RETURN-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// RETURN-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// RETURN-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// RETURN-NEXT:                 ;
// RETURN-NEXT:                 default %[[VALUE1]]:
// RETURN-NEXT:                     break %[[VALUE1]];
// RETURN-NEXT:             }
// RETURN-NEXT:         label %[[VALUE_done:[0-9]+]] done:
// RETURN-NEXT:             ;
// RETURN-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// RETURN-NEXT:             goto %[[VALUE_done]];
// RETURN-NEXT:         return read<i32>(%[[VALUE_x]]);
// RETURN-NEXT:     }
// RETURN-NEXT:     fn %[[VALUE_valueless:[0-9]+]] @valueless(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// RETURN-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// RETURN-NEXT:             return;
// RETURN-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_g]], read<i32>(%[[VALUE_x_2]]));
// RETURN-NEXT:     }
// RETURN-NEXT: }
// SLATE-FILECHECK-END RETURN
// SLATE-FILECHECK-BEGIN FALLTHROUGH
// FALLTHROUGH: module {
// FALLTHROUGH-NEXT:     target "x86_64-unknown-linux-gnu" {
// FALLTHROUGH-NEXT:         endian = little;
// FALLTHROUGH-NEXT:         pointer [size=8, align=8];
// FALLTHROUGH-NEXT:         stack_alignment = 16;
// FALLTHROUGH-NEXT:         long_double = f80;
// FALLTHROUGH-NEXT:         storage bool [size=1, align=1];
// FALLTHROUGH-NEXT:         storage i8, u8 [size=1, align=1];
// FALLTHROUGH-NEXT:         storage i16, u16 [size=2, align=2];
// FALLTHROUGH-NEXT:         storage i32, u32 [size=4, align=4];
// FALLTHROUGH-NEXT:         storage i64, u64 [size=8, align=8];
// FALLTHROUGH-NEXT:         storage i128, u128 [size=16, align=16];
// FALLTHROUGH-NEXT:         storage bf16 [size=2, align=2];
// FALLTHROUGH-NEXT:         storage f16 [size=2, align=2];
// FALLTHROUGH-NEXT:         storage f32 [size=4, align=4];
// FALLTHROUGH-NEXT:         storage f64 [size=8, align=8];
// FALLTHROUGH-NEXT:         storage f80 [size=16, align=16];
// FALLTHROUGH-NEXT:         storage f128 [size=16, align=16];
// FALLTHROUGH-NEXT:         storage d32 [size=4, align=4];
// FALLTHROUGH-NEXT:         storage d64 [size=8, align=8];
// FALLTHROUGH-NEXT:         storage d128 [size=16, align=16];
// FALLTHROUGH-NEXT:     }
// FALLTHROUGH-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// FALLTHROUGH-NEXT:     fn %[[VALUE_attributed:[0-9]+]] @attributed(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// FALLTHROUGH-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_x]])
// FALLTHROUGH-NEXT:             {
// FALLTHROUGH-NEXT:                 case %[[VALUE1]] const<i32>(1):
// FALLTHROUGH-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// FALLTHROUGH-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// FALLTHROUGH-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// FALLTHROUGH-NEXT:                 ;
// FALLTHROUGH-NEXT:                 default %[[VALUE1]]:
// FALLTHROUGH-NEXT:                     break %[[VALUE1]];
// FALLTHROUGH-NEXT:             }
// FALLTHROUGH-NEXT:         label %[[VALUE_done:[0-9]+]] done:
// FALLTHROUGH-NEXT:             ;
// FALLTHROUGH-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// FALLTHROUGH-NEXT:             goto %[[VALUE_done]];
// FALLTHROUGH-NEXT:         return read<i32>(%[[VALUE_x]]);
// FALLTHROUGH-NEXT:     }
// FALLTHROUGH-NEXT:     fn %[[VALUE_misplaced:[0-9]+]] @misplaced(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// FALLTHROUGH-NEXT:         switch %[[VALUE4:[0-9]+]] read<i32>(%[[VALUE_x_2]])
// FALLTHROUGH-NEXT:             {
// FALLTHROUGH-NEXT:                 case %[[VALUE4]] const<i32>(1):
// FALLTHROUGH-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// FALLTHROUGH-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// FALLTHROUGH-NEXT:                     write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE6]]));
// FALLTHROUGH-NEXT:                 default %[[VALUE4]]:
// FALLTHROUGH-NEXT:                     break %[[VALUE4]];
// FALLTHROUGH-NEXT:             }
// FALLTHROUGH-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// FALLTHROUGH-NEXT:     }
// FALLTHROUGH-NEXT: }
// SLATE-FILECHECK-END FALLTHROUGH
