#include <assert.h>

int   a, *b, c, d, f, **i, p, q, *r;
short o, j;

static int __attribute__((noinline, noclone)) fn1(int *p1, int **p2) {
  int **e = &b;
  for (; p; p++)
    *p1 = 1;
  *e = *p2 = &d;

  assert(r);

  return c;
}

static int **__attribute__((noinline, noclone)) fn2(void) {
  for (f = 0; f != 42; f++) {
    int *g[3] = {0, 0, 0};
    for (o = 0; o; o--)
      for (; a > 1;) {
        int **h[1] = {&g[2]};
      }
  }
  return &r;
}

int main(void) {
  i = fn2();
  fn1(b, i);
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
// DEFAULT-NEXT:     global %2 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 i: ptr<ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 p: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 q: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 r: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 o: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 j: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([105, 110, 116, 32, 102, 110, 49, 40, 105, 110, 116, 32, 42, 44, 32, 105, 110, 116, 32, 42, 42, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @__assert_fail(%21 __assertion: ptr<const i8>, %22 __file: ptr<const i8>, %23 __line: u32, %24 __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @__assert_single_arg(%25 <unnamed>: bool) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %13 @fn1(%14 p1: ptr<i32>, %15 p2: ptr<ptr<i32>>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 e: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%3);
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%14)), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%15)), addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%16)), addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         const<u64>(1);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<ptr<i32>>(read<ptr<i32>>(%10), null<ptr<i32>>)
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%27)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%28)), reinterpret<u32, reason=arg, fits=always>(const<i32>(12)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%29)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @fn2() -> ptr<ptr<i32>> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%6), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %18 g: array<ptr<i32>, 3> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 3>, zero_fill=false>(index0 = null<ptr<i32>>, index1 = null<ptr<i32>>, index2 = null<ptr<i32>>);
// DEFAULT-NEXT:                     for %31
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i16>(%11, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         condition: ne<i16>(read<i16>(%11), const<i16>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %37: i16 [synthetic] = read<i16>(%11);
// DEFAULT-NEXT:                             let %38: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%37)), const<i32>(1)));
// DEFAULT-NEXT:                             write<i16>(%11, read<i16>(%38));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             for %32
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                 condition: gt<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:                                 increment: omitted
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %19 h: array<ptr<ptr<i32>>, 1> [storage=automatic] = aggregate<array<ptr<ptr<i32>>, 1>, zero_fill=false>(index0 = addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(3)>(%18), const<i32>(2)))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return addr_of<ptr<ptr<i32>>>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%7, call<ptr<ptr<i32>>, signature=fn() -> ptr<ptr<i32>>>(%17));
// DEFAULT-NEXT:         call<ptr<ptr<i32>>, signature=fn() -> ptr<ptr<i32>>>(%17);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>, ptr<ptr<i32>>) -> i32>(%13, read<ptr<i32>>(%3), read<ptr<ptr<i32>>>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
