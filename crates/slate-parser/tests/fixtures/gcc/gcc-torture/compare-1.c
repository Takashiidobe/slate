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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @ieq(%2 x: i32, %3 y: i32, %4 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%2), read<i32>(%3)), ge<i32>(read<i32>(%2), read<i32>(%3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%2), read<i32>(%3)), eq<i32>(read<i32>(%2), read<i32>(%3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_and<bool>(le<i32>(read<i32>(%2), read<i32>(%3)), le<i32>(read<i32>(%3), read<i32>(%2)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(read<i32>(%3), read<i32>(%2)), le<i32>(read<i32>(%2), read<i32>(%3)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @ine(%6 x: i32, %7 y: i32, %8 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%6), read<i32>(%7)), gt<i32>(read<i32>(%6), read<i32>(%7)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%8), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ilt(%10 x: i32, %11 y: i32, %12 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%10), read<i32>(%11)), ne<i32>(read<i32>(%10), read<i32>(%11)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%12), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @ile(%14 x: i32, %15 y: i32, %16 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%14), read<i32>(%15)), eq<i32>(read<i32>(%14), read<i32>(%15)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%16), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%16), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @igt(%18 x: i32, %19 y: i32, %20 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%18), read<i32>(%19)), ne<i32>(read<i32>(%18), read<i32>(%19)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%20), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%20), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @ige(%22 x: i32, %23 y: i32, %24 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(gt<i32>(read<i32>(%22), read<i32>(%23)), eq<i32>(read<i32>(%22), read<i32>(%23)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<i32>(read<i32>(%24), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%24), const<i32>(0))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%1, const<i32>(1), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%1, const<i32>(3), const<i32>(3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%1, const<i32>(5), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%5, const<i32>(1), const<i32>(4), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%5, const<i32>(3), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%5, const<i32>(5), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%9, const<i32>(1), const<i32>(4), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%9, const<i32>(3), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%9, const<i32>(5), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%13, const<i32>(1), const<i32>(4), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%13, const<i32>(3), const<i32>(3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%13, const<i32>(5), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%17, const<i32>(1), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%17, const<i32>(3), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%17, const<i32>(5), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%21, const<i32>(1), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%21, const<i32>(3), const<i32>(3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32, i32) -> i32>(%21, const<i32>(5), const<i32>(2), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
