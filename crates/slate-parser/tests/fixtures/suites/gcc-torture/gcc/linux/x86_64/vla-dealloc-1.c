/* VLAs should be deallocated on a jump to before their definition,
   including a jump to a label in an inner scope.  PR 19771.  */

#if (__SIZEOF_INT__ <= 2)
#define LIMIT 10000
#else
#define LIMIT 1000000
#endif

void *volatile p;

int main(void) {
  int n = 0;
  if (0) {
  lab:;
  }
  int x[n % 1000 + 1];
  x[0]        = 1;
  x[n % 1000] = 2;
  p           = x;
  n++;
  if (n < LIMIT)
    goto lab;
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
// DEFAULT-NEXT:     global %0 p: volatile ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %2 lab:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %5: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%3), const<i32>(1000)), const<i32>(1))));
// DEFAULT-NEXT:         let %4 x: vla<i32, %5> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%4), const<i32>(0))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%4), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%3), const<i32>(1000)))), const<i32>(2));
// DEFAULT-NEXT:         write<ptr<void>, volatile>(%0, pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<i32>, length=None>(%4)));
// DEFAULT-NEXT:         let %6: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%7));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%3), const<i32>(1000000))
// DEFAULT-NEXT:             goto %2;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
