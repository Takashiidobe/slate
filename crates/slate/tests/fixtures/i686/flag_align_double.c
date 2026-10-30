/* { dg-options "-malign-double" } */
int printf(const char *, ...);

struct wide {
  char c;
  double d;
  int i;
  long long l;
};

struct nested {
  char tag;
  struct wide inner;
  unsigned long long u;
};

struct arrays {
  short s;
  double d[2];
  char tail;
};

union either {
  char c;
  double d;
};

struct holds_union {
  char c;
  union either u;
};

struct bits {
  char c;
  long long b : 5;
  long long rest : 40;
};

struct complex_member {
  char c;
  _Complex double z;
};

static unsigned offset(const void *base, const void *field) {
  return (unsigned)((const char *)field - (const char *)base);
}

static void dump(const char *name, const void *object, unsigned size) {
  const unsigned char *bytes = object;
  printf("%s:", name);
  for (unsigned i = 0; i < size; i++)
    printf(" %02x", bytes[i]);
  printf("\n");
}

static double sum_wide(struct wide w) { return w.c + w.d + w.i + (double)w.l; }

static struct wide make_wide(int seed) {
  struct wide w = {(char)seed, seed * 1.5, seed * 3, seed * 7LL};
  return w;
}

int main(void) {
  struct wide w = {1, 2.5, 3, 4};
  printf("%zu %zu %zu %zu\n", sizeof(struct wide), __builtin_offsetof(struct wide, d),
         __builtin_offsetof(struct wide, l), _Alignof(struct wide));
  printf("%d %g %d %lld\n", w.c, w.d, w.i, w.l);
  printf("%u %u %u\n", offset(&w, &w.d), offset(&w, &w.i), offset(&w, &w.l));
  printf("%zu %zu %zu %zu\n", _Alignof(double), _Alignof(long long),
         _Alignof(unsigned long long), _Alignof(_Complex double));

  struct wide image;
  __builtin_memset(&image, 0xaa, sizeof image);
  image.c = 1;
  image.d = 2.5;
  image.i = 3;
  image.l = 4;
  dump("wide", &image, sizeof image);

  struct nested n;
  __builtin_memset(&n, 0, sizeof n);
  n.tag = 9;
  n.inner.c = w.c;
  n.inner.d = w.d;
  n.inner.i = w.i;
  n.inner.l = w.l;
  n.u = 0x0102030405060708ULL;
  printf("nested %zu %zu %u %u %u\n", sizeof n, _Alignof(struct nested), offset(&n, &n.inner),
         offset(&n, &n.inner.l), offset(&n, &n.u));
  dump("nested", &n, sizeof n);

  struct arrays a = {7, {1.0, -2.0}, 'z'};
  printf("arrays %zu %u %u %u\n", sizeof a, offset(&a, &a.d[0]), offset(&a, &a.d[1]),
         offset(&a, &a.tail));

  struct holds_union h = {3, {0}};
  h.u.d = 6.25;
  printf("union %zu %zu %zu %u %g\n", sizeof(union either), sizeof h, _Alignof(struct holds_union),
         offset(&h, &h.u), h.u.d);

  struct bits b;
  __builtin_memset(&b, 0, sizeof b);
  b.c = 1;
  b.b = -3;
  b.rest = 123456789LL;
  printf("bits %zu %zu %d %lld\n", sizeof b, _Alignof(struct bits), (int)b.b, (long long)b.rest);
  dump("bits", &b, sizeof b);

  struct complex_member cm = {2, 0};
  cm.z = 1.5 + 2.5i;
  printf("complex %zu %u %g %g\n", sizeof cm, offset(&cm, &cm.z), __real__ cm.z, __imag__ cm.z);

  struct wide list[3];
  for (int i = 0; i < 3; i++)
    list[i] = make_wide(i + 1);
  printf("list %zu %u %u\n", sizeof list, offset(&list[0], &list[1]), offset(&list[0], &list[2].l));
  for (int i = 0; i < 3; i++)
    printf("%g\n", sum_wide(list[i]));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[ARG_0]]: *mut (), [[ARG_1:arg[0-9]+]]: i32, [[ARG_2:arg[0-9]+]]: u32) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-malign-double" }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 7],
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     __slate_pad_4: [u8; 4],
// CHECK-NEXT:     l: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct nested {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     inner: wide,
// CHECK-NEXT:     u: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct arrays {
// CHECK-NEXT:     s: i16,
// CHECK-NEXT:     __slate_pad_1: [u8; 6],
// CHECK-NEXT:     d: [f64; 2],
// CHECK-NEXT:     tail: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union either {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holds_union {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     u: either,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct complex_member {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 7],
// CHECK-NEXT:     z: num_complex::Complex<f64>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 6], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub b: i64,
// CHECK-NEXT:         #[bits(40)]
// CHECK-NEXT:         pub rest: i64,
// CHECK-NEXT:         #[bits(3, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn offset(base: *const (), field: *const ()) -> u32 {
// CHECK-NEXT:     ((unsafe { (field as *const i8).offset_from(base as *const i8) }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dump(name: *const i8, object: *const (), size: u32) {
// CHECK-NEXT:     let bytes: *const u8 = object as *const u8;
// CHECK-NEXT:     unsafe { printf(c"%s:".as_ptr(), name) };
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         unsafe { printf(c" %02x".as_ptr(), (*bytes.add(i as usize) as u32) as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { printf(c"\n".as_ptr()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum_wide(w: wide) -> f64 {
// CHECK-NEXT:     ((w.c as i32) as f64) + w.d + (w.i as f64) + (w.l as f64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn make_wide(seed: i32) -> wide {
// CHECK-NEXT:     let w: wide = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = seed as i8;
// CHECK-NEXT:         [[T_0]].d = (seed as f64) * 1.5f64;
// CHECK-NEXT:         [[T_0]].i = seed * 3;
// CHECK-NEXT:         [[T_0]].l = (seed as i64) * 7;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     return w;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut w: wide = {
// CHECK-NEXT:         let mut [[T_0]]: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         [[T_0]].d = 2.5f64;
// CHECK-NEXT:         [[T_0]].i = 3i32;
// CHECK-NEXT:         [[T_0]].l = 4i64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 32u32, 8u32, 24u32, 8u32) };
// CHECK-NEXT:     unsafe { printf(c"%d %g %d %lld\n".as_ptr(), w.c as i32, w.d, w.i, w.l) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %u\n".as_ptr(),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w.d) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w.i) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(w.l) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 8u32, 8u32, 8u32, 8u32) };
// CHECK-NEXT:     let mut image: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(image) as *mut (), 170i32, 32u32) };
// CHECK-NEXT:     image.c = 1;
// CHECK-NEXT:     image.d = 2.5f64;
// CHECK-NEXT:     image.i = 3;
// CHECK-NEXT:     image.l = 4;
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"wide".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(image) as *const (),
// CHECK-NEXT:         32u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut n: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(n) as *mut (), 0i32, 48u32) };
// CHECK-NEXT:     n.tag = 9;
// CHECK-NEXT:     n.inner.c = w.c;
// CHECK-NEXT:     n.inner.d = w.d;
// CHECK-NEXT:     n.inner.i = w.i;
// CHECK-NEXT:     n.inner.l = w.l;
// CHECK-NEXT:     n.u = 72623859790382856;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"nested %zu %zu %u %u %u\n".as_ptr(),
// CHECK-NEXT:             48u32,
// CHECK-NEXT:             8u32,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n.inner) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n.inner.l) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(n.u) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"nested".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(n) as *const (),
// CHECK-NEXT:         48u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut a: arrays = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: arrays = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].s = 7i16;
// CHECK-NEXT:         [[T_1]].d = [1.0f64, -2.0f64];
// CHECK-NEXT:         [[T_1]].tail = 122i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"arrays %zu %u %u %u\n".as_ptr(),
// CHECK-NEXT:             32u32,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *const (),
// CHECK-NEXT:                 (a.d.as_mut_ptr() as *mut f64).add(0) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *const (),
// CHECK-NEXT:                 (a.d.as_mut_ptr() as *mut f64).add(1) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(a.tail) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut h: holds_union = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: holds_union = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].c = 3i8;
// CHECK-NEXT:         [[T_2]].u = {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: either = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].c = 0i8;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         h.u.d = 6.25f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"union %zu %zu %zu %u %g\n".as_ptr(),
// CHECK-NEXT:             8u32,
// CHECK-NEXT:             16u32,
// CHECK-NEXT:             8u32,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(h) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(h.u) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             h.u.d,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(b) as *mut (), 0i32, 8u32) };
// CHECK-NEXT:     b.c = 1;
// CHECK-NEXT:     b.__slate_bits_0.__set_b(-3i64);
// CHECK-NEXT:     b.__slate_bits_0.__set_rest(123456789i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"bits %zu %zu %d %lld\n".as_ptr(),
// CHECK-NEXT:             8u32,
// CHECK-NEXT:             8u32,
// CHECK-NEXT:             b.__slate_bits_0.__get_b() as i32,
// CHECK-NEXT:             b.__slate_bits_0.__get_rest(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bits".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(b) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut cm: complex_member = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: complex_member = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]].c = 2i8;
// CHECK-NEXT:         [[T_4]].z = num_complex::Complex {
// CHECK-NEXT:             re: 0i32 as f64,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     cm.z = 1.5f64
// CHECK-NEXT:         + num_complex::Complex {
// CHECK-NEXT:             re: 0.0f64,
// CHECK-NEXT:             im: 2.5f64,
// CHECK-NEXT:         };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"complex %zu %u %g %g\n".as_ptr(),
// CHECK-NEXT:             24u32,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cm) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(cm.z) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             cm.z.re,
// CHECK-NEXT:             cm.z.im,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut list: [wide; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         list[i as usize] = make_wide(i + 1);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"list %zu %u %u\n".as_ptr(),
// CHECK-NEXT:             96u32,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 (list.as_mut_ptr() as *mut wide).add(0) as *const (),
// CHECK-NEXT:                 (list.as_mut_ptr() as *mut wide).add(1) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 (list.as_mut_ptr() as *mut wide).add(0) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(list[2].l) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         unsafe { printf(c"%g\n".as_ptr(), sum_wide(list[i as usize])) };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
