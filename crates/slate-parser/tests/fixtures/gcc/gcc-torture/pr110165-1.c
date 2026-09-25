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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 t: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %1 @f(%2 t: @type0, %3 a: i32, %4 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i32>, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 bd: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             let %19: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:             let %20: i32 [synthetic] = or<i32>(read<i32>(%19), read<i32>(%4));
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(%20));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 t: @type0 [storage=automatic];
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%8), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %9 a: i32 [storage=automatic] = const<i32>(16);
// DEFAULT-NEXT:                     let %10 b: i32 [storage=automatic] = const<i32>(15);
// DEFAULT-NEXT:                     let %11 c: i32 [storage=automatic] = or<i32>(read<i32>(%9), read<i32>(%10));
// DEFAULT-NEXT:                     let %12 t: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i32>(%8));
// DEFAULT-NEXT:                     let %13 r: i32 [storage=automatic] = call<i32, signature=fn(@type0, i32, i32) -> i32, abi=sysv64(coerce<i32>, scalar, scalar) -> scalar>(%1, copy<@type0, reason=arg>(read<@type0>(%12)), read<i32>(%9), read<i32>(%10));
// DEFAULT-NEXT:                     let %14 exp: i32 [storage=automatic] = conditional<i32>(ne<i32>(read<i32>(%8), const<i32>(0)), or<i32>(read<i32>(%9), read<i32>(%10)), read<i32>(%9));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%14), read<i32>(%13))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
