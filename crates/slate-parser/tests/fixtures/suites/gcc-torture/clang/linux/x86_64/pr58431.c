char         a, h;
int          b, d, e, g, j, k;
volatile int c;
short        i;

int main() {
  int m;

  m = i ^= 1;
  for (b = 0; b < 1; b++) {
    char o = m;
    g      = k;
    j      = j || c;
    if (a != o)
      for (; d < 1; d++)
        ;
    else {
      char *p = &h;
      *p      = 1;
      for (; e; e++)
        ;
    }
  }

  if (h != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(xor<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE0]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_i]], read<i16>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_m]], widen<i32, reason=assign>(read<i16>(%[[VALUE1]])));
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_b]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_o:[0-9]+]] o: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_m]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE_k]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_j]], from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0)), ne<i32>(read<i32, volatile>(%[[VALUE_c]]), const<i32>(0)))));
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_a]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_o]])))
// DEFAULT-NEXT:                         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%[[VALUE_d]]), const<i32>(1))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 ;
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_h]]);
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE_p]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                             for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                 condition: ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_h]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
