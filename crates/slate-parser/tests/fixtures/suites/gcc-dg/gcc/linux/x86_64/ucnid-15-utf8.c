/* Test combinations of UTF-8 in various parts of identifiers.  */
/* { dg-do run } */
/* { dg-xfail-if "" { "powerpc-ibm-aix*" } } */
/* { dg-skip-if "" { ! ucn } } */
/* { dg-options "-std=c99" } */

extern void abort (void);

int π = 3;
int π² = 9;
int πp1 = 4;
int twoπ = 6;
int four_plus_π_ = 7;
int 😀ÀÁÂÃÄÅßàáâãäaåbæçèéêcëìígîïð7ñ9__òóô4õöÆ3ÇÈÉÊËabcÌÍÎÏÐÑÒÓÔÕÖ😄😅🤣😂_ÿ = 2;
int π\u03C0 = 9;

int main() {
  if (π != 3)
	abort ();

  if (π² != 9)
	abort ();

  if (πp1 != 4)
	abort ();

  if (twoπ != 6)
	abort ();

  if (four_plus_π_ != 7)
	abort () ;

  if (😀ÀÁÂÃÄÅßàáâãäaåbæçèéêcëìígîïð7ñ9__òóô4õöÆ3ÇÈÉÊËabcÌÍÎÏÐÑÒÓÔÕÖ😄😅🤣😂_ÿ != 2)
    abort ();

  if(ππ != π²)
    abort ();
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
// DEFAULT-NEXT:     global %[[VALUE0:[0-9]+]] π: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE1:[0-9]+]] π²: i32 [storage=static] = const<i32>(9) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE2:[0-9]+]] πp1: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE3:[0-9]+]] twoπ: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE4:[0-9]+]] four_plus_π_: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE5:[0-9]+]] 😀ÀÁÂÃÄÅßàáâãäaåbæçèéêcëìígîïð7ñ9__òóô4õöÆ3ÇÈÉÊËabcÌÍÎÏÐÑÒÓÔÕÖ😄😅🤣😂_ÿ: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE6:[0-9]+]] ππ: i32 [storage=static] = const<i32>(9) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE0]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE2]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE3]]), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE5]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE6]]), read<i32>(%[[VALUE1]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
