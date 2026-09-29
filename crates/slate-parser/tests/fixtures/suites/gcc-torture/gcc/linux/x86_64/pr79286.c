int        a = 0, c = 0;
static int d[][8] = {};

int main() {
  int e;
  for (int b = 0; b < 4; b++) {
    __builtin_printf("%d\n", b, e);
    while (a && c++)
      e = d[300000000000000000][0];
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %1 c: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %2 d: array<array<i32, 8>, 0> [storage=static] = aggregate<array<array<i32, 8>, 0>, zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %8 @__builtin_printf(%7 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 e: i32 [storage=automatic];
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %5 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%12));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%8, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), read<i32>(%5), read<i32>(%4));
// DEFAULT-NEXT:                     while %10 {
// DEFAULT-NEXT:                         let %13: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:                             let %14: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                             let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%1, read<i32>(%15));
// DEFAULT-NEXT:                             write<bool>(%13, ne<i32>(read<i32>(%14), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%13, const<bool>(false));
// DEFAULT-NEXT:                         yield read<bool>(%13);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(deref(ptr_offset<ptr<array<i32, 8>>, subtract=false, element=array<i32, 8>, overflow=ub>(array_decay<ptr<array<i32, 8>>, length=Some(0)>(%2), const<i64>(300000000000000000)))), const<i32>(0)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
