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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i8;
// DEFAULT-NEXT:         field2 c: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 1, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_b:[0-9]+]] b = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 a: i16;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 6]];
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = struct {
// DEFAULT-NEXT:         field0 c: u32 : 4;
// DEFAULT-NEXT:         field1 b: u32 : 14;
// DEFAULT-NEXT:         field2 a: u32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_d:[0-9]+]] d = struct {
// DEFAULT-NEXT:         field0 a: u32 : 14;
// DEFAULT-NEXT:         field1 b: u32 : 14;
// DEFAULT-NEXT:         field2 c: u32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = struct {
// DEFAULT-NEXT:         field0 c: i32 : 4;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 a: i32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_f:[0-9]+]] f = struct {
// DEFAULT-NEXT:         field0 a: i32 : 14;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 c: i32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_gx:[0-9]+]] gx = struct {
// DEFAULT-NEXT:         field0 c: i32 : 4;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 a: i32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_gy:[0-9]+]] gy = struct {
// DEFAULT-NEXT:         field0 b: i32 : 14;
// DEFAULT-NEXT:         field1 a: i32 : 14;
// DEFAULT-NEXT:         field2 c: i32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_hx:[0-9]+]] hx = struct {
// DEFAULT-NEXT:         field0 a: i32 : 14;
// DEFAULT-NEXT:         field1 b: i32 : 14;
// DEFAULT-NEXT:         field2 c: i32 : 4;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 1, 3], bit_offsets=[Some(0), Some(14), Some(28)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_hy:[0-9]+]] hy = struct {
// DEFAULT-NEXT:         field0 c: i32 : 4;
// DEFAULT-NEXT:         field1 a: i32 : 14;
// DEFAULT-NEXT:         field2 b: i32 : 14;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(4), Some(18)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(65)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_3:[0-9]+]] x: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(9)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_3:[0-9]+]] y: @type[[TYPE_a]] [storage=static] = aggregate<@type[[TYPE_a]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(33)), field1 = truncate<i8, reason=assign, fits=always>(const<i32>(18)), field2 = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_4:[0-9]+]] x: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_4:[0-9]+]] y: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_5:[0-9]+]] x: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_5:[0-9]+]] y: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(2)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_6:[0-9]+]] x: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(66)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(9))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_6:[0-9]+]] y: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(18)), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_7:[0-9]+]] x: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_7:[0-9]+]] y: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_8:[0-9]+]] x: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_8:[0-9]+]] y: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_9:[0-9]+]] x: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(9))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_9:[0-9]+]] y: @type[[TYPE_c]] [storage=static] = aggregate<@type[[TYPE_c]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2))), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(18)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_10:[0-9]+]] x: @type[[TYPE_d]] [storage=static] = aggregate<@type[[TYPE_d]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_10:[0-9]+]] y: @type[[TYPE_d]] [storage=static] = aggregate<@type[[TYPE_d]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(65)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_11:[0-9]+]] x: @type[[TYPE_d]] [storage=static] = aggregate<@type[[TYPE_d]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_11:[0-9]+]] y: @type[[TYPE_d]] [storage=static] = aggregate<@type[[TYPE_d]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_12:[0-9]+]] x: @type[[TYPE_d]] [storage=static] = aggregate<@type[[TYPE_d]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(9)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(66)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_12:[0-9]+]] y: @type[[TYPE_d]] [storage=static] = aggregate<@type[[TYPE_d]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(33)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(18)), field2 = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(const<i32>(2)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_13:[0-9]+]] x: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_13:[0-9]+]] y: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_14:[0-9]+]] x: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_14:[0-9]+]] y: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_15:[0-9]+]] x: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = neg<i32, overflow=ub>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_15:[0-9]+]] y: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = neg<i32, overflow=ub>(const<i32>(9))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_16:[0-9]+]] x: @type[[TYPE_e]] [storage=static] = aggregate<@type[[TYPE_e]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_17:[0-9]+]] x: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(65)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_16:[0-9]+]] y: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_18:[0-9]+]] x: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_17:[0-9]+]] y: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_19:[0-9]+]] x: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(33)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_18:[0-9]+]] y: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(9)), field1 = neg<i32, overflow=ub>(const<i32>(66)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_20:[0-9]+]] x: @type[[TYPE_f]] [storage=static] = aggregate<@type[[TYPE_f]], zero_fill=false>(field0 = const<i32>(0), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_21:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(65))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_19:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_22:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = neg<i32, overflow=ub>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_20:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(66)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_23:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = neg<i32, overflow=ub>(const<i32>(33))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_21:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(66)), field1 = neg<i32, overflow=ub>(const<i32>(9)), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_24:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(32), field2 = const<i32>(16)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_22:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = const<i32>(512), field1 = const<i32>(256), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_25:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(512), field2 = const<i32>(256)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_23:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = const<i32>(32), field1 = const<i32>(16), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_26:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(65056), field2 = const<i32>(64784)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_24:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = const<i32>(49711), field1 = const<i32>(49439), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_27:[0-9]+]] x: @type[[TYPE_gx]] [storage=static] = aggregate<@type[[TYPE_gx]], zero_fill=false>(field0 = not<i32>(const<i32>(1)), field1 = const<i32>(49711), field2 = const<i32>(49439)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_25:[0-9]+]] y: @type[[TYPE_gy]] [storage=static] = aggregate<@type[[TYPE_gy]], zero_fill=false>(field0 = const<i32>(65056), field1 = const<i32>(64784), field2 = not<i32>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_28:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(65)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_26:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = neg<i32, overflow=ub>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_29:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = neg<i32, overflow=ub>(const<i32>(2)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_27:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(1)), field2 = neg<i32, overflow=ub>(const<i32>(66))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_30:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(33)), field1 = neg<i32, overflow=ub>(const<i32>(18)), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_28:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = neg<i32, overflow=ub>(const<i32>(9)), field2 = neg<i32, overflow=ub>(const<i32>(66))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_31:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = const<i32>(16), field1 = const<i32>(32), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_29:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(256), field2 = const<i32>(512)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_32:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = const<i32>(256), field1 = const<i32>(512), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_30:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(16), field2 = const<i32>(32)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_33:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = const<i32>(64784), field1 = const<i32>(65056), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_31:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(49439), field2 = const<i32>(49711)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_34:[0-9]+]] x: @type[[TYPE_hx]] [storage=static] = aggregate<@type[[TYPE_hx]], zero_fill=false>(field0 = const<i32>(49439), field1 = const<i32>(49711), field2 = not<i32>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y_32:[0-9]+]] y: @type[[TYPE_hy]] [storage=static] = aggregate<@type[[TYPE_hy]], zero_fill=false>(field0 = not<i32>(const<i32>(2)), field1 = const<i32>(64784), field2 = const<i32>(65056)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_a1:[0-9]+]] @a1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_x]]))), and<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_y]]))), not<i32>(const<i32>(64)))), eq<i32>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_x]]))), widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_y]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_a2:[0-9]+]] @a2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_x_2]]))), widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_y_2]])))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_x_2]]))), not<i32>(const<i32>(64))), widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_y_2]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_a3:[0-9]+]] @a3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_x_3]]))), not<i32>(const<i32>(8))), and<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_y_3]]))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_x_3]]))), not<i32>(const<i32>(64))), and<i32>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_y_3]]))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b1:[0-9]+]] @b1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_x_4]]))), and<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_y_4]]))), not<i32>(const<i32>(64)))), eq<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_x_4]]))), widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_y_4]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b2:[0-9]+]] @b2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_x_5]]))), widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_y_5]])))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_x_5]]))), not<i32>(const<i32>(64))), widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_y_5]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b3:[0-9]+]] @b3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_x_6]]))), not<i32>(const<i32>(8))), and<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_y_6]]))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_x_6]]))), not<i32>(const<i32>(64))), and<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_y_6]]))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c1:[0-9]+]] @c1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_7]]))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_7]]))), not<i32>(const<i32>(64)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_7]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_7]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c2:[0-9]+]] @c2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_8]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_8]])))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_8]]))), not<i32>(const<i32>(64))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_8]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c3:[0-9]+]] @c3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_9]]))), not<i32>(const<i32>(8))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_9]]))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_9]]))), not<i32>(const<i32>(64))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_9]]))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_d1:[0-9]+]] @d1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_10]]))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_10]]))), not<i32>(const<i32>(64)))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_10]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_d2:[0-9]+]] @d2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_11]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_11]])))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_11]]))), not<i32>(const<i32>(64))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_11]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_d3:[0-9]+]] @d3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_12]]))), not<i32>(const<i32>(8))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_12]]))), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_12]]))), not<i32>(const<i32>(64))), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_12]]))), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_e1:[0-9]+]] @e1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_13]])), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_13]])), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_13]])), read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_13]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_e2:[0-9]+]] @e2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_14]])), read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_14]]))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_14]])), not<i32>(const<i32>(64))), read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_14]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_e3:[0-9]+]] @e3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_15]])), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_15]])), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_15]])), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_15]])), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_e4:[0-9]+]] @e4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_16]])), const<i32>(0)), ne<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_16]])), const<i32>(8192)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_17]])), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_16]])), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_17]])), read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_16]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_18]])), read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_17]]))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_18]])), not<i32>(const<i32>(64))), read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_17]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_19]])), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_18]])), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_19]])), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_18]])), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_20]])), const<i32>(0)), ne<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_20]])), const<i32>(8192)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g1:[0-9]+]] @g1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_21]])), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_19]])), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_21]])), read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_19]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_22]])), read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_20]]))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_22]])), not<i32>(const<i32>(64))), read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_20]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g3:[0-9]+]] @g3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_23]])), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_21]])), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_23]])), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_21]])), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g4:[0-9]+]] @g4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_24]])), const<i32>(240)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_22]])), const<i32>(3840))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_24]])), const<i32>(240)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_22]])), const<i32>(3840)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g5:[0-9]+]] @g5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_25]])), const<i32>(3840)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_23]])), const<i32>(240))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_25]])), const<i32>(3840)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_23]])), const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g6:[0-9]+]] @g6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_26]])), const<i32>(1023)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_24]])), const<i32>(16368))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_26]])), const<i32>(1023)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_24]])), const<i32>(16368)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g7:[0-9]+]] @g7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_x_27]])), const<i32>(16368)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_y_25]])), const<i32>(1023))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_x_27]])), const<i32>(16368)), and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_y_25]])), const<i32>(1023)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h1:[0-9]+]] @h1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_28]])), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_26]])), not<i32>(const<i32>(64)))), eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_28]])), read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_26]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h2:[0-9]+]] @h2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_29]])), read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_27]]))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_29]])), not<i32>(const<i32>(64))), read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_27]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h3:[0-9]+]] @h3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_30]])), not<i32>(const<i32>(8))), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_28]])), not<i32>(const<i32>(32)))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_30]])), not<i32>(const<i32>(64))), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_28]])), not<i32>(const<i32>(16))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h4:[0-9]+]] @h4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_31]])), const<i32>(240)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_29]])), const<i32>(3840))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_31]])), const<i32>(240)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_29]])), const<i32>(3840)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h5:[0-9]+]] @h5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_32]])), const<i32>(3840)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_30]])), const<i32>(240))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_32]])), const<i32>(3840)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_30]])), const<i32>(240)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h6:[0-9]+]] @h6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_33]])), const<i32>(1023)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_31]])), const<i32>(16368))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_33]])), const<i32>(1023)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_31]])), const<i32>(16368)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h7:[0-9]+]] @h7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<i32>(and<i32>(read<i32>(bitfield0<unit=0, bytes=0..4, bits=0..14>(%[[VALUE_x_34]])), const<i32>(16368)), and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=4..18>(%[[VALUE_y_32]])), const<i32>(1023))), eq<i32>(and<i32>(read<i32>(bitfield1<unit=0, bytes=0..4, bits=14..28>(%[[VALUE_x_34]])), const<i32>(16368)), and<i32>(read<i32>(bitfield2<unit=0, bytes=0..4, bits=18..32>(%[[VALUE_y_32]])), const<i32>(1023)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_a1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_a2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_a3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_b1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_b2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_b3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_c1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_c2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_c3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_d1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_d2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_d3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_e1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_e2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_e3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_e4]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_f1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_f2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_f3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_f4]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g4]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g5]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g6]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_g7]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h1]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h2]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h3]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h4]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h5]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h6]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_h7]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
