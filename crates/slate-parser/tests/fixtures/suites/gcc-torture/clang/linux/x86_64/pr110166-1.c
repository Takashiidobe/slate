struct s {
  int t  : 1;
  int t1 : 1;
};

int f(struct s t) __attribute__((noinline));
int f(struct s t) {
  int c = t.t;
  int d = t.t1;
  if (c > d)
    t.t = d;
  else
    t.t = c;
  return t.t;
}

int main(void) {
  struct s t;
  for (int i = -1; i <= 0; i++) {
    for (int j = -1; j <= 0; j++) {
      struct s t   = {i, j};
      int      r   = f(t);
      int      exp = i < j ? i : j;
      if (exp != r)
        __builtin_abort();
    }
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 t: i32 : 1;
// DEFAULT-NEXT:         field1 t1: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(1)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_t:[0-9]+]] t: @type[[TYPE_s]]) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_t]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = read<i32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_t]]));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_d]]))
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_t]]), read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_t]]), read<i32>(%[[VALUE_c]]));
// DEFAULT-NEXT:         return read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_t]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_t_3:[0-9]+]] t: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = read<i32>(%[[VALUE_i]]), field1 = read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                                 let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = call<i32, signature=fn(@type[[TYPE_s]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_f]], copy<@type[[TYPE_s]], reason=arg>(read<@type[[TYPE_s]]>(%[[VALUE_t_3]])));
// DEFAULT-NEXT:                                 let %[[VALUE_exp:[0-9]+]] exp: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]])), read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_exp]]), read<i32>(%[[VALUE_r]]))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
