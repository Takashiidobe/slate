void abort(void);
void exit(int);

struct a {
  char  a, b;
  short c;
};

int a1() {
  static struct a x = {1, 2, ~1}, y = {65, 2, ~2};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int a2() {
  static struct a x = {1, 66, ~1}, y = {1, 2, ~2};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int a3() {
  static struct a x = {9, 66, ~1}, y = {33, 18, ~2};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

struct b {
  int   c;
  short b, a;
};

int b1() {
  static struct b x = {~1, 2, 1}, y = {~2, 2, 65};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int b2() {
  static struct b x = {~1, 66, 1}, y = {~2, 2, 1};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int b3() {
  static struct b x = {~1, 66, 9}, y = {~2, 18, 33};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

struct c {
  unsigned int c : 4, b : 14, a : 14;
} __attribute__((aligned));

int c1() {
  static struct c x = {~1, 2, 1}, y = {~2, 2, 65};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int c2() {
  static struct c x = {~1, 66, 1}, y = {~2, 2, 1};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int c3() {
  static struct c x = {~1, 66, 9}, y = {~2, 18, 33};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

struct d {
  unsigned int a : 14, b : 14, c : 4;
} __attribute__((aligned));

int d1() {
  static struct d x = {1, 2, ~1}, y = {65, 2, ~2};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int d2() {
  static struct d x = {1, 66, ~1}, y = {1, 2, ~2};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int d3() {
  static struct d x = {9, 66, ~1}, y = {33, 18, ~2};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

struct e {
  int c : 4, b : 14, a : 14;
} __attribute__((aligned));

int e1() {
  static struct e x = {~1, -2, -65}, y = {~2, -2, -1};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int e2() {
  static struct e x = {~1, -2, -1}, y = {~2, -66, -1};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int e3() {
  static struct e x = {~1, -18, -33}, y = {~2, -66, -9};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

int e4() {
  static struct e x = {-1, -1, 0};

  return x.a == 0 && x.b & 0x2000;
}

struct f {
  int a : 14, b : 14, c : 4;
} __attribute__((aligned));

int f1() {
  static struct f x = {-65, -2, ~1}, y = {-1, -2, ~2};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int f2() {
  static struct f x = {-1, -2, ~1}, y = {-1, -66, ~2};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int f3() {
  static struct f x = {-33, -18, ~1}, y = {-9, -66, ~2};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

int f4() {
  static struct f x = {0, -1, -1};

  return x.a == 0 && x.b & 0x2000;
}

struct gx {
  int c : 4, b : 14, a : 14;
} __attribute__((aligned));
struct gy {
  int b : 14, a : 14, c : 4;
} __attribute__((aligned));

int g1() {
  static struct gx x = {~1, -2, -65};
  static struct gy y = {-2, -1, ~2};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int g2() {
  static struct gx x = {~1, -2, -1};
  static struct gy y = {-66, -1, ~2};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int g3() {
  static struct gx x = {~1, -18, -33};
  static struct gy y = {-66, -9, ~2};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

int g4() {
  static struct gx x = {~1, 0x0020, 0x0010};
  static struct gy y = {0x0200, 0x0100, ~2};

  return ((x.a & 0x00f0) == (y.a & 0x0f00) && (x.b & 0x00f0) == (y.b & 0x0f00));
}

int g5() {
  static struct gx x = {~1, 0x0200, 0x0100};
  static struct gy y = {0x0020, 0x0010, ~2};

  return ((x.a & 0x0f00) == (y.a & 0x00f0) && (x.b & 0x0f00) == (y.b & 0x00f0));
}

int g6() {
  static struct gx x = {~1, 0xfe20, 0xfd10};
  static struct gy y = {0xc22f, 0xc11f, ~2};

  return ((x.a & 0x03ff) == (y.a & 0x3ff0) && (x.b & 0x03ff) == (y.b & 0x3ff0));
}

int g7() {
  static struct gx x = {~1, 0xc22f, 0xc11f};
  static struct gy y = {0xfe20, 0xfd10, ~2};

  return ((x.a & 0x3ff0) == (y.a & 0x03ff) && (x.b & 0x3ff0) == (y.b & 0x03ff));
}

struct hx {
  int a : 14, b : 14, c : 4;
} __attribute__((aligned));
struct hy {
  int c : 4, a : 14, b : 14;
} __attribute__((aligned));

int h1() {
  static struct hx x = {-65, -2, ~1};
  static struct hy y = {~2, -1, -2};

  return (x.a == (y.a & ~64) && x.b == y.b);
}

int h2() {
  static struct hx x = {-1, -2, ~1};
  static struct hy y = {~2, -1, -66};

  return (x.a == y.a && (x.b & ~64) == y.b);
}

int h3() {
  static struct hx x = {-33, -18, ~1};
  static struct hy y = {~2, -9, -66};

  return ((x.a & ~8) == (y.a & ~32) && (x.b & ~64) == (y.b & ~16));
}

int h4() {
  static struct hx x = {0x0010, 0x0020, ~1};
  static struct hy y = {~2, 0x0100, 0x0200};

  return ((x.a & 0x00f0) == (y.a & 0x0f00) && (x.b & 0x00f0) == (y.b & 0x0f00));
}

int h5() {
  static struct hx x = {0x0100, 0x0200, ~1};
  static struct hy y = {~2, 0x0010, 0x0020};

  return ((x.a & 0x0f00) == (y.a & 0x00f0) && (x.b & 0x0f00) == (y.b & 0x00f0));
}

int h6() {
  static struct hx x = {0xfd10, 0xfe20, ~1};
  static struct hy y = {~2, 0xc11f, 0xc22f};

  return ((x.a & 0x03ff) == (y.a & 0x3ff0) && (x.b & 0x03ff) == (y.b & 0x3ff0));
}

int h7() {
  static struct hx x = {0xc11f, 0xc22f, ~1};
  static struct hy y = {~2, 0xfd10, 0xfe20};

  return ((x.a & 0x3ff0) == (y.a & 0x03ff) && (x.b & 0x3ff0) == (y.b & 0x03ff));
}

int main() {
  if (!a1())
    abort();
  if (!a2())
    abort();
  if (!a3())
    abort();
  if (!b1())
    abort();
  if (!b2())
    abort();
  if (!b3())
    abort();
  if (!c1())
    abort();
  if (!c2())
    abort();
  if (!c3())
    abort();
  if (!d1())
    abort();
  if (!d2())
    abort();
  if (!d3())
    abort();
  if (!e1())
    abort();
  if (!e2())
    abort();
  if (!e3())
    abort();
  if (!e4())
    abort();
  if (!f1())
    abort();
  if (!f2())
    abort();
  if (!f3())
    abort();
  if (!f4())
    abort();
  if (!g1())
    abort();
  if (!g2())
    abort();
  if (!g3())
    abort();
  if (g4())
    abort();
  if (g5())
    abort();
  if (!g6())
    abort();
  if (!g7())
    abort();
  if (!h1())
    abort();
  if (!h2())
    abort();
  if (!h3())
    abort();
  if (h4())
    abort();
  if (h5())
    abort();
  if (!h6())
    abort();
  if (!h7())
    abort();
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
// DEFAULT-NEXT:     type @type0 a = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 1, 2]];
// DEFAULT-NEXT:     type @type1 b = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 a: i16;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 6]];
// DEFAULT-NEXT:     type @type2 c = struct {
// DEFAULT-NEXT:         field0 c: u32 : 4;
// DEFAULT-NEXT:         field1 b: u32 : 14;
// DEFAULT-NEXT:         field2 a: u32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type3 d = struct {
// DEFAULT-NEXT:         field0 a: u32 : 14;
// DEFAULT-NEXT:         field1 b: u32 : 14;
// DEFAULT-NEXT:         field2 c: u32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type4 e = struct {
// DEFAULT-NEXT:         field0 c: i32 : 4;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 a: i32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type5 f = struct {
// DEFAULT-NEXT:         field0 a: i32 : 14;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 c: i32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type6 gx = struct {
// DEFAULT-NEXT:         field0 c: i32 : 4;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 a: i32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type7 gy = struct {
// DEFAULT-NEXT:         field0 b: i32 : 14;
// DEFAULT-NEXT:         field1 a: i32 : 14;
// DEFAULT-NEXT:         field2 c: i32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type8 hx = struct {
// DEFAULT-NEXT:         field0 a: i32 : 14;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 c: i32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type9 hy = struct {
// DEFAULT-NEXT:         field0 c: i32 : 4;
// DEFAULT-NEXT:         field1 a: i32 : 14;
// DEFAULT-NEXT:         field2 b: i32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %4 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %5 y: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(65)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %7 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %8 y: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %10 x: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(9)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %11 y: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(33)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(18)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %14 x: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %15 y: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %17 x: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %18 y: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %20 x: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(9))) [linkage=internal];
// DEFAULT-NEXT:     global %21 y: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(18)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %24 x: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %25 y: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %27 x: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %28 y: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %30 x: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(9))) [linkage=internal];
// DEFAULT-NEXT:     global %31 y: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(18)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %34 x: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %35 y: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %37 x: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %38 y: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %40 x: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(9)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %41 y: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(33)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(18)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %44 x: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %45 y: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %47 x: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %48 y: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %50 x: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = neg<i32, overflow=ub>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %51 y: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = neg<i32, overflow=ub>(const<i32>(9))) [linkage=internal];
// DEFAULT-NEXT:     global %53 x: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %56 x: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(65)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %57 y: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %59 x: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %60 y: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %62 x: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(33)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %63 y: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(9)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %65 x: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(0), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %69 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %70 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %72 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %73 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(66)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %75 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = neg<i32, overflow=ub>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %76 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(66)), field1 = neg<i32, overflow=ub>(const<i32>(9)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %78 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(32), field2 = const<i32>(16)) [linkage=internal];
// DEFAULT-NEXT:     global %79 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = const<i32>(512), field1 = const<i32>(256), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %81 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(512), field2 = const<i32>(256)) [linkage=internal];
// DEFAULT-NEXT:     global %82 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = const<i32>(32), field1 = const<i32>(16), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %84 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(65056), field2 = const<i32>(64784)) [linkage=internal];
// DEFAULT-NEXT:     global %85 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = const<i32>(49711), field1 = const<i32>(49439), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %87 x: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(49711), field2 = const<i32>(49439)) [linkage=internal];
// DEFAULT-NEXT:     global %88 y: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = const<i32>(65056), field1 = const<i32>(64784), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %92 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(65)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %93 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = neg<i32, overflow=ub>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %95 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %96 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = neg<i32, overflow=ub>(const<i32>(66))) [linkage=internal];
// DEFAULT-NEXT:     global %98 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(33)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %99 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(9)), field2 = neg<i32, overflow=ub>(const<i32>(66))) [linkage=internal];
// DEFAULT-NEXT:     global %101 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = const<i32>(16), field1 = const<i32>(32), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %102 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(256), field2 = const<i32>(512)) [linkage=internal];
// DEFAULT-NEXT:     global %104 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = const<i32>(256), field1 = const<i32>(512), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %105 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(16), field2 = const<i32>(32)) [linkage=internal];
// DEFAULT-NEXT:     global %107 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = const<i32>(64784), field1 = const<i32>(65056), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %108 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(49439), field2 = const<i32>(49711)) [linkage=internal];
// DEFAULT-NEXT:     global %110 x: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = const<i32>(49439), field1 = const<i32>(49711), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %111 y: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(64784), field2 = const<i32>(65056)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%113 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @a1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(field0(%4))), and<i32>(widen<i32, reason=promotion>(read<i8>(field0(%5))), not<i32>(const<i32>(64)))), eq<i32>(widen<i32, reason=promotion>(read<i8>(field1(%4))), widen<i32, reason=promotion>(read<i8>(field1(%5))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @a2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(field0(%7))), widen<i32, reason=promotion>(read<i8>(field0(%8)))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(field1(%7))), not<i32>(const<i32>(64))), widen<i32, reason=promotion>(read<i8>(field1(%8))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @a3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(field0(%10))), not<i32>(const<i32>(8))), and<i32>(widen<i32, reason=promotion>(read<i8>(field0(%11))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(field1(%10))), not<i32>(const<i32>(64))), and<i32>(widen<i32, reason=promotion>(read<i8>(field1(%11))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @b1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(field2(%14))), and<i32>(widen<i32, reason=promotion>(read<i16>(field2(%15))), not<i32>(const<i32>(64)))), eq<i32>(widen<i32, reason=promotion>(read<i16>(field1(%14))), widen<i32, reason=promotion>(read<i16>(field1(%15))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @b2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(field2(%17))), widen<i32, reason=promotion>(read<i16>(field2(%18)))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(field1(%17))), not<i32>(const<i32>(64))), widen<i32, reason=promotion>(read<i16>(field1(%18))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @b3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(field2(%20))), not<i32>(const<i32>(8))), and<i32>(widen<i32, reason=promotion>(read<i16>(field2(%21))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(field1(%20))), not<i32>(const<i32>(64))), and<i32>(widen<i32, reason=promotion>(read<i16>(field1(%21))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @c1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%24))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%25))), not<i32>(const<i32>(64)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%24))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%25))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @c2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%27))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%28)))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%27))), not<i32>(const<i32>(64))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%28))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @c3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%30))), not<i32>(const<i32>(8))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%31))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%30))), not<i32>(const<i32>(64))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%31))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @d1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%34))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%35))), not<i32>(const<i32>(64)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%34))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%35))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @d2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%37))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%38)))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%37))), not<i32>(const<i32>(64))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%38))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @d3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%40))), not<i32>(const<i32>(8))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%41))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%40))), not<i32>(const<i32>(64))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%41))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @e1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%44)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%45)), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%44)), read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%45)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @e2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%47)), read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%48))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%47)), not<i32>(const<i32>(64))), read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%48)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @e3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%50)), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%51)), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%50)), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%51)), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @e4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%53)), const<i32>(0)), ne<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%53)), const<i32>(8192)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @f1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%56)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%57)), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%56)), read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%57)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @f2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%59)), read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%60))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%59)), not<i32>(const<i32>(64))), read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%60)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @f3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%62)), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%63)), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%62)), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%63)), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @f4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%65)), const<i32>(0)), ne<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%65)), const<i32>(8192)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @g1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%69)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%70)), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%69)), read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%70)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @g2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%72)), read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%73))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%72)), not<i32>(const<i32>(64))), read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%73)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @g3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%75)), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%76)), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%75)), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%76)), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @g4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%78)), const<i32>(240)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%79)), const<i32>(3840))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%78)), const<i32>(240)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%79)), const<i32>(3840)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @g5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%81)), const<i32>(3840)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%82)), const<i32>(240))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%81)), const<i32>(3840)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%82)), const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @g6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%84)), const<i32>(1023)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%85)), const<i32>(16368))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%84)), const<i32>(1023)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%85)), const<i32>(16368)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @g7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%87)), const<i32>(16368)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%88)), const<i32>(1023))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%87)), const<i32>(16368)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%88)), const<i32>(1023)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @h1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%92)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%93)), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%92)), read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%93)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @h2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%95)), read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%96))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%95)), not<i32>(const<i32>(64))), read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%96)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %97 @h3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%98)), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%99)), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%98)), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%99)), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @h4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%101)), const<i32>(240)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%102)), const<i32>(3840))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%101)), const<i32>(240)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%102)), const<i32>(3840)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %103 @h5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%104)), const<i32>(3840)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%105)), const<i32>(240))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%104)), const<i32>(3840)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%105)), const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @h6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%107)), const<i32>(1023)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%108)), const<i32>(16368))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%107)), const<i32>(1023)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%108)), const<i32>(16368)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %109 @h7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%110)), const<i32>(16368)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%111)), const<i32>(1023))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%110)), const<i32>(16368)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%111)), const<i32>(1023)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%3), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%6), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%9), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%13), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%16), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%19), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%23), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%29), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%33), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%36), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%39), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%43), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%46), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%49), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%52), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%55), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%58), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%61), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%64), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%68), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%71), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%74), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%77), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%80), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%83), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%86), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%91), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%94), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%97), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%100), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%103), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%106), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%109), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
