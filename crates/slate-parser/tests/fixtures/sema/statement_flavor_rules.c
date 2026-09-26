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
// RETURN: Error:   × invalid in this context: non-void function should return a value
// SLATE-FILECHECK-END RETURN
// SLATE-FILECHECK-BEGIN FALLTHROUGH
// FALLTHROUGH: Error:   × invalid in this context: fallthrough attribute on a non-empty statement
// SLATE-FILECHECK-END FALLTHROUGH
// SLATE-FILECHECK-BEGIN NESTED
// NESTED: Error:   × invalid in this context: function definition is not allowed here
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
// VALID-NEXT:     fn %0 @g(%4 <unnamed>: i32) -> i32 [linkage=external];
// VALID-NEXT:     fn %1 @attributed(%3 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// VALID-NEXT:         switch %5 read<i32>(%3)
// VALID-NEXT:             {
// VALID-NEXT:                 case %5 const<i32>(1):
// VALID-NEXT:                     let %6: i32 [synthetic] = read<i32>(%3);
// VALID-NEXT:                     let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// VALID-NEXT:                     write<i32>(%3, read<i32>(%7));
// VALID-NEXT:                 ;
// VALID-NEXT:                 default %5:
// VALID-NEXT:                     break %5;
// VALID-NEXT:             }
// VALID-NEXT:         label %2 done:
// VALID-NEXT:             ;
// VALID-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// VALID-NEXT:             goto %2;
// VALID-NEXT:         return read<i32>(%3);
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
