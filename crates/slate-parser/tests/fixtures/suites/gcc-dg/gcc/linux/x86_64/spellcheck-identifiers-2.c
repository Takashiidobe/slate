/* PR c/71858 */
/* Make sure anticipated builtins are not considered before they are declared.  */
/* { dg-do compile } */
/* { dg-options "-fpermissive -Wimplicit-function-declaration -fdiagnostics-show-caret" } */

int sscafn (const char *, const char *, ...);

int
test_1 (const char *p)
{
  int i;
  return ssacnf (p, "%d", &i); /* { dg-warning "10: implicit declaration of function .ssacnf.; did you mean .sscafn.?" } */
  /* { dg-begin-multiline-output "" }
   return ssacnf (p, "%d", &i);
          ^~~~~~
          sscafn
   { dg-end-multiline-output "" } */
}

int scafn (const char *, ...);
int scanf (const char *, ...);

int
test_2 (void)
{
  int i;
  return sacnf ("%d", &i); /* { dg-warning "10: implicit declaration of function .sacnf.; did you mean .scanf.?" } */
  /* { dg-begin-multiline-output "" }
   return sacnf ("%d", &i);
          ^~~~~
          scanf
   { dg-end-multiline-output "" } */
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sscafn:[0-9]+]] @sscafn(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ssacnf:[0-9]+]] @ssacnf(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_1:[0-9]+]] @test_1(%[[VALUE_p:[0-9]+]] p: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_ssacnf]], read<ptr<const i8>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]]), addr_of<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_scafn:[0-9]+]] @scafn(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scanf:[0-9]+]] @scanf(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sacnf:[0-9]+]] @sacnf(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_2:[0-9]+]] @test_2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_sacnf]], array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]]), addr_of<ptr<i32>>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
