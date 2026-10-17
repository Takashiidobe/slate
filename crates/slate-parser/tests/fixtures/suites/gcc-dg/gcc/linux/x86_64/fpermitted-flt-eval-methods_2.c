/* { dg-do run } */
/* { dg-options "-fpermitted-flt-eval-methods=c11" } */

/* Test that we only see the C99/C11 values for __FLT_EVAL_METHOD__ if
   we are compiling with -fpermitted-flt-eval-methods=c11.  */

int main (int argc, char** argv)
{
  switch (__FLT_EVAL_METHOD__)
    {
      case 0:
      case 1:
      case 2:
      case -1:
	return 0;
      default:
	return 1;
    }
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fpermitted-flt-eval-methods=c11
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
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] const<i32>(0)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(0):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                             case %[[VALUE0]] const<i32>(-1):
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
