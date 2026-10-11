
// Check that the first finally block passes the enclosing function's frame
// pointer to the second finally block, instead of generating it via localaddr.

int
main(void) {
  int Check = 0;
  __try {
    Check = 3;
  } __finally {
    __try {
      Check += 2;
    } __finally {
      Check += 4;
    }
  }
  return Check;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_Check:[0-9]+]] Check: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_Check]], const<i32>(3));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_Check]]);
// DEFAULT-NEXT:                                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(2));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_Check]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         finally:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_Check]]);
// DEFAULT-NEXT:                                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(4));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_Check]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_Check]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
