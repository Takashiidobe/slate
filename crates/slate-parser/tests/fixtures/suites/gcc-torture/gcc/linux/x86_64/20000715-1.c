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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_y_2]]))
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y_2]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y_2]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_y_3]]))
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_3]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE6]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y_3]]);
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y_3]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE6]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_z]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_z]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y_3]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_xx:[0-9]+]] xx: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_yy:[0-9]+]] yy: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_xx]]), read<i32>(%[[VALUE_yy]]))
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_4]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE11]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y_4]]);
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y_4]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE11]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE11]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y_4]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init_xy:[0-9]+]] @init_xy() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_init_xy]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]]))
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE16]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE16]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE16]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_init_xy]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]]))
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE21]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE21]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_z_2]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_z_2]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_xx_2:[0-9]+]] xx: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_yy_2:[0-9]+]] yy: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_z_3:[0-9]+]] z: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_init_xy]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_xx_2]]), read<i32>(%[[VALUE_y]]))
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE26]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE26]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_z_3]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_z_3]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test4]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test5]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test6]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
