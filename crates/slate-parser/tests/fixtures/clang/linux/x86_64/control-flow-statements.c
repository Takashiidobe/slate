int if_else(int x) {
  if (x) {
    return 1;
  } else {
    return 0;
  }
}

int while_loop(int x) {
  while (x) {
    break;
  }
  return x;
}

int do_while_loop(int x) {
  do {
    continue;
  } while (x);
  return x;
}

int for_loop(int x) {
  for (x; x; x) {
    break;
  }
  return x;
}

int switch_stmt(int x) {
  switch (x) {
  case 1:
    return 1;
  case 2:
    return 2;
  default:
    return 0;
  }
}

int labels_and_goto(int x) {
  goto done;
done:
  return x;
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
// DEFAULT-NEXT:     fn %0 @if_else(%1 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @while_loop(%3 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %13 ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 break %13;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @do_while_loop(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         do %14
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 continue %14;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(%5), const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @for_loop(%7 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 read<i32>(%7);
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 read<i32>(%7);
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     break %15;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @switch_stmt(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %16 read<i32>(%9)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %16 const<i32>(1):
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:                 case %16 const<i32>(2):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %16:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @labels_and_goto(%12 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         goto %11;
// DEFAULT-NEXT:         label %11 done:
// DEFAULT-NEXT:             return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
