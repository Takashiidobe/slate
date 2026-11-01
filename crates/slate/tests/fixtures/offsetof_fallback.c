#include <stddef.h>
#include <stdio.h>

struct Inner {
  char tag;
  int  value;
};

union Either {
  char         small;
  struct Inner inner;
};

struct Outer {
  char         first;
  struct Inner inner;
  union Either either;
  int          type;
  int          list[4];
  struct Inner items[2];
};

struct Packed {
  char tag;
  int  value;
} __attribute__((packed));

struct Spaced {
  char tag;
  int  value __attribute__((aligned(32)));
  char last;
};

struct Bits {
  unsigned low : 3;
  unsigned high : 5;
  int      after;
};

struct Holder {
  long double wide;
  char        tag;
};

struct Flexible {
  int  count;
  char data[];
};

struct Anonymous {
  char tag;
  struct {
    short left;
    int   right;
  };
  union {
    char   small;
    double large;
  };
};

struct Raised {
  char tag;
  int  value;
} __attribute__((aligned(16)));

struct Erasing {
  char          tag;
  struct Raised raised;
} __attribute__((packed));

static char table[offsetof(struct Inner, value)];
static const unsigned long global_offset = offsetof(struct Outer, type) * 2;

enum { TYPE_OFFSET = offsetof(struct Outer, type) };

_Static_assert(offsetof(struct Inner, tag) == 0, "first member");

static int classify(unsigned long offset) {
  switch (offset) {
  case offsetof(struct Inner, tag):
    return 1;
  case offsetof(struct Inner, value):
    return 2;
  default:
    return 0;
  }
}

static void report_folded(int index) {
  printf("%d\n", (int)offsetof(struct Outer, list[2]));
  printf("%d\n", (int)offsetof(struct Outer, items[1].value));
  printf("%d\n", (int)offsetof(struct Outer, list[index]));
  printf("%d\n", (int)offsetof(struct Holder, tag));
  printf("%d\n", (int)offsetof(struct Flexible, data));
  printf("%d\n", (int)offsetof(struct Anonymous, right));
  printf("%d\n", (int)offsetof(struct Anonymous, large));
  printf("%d\n", (int)offsetof(struct Erasing, raised.value));
}

static void report_kept(void) {
  printf("%d\n", (int)offsetof(struct Inner, value));
  printf("%d\n", (int)offsetof(struct Outer, inner));
  printf("%d\n", (int)offsetof(struct Outer, inner.value));
  printf("%d\n", (int)offsetof(struct Outer, either.inner.value));
  printf("%d\n", (int)offsetof(struct Outer, type));
  printf("%d\n", (int)offsetof(struct Outer, list));
  printf("%d\n", (int)offsetof(union Either, small));
  printf("%d\n", (int)offsetof(union Either, inner.value));
  printf("%d\n", (int)offsetof(struct Packed, value));
  printf("%d\n", (int)offsetof(struct Spaced, value));
  printf("%d\n", (int)offsetof(struct Spaced, last));
  printf("%d\n", (int)offsetof(struct Bits, after));
  printf("%d\n", (int)offsetof(struct Anonymous, tag));
  printf("%d\n", (int)offsetof(struct Erasing, raised));
}

static int value_of(int *value) {
  struct Inner *owner =
      (struct Inner *)((char *)value - offsetof(struct Inner, value));
  return owner->tag + owner->value;
}

int main(void) {
  struct Inner inner = {1, 41};
  report_folded(3);
  report_kept();
  printf("%d\n", (int)sizeof(table));
  printf("%lu\n", global_offset);
  printf("%d\n", (int)TYPE_OFFSET);
  printf("%d\n", classify(offsetof(struct Inner, tag)));
  printf("%d\n", classify(offsetof(struct Inner, value)));
  printf("%d\n", classify(offsetof(struct Outer, type)));
  printf("%d\n", value_of(&inner.value));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Inner {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Either {
// CHECK-NEXT:     small: i8,
// CHECK-NEXT:     inner: Inner,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Outer {
// CHECK-NEXT:     first: i8,
// CHECK-NEXT:     inner: Inner,
// CHECK-NEXT:     either: Either,
// CHECK-NEXT:     r#type: i32,
// CHECK-NEXT:     list: [i32; 4],
// CHECK-NEXT:     items: [Inner; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Packed {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Spaced {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 31],
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     last: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT:     after: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Anonymous {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_offsetof_fallback_c_52,
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: __SlateRecord_offsetof_fallback_c_56,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_offsetof_fallback_c_52 {
// CHECK-NEXT:     left: i16,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_offsetof_fallback_c_56 {
// CHECK-NEXT:     small: i8,
// CHECK-NEXT:     large: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Raised {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Erasing {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     raised: std::mem::MaybeUninit<[u8; 16]>,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
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
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub low: u32,
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub high: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut table: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut global_offset: u64 = (std::mem::offset_of!(Outer, r#type) as u64)
// CHECK-NEXT:     .wrapping_mul(2u64);
// CHECK-NEXT: fn classify(offset: u64) -> i32 {
// CHECK-NEXT:     match offset {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn report_folded(index: i32) {
// CHECK-NEXT:     println!("{}", 32i32);
// CHECK-NEXT:     println!("{}", 52i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (24u64.wrapping_add(((index as i64) as u64).wrapping_mul(4)) as u32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", 4i32);
// CHECK-NEXT:     println!("{}", 8i32);
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", 5i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn report_kept() {
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Inner, value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, inner) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, inner.value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, either.inner.value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, r#type) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Outer, list) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Either, small) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Either, inner.value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Packed, value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Spaced, value) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Spaced, last) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Bits, after) as i32);
// CHECK-NEXT:     println!("{}", std::mem::offset_of!(Anonymous, tag) as i32);
// CHECK-NEXT:     println!("{}", 1i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn value_of(value: *mut i32) -> i32 {
// CHECK-NEXT:     let owner: *mut Inner = (unsafe {
// CHECK-NEXT:         (value as *mut i8).offset(-(std::mem::offset_of!(Inner, value) as isize))
// CHECK-NEXT:     }) as *mut Inner;
// CHECK-NEXT:     ((unsafe { (*owner).tag }) as i32) + unsafe { (*owner).value }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut inner: Inner = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Inner = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = 1i8;
// CHECK-NEXT:         [[T_0]].value = 41i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     report_folded(3i32);
// CHECK-NEXT:     report_kept();
// CHECK-NEXT:     println!("{}", std::mem::size_of::< [i8; 4] > () as i32);
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), global_offset) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", 20i32);
// CHECK-NEXT:     println!("{}", classify(std::mem::offset_of!(Inner, tag) as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::offset_of!(Inner, value) as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::offset_of!(Outer, r#type) as u64));
// CHECK-NEXT:     println!("{}", value_of(std::ptr::addr_of_mut!(inner.value)));
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
