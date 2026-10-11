/* { dg-do run } */
/* { dg-xfail-if "" { powerpc-ibm-aix* } } */
/* { dg-skip-if "" { ! ucn } } */
/* { dg-options "-std=c99 -g" } */
void abort (void);

static int \u00C0 = 1;
static int \u00C1 = 2;
static int \U000000C2 = 3;
static int wh\u00ff = 4;
static int a\u00c4b\u0441\U000003b4e = 5;

int main (void)
{
  
  if (\u00C0 != 1)
    abort ();
  if (\u00c1 != 2)
    abort ();
  if (\u00C2 != 3)
    abort ();
  if (wh\u00ff != 4)
    abort ();
  if (a\u00c4b\u0441\U000003b4e != 5)
    abort ();
  
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %[[VALUE0:[0-9]+]] À: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE1:[0-9]+]] Á: i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE2:[0-9]+]] Â: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE3:[0-9]+]] whÿ: i32 [storage=static] = const<i32>(4) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE4:[0-9]+]] aÄbсδe: i32 [storage=static] = const<i32>(5) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE0]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE2]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE3]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
