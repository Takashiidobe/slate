extern void abort(void);

static unsigned char g_2 = 1;
static int           g_9;
static int          *l_8 = &g_9;

static void func_12(int p_13) {
  int *l_17  = &g_9;
  *l_17     &= 0 < p_13;
}

int main(void) {
  unsigned char l_11  = 254;
  *l_8               |= g_2;
  l_11               |= *l_8;
  func_12(l_11);
  if (g_9 != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_g_2:[0-9]+]] g_2: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g_9:[0-9]+]] g_9: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_l_8:[0-9]+]] l_8: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_g_9]]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_func_12:[0-9]+]] @func_12(%[[VALUE_p_13:[0-9]+]] p_13: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_l_17:[0-9]+]] l_17: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_g_9]]);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_l_17]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = and<i32>(read<i32>(%[[VALUE1]]), from_bool<i32, reason=promotion>(lt<i32>(const<i32>(0), read<i32>(%[[VALUE_p_13]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE0]])), read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_l_11:[0-9]+]] l_11: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(254)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_l_8]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE3]])));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE4]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_g_2]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE3]])), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_l_11]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE6]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_l_8]]))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_l_11]], read<u8>(%[[VALUE7]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_func_12]], reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%[[VALUE_l_11]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_g_9]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
