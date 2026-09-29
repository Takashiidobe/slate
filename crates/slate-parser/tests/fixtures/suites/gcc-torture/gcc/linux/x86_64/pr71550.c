
extern void exit(int);

int      a = 3, b, c, f, g, h;
unsigned d;
char    *e;

int main() {
  for (; a; a--) {
    int i;
    if (h && i)
      __builtin_printf("%d%d", c, f);
    i = 0;
    for (; i < 2; i++)
      if (g)
        for (; d < 10; d++)
          b = *e;
    i = 0;
    for (; i < 1; i++)
      ;
  }
  exit(0);
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 100, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_printf:[0-9]+]] @__builtin_printf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:                     if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))
// DEFAULT-NEXT:                         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE___builtin_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]])), read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_f]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                     for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0))
// DEFAULT-NEXT:                                 for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                     condition: lt<u32>(read<u32>(%[[VALUE_d]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE9:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                                         let %[[VALUE10:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE9]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                                         write<u32>(%[[VALUE_d]], read<u32>(%[[VALUE10]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_b]], widen<i32, reason=assign>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_e]])))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                     for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
