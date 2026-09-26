void abort(void);
void exit(int);

void test1(void) {
  int x = 3, y = 2;

  if ((x < y ? x++ : y++) != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test2(void) {
  int x = 3, y = 2, z;

  z = (x < y) ? x++ : y++;
  if (z != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test3(void) {
  int x = 3, y = 2;
  int xx = 3, yy = 2;

  if ((xx < yy ? x++ : y++) != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

int x, y;

static void init_xy(void) {
  x = 3;
  y = 2;
}

void test4(void) {
  init_xy();
  if ((x < y ? x++ : y++) != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test5(void) {
  int z;

  init_xy();
  z = (x < y) ? x++ : y++;
  if (z != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test6(void) {
  int xx = 3, yy = 2;
  int z;

  init_xy();
  z = (xx < y) ? x++ : y++;
  if (z != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
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
// DEFAULT-NEXT:     global %14 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 y: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%25 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 x: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %4 y: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %26: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%3), read<i32>(%4))
// DEFAULT-NEXT:             let %27: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:             let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(%28));
// DEFAULT-NEXT:             write<i32>(%26, read<i32>(%27));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %29: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:             let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%4, read<i32>(%30));
// DEFAULT-NEXT:             write<i32>(%26, read<i32>(%29));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%26), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 x: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %7 y: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %8 z: i32 [storage=automatic];
// DEFAULT-NEXT:         let %31: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%6), read<i32>(%7))
// DEFAULT-NEXT:             let %32: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%33));
// DEFAULT-NEXT:             write<i32>(%31, read<i32>(%32));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %34: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%35));
// DEFAULT-NEXT:             write<i32>(%31, read<i32>(%34));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%31));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 x: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %11 y: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %12 xx: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %13 yy: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %36: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%12), read<i32>(%13))
// DEFAULT-NEXT:             let %37: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%38));
// DEFAULT-NEXT:             write<i32>(%36, read<i32>(%37));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %39: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:             let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%11, read<i32>(%40));
// DEFAULT-NEXT:             write<i32>(%36, read<i32>(%39));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%36), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @init_xy() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%14, const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%15, const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         let %41: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%14), read<i32>(%15))
// DEFAULT-NEXT:             let %42: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:             let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%43));
// DEFAULT-NEXT:             write<i32>(%41, read<i32>(%42));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %44: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:             let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%15, read<i32>(%45));
// DEFAULT-NEXT:             write<i32>(%41, read<i32>(%44));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%41), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%14), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%15), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 z: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         let %46: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%14), read<i32>(%15))
// DEFAULT-NEXT:             let %47: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:             let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%48));
// DEFAULT-NEXT:             write<i32>(%46, read<i32>(%47));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %49: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:             let %50: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%15, read<i32>(%50));
// DEFAULT-NEXT:             write<i32>(%46, read<i32>(%49));
// DEFAULT-NEXT:         write<i32>(%19, read<i32>(%46));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%19), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%14), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%15), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21 xx: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %22 yy: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %23 z: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         let %51: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%21), read<i32>(%15))
// DEFAULT-NEXT:             let %52: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:             let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%53));
// DEFAULT-NEXT:             write<i32>(%51, read<i32>(%52));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %54: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:             let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%15, read<i32>(%55));
// DEFAULT-NEXT:             write<i32>(%51, read<i32>(%54));
// DEFAULT-NEXT:         write<i32>(%23, read<i32>(%51));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%23), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%14), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%15), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
