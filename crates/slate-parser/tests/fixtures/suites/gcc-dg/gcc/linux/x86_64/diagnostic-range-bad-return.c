/* { dg-options "-fpermissive -fdiagnostics-show-caret -Wreturn-local-addr" } */

int *address_of_local (void)
{
  int some_local;
  return &some_local; /* { dg-warning "function returns address of local variable" } */
/* { dg-begin-multiline-output "" }
   return &some_local;
          ^~~~~~~~~~~
   { dg-end-multiline-output "" } */
}

void surplus_return_when_void_1 (void)
{
  return 500; /* { dg-warning "'return' with a value, in function returning void" } */
/* { dg-begin-multiline-output "" }
   return 500;
          ^~~
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
 void surplus_return_when_void_1 (void)
      ^~~~~~~~~~~~~~~~~~~~~~~~~~
   { dg-end-multiline-output "" } */
}

void surplus_return_when_void_2 (int i, int j)
{
  return i * j; /* { dg-warning "'return' with a value, in function returning void" } */
/* { dg-begin-multiline-output "" }
   return i * j;
          ~~^~~
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
 void surplus_return_when_void_2 (int i, int j)
      ^~~~~~~~~~~~~~~~~~~~~~~~~~
   { dg-end-multiline-output "" } */
}

int missing_return_value (void)
{
  return; /* { dg-warning "'return' with no value, in function returning non-void" } */
/* { dg-begin-multiline-output "" }
   return;
   ^~~~~~
   { dg-end-multiline-output "" } */
/* { dg-begin-multiline-output "" }
 int missing_return_value (void)
     ^~~~~~~~~~~~~~~~~~~~
   { dg-end-multiline-output "" } */
/* TODO: ideally we'd underline the return type i.e. "int", but that
   location isn't captured.  */
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
// DEFAULT-NEXT:     fn %[[VALUE_address_of_local:[0-9]+]] @address_of_local() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_some_local:[0-9]+]] some_local: i32 [storage=automatic];
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_some_local]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_surplus_return_when_void_1:[0-9]+]] @surplus_return_when_void_1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(500);
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_surplus_return_when_void_2:[0-9]+]] @surplus_return_when_void_2(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_j:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_missing_return_value:[0-9]+]] @missing_return_value() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
