/* PR tree-optimization/59387 */

int  a, *d, **e = &d, f;
char c;
struct S {
  int f1;
} b;

int main() {
  for (a = -19; a; a++) {
    for (b.f1 = 0; b.f1 < 24; b.f1++)
      c--;
    *e = &f;
    if (!d)
      return 0;
  }
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 f1: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 d: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 e: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%1) [linkage=external];
// DEFAULT-NEXT:     global %3 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%0, neg<i32, overflow=ub>(const<i32>(19)));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %9
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(field0(%6), const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(field0(%6)), const<i32>(24))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %12: i32 [synthetic] = read<i32>(field0(%6));
// DEFAULT-NEXT:                             let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(field0(%6), read<i32>(%13));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %14: i8 [synthetic] = read<i8>(%4);
// DEFAULT-NEXT:                             let %15: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%14)), const<i32>(1)));
// DEFAULT-NEXT:                             write<i8>(%4, read<i8>(%15));
// DEFAULT-NEXT:                     write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%2)), addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:                     if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%1), null<ptr<i32>>))
// DEFAULT-NEXT:                         return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
