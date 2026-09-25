extern void abort(void);

int a[20], b, c;

int fn1() {
  int d, e, f, g = 0;

  a[12] = 1;
  for (e = 0; e < 3; e++)
    for (d = 0; d < 2; d++) {
      for (f = 0; f < 2; f++) {
        g ^= a[12] > 1;
        if (g)
          return 0;
        if (b)
          break;
      }
      for (c = 0; c < 1; c++)
        a[d] = a[e * 3 + 9];
    }
  return 0;
}

int main() {
  fn1();
  if (a[0] != 0)
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
// DEFAULT-NEXT:     global %1 a: array<i32, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @fn1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 g: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%1), const<i32>(12))), const<i32>(1));
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %11
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%5), const<i32>(2))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %16: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%5, read<i32>(%17));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %12
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%7), const<i32>(2))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %18: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                                     let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%7, read<i32>(%19));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %20: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                                         let %21: i32 [synthetic] = xor<i32>(read<i32>(%20), from_bool<i32, reason=promotion>(gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%1), const<i32>(12)))), const<i32>(1))));
// DEFAULT-NEXT:                                         write<i32>(%8, read<i32>(%21));
// DEFAULT-NEXT:                                         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                                             return const<i32>(0);
// DEFAULT-NEXT:                                         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                                             break %12;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             for %13
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %22: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                                     let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%3, read<i32>(%23));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%1), read<i32>(%5))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%1), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%6), const<i32>(3)), const<i32>(9))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%4);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(20)>(%1), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
