/* { dg-options "-fno-builtin-abort" } */

int           a, b, m, n, o, p, s, u, i;
char          c, q, y;
short         d;
unsigned char e;
static int    f, h;
static short  g, r, v;
unsigned      t;

extern void abort();

int fn1(int p1) { return a ? p1 : p1 + a; }

unsigned char fn2(unsigned char p1, int p2) { return p2 >= 2 ? p1 : p1 >> p2; }

static short fn3() {
  int w, x = 0;
  for (; p < 31; p++) {
    s = fn1(c | ((1 && c) == c));
    t = fn2(s, x);
    c = (unsigned)c > -(unsigned)((o = (m = d = t) == p) <= 4UL) && n;
    v = -c;
    y = 1;
    for (; y; y++)
      e = v == 1;
    d = 0;
    for (; h != 2;) {
      for (;;) {
        if (!m)
          abort();
        r = 7 - f;
        x = e = i | r;
        q     = u * g;
        w     = b == q;
        if (w)
          break;
      }
      break;
    }
  }
  return x;
}

int main() {
  fn3();
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
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p1:[0-9]+]] p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), read<i32>(%[[VALUE_p1]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_p1]]), read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_p1_2:[0-9]+]] p1: u8, %[[VALUE_p2:[0-9]+]] p2: i32) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%[[VALUE_p2]]), const<i32>(2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_p1_2]]))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_p1_2]]))), read<i32>(%[[VALUE_p2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3() -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_p]]), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_p]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_s]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn1]], or<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), from_bool<i32, reason=promotion>(eq<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i8>(read<i8>(%[[VALUE_c]]), const<i8>(0)))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])))))));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn1]], or<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), from_bool<i32, reason=promotion>(eq<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i8>(read<i8>(%[[VALUE_c]]), const<i8>(0)))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]]))))));
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_t]], widen<u32, reason=assign>(call<u8, signature=fn(u8, i32) -> u8>(%[[VALUE_fn2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_s]]))), read<i32>(%[[VALUE_x]]))));
// DEFAULT-NEXT:                     widen<u32, reason=assign>(call<u8, signature=fn(u8, i32) -> u8>(%[[VALUE_fn2]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_s]]))), read<i32>(%[[VALUE_x]])));
// DEFAULT-NEXT:                     write<i16>(%[[VALUE_d]], reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_t]]))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_m]], widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_t]])))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_o]], from_bool<i32, reason=assign>(eq<i32>(widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_t]])))), read<i32>(%[[VALUE_p]]))));
// DEFAULT-NEXT:                     write<i8>(%[[VALUE_c]], from_bool<i8, reason=assign>(logical_and<bool>(gt<u32>(reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(%[[VALUE_c]]))), neg<u32, overflow=wrap>(from_bool<u32, reason=explicit>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(from_bool<i32, reason=assign>(eq<i32>(widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_t]])))), read<i32>(%[[VALUE_p]]))))), const<u64>(4))))), ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i16>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])))));
// DEFAULT-NEXT:                     write<i8>(%[[VALUE_y]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i8>(read<i8>(%[[VALUE_y]]), const<i8>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_y]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE4]])), const<i32>(1)));
// DEFAULT-NEXT:                             write<i8>(%[[VALUE_y]], read<i8>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<u8>(%[[VALUE_e]], from_bool<u8, reason=assign>(eq<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_v]])), const<i32>(1))));
// DEFAULT-NEXT:                     write<i16>(%[[VALUE_d]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(2))
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                     condition: omitted
// DEFAULT-NEXT:                                     increment: omitted
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             if not<bool>(ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(0)))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                             write<i16>(%[[VALUE_r]], truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(7), read<i32>(%[[VALUE_f]]))));
// DEFAULT-NEXT:                                             write<u8>(%[[VALUE_e]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(read<i32>(%[[VALUE_i]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_r]]))))));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_x]], reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(read<i32>(%[[VALUE_i]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_r]]))))))));
// DEFAULT-NEXT:                                             write<i8>(%[[VALUE_q]], truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_u]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_g]])))));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_w]], from_bool<i32, reason=assign>(eq<i32>(read<i32>(%[[VALUE_b]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_q]])))));
// DEFAULT-NEXT:                                             if ne<i32>(read<i32>(%[[VALUE_w]]), const<i32>(0))
// DEFAULT-NEXT:                                                 break %[[VALUE7]];
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                 break %[[VALUE6]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i16, signature=fn() -> i16>(%[[VALUE_fn3]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
