struct C {
  unsigned int c;
  struct D {
    unsigned int columns       : 4;
    unsigned int fore          : 12;
    unsigned int back          : 6;
    unsigned int fragment      : 1;
    unsigned int standout      : 1;
    unsigned int underline     : 1;
    unsigned int strikethrough : 1;
    unsigned int reverse       : 1;
    unsigned int blink         : 1;
    unsigned int half          : 1;
    unsigned int bold          : 1;
    unsigned int invisible     : 1;
    unsigned int pad           : 1;
  } attr;
};

struct A {
  struct C    *data;
  unsigned int len;
};

struct B {
  struct A     *cells;
  unsigned char soft_wrapped : 1;
};

struct E {
  long     row, col;
  struct C defaults;
};

__attribute__((noinline)) void foo(struct E *screen, unsigned int c,
                                   int columns, struct B *row) {
  struct D attr;
  long     col;
  int      i;
  col                        = screen->col;
  attr                       = screen->defaults.attr;
  attr.columns               = columns;
  row->cells->data[col].c    = c;
  row->cells->data[col].attr = attr;
  col++;
  attr.fragment = 1;
  for (i = 1; i < columns; i++) {
    row->cells->data[col].c    = c;
    row->cells->data[col].attr = attr;
    col++;
  }
}

int main(void) {
  struct E e = {.row      = 5,
                .col      = 0,
                .defaults = {6, {-1, -1, -1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}}};
  struct C c[4];
  struct A a = {c, 4};
  struct B b = {&a, 1};
  struct D d;
  __builtin_memset(&c, 0, sizeof c);
  foo(&e, 65, 2, &b);
  d         = e.defaults.attr;
  d.columns = 2;
  if (__builtin_memcmp(&d, &c[0].attr, sizeof d))
    __builtin_abort();
  d.fragment = 1;
  if (__builtin_memcmp(&d, &c[1].attr, sizeof d))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_3:__v[0-9]+]]: *const (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     c: u32,
// CHECK-NEXT:     attr: D,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct D {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     data: *mut C,
// CHECK-NEXT:     len: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     cells: *mut A,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct E {
// CHECK-NEXT:     row: i64,
// CHECK-NEXT:     col: i64,
// CHECK-NEXT:     defaults: C,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
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
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub columns: u32,
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub fore: u32,
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub back: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub fragment: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub standout: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub underline: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub strikethrough: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub reverse: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub blink: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub half: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub bold: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub invisible: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub pad: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub soft_wrapped: u8,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut screen: &E, mut c: u32, mut columns: i32, mut row: &B) {
// CHECK-NEXT:     let mut attr: D = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut col: i64 = unsafe { (*(screen as *const E)).col };
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     attr = unsafe { (*(screen as *const E)).defaults.attr };
// CHECK-NEXT:     attr.__slate_bits_0.__set_columns(columns as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { (*(row as *const B)).cells }).data }.offset(col as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .c = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*unsafe { (*(row as *const B)).cells }).data }.offset(col as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .attr = attr;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i64 = col;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i64 = [[V_6]] + ((1 as i32) as i64);
// CHECK-NEXT:     col = [[V_7]];
// CHECK-NEXT:     attr.__slate_bits_0.__set_fragment((1 as i32) as u32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < columns {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { (*(row as *const B)).cells }).data }
// CHECK-NEXT:                     .offset(col as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 .c = c;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { (*(row as *const B)).cells }).data }
// CHECK-NEXT:                     .offset(col as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 .attr = attr;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i64 = col;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i64 = [[V_8]] + ((1 as i32) as i64);
// CHECK-NEXT:         col = [[V_9]];
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:         i = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: E = E {
// CHECK-NEXT:         row: (5 as i32) as i64,
// CHECK-NEXT:         col: (0 as i32) as i64,
// CHECK-NEXT:         defaults: C {
// CHECK-NEXT:             c: (6 as i32) as u32,
// CHECK-NEXT:             attr: {
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: D = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_columns(-(1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_fore(-(1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_back(-(1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_fragment((1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_standout((0 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_underline((1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_strikethrough((0 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_reverse((1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_blink((0 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_half((1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_bold((0 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_invisible((1 as i32) as u32);
// CHECK-NEXT:                 [[T_0]].__slate_bits_0.__set_pad((0 as i32) as u32);
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             },
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: __SlateAlign16<[C; 4]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     let mut a: A = A {
// CHECK-NEXT:         data: c.0.as_mut_ptr() as *mut C,
// CHECK-NEXT:         len: (4 as i32) as u32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: B = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: B = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].cells = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_soft_wrapped(((1 as i32) as i8) as u8);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut d: D = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(std::ptr::addr_of_mut!(c.0) as *mut (), 0 as i32, 32 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(e)) },
// CHECK-NEXT:         (65 as i32) as u32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(b)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     d = e.defaults.attr;
// CHECK-NEXT:     d.__slate_bits_0.__set_columns((2 as i32) as u32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(d) as *const (),
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::ptr::addr_of_mut!(
// CHECK-NEXT:                     (* unsafe { (c.0.as_mut_ptr() as * mut C).offset((0 as i32) as isize)
// CHECK-NEXT:                     }).attr
// CHECK-NEXT:                 )
// CHECK-NEXT:             }) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     d.__slate_bits_0.__set_fragment((1 as i32) as u32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(d) as *const (),
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::ptr::addr_of_mut!(
// CHECK-NEXT:                     (* unsafe { (c.0.as_mut_ptr() as * mut C).offset((1 as i32) as isize)
// CHECK-NEXT:                     }).attr
// CHECK-NEXT:                 )
// CHECK-NEXT:             }) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
