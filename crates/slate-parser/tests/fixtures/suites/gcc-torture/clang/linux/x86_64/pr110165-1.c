struct s {
  int t : 1;
};

int f(struct s t, int a, int b) __attribute__((noinline));
int f(struct s t, int a, int b) {
  int bd = t.t;
  if (bd)
    a |= b;
  return a;
}

int main(void) {
  struct s t;
  for (int i = -1; i <= 1; i++) {
    int      a   = 0x10;
    int      b   = 0x0f;
    int      c   = a | b;
    struct s t   = {i};
    int      r   = f(t, a, b);
    int      exp = (i != 0) ? a | b : a;
    if (exp != r)
      __builtin_abort();
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
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_t:[0-9]+]] t: @type[[TYPE_s]], %[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_bd:[0-9]+]] bd: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_t]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bd]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic] = const<i32>(16);
// DEFAULT-NEXT:                     let %[[VALUE_b_2:[0-9]+]] b: i32 [storage=automatic] = const<i32>(15);
// DEFAULT-NEXT:                     let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = or<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:                     let %[[VALUE_t_3:[0-9]+]] t: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                     let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = call<i32, signature=fn(@type[[TYPE_s]], i32, i32) -> i32, abi=sysv64(native_c, scalar, scalar) -> scalar>(%[[VALUE_f]], copy<@type[[TYPE_s]], reason=arg>(read<@type[[TYPE_s]]>(%[[VALUE_t_3]])), read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:                     let %[[VALUE_exp:[0-9]+]] exp: i32 [storage=automatic] = conditional<i32>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), or<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])), read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_exp]]), read<i32>(%[[VALUE_r]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
