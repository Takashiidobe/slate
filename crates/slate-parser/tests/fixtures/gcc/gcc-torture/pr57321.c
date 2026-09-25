/* PR tree-optimization/57321 */

int a = 1, *b, **c;

static int foo(int *p) {
  if (*p == a) {
    int  *i[7][5] = {{0}};
    int **j[1][1];
    j[0][0] = &i[0][0];
    *b      = &p != c;
  }
  return 0;
}

int main() {
  int i = 0;
  foo(&i);
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: ptr<ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(deref(read<ptr<i32>>(%4))), read<i32>(%0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 i: array<array<ptr<i32>, 5>, 7> [storage=automatic] = aggregate<array<array<ptr<i32>, 5>, 7>, zero_fill=true>(index0 = aggregate<array<ptr<i32>, 5>, zero_fill=true>(index0 = null<ptr<i32>>));
// DEFAULT-NEXT:                 let %6 j: array<array<ptr<ptr<i32>>, 1>, 1> [storage=automatic];
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<ptr<i32>>>, subtract=false, element=ptr<ptr<i32>>, overflow=ub>(array_decay<ptr<ptr<ptr<i32>>>, length=Some(1)>(deref(ptr_offset<ptr<array<ptr<ptr<i32>>, 1>>, subtract=false, element=array<ptr<ptr<i32>>, 1>, overflow=ub>(array_decay<ptr<array<ptr<ptr<i32>>, 1>>, length=Some(1)>(%6), const<i32>(0)))), const<i32>(0))), addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(5)>(deref(ptr_offset<ptr<array<ptr<i32>, 5>>, subtract=false, element=array<ptr<i32>, 5>, overflow=ub>(array_decay<ptr<array<ptr<i32>, 5>>, length=Some(7)>(%5), const<i32>(0)))), const<i32>(0)))));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%1)), from_bool<i32, reason=assign>(ne<ptr<ptr<i32>>>(addr_of<ptr<ptr<i32>>>(%4), read<ptr<ptr<i32>>>(%2))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>) -> i32>(%3, addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
