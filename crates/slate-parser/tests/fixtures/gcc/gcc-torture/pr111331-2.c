
int a;
int b;

int main() {
  int d = b + 30;
  {
    int t;
    if (d < 29)
      t = 29;
    else
      t = (d > 28) ? 28 : d;
    a = t;
  }
  volatile int t = a;
  if (a != 28)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 d: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%1), const<i32>(30));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %4 t: i32 [storage=automatic];
// DEFAULT-NEXT:             if lt<i32>(read<i32>(%3), const<i32>(29))
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(29));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<i32>(%4, conditional<i32>(gt<i32>(read<i32>(%3), const<i32>(28)), const<i32>(28), read<i32>(%3)));
// DEFAULT-NEXT:             write<i32>(%0, read<i32>(%4));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %5 t: volatile i32 [storage=automatic] = read<i32>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(28))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
