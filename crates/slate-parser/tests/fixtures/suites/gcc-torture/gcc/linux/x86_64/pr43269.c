int g_21;
int g_211;
int g_261;

static void __attribute__((noinline, noclone)) func_32(int b) {
  if (b) {
  lbl_370:
    g_21 = 1;
  }

  for (g_261 = -1; g_261 > -2; g_261--) {
    if (g_211 + 1) {
      return;
    } else {
      g_21 = 1;
      goto lbl_370;
    }
  }
}

extern void abort(void);

int main(void) {
  func_32(0);
  if (g_261 != -1)
    abort();
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
// DEFAULT-NEXT:     global %[[VALUE_g_21:[0-9]+]] g_21: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g_211:[0-9]+]] g_211: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g_261:[0-9]+]] g_261: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func_32:[0-9]+]] @func_32(%[[VALUE_b:[0-9]+]] b: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %[[VALUE_lbl_370:[0-9]+]] lbl_370:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_g_21]], const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_g_261]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%[[VALUE_g_261]]), neg<i32, overflow=ub>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_g_261]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_g_261]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_g_211]]), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             return;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_g_21]], const<i32>(1));
// DEFAULT-NEXT:                             goto %[[VALUE_lbl_370]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_func_32]], const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_g_261]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
