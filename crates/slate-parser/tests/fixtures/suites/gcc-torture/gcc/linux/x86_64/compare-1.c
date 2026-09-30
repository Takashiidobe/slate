/* Copyright (C) 2002 Free Software Foundation.

   Test for correctness of composite comparisons.

   Written by Roger Sayle, 3rd June 2002.  */

extern void abort(void);

int ieq(int x, int y, int ok) {
  if ((x <= y) && (x >= y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();

  if ((x <= y) && (x == y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();

  if ((x <= y) && (y <= x)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();

  if ((y == x) && (x <= y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ine(int x, int y, int ok) {
  if ((x < y) || (x > y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ilt(int x, int y, int ok) {
  if ((x < y) && (x != y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ile(int x, int y, int ok) {
  if ((x < y) || (x == y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int igt(int x, int y, int ok) {
  if ((x > y) && (x != y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ige(int x, int y, int ok) {
  if ((x > y) || (x == y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int main() {
  ieq(1, 4, 0);
  ieq(3, 3, 1);
  ieq(5, 2, 0);

  ine(1, 4, 1);
  ine(3, 3, 0);
  ine(5, 2, 1);

  ilt(1, 4, 1);
  ilt(3, 3, 0);
  ilt(5, 2, 0);

  ile(1, 4, 1);
  ile(3, 3, 1);
  ile(5, 2, 0);

  igt(1, 4, 0);
  igt(3, 3, 0);
  igt(5, 2, 1);

  ige(1, 4, 0);
  ige(3, 3, 1);
  ige(5, 2, 1);

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_ieq:[0-9]+]] @ieq(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_ok:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])), ge<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])), eq<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])), le<i32>(read<i32>(%[[VALUE_y]]), read<i32>(%[[VALUE_x]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_y]]), read<i32>(%[[VALUE_x]])), le<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ine:[0-9]+]] @ine(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32, %[[VALUE_ok_2:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_y_2]])), gt<i32>(read<i32>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_y_2]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_2]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_2]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ilt:[0-9]+]] @ilt(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32, %[[VALUE_ok_3:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_y_3]])), ne<i32>(read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_y_3]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_3]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_3]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ile:[0-9]+]] @ile(%[[VALUE_x_4:[0-9]+]] x: i32, %[[VALUE_y_4:[0-9]+]] y: i32, %[[VALUE_ok_4:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%[[VALUE_x_4]]), read<i32>(%[[VALUE_y_4]])), eq<i32>(read<i32>(%[[VALUE_x_4]]), read<i32>(%[[VALUE_y_4]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_4]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_4]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_igt:[0-9]+]] @igt(%[[VALUE_x_5:[0-9]+]] x: i32, %[[VALUE_y_5:[0-9]+]] y: i32, %[[VALUE_ok_5:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_x_5]]), read<i32>(%[[VALUE_y_5]])), ne<i32>(read<i32>(%[[VALUE_x_5]]), read<i32>(%[[VALUE_y_5]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_5]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_5]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ige:[0-9]+]] @ige(%[[VALUE_x_6:[0-9]+]] x: i32, %[[VALUE_y_6:[0-9]+]] y: i32, %[[VALUE_ok_6:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(gt<i32>(read<i32>(%[[VALUE_x_6]]), read<i32>(%[[VALUE_y_6]])), eq<i32>(read<i32>(%[[VALUE_x_6]]), read<i32>(%[[VALUE_y_6]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok_6]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_ok_6]]), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ieq]], const<i32>(1), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ieq]], const<i32>(3), const<i32>(3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ieq]], const<i32>(5), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ine]], const<i32>(1), const<i32>(4), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ine]], const<i32>(3), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ine]], const<i32>(5), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ilt]], const<i32>(1), const<i32>(4), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ilt]], const<i32>(3), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ilt]], const<i32>(5), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ile]], const<i32>(1), const<i32>(4), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ile]], const<i32>(3), const<i32>(3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ile]], const<i32>(5), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_igt]], const<i32>(1), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_igt]], const<i32>(3), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_igt]], const<i32>(5), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ige]], const<i32>(1), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ige]], const<i32>(3), const<i32>(3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_ige]], const<i32>(5), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
