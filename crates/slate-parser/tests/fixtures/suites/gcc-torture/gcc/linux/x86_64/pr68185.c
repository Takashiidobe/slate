
int   a, b, d = 1, e, f, o, u, w = 1, z;
short c, q, t;

int main() {
  char g;
  for (; d; d--) {
    while (o)
      for (; e;) {
        c     = b;
        int h = o = z;
        for (; u;)
          for (; a;)
            ;
      }
    if (t < 1)
      g = w;
    f = g;
    g && (q = 1);
  }

  if (q != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i8 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     while %[[VALUE3:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_o]]), const<i32>(0))
// DEFAULT-NEXT:                         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                             condition: ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))
// DEFAULT-NEXT:                             increment: omitted
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i16>(%[[VALUE_c]], truncate<i16, reason=assign, fits=unknown>(read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:                                     let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_o]], read<i32>(%[[VALUE_z]]));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_h]], read<i32>(%[[VALUE_z]]));
// DEFAULT-NEXT:                                     for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                         condition: ne<i32>(read<i32>(%[[VALUE_u]]), const<i32>(0))
// DEFAULT-NEXT:                                         increment: omitted
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                                                 init:
// DEFAULT-NEXT:                                                 condition: ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:                                                 increment: omitted
// DEFAULT-NEXT:                                                 body:
// DEFAULT-NEXT:                                                     ;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_t]])), const<i32>(1))
// DEFAULT-NEXT:                         write<i8>(%[[VALUE_g]], truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_w]])));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_f]], widen<i32, reason=assign>(read<i8>(%[[VALUE_g]])));
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i8>(read<i8>(%[[VALUE_g]]), const<i8>(0))
// DEFAULT-NEXT:                         write<i16>(%[[VALUE_q]], truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<bool>(%[[VALUE7]], ne<i16>(truncate<i16, reason=assign, fits=always>(const<i32>(1)), const<i16>(0)));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE7]], const<bool>(false));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_q]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
