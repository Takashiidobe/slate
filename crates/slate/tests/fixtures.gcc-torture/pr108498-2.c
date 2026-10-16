/* PR tree-optimization/108498 */
/* { dg-require-effective-target int32plus } */

struct U {
  char c[16];
};
struct V {
  char c[16];
};
struct S {
  unsigned int a : 3, b : 8, c : 21;
  struct U     d;
  unsigned int e;
  struct V     f;
  unsigned int g : 5, h : 27;
};
struct T {
  unsigned int a : 16, b : 8, c : 8;
  struct U     d;
  unsigned int e;
  struct V     f;
  unsigned int g : 5, h : 27;
};

__attribute__((noipa)) void foo(struct S *p) {
  p->b = 231;
  p->c = 42;
  p->d = (struct U){"abcdefghijklmno"};
  p->e = 0xdeadbeef;
  p->f = (struct V){"ABCDEFGHIJKLMNO"};
}

__attribute__((noipa)) void bar(struct S *p) {
  p->b = 231;
  p->c = 42;
  p->d = (struct U){"abcdefghijklmno"};
  p->e = 0xdeadbeef;
  p->f = (struct V){"ABCDEFGHIJKLMNO"};
  p->g = 12;
}

__attribute__((noipa)) void baz(struct T *p) {
  p->c = 42;
  p->d = (struct U){"abcdefghijklmno"};
  p->e = 0xdeadbeef;
  p->f = (struct V){"ABCDEFGHIJKLMNO"};
  p->g = 12;
}

int main() {
  if (__CHAR_BIT__ != 8 || __SIZEOF_INT__ != 4)
    return 0;
  struct S s = {};
  struct T t = {};
  foo(&s);
  if (s.a != 0 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 0 || s.h != 0)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  s.a = 7;
  s.g = 31;
  s.h = (1U << 27) - 1;
  foo(&s);
  if (s.a != 7 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 31 ||
      s.h != (1U << 27) - 1)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  bar(&s);
  if (s.a != 0 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 12 || s.h != 0)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  s.a = 7;
  s.g = 31;
  s.h = (1U << 27) - 1;
  bar(&s);
  if (s.a != 7 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 12 ||
      s.h != (1U << 27) - 1)
    __builtin_abort();
  baz(&t);
  if (t.a != 0 || t.b != 0 || t.c != 42 ||
      __builtin_memcmp(&t.d.c, "abcdefghijklmno", 16) || t.e != 0xdeadbeef ||
      __builtin_memcmp(&t.f.c, "ABCDEFGHIJKLMNO", 16) || t.g != 12 || t.h != 0)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  t.a = 7;
  t.b = 255;
  t.g = 31;
  t.h = (1U << 27) - 1;
  baz(&t);
  if (t.a != 7 || t.b != 255 || t.c != 42 ||
      __builtin_memcmp(&t.d.c, "abcdefghijklmno", 16) || t.e != 0xdeadbeef ||
      __builtin_memcmp(&t.f.c, "ABCDEFGHIJKLMNO", 16) || t.g != 12 ||
      t.h != (1U << 27) - 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/108498
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct U {
// CHECK-NEXT:     c: [i8; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct V {
// CHECK-NEXT:     c: [i8; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT:     d: U,
// CHECK-NEXT:     e: u32,
// CHECK-NEXT:     f: V,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits2U1,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT:     d: U,
// CHECK-NEXT:     e: u32,
// CHECK-NEXT:     f: V,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits3U1,
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(21)]
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
// CHECK-NEXT:     pub struct __SlateBits2U1 {
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub g: u32,
// CHECK-NEXT:         #[bits(27)]
// CHECK-NEXT:         pub h: u32,
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
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(8)]
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
// CHECK-NEXT:     pub struct __SlateBits3U1 {
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub g: u32,
// CHECK-NEXT:         #[bits(27)]
// CHECK-NEXT:         pub h: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: &mut S) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: V = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).__slate_bits_0.__set_b((231 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).__slate_bits_0.__set_c((42 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).d = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = U {
// CHECK-NEXT:                     c: [
// CHECK-NEXT:                         97 as i8, 98 as i8, 99 as i8, 100 as i8, 101 as i8, 102 as i8, 103 as i8,
// CHECK-NEXT:                         104 as i8, 105 as i8, 106 as i8, 107 as i8, 108 as i8, 109 as i8,
// CHECK-NEXT:                         110 as i8, 111 as i8, 0 as i8,
// CHECK-NEXT:                     ],
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).e = 3735928559 as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).f = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = V {
// CHECK-NEXT:                     c: [
// CHECK-NEXT:                         65 as i8, 66 as i8, 67 as i8, 68 as i8, 69 as i8, 70 as i8, 71 as i8,
// CHECK-NEXT:                         72 as i8, 73 as i8, 74 as i8, 75 as i8, 76 as i8, 77 as i8, 78 as i8,
// CHECK-NEXT:                         79 as i8, 0 as i8,
// CHECK-NEXT:                     ],
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: &mut S) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: V = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).__slate_bits_0.__set_b((231 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).__slate_bits_0.__set_c((42 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).d = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_2]] = U {
// CHECK-NEXT:                     c: [
// CHECK-NEXT:                         97 as i8, 98 as i8, 99 as i8, 100 as i8, 101 as i8, 102 as i8, 103 as i8,
// CHECK-NEXT:                         104 as i8, 105 as i8, 106 as i8, 107 as i8, 108 as i8, 109 as i8,
// CHECK-NEXT:                         110 as i8, 111 as i8, 0 as i8,
// CHECK-NEXT:                     ],
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).e = 3735928559 as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).f = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_3]] = V {
// CHECK-NEXT:                     c: [
// CHECK-NEXT:                         65 as i8, 66 as i8, 67 as i8, 68 as i8, 69 as i8, 70 as i8, 71 as i8,
// CHECK-NEXT:                         72 as i8, 73 as i8, 74 as i8, 75 as i8, 76 as i8, 77 as i8, 78 as i8,
// CHECK-NEXT:                         79 as i8, 0 as i8,
// CHECK-NEXT:                     ],
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut S)).__slate_bits_1.__set_g((12 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut p: &mut T) {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: V = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut T)).__slate_bits_0.__set_c((42 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut T)).d = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_4]] = U {
// CHECK-NEXT:                     c: [
// CHECK-NEXT:                         97 as i8, 98 as i8, 99 as i8, 100 as i8, 101 as i8, 102 as i8, 103 as i8,
// CHECK-NEXT:                         104 as i8, 105 as i8, 106 as i8, 107 as i8, 108 as i8, 109 as i8,
// CHECK-NEXT:                         110 as i8, 111 as i8, 0 as i8,
// CHECK-NEXT:                     ],
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut T)).e = 3735928559 as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut T)).f = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_5]] = V {
// CHECK-NEXT:                     c: [
// CHECK-NEXT:                         65 as i8, 66 as i8, 67 as i8, 68 as i8, 69 as i8, 70 as i8, 71 as i8,
// CHECK-NEXT:                         72 as i8, 73 as i8, 74 as i8, 75 as i8, 76 as i8, 77 as i8, 78 as i8,
// CHECK-NEXT:                         79 as i8, 0 as i8,
// CHECK-NEXT:                     ],
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_5]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut T)).__slate_bits_1.__set_g((12 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as i32) != (8 as i32) || (4 as i32) != (4 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut s: S = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut t: T = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: T = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (s.__slate_bits_0.__get_a() as i32) != (0 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_b() as i32) != (231 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_c() as i32) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.d.c) as *const (),
// CHECK-NEXT:                 b"abcdefghijklmno\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] || s.e != (3735928559 as u32) {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.f.c) as *const (),
// CHECK-NEXT:                 b"ABCDEFGHIJKLMNO\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_7]]
// CHECK-NEXT:         || (s.__slate_bits_1.__get_g() as i32) != (0 as i32)
// CHECK-NEXT:         || (s.__slate_bits_1.__get_h() as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(s) as *mut (), 0 as i32, 44 as u64) };
// CHECK-NEXT:     s.__slate_bits_0.__set_a((7 as i32) as u32);
// CHECK-NEXT:     s.__slate_bits_1.__set_g((31 as i32) as u32);
// CHECK-NEXT:     s.__slate_bits_1
// CHECK-NEXT:         .__set_h(((1 as u32) << (27 as i32)).wrapping_sub((1 as i32) as u32));
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (s.__slate_bits_0.__get_a() as i32) != (7 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_b() as i32) != (231 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_c() as i32) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_8]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_8]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.d.c) as *const (),
// CHECK-NEXT:                 b"abcdefghijklmno\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_8]] || s.e != (3735928559 as u32) {
// CHECK-NEXT:         [[V_9]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.f.c) as *const (),
// CHECK-NEXT:                 b"ABCDEFGHIJKLMNO\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_9]]
// CHECK-NEXT:         || (s.__slate_bits_1.__get_g() as i32) != (31 as i32)
// CHECK-NEXT:         || ((s.__slate_bits_1.__get_h() as i32) as u32)
// CHECK-NEXT:             != ((1 as u32) << (27 as i32)).wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(s) as *mut (), 0 as i32, 44 as u64) };
// CHECK-NEXT:     bar(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (s.__slate_bits_0.__get_a() as i32) != (0 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_b() as i32) != (231 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_c() as i32) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.d.c) as *const (),
// CHECK-NEXT:                 b"abcdefghijklmno\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_10]] || s.e != (3735928559 as u32) {
// CHECK-NEXT:         [[V_11]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_11]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.f.c) as *const (),
// CHECK-NEXT:                 b"ABCDEFGHIJKLMNO\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_11]]
// CHECK-NEXT:         || (s.__slate_bits_1.__get_g() as i32) != (12 as i32)
// CHECK-NEXT:         || (s.__slate_bits_1.__get_h() as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(s) as *mut (), 0 as i32, 44 as u64) };
// CHECK-NEXT:     s.__slate_bits_0.__set_a((7 as i32) as u32);
// CHECK-NEXT:     s.__slate_bits_1.__set_g((31 as i32) as u32);
// CHECK-NEXT:     s.__slate_bits_1
// CHECK-NEXT:         .__set_h(((1 as u32) << (27 as i32)).wrapping_sub((1 as i32) as u32));
// CHECK-NEXT:     bar(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (s.__slate_bits_0.__get_a() as i32) != (7 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_b() as i32) != (231 as i32)
// CHECK-NEXT:         || (s.__slate_bits_0.__get_c() as i32) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_12]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_12]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.d.c) as *const (),
// CHECK-NEXT:                 b"abcdefghijklmno\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_12]] || s.e != (3735928559 as u32) {
// CHECK-NEXT:         [[V_13]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_13]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(s.f.c) as *const (),
// CHECK-NEXT:                 b"ABCDEFGHIJKLMNO\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_13]]
// CHECK-NEXT:         || (s.__slate_bits_1.__get_g() as i32) != (12 as i32)
// CHECK-NEXT:         || ((s.__slate_bits_1.__get_h() as i32) as u32)
// CHECK-NEXT:             != ((1 as u32) << (27 as i32)).wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     baz(unsafe { &mut (*std::ptr::addr_of_mut!(t)) });
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (t.__slate_bits_0.__get_a() as i32) != (0 as i32)
// CHECK-NEXT:         || (t.__slate_bits_0.__get_b() as i32) != (0 as i32)
// CHECK-NEXT:         || (t.__slate_bits_0.__get_c() as i32) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_14]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_14]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(t.d.c) as *const (),
// CHECK-NEXT:                 b"abcdefghijklmno\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_14]] || t.e != (3735928559 as u32) {
// CHECK-NEXT:         [[V_15]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_15]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(t.f.c) as *const (),
// CHECK-NEXT:                 b"ABCDEFGHIJKLMNO\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_15]]
// CHECK-NEXT:         || (t.__slate_bits_1.__get_g() as i32) != (12 as i32)
// CHECK-NEXT:         || (t.__slate_bits_1.__get_h() as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(s) as *mut (), 0 as i32, 44 as u64) };
// CHECK-NEXT:     t.__slate_bits_0.__set_a((7 as i32) as u32);
// CHECK-NEXT:     t.__slate_bits_0.__set_b((255 as i32) as u32);
// CHECK-NEXT:     t.__slate_bits_1.__set_g((31 as i32) as u32);
// CHECK-NEXT:     t.__slate_bits_1
// CHECK-NEXT:         .__set_h(((1 as u32) << (27 as i32)).wrapping_sub((1 as i32) as u32));
// CHECK-NEXT:     baz(unsafe { &mut (*std::ptr::addr_of_mut!(t)) });
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (t.__slate_bits_0.__get_a() as i32) != (7 as i32)
// CHECK-NEXT:         || (t.__slate_bits_0.__get_b() as i32) != (255 as i32)
// CHECK-NEXT:         || (t.__slate_bits_0.__get_c() as i32) != (42 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_16]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_16]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(t.d.c) as *const (),
// CHECK-NEXT:                 b"abcdefghijklmno\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_16]] || t.e != (3735928559 as u32) {
// CHECK-NEXT:         [[V_17]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_17]] = (unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(t.f.c) as *const (),
// CHECK-NEXT:                 b"ABCDEFGHIJKLMNO\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((16 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_17]]
// CHECK-NEXT:         || (t.__slate_bits_1.__get_g() as i32) != (12 as i32)
// CHECK-NEXT:         || ((t.__slate_bits_1.__get_h() as i32) as u32)
// CHECK-NEXT:             != ((1 as u32) << (27 as i32)).wrapping_sub((1 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
