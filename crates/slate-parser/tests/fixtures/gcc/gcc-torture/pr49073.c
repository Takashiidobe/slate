/* PR tree-optimization/49073 */

extern void abort(void);
int         a[] = {1, 2, 3, 4, 5, 6, 7}, c;

int main() {
  int   d = 1, i = 1;
  _Bool f = 0;
  do {
    d = a[i];
    if (f && d == 4) {
      ++c;
      break;
    }
    i++;
    f = (d == 3);
  } while (d < 7);
  if (c != 1)
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
// DEFAULT-NEXT:     global %1 a: array<i32, 7> [storage=static] = aggregate<array<i32, 7>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5), index5 = const<i32>(6), index6 = const<i32>(7)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 d: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %6 f: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %7
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%1), read<i32>(%5)))));
// DEFAULT-NEXT:                 if logical_and<bool>(read<bool>(%6), eq<i32>(read<i32>(%4), const<i32>(4)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %8: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                         let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%2, read<i32>(%9));
// DEFAULT-NEXT:                         break %7;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%11));
// DEFAULT-NEXT:                 write<bool>(%6, eq<i32>(read<i32>(%4), const<i32>(3)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while lt<i32>(read<i32>(%4), const<i32>(7));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
