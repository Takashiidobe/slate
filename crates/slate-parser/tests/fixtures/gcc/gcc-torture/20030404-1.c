/* This exposed a bug in tree-ssa-ccp.c.  Since 'j' and 'i' are never
   defined, CCP was not traversing the edges out of the if(), which caused
   the PHI node for 'k' at the top of the while to only be visited once.
   This ended up causing CCP to think that 'k' was the constant '1'.  */
void abort(void);

int main(void) {
  int i, j, k;

  k = 0;
  while (k < 10) {
    k++;
    if (j > i)
      j = 5;
    else
      j = 3;
  }

  if (k != 10)
    abort();

  return 0;
}


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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 k: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:         while %5 lt<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%7));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%3), read<i32>(%2))
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(3));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
