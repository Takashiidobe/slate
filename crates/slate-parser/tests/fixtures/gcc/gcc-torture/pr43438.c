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
// DEFAULT-NEXT:     global %1 g_2: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %2 g_9: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 l_8: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @func_12(%5 p_13: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 l_17: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%2);
// DEFAULT-NEXT:         let %9: ptr<i32> [synthetic] = read<ptr<i32>>(%6);
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%9)));
// DEFAULT-NEXT:         let %11: i32 [synthetic] = and<i32>(read<i32>(%10), from_bool<i32, reason=promotion>(lt<i32>(const<i32>(0), read<i32>(%5))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%9)), read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 l_11: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(254)));
// DEFAULT-NEXT:         let %12: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%12)));
// DEFAULT-NEXT:         let %14: i32 [synthetic] = or<i32>(read<i32>(%13), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%1))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), read<i32>(%14));
// DEFAULT-NEXT:         let %15: u8 [synthetic] = read<u8>(%8);
// DEFAULT-NEXT:         let %16: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%15))), read<i32>(deref(read<ptr<i32>>(%3))))));
// DEFAULT-NEXT:         write<u8>(%8, read<u8>(%16));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(read<u8>(%8))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
