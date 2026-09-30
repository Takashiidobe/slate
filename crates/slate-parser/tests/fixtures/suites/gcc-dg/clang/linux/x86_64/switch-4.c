/* PR middle-end/17657 */
/* { dg-do compile } */
/* { dg-options "-O2" } */

extern signed char foo(int);

void bar (void)
{
  signed char tmp = foo (0);
  int t1 = tmp;
  switch (t1)
    {
    case 1: foo (1); break;
    case 2: foo (2); break;
    case 3: foo (3); break;
    case 4: foo (4); break;
    case 5: foo (5); break;
    case 6: foo (6); break;
    case 7: foo (7); break;
    case 255: foo (8); break;
    default: break;
    }
}

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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i8 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: i8 [storage=automatic] = call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_t1:[0-9]+]] t1: i32 [storage=automatic] = widen<i32, reason=assign>(read<i8>(%[[VALUE_tmp]]));
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_t1]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(1):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(1));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(2):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(2));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(3):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(3));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(4):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(4));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(5):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(5));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(6):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(6));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(7):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(7));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(255):
// DEFAULT-NEXT:                     call<i8, signature=fn(i32) -> i8>(%[[VALUE_foo]], const<i32>(8));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     break %[[VALUE1]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
