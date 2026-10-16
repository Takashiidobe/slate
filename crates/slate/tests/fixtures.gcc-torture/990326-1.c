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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct a {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i8,
// CHECK-NEXT:     c: i16,
// CHECK-NEXT: }
// CHECK-NEXT: fn a1() -> i32 {
// CHECK-NEXT:     (((unsafe { [[X_0:x_[0-9]+]].a }) as i32) == ((unsafe { [[Y_0:y_[0-9]+]].a }) as i32) & !(64 as i32)
// CHECK-NEXT:         && ((unsafe { [[X_0]].b }) as i32) == ((unsafe { [[Y_0]].b }) as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_0]]: a = a {
// CHECK-NEXT:     a: (1 as i32) as i8,
// CHECK-NEXT:     b: (2 as i32) as i8,
// CHECK-NEXT:     c: !(1 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_0]]: a = a {
// CHECK-NEXT:     a: (65 as i32) as i8,
// CHECK-NEXT:     b: (2 as i32) as i8,
// CHECK-NEXT:     c: !(2 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: fn a2() -> i32 {
// CHECK-NEXT:     (((unsafe { [[X_1:x_[0-9]+]].a }) as i32) == ((unsafe { [[Y_1:y_[0-9]+]].a }) as i32)
// CHECK-NEXT:         && ((unsafe { [[X_1]].b }) as i32) & !(64 as i32) == ((unsafe { [[Y_1]].b }) as i32))
// CHECK-NEXT:         as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_1]]: a = a {
// CHECK-NEXT:     a: (1 as i32) as i8,
// CHECK-NEXT:     b: (66 as i32) as i8,
// CHECK-NEXT:     c: !(1 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_1]]: a = a {
// CHECK-NEXT:     a: (1 as i32) as i8,
// CHECK-NEXT:     b: (2 as i32) as i8,
// CHECK-NEXT:     c: !(2 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: fn a3() -> i32 {
// CHECK-NEXT:     (((unsafe { [[X_2:x_[0-9]+]].a }) as i32) & !(8 as i32)
// CHECK-NEXT:         == ((unsafe { [[Y_2:y_[0-9]+]].a }) as i32) & !(32 as i32)
// CHECK-NEXT:         && ((unsafe { [[X_2]].b }) as i32) & !(64 as i32)
// CHECK-NEXT:             == ((unsafe { [[Y_2]].b }) as i32) & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_2]]: a = a {
// CHECK-NEXT:     a: (9 as i32) as i8,
// CHECK-NEXT:     b: (66 as i32) as i8,
// CHECK-NEXT:     c: !(1 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_2]]: a = a {
// CHECK-NEXT:     a: (33 as i32) as i8,
// CHECK-NEXT:     b: (18 as i32) as i8,
// CHECK-NEXT:     c: !(2 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits6U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits7U0 {
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits9U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(14)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct b {
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     b: i16,
// CHECK-NEXT:     a: i16,
// CHECK-NEXT: }
// CHECK-NEXT: fn b1() -> i32 {
// CHECK-NEXT:     (((unsafe { [[X_3:x_[0-9]+]].a }) as i32) == ((unsafe { [[Y_3:y_[0-9]+]].a }) as i32) & !(64 as i32)
// CHECK-NEXT:         && ((unsafe { [[X_3]].b }) as i32) == ((unsafe { [[Y_3]].b }) as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_3]]: b = b {
// CHECK-NEXT:     c: !(1 as i32),
// CHECK-NEXT:     b: (2 as i32) as i16,
// CHECK-NEXT:     a: (1 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_3]]: b = b {
// CHECK-NEXT:     c: !(2 as i32),
// CHECK-NEXT:     b: (2 as i32) as i16,
// CHECK-NEXT:     a: (65 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: fn b2() -> i32 {
// CHECK-NEXT:     (((unsafe { [[X_4:x_[0-9]+]].a }) as i32) == ((unsafe { [[Y_4:y_[0-9]+]].a }) as i32)
// CHECK-NEXT:         && ((unsafe { [[X_4]].b }) as i32) & !(64 as i32) == ((unsafe { [[Y_4]].b }) as i32))
// CHECK-NEXT:         as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_4]]: b = b {
// CHECK-NEXT:     c: !(1 as i32),
// CHECK-NEXT:     b: (66 as i32) as i16,
// CHECK-NEXT:     a: (1 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_4]]: b = b {
// CHECK-NEXT:     c: !(2 as i32),
// CHECK-NEXT:     b: (2 as i32) as i16,
// CHECK-NEXT:     a: (1 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: fn b3() -> i32 {
// CHECK-NEXT:     (((unsafe { [[X_5:x_[0-9]+]].a }) as i32) & !(8 as i32)
// CHECK-NEXT:         == ((unsafe { [[Y_5:y_[0-9]+]].a }) as i32) & !(32 as i32)
// CHECK-NEXT:         && ((unsafe { [[X_5]].b }) as i32) & !(64 as i32)
// CHECK-NEXT:             == ((unsafe { [[Y_5]].b }) as i32) & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_5]]: b = b {
// CHECK-NEXT:     c: !(1 as i32),
// CHECK-NEXT:     b: (66 as i32) as i16,
// CHECK-NEXT:     a: (9 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_5]]: b = b {
// CHECK-NEXT:     c: !(2 as i32),
// CHECK-NEXT:     b: (18 as i32) as i16,
// CHECK-NEXT:     a: (33 as i32) as i16,
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct c {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn c1() -> i32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!([[X_6:x_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         == ((unsafe { (*std::ptr::addr_of!([[Y_6:y_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:         && ((unsafe { (*std::ptr::addr_of!([[X_6]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:             == ((unsafe { (*std::ptr::addr_of!([[Y_6]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_6]]: c = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: c = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_6]]: c = {
// CHECK-NEXT:     let mut [[T_0]]: c = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((65 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn c2() -> i32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!([[X_7:x_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         == ((unsafe { (*std::ptr::addr_of!([[Y_7:y_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         && ((unsafe { (*std::ptr::addr_of!([[X_7]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == ((unsafe { (*std::ptr::addr_of!([[Y_7]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_7]]: c = {
// CHECK-NEXT:     let mut [[T_0]]: c = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((66 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_7]]: c = {
// CHECK-NEXT:     let mut [[T_0]]: c = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn c3() -> i32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!([[X_8:x_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         & !(8 as i32)
// CHECK-NEXT:         == ((unsafe { (*std::ptr::addr_of!([[Y_8:y_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:             & !(32 as i32)
// CHECK-NEXT:         && ((unsafe { (*std::ptr::addr_of!([[X_8]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == ((unsafe { (*std::ptr::addr_of!([[Y_8]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:                 & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_8]]: c = {
// CHECK-NEXT:     let mut [[T_0]]: c = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((66 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((9 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_8]]: c = {
// CHECK-NEXT:     let mut [[T_0]]: c = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((18 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((33 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct d {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn d1() -> i32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!([[X_9:x_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         == ((unsafe { (*std::ptr::addr_of!([[Y_9:y_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:         && ((unsafe { (*std::ptr::addr_of!([[X_9]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:             == ((unsafe { (*std::ptr::addr_of!([[Y_9]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_9]]: d = {
// CHECK-NEXT:     let mut [[T_0]]: d = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_9]]: d = {
// CHECK-NEXT:     let mut [[T_0]]: d = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((65 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn d2() -> i32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!([[X_10:x_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         == ((unsafe { (*std::ptr::addr_of!([[Y_10:y_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         && ((unsafe { (*std::ptr::addr_of!([[X_10]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == ((unsafe { (*std::ptr::addr_of!([[Y_10]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_10]]: d = {
// CHECK-NEXT:     let mut [[T_0]]: d = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((66 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_10]]: d = {
// CHECK-NEXT:     let mut [[T_0]]: d = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn d3() -> i32 {
// CHECK-NEXT:     (((unsafe { (*std::ptr::addr_of!([[X_11:x_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:         & !(8 as i32)
// CHECK-NEXT:         == ((unsafe { (*std::ptr::addr_of!([[Y_11:y_[0-9]+]].__slate_bits_0)).__get_a() }) as i32)
// CHECK-NEXT:             & !(32 as i32)
// CHECK-NEXT:         && ((unsafe { (*std::ptr::addr_of!([[X_11]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == ((unsafe { (*std::ptr::addr_of!([[Y_11]].__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:                 & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_11]]: d = {
// CHECK-NEXT:     let mut [[T_0]]: d = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((9 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((66 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_11]]: d = {
// CHECK-NEXT:     let mut [[T_0]]: d = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((33 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((18 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct e {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn e1() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_12:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_12:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_12]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_12]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_12]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(65 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_12]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn e2() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_13:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == unsafe { (*std::ptr::addr_of!([[Y_13:y_[0-9]+]].__slate_bits_0)).__get_a() }
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_13]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_13]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_13]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_13]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn e3() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_14:x_[0-9]+]].__slate_bits_0)).__get_a() }) & !(8 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_14:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(32 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_14]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_14]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_14]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(18 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(33 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_14]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(9 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn e4() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_15:x_[0-9]+]].__slate_bits_0)).__get_a() }) == (0 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_15]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (8192 as i32) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_15]]: e = {
// CHECK-NEXT:     let mut [[T_0]]: e = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(0 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct f {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn f1() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_16:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_15:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_16]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_15]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_16]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(65 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_15]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn f2() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_17:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == unsafe { (*std::ptr::addr_of!([[Y_16:y_[0-9]+]].__slate_bits_0)).__get_a() }
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_17]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_16]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_17]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_16]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn f3() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_18:x_[0-9]+]].__slate_bits_0)).__get_a() }) & !(8 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_17:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(32 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_18]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_17]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_18]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(33 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(18 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_17]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(9 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn f4() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_19:x_[0-9]+]].__slate_bits_0)).__get_a() }) == (0 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_19]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (8192 as i32) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_19]]: f = {
// CHECK-NEXT:     let mut [[T_0]]: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(0 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(-(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct gx {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits6U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct gy {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits7U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn g1() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_20:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_18:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_20]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_18]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_20]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(65 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_18]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g2() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_21:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == unsafe { (*std::ptr::addr_of!([[Y_19:y_[0-9]+]].__slate_bits_0)).__get_a() }
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_21]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_19]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_21]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_19]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g3() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_22:x_[0-9]+]].__slate_bits_0)).__get_a() }) & !(8 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_20:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(32 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_22]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_20]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_22]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(18 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(33 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_20]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(9 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g4() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_23:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (240 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_21:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (3840 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_23]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (240 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_21]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (3840 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_23]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(32 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(16 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_21]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(512 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(256 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g5() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_24:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (3840 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_22:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (240 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_24]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (3840 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_22]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (240 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_24]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(512 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(256 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_22]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(32 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(16 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g6() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_25:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (1023 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_23:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (16368 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_25]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (1023 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_23]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (16368 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_25]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(65056 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(64784 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_23]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(49711 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(49439 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g7() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_26:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (16368 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_24:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (1023 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_26]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (16368 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_24]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (1023 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_26]]: gx = {
// CHECK-NEXT:     let mut [[T_0]]: gx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(49711 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(49439 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_24]]: gy = {
// CHECK-NEXT:     let mut [[T_0]]: gy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(65056 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(64784 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hx {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hy {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits9U0,
// CHECK-NEXT: }
// CHECK-NEXT: fn h1() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_27:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_25:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_27]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_25]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_27]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(65 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_25]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn h2() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_28:x_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:         == unsafe { (*std::ptr::addr_of!([[Y_26:y_[0-9]+]].__slate_bits_0)).__get_a() }
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_28]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == unsafe { (*std::ptr::addr_of!([[Y_26]].__slate_bits_0)).__get_b() }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_28]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_26]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(1 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn h3() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_29:x_[0-9]+]].__slate_bits_0)).__get_a() }) & !(8 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_27:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & !(32 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_29]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & !(64 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_27]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & !(16 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_29]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(33 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(18 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_27]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(9 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(66 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn h4() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_30:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (240 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_28:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (3840 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_30]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (240 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_28]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (3840 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_30]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(16 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(32 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_28]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(256 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(512 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn h5() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_31:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (3840 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_29:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (240 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_31]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (3840 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_29]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (240 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_31]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(256 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(512 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_29]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(16 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(32 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn h6() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_32:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (1023 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_30:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (16368 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_32]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (1023 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_30]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (16368 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_32]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(64784 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(65056 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_30]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(49439 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(49711 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn h7() -> i32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!([[X_33:x_[0-9]+]].__slate_bits_0)).__get_a() }) & (16368 as i32)
// CHECK-NEXT:         == (unsafe { (*std::ptr::addr_of!([[Y_31:y_[0-9]+]].__slate_bits_0)).__get_a() })
// CHECK-NEXT:             & (1023 as i32)
// CHECK-NEXT:         && (unsafe { (*std::ptr::addr_of!([[X_33]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:             & (16368 as i32)
// CHECK-NEXT:             == (unsafe { (*std::ptr::addr_of!([[Y_31]].__slate_bits_0)).__get_b() })
// CHECK-NEXT:                 & (1023 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[X_33]]: hx = {
// CHECK-NEXT:     let mut [[T_0]]: hx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(49439 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(49711 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(1 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut [[Y_31]]: hy = {
// CHECK-NEXT:     let mut [[T_0]]: hy = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(!(2 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(64784 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(65056 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !(a1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(a2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(a3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(b1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(b2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(b3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(c1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(c2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(c3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(d1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(d2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(d3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(e1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(e2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(e3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(e4() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(f1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(f2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(f3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(f4() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(g1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(g2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(g3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if g4() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if g5() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(g6() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(g7() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(h1() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(h2() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(h3() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if h4() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if h5() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(h6() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(h7() != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
