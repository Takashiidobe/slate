extern void exit(int);

int a, b, c, d, e, f[3];

int main() {
  while (d)
    while (1)
      ;
  int g = 0, h, i = 0;
  for (; g < 21; g += 9) {
    int j = 1;
    for (h = 0; h < 3; h++)
      f[h] = 1;
    for (; j < 10; j++) {
      d = i && (b ? 0 : c);
      i = 1;
      if (g)
        a = e;
    }
  }
  exit(0);
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: array<i32, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         while %13 ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             while %14 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         let %8 g: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %9 h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(21))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(9));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 j: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:                     for %16
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%9), const<i32>(3))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                             let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%9, read<i32>(%21));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%6), read<i32>(%9))), const<i32>(1));
// DEFAULT-NEXT:                     for %17
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%11), const<i32>(10))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %22: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                             let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%11, read<i32>(%23));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%4, from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%10), const<i32>(0)), ne<i32>(conditional<i32>(ne<i32>(read<i32>(%2), const<i32>(0)), const<i32>(0), read<i32>(%3)), const<i32>(0)))));
// DEFAULT-NEXT:                                 write<i32>(%10, const<i32>(1));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                                     write<i32>(%1, read<i32>(%5));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
