/* PR rtl-optimization/71532 */
/* { dg-do run } */
/* { dg-additional-options "-mtune=slm" { target i?86-*-* x86_64-*-* } } */

__attribute__((noinline, noclone, pure)) int foo(int a, int b, int c, int d,
                                                 int e, int f, int g, int h,
                                                 int i, int j, int k, int l) {
  a++;
  b++;
  c++;
  d++;
  e++;
  f++;
  g++;
  h++;
  i++;
  j++;
  k++;
  l++;
  asm volatile("" : : "g"(&a), "g"(&b), "g"(&c), "g"(&d) : "memory");
  asm volatile("" : : "g"(&e), "g"(&f), "g"(&g), "g"(&h) : "memory");
  asm volatile("" : : "g"(&i), "g"(&j), "g"(&k), "g"(&l) : "memory");
  return a + b + c + d + e + f + g + h + i + j + k + l;
}

__attribute__((noinline, noclone, pure)) int bar(int a, int b, int c, int d,
                                                 int e, int f, int g, int h,
                                                 int i, int j, int k, int l) {
  a++;
  b++;
  c++;
  d++;
  e++;
  f++;
  g++;
  h++;
  i++;
  j++;
  k++;
  l++;
  asm volatile("" : : "g"(&a), "g"(&b), "g"(&c), "g"(&d) : "memory");
  asm volatile("" : : "g"(&e), "g"(&f), "g"(&g), "g"(&h) : "memory");
  asm volatile("" : : "g"(&i), "g"(&j), "g"(&k), "g"(&l) : "memory");
  return 2 * a + b + c + d + e + f + g + h + i + j + k + l;
}

__attribute__((noinline, noclone)) int test() {
  int a  = foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
  a     += bar(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
  return a;
}

int
main() {
  if (test() != 182)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 a: i32, %2 b: i32, %3 c: i32, %4 d: i32, %5 e: i32, %6 f: i32, %7 g: i32, %8 h: i32, %9 i: i32, %10 j: i32, %11 k: i32, %12 l: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [memory=read] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%31));
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%33));
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%35));
// DEFAULT-NEXT:         let %36: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%37));
// DEFAULT-NEXT:         let %38: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%39));
// DEFAULT-NEXT:         let %40: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%41));
// DEFAULT-NEXT:         let %42: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%43));
// DEFAULT-NEXT:         let %44: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%45));
// DEFAULT-NEXT:         let %46: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%47));
// DEFAULT-NEXT:         let %48: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%49));
// DEFAULT-NEXT:         let %50: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%51));
// DEFAULT-NEXT:         let %52: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:         let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(%53));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%1);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%2);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%3);
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%4);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%5);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%6);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%7);
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%8);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%9);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%10);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%11);
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%12);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%1), read<i32>(%2)), read<i32>(%3)), read<i32>(%4)), read<i32>(%5)), read<i32>(%6)), read<i32>(%7)), read<i32>(%8)), read<i32>(%9)), read<i32>(%10)), read<i32>(%11)), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar(%14 a: i32, %15 b: i32, %16 c: i32, %17 d: i32, %18 e: i32, %19 f: i32, %20 g: i32, %21 h: i32, %22 i: i32, %23 j: i32, %24 k: i32, %25 l: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [memory=read] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %54: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:         let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%14, read<i32>(%55));
// DEFAULT-NEXT:         let %56: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:         let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%57));
// DEFAULT-NEXT:         let %58: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:         let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%16, read<i32>(%59));
// DEFAULT-NEXT:         let %60: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:         let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%17, read<i32>(%61));
// DEFAULT-NEXT:         let %62: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%63));
// DEFAULT-NEXT:         let %64: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:         let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%19, read<i32>(%65));
// DEFAULT-NEXT:         let %66: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:         let %67: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%66), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%20, read<i32>(%67));
// DEFAULT-NEXT:         let %68: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:         let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%21, read<i32>(%69));
// DEFAULT-NEXT:         let %70: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:         let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%22, read<i32>(%71));
// DEFAULT-NEXT:         let %72: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:         let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%72), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%23, read<i32>(%73));
// DEFAULT-NEXT:         let %74: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:         let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%24, read<i32>(%75));
// DEFAULT-NEXT:         let %76: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:         let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%25, read<i32>(%77));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%14);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%15);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%16);
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%17);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%18);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%19);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%20);
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%21);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%22);
// DEFAULT-NEXT:             in 1 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%23);
// DEFAULT-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%24);
// DEFAULT-NEXT:             in 3 "g" [reg | mem | imm] -> reg width 64 addr_of<ptr<i32>>(%25);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), read<i32>(%14)), read<i32>(%15)), read<i32>(%16)), read<i32>(%17)), read<i32>(%18)), read<i32>(%19)), read<i32>(%20)), read<i32>(%21)), read<i32>(%22)), read<i32>(%23)), read<i32>(%24)), read<i32>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @test() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 a: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32) -> i32>(%0, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12));
// DEFAULT-NEXT:         let %78: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:         let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%78), call<i32, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32) -> i32>(%13, const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12)));
// DEFAULT-NEXT:         write<i32>(%27, read<i32>(%79));
// DEFAULT-NEXT:         return read<i32>(%27);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%26), const<i32>(182))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
