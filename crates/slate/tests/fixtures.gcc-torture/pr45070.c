/* PR45070 */
extern void abort(void);

struct packed_ushort {
  unsigned short ucs;
} __attribute__((packed));

struct source {
  int pos, length;
  int flag;
};

static void __attribute__((noinline)) fetch(struct source *p) {
  p->length = 128;
}

static struct packed_ushort __attribute__((noinline)) next(struct source *p) {
  struct packed_ushort rv;

  if (p->pos >= p->length) {
    if (p->flag) {
      p->flag = 0;
      fetch(p);
      return next(p);
    }
    p->flag = 1;
    rv.ucs  = 0xffff;
    return rv;
  }
  rv.ucs = 0;
  return rv;
}

int main(void) {
  struct source s;
  int           i;

  s.pos    = 0;
  s.length = 0;
  s.flag   = 0;

  for (i = 0; i < 16; i++) {
    struct packed_ushort rv = next(&s);
    if ((i == 0 && rv.ucs != 0xffff) || (i > 0 && rv.ucs != 0))
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR45070
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct packed_ushort {
// CHECK-NEXT:     ucs: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct source {
// CHECK-NEXT:     pos: i32,
// CHECK-NEXT:     length: i32,
// CHECK-NEXT:     flag: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fetch(mut p: &mut source) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(p as *mut source)).length = 128 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn next(mut p: &mut source) -> packed_ushort {
// CHECK-NEXT:     let mut rv: packed_ushort = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (unsafe { (*(p as *mut source)).pos }) >= unsafe { (*(p as *mut source)).length } {
// CHECK-NEXT:         if (unsafe { (*(p as *mut source)).flag }) != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(p as *mut source)).flag = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             fetch(p);
// CHECK-NEXT:             return next(p);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(p as *mut source)).flag = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         rv.ucs = ((65535 as i32) as i16) as u16;
// CHECK-NEXT:         return rv;
// CHECK-NEXT:     }
// CHECK-NEXT:     rv.ucs = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     return rv;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: source = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     s.pos = 0 as i32;
// CHECK-NEXT:     s.length = 0 as i32;
// CHECK-NEXT:     s.flag = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         let mut rv: packed_ushort = next(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
// CHECK-NEXT:         if i == (0 as i32) && ((rv.ucs as u32) as i32) != (65535 as i32)
// CHECK-NEXT:             || i > (0 as i32) && ((rv.ucs as u32) as i32) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
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
