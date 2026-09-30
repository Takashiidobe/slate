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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: ptr<ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, {{[0-9]+}}> [storage=static] = code_units<array<i8, {{[0-9]+}}>>({{\[[0-9, ]+\]}}) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 110, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___assert_fail:[0-9]+]] @__assert_fail(%[[VALUE___assertion:[0-9]+]] __assertion: ptr<const i8>, %[[VALUE___file:[0-9]+]] __file: ptr<const i8>, %[[VALUE___line:[0-9]+]] __line: u32, %[[VALUE___function:[0-9]+]] __function: ptr<const i8>) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___assert_single_arg:[0-9]+]] @__assert_single_arg(%[[VALUE0:[0-9]+]] <unnamed>: bool) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p1:[0-9]+]] p1: ptr<i32>, %[[VALUE_p2:[0-9]+]] p2: ptr<ptr<i32>>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_p]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_p]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_p1]])), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic] = addr_of<ptr<i32>>(%[[VALUE_d]]);
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_p2]])), read<ptr<i32>>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_e]])), read<ptr<i32>>(%[[VALUE4]]));
// DEFAULT-NEXT:         const<u64>(1);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_r]]), null<ptr<i32>>)
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>, ptr<const i8>, u32, ptr<const i8>) -> void>(%[[VALUE___assert_fail]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some({{[0-9]+}})>(%[[VALUE_str_2]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(12)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> ptr<ptr<i32>> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f]], const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_f]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_g:[0-9]+]] g: array<ptr<i32>, 3> [storage=automatic] [align=16] = aggregate<array<ptr<i32>, 3>, zero_fill=false>(index0 = null<ptr<i32>>, index1 = null<ptr<i32>>, index2 = null<ptr<i32>>);
// DEFAULT-NEXT:                     for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i16>(%[[VALUE_o]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         condition: ne<i16>(read<i16>(%[[VALUE_o]]), const<i16>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_o]]);
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE9]])), const<i32>(1)));
// DEFAULT-NEXT:                             write<i16>(%[[VALUE_o]], read<i16>(%[[VALUE10]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                 condition: gt<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1))
// DEFAULT-NEXT:                                 increment: omitted
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_h:[0-9]+]] h: array<ptr<ptr<i32>>, 1> [storage=automatic] = aggregate<array<ptr<ptr<i32>>, 1>, zero_fill=false>(index0 = addr_of<ptr<ptr<i32>>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(3)>(%[[VALUE_g]]), const<i32>(2)))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return addr_of<ptr<ptr<i32>>>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_i]], call<ptr<ptr<i32>>, signature=fn() -> ptr<ptr<i32>>>(%[[VALUE_fn2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>, ptr<ptr<i32>>) -> i32>(%[[VALUE_fn1]], read<ptr<i32>>(%[[VALUE_b]]), read<ptr<ptr<i32>>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
