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
// DEFAULT-NEXT:     global %6 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 b: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 i: ptr<ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 p: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 q: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 r: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 o: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 j: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 110, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @__assert_fail(%25 __assertion: ptr<const i8>, %26 __file: ptr<const i8>, %27 __line: u32, %28 __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @__assert_single_arg(%29 <unnamed>: bool) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %17 @fn1(%18 p1: ptr<i32>, %19 p2: ptr<ptr<i32>>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 e: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%7);
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%38));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%18)), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%19)), addr_of<ptr<i32>>(%9));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%20)), addr_of<ptr<i32>>(%9));
// DEFAULT-NEXT:         const<u64>(1);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<ptr<i32>>(read<ptr<i32>>(%14), null<ptr<i32>>)
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%31)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%32)), reinterpret<u32, reason=arg, fits=always>(const<i32>(12)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%33)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @fn2() -> ptr<ptr<i32>> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%10), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%40));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %22 g: array<ptr<i32>, 3> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 3>, zero_fill=false>(index0 = null<ptr<i32>>, index1 = null<ptr<i32>>, index2 = null<ptr<i32>>);
// DEFAULT-NEXT:                     for %35
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i16>(%15, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         condition: ne<i16>(read<i16>(%15), const<i16>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %41: i16 [synthetic] = read<i16>(%15);
// DEFAULT-NEXT:                             let %42: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%41)), const<i32>(1)));
// DEFAULT-NEXT:                             write<i16>(%15, read<i16>(%42));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             for %36
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                 condition: gt<i32>(read<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:                                 increment: omitted
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %23 h: array<ptr<ptr<i32>>, 1> [storage=automatic] = aggregate<array<ptr<ptr<i32>>, 1>, zero_fill=false>(index0 = addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(3)>(%22), const<i32>(2)))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return addr_of<ptr<ptr<i32>>>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%11, call<ptr<ptr<i32>>, signature=fn() -> ptr<ptr<i32>>>(%21));
// DEFAULT-NEXT:         call<ptr<ptr<i32>>, signature=fn() -> ptr<ptr<i32>>>(%21);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>, ptr<ptr<i32>>) -> i32>(%17, read<ptr<i32>>(%7), read<ptr<ptr<i32>>>(%11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
