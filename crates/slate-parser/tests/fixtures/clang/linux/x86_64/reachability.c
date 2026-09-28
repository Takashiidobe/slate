int early_return(int a) {
  return a;
  a += 1;
}

int loop_break(int a) {
  while (a > 0) {
    a -= 1;
    break;
    a -= 1;
  }
  return a;
}

int jump_target(int a) {
  goto skip;
  a += 1;
skip:
  return a;
}

int branch(int a) {
  if (a > 0) {
    return 1;
  } else {
    return 0;
  }
  a += 1;
}

int main(void) {
  return early_return(1) + loop_break(2) + jump_target(3) + branch(4);
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
// DEFAULT-NEXT:     fn %0 @early_return(%1 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @loop_break(%3 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %10 gt<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%14));
// DEFAULT-NEXT:                 break %10;
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%16));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @jump_target(%6 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         goto %5;
// DEFAULT-NEXT:         let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:         label %5 skip:
// DEFAULT-NEXT:             return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @branch(%8 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %19: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(2))), call<i32, signature=fn(i32) -> i32>(%4, const<i32>(3))), call<i32, signature=fn(i32) -> i32>(%7, const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
