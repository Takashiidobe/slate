
extern void exit(int);

int      a = 3, b, c, f, g, h;
unsigned d;
char    *e;

int main() {
  for (; a; a--) {
    int i;
    if (h && i)
      __builtin_printf("%d%d", c, f);
    i = 0;
    for (; i < 2; i++)
      if (g)
        for (; d < 10; d++)
          b = *e;
    i = 0;
    for (; i < 1; i++)
      ;
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 d: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 e: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 100, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:                     if logical_and<bool>(ne<i32>(read<i32>(%6), const<i32>(0)), ne<i32>(read<i32>(%10), const<i32>(0)))
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%13)), read<i32>(%3), read<i32>(%4));
// DEFAULT-NEXT:                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                     for %14
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%10), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%20));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:                                 for %15
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                     condition: lt<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %21: u32 [synthetic] = read<u32>(%7);
// DEFAULT-NEXT:                                         let %22: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%21), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                                         write<u32>(%7, read<u32>(%22));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         write<i32>(%2, widen<i32, reason=assign>(read<i8>(deref(read<ptr<i8>>(%8)))));
// DEFAULT-NEXT:                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                     for %16
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%10), const<i32>(1))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %23: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%24));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
