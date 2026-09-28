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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 t: i32 : 1;
// DEFAULT-NEXT:         field1 t1: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(1)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %1 @f(%2 t: @type0) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 c: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2));
// DEFAULT-NEXT:         let %4 d: i32 [storage=automatic] = read<i32>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%2));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%3), read<i32>(%4))
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), read<i32>(%4));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2), read<i32>(%3));
// DEFAULT-NEXT:         return read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 t: @type0 [storage=automatic];
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %7 i: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %14
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %8 j: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(%19));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %9 t: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i32>(%7), field1 = read<i32>(%8));
// DEFAULT-NEXT:                                 let %10 r: i32 [storage=automatic] = call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%1, copy<@type0, reason=arg>(read<@type0>(%9)));
// DEFAULT-NEXT:                                 let %11 exp: i32 [storage=automatic] = conditional<i32>(lt<i32>(read<i32>(%7), read<i32>(%8)), read<i32>(%7), read<i32>(%8));
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%11), read<i32>(%10))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
