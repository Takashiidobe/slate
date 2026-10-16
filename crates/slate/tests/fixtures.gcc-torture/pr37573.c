/* PR tree-optimization/37573 */
/* { dg-require-effective-target int32plus } */

struct S {
  unsigned int *a;
  unsigned int  b;
  unsigned int  c[624];
};

static unsigned char __attribute__((noinline)) foo(struct S *s) {
  unsigned int r;
  if (!--s->b) {
    unsigned int *c = s->c;
    unsigned int  i;
    s->a = c;
    for (i = 0; i < 227; i++)
      c[i] = ((((c[i] ^ c[i + 1]) & 0x7ffffffe) ^ c[i]) >> 1) ^
             ((0 - (c[i + 1] & 1)) & 0x9908b0df) ^ c[i + 397];
  }
  r  = *(s->a++);
  r ^= (r >> 11);
  r ^= ((r & 0xff3a58ad) << 7);
  r ^= ((r & 0xffffdf8c) << 15);
  r ^= (r >> 18);
  return (unsigned char)(r >> 1);
}

static void __attribute__((noinline)) bar(unsigned char *p, unsigned int q,
                                          unsigned int r) {
  struct S      s;
  unsigned int  i;
  unsigned int *c = s.c;
  *c              = r;
  for (i = 1; i < 624; i++)
    c[i] = i + 0x6c078965 * ((c[i - 1] >> 30) ^ c[i - 1]);
  s.b = 1;
  while (q--)
    *p++ ^= foo(&s);
};

static unsigned char p[23] = {0xc0, 0x49, 0x17, 0x32, 0x62, 0x1e, 0x2e, 0xd5,
                              0x4c, 0x19, 0x28, 0x49, 0x91, 0xe4, 0x72, 0x83,
                              0x91, 0x3d, 0x93, 0x83, 0xb3, 0x61, 0x38};

static unsigned char q[23] = {0x3e, 0x41, 0x55, 0x54, 0x4f, 0x49, 0x54, 0x20,
                              0x55, 0x4e, 0x49, 0x43, 0x4f, 0x44, 0x45, 0x20,
                              0x53, 0x43, 0x52, 0x49, 0x50, 0x54, 0x3c};

int main(void) {
  unsigned int s;
  s = 23;
  bar(p, s, s + 0xa25e);
  if (__builtin_memcmp(p, q, s) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/37573
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: *mut u32,
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: [u32; 624],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut s: &mut S) -> u8 {
// CHECK-NEXT:     let mut r: u32 = 0 as u32;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = unsafe { (*s).b };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = [[V_3]].wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*s).b = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if !([[V_4]] != (0 as u32)) {
// CHECK-NEXT:         let mut c: *mut u32 = unsafe { (*(s as *mut S)).c.as_mut_ptr() as *mut u32 };
// CHECK-NEXT:         let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(s as *mut S)).a = c;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < ((227 as i32) as u32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { c.offset(i as isize) } =
// CHECK-NEXT:                     (((unsafe { *unsafe { c.offset(i as isize) } })
// CHECK-NEXT:                         ^ unsafe {
// CHECK-NEXT:                             *unsafe { c.offset(i.wrapping_add((1 as i32) as u32) as isize) }
// CHECK-NEXT:                         })
// CHECK-NEXT:                         & ((2147483646 as i32) as u32)
// CHECK-NEXT:                         ^ unsafe { *unsafe { c.offset(i as isize) } })
// CHECK-NEXT:                         >> (1 as i32)
// CHECK-NEXT:                         ^ ((0 as i32) as u32).wrapping_sub(
// CHECK-NEXT:                             (unsafe {
// CHECK-NEXT:                                 *unsafe { c.offset(i.wrapping_add((1 as i32) as u32) as isize) }
// CHECK-NEXT:                             }) & ((1 as i32) as u32),
// CHECK-NEXT:                         ) & (2567483615 as u32)
// CHECK-NEXT:                         ^ unsafe {
// CHECK-NEXT:                             *unsafe { c.offset(i.wrapping_add((397 as i32) as u32) as isize) }
// CHECK-NEXT:                         };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: u32 = [[V_5]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:             i = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: *mut u32 = unsafe { (*s).a };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: *mut u32 = unsafe { [[V_7]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*s).a = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r = unsafe { *[[V_7]] };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u32 = r;
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u32 = [[V_9]] ^ r >> (11 as i32);
// CHECK-NEXT:     r = [[V_10]];
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u32 = r;
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u32 = [[V_11]] ^ (r & (4282013869 as u32)) << (7 as i32);
// CHECK-NEXT:     r = [[V_12]];
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: u32 = r;
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: u32 = [[V_13]] ^ (r & (4294958988 as u32)) << (15 as i32);
// CHECK-NEXT:     r = [[V_14]];
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: u32 = r;
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: u32 = [[V_15]] ^ r >> (18 as i32);
// CHECK-NEXT:     r = [[V_16]];
// CHECK-NEXT:     (r >> (1 as i32)) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut [[P_0:p_[0-9]+]]: *mut u8, mut [[Q_0:q_[0-9]+]]: u32, mut r: u32) {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: u32 = (1 as i32) as u32;
// CHECK-NEXT:     let mut c: *mut u32 = s.c.as_mut_ptr() as *mut u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *c = r;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < ((624 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { c.offset(i as isize) } =
// CHECK-NEXT:                 i.wrapping_add(((1812433253 as i32) as u32).wrapping_mul(
// CHECK-NEXT:                     (unsafe { *unsafe { c.offset(i.wrapping_sub((1 as i32) as u32) as isize) } })
// CHECK-NEXT:                         >> (30 as i32)
// CHECK-NEXT:                         ^ unsafe {
// CHECK-NEXT:                             *unsafe { c.offset(i.wrapping_sub((1 as i32) as u32) as isize) }
// CHECK-NEXT:                         },
// CHECK-NEXT:                 ));
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: u32 = [[V_17]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_18]];
// CHECK-NEXT:     }
// CHECK-NEXT:     s.b = (1 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: u32 = [[Q_0]];
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: u32 = [[V_19]].wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:         [[Q_0]] = [[V_20]];
// CHECK-NEXT:         if !([[V_19]] != (0 as u32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: *mut u8 = [[P_0]];
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: *mut u8 = unsafe { [[V_21]].offset((1 as i32) as isize) };
// CHECK-NEXT:         [[P_0]] = [[V_22]];
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: u8 = unsafe { *[[V_21]] };
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: u8 = (((([[V_23]] as u32) as i32)
// CHECK-NEXT:             ^ ((foo(unsafe { &mut (*std::ptr::addr_of_mut!(s)) }) as u32) as i32))
// CHECK-NEXT:             as i8) as u8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_21]] = [[V_24]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: __SlateAlign16<[u8; 23]> = __SlateAlign16([
// CHECK-NEXT:     ((192 as i32) as i8) as u8,
// CHECK-NEXT:     ((73 as i32) as i8) as u8,
// CHECK-NEXT:     ((23 as i32) as i8) as u8,
// CHECK-NEXT:     ((50 as i32) as i8) as u8,
// CHECK-NEXT:     ((98 as i32) as i8) as u8,
// CHECK-NEXT:     ((30 as i32) as i8) as u8,
// CHECK-NEXT:     ((46 as i32) as i8) as u8,
// CHECK-NEXT:     ((213 as i32) as i8) as u8,
// CHECK-NEXT:     ((76 as i32) as i8) as u8,
// CHECK-NEXT:     ((25 as i32) as i8) as u8,
// CHECK-NEXT:     ((40 as i32) as i8) as u8,
// CHECK-NEXT:     ((73 as i32) as i8) as u8,
// CHECK-NEXT:     ((145 as i32) as i8) as u8,
// CHECK-NEXT:     ((228 as i32) as i8) as u8,
// CHECK-NEXT:     ((114 as i32) as i8) as u8,
// CHECK-NEXT:     ((131 as i32) as i8) as u8,
// CHECK-NEXT:     ((145 as i32) as i8) as u8,
// CHECK-NEXT:     ((61 as i32) as i8) as u8,
// CHECK-NEXT:     ((147 as i32) as i8) as u8,
// CHECK-NEXT:     ((131 as i32) as i8) as u8,
// CHECK-NEXT:     ((179 as i32) as i8) as u8,
// CHECK-NEXT:     ((97 as i32) as i8) as u8,
// CHECK-NEXT:     ((56 as i32) as i8) as u8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: __SlateAlign16<[u8; 23]> = __SlateAlign16([
// CHECK-NEXT:     ((62 as i32) as i8) as u8,
// CHECK-NEXT:     ((65 as i32) as i8) as u8,
// CHECK-NEXT:     ((85 as i32) as i8) as u8,
// CHECK-NEXT:     ((84 as i32) as i8) as u8,
// CHECK-NEXT:     ((79 as i32) as i8) as u8,
// CHECK-NEXT:     ((73 as i32) as i8) as u8,
// CHECK-NEXT:     ((84 as i32) as i8) as u8,
// CHECK-NEXT:     ((32 as i32) as i8) as u8,
// CHECK-NEXT:     ((85 as i32) as i8) as u8,
// CHECK-NEXT:     ((78 as i32) as i8) as u8,
// CHECK-NEXT:     ((73 as i32) as i8) as u8,
// CHECK-NEXT:     ((67 as i32) as i8) as u8,
// CHECK-NEXT:     ((79 as i32) as i8) as u8,
// CHECK-NEXT:     ((68 as i32) as i8) as u8,
// CHECK-NEXT:     ((69 as i32) as i8) as u8,
// CHECK-NEXT:     ((32 as i32) as i8) as u8,
// CHECK-NEXT:     ((83 as i32) as i8) as u8,
// CHECK-NEXT:     ((67 as i32) as i8) as u8,
// CHECK-NEXT:     ((82 as i32) as i8) as u8,
// CHECK-NEXT:     ((73 as i32) as i8) as u8,
// CHECK-NEXT:     ((80 as i32) as i8) as u8,
// CHECK-NEXT:     ((84 as i32) as i8) as u8,
// CHECK-NEXT:     ((60 as i32) as i8) as u8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: u32 = (23 as i32) as u32;
// CHECK-NEXT:     bar(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(p.0) as *mut u8 },
// CHECK-NEXT:         s,
// CHECK-NEXT:         s.wrapping_add((41566 as i32) as u32),
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(p.0) as *mut u8 }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(q.0) as *mut u8 }) as *const (),
// CHECK-NEXT:             s as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
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
