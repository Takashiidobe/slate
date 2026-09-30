void exit(int);

int a, b, c, d = 1, e;

static signed char foo() {
  int f, g = a;

  for (f = 1; f < 3; f++)
    for (; b < 1; b++) {
      if (d)
        for (c = 0; c < 4; c++)
          for (f = 0; f < 3; f++) {
            for (e = 0; e < 1; e++)
              a = g;
            if (f)
              break;
          }
      else if (f)
        continue;
      return 0;
    }
  return 0;
}

int main() {
  foo();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_f]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_f]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_f]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_b]]), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:                                 for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_c]], const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(4))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                             init:
// DEFAULT-NEXT:                                                 write<i32>(%[[VALUE_f]], const<i32>(0));
// DEFAULT-NEXT:                                             condition: lt<i32>(read<i32>(%[[VALUE_f]]), const<i32>(3))
// DEFAULT-NEXT:                                             increment: {
// DEFAULT-NEXT:                                                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_f]]);
// DEFAULT-NEXT:                                                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                                 write<i32>(%[[VALUE_f]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                                                 yield void;
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                             body:
// DEFAULT-NEXT:                                                 {
// DEFAULT-NEXT:                                                     for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                                         init:
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_e]], const<i32>(0));
// DEFAULT-NEXT:                                                         condition: lt<i32>(read<i32>(%[[VALUE_e]]), const<i32>(1))
// DEFAULT-NEXT:                                                         increment: {
// DEFAULT-NEXT:                                                             let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                                                             let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                                                             yield void;
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         body:
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE_g]]));
// DEFAULT-NEXT:                                                     if ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(0))
// DEFAULT-NEXT:                                                         break %[[VALUE10]];
// DEFAULT-NEXT:                                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(0))
// DEFAULT-NEXT:                                     continue %[[VALUE4]];
// DEFAULT-NEXT:                             return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i8, signature=fn() -> i8>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
