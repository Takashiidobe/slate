/* PR rtl-optimization/63659 */

int           a, b, c, *d = &b, g, h, i;
unsigned char e;
char          f;

int main() {
  while (a) {
    for (a = 0; a; a++)
      for (; c; c++)
        ;
    if (i)
      break;
  }

  char j = c, k = -1, l;
  l = g = j >> h;
  f     = l == 0 ? k : k % l;
  e     = 0 ? 0 : f;
  *d    = e;

  if (b != 255)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_b]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_a]], const<i32>(0));
// DEFAULT-NEXT:                     condition: ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 ;
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i8 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_g]], shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_j]])), read<i32>(%[[VALUE_h]])));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_l]], truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_j]])), read<i32>(%[[VALUE_h]]))));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_f]], truncate<i8, reason=assign, fits=unknown>(conditional<i32>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_l]])), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_k]])), rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_k]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_l]]))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_e]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), const<i32>(0), widen<i32, reason=promotion>(read<i8>(%[[VALUE_f]]))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_d]])), reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(%[[VALUE_e]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(255))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
