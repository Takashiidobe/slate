/* PR rtl-optimization/93908 */

struct T {
  int            b;
  int            c;
  unsigned short d;
  unsigned       e : 1, f : 1, g : 1, h : 2, i : 1, j : 1;
  signed int     k : 2;
};

struct S {
  struct T s;
  char     c[64];
} buf[2];

__attribute__((noipa)) void *baz(void) {
  static int cnt;
  return (void *)&buf[cnt++];
}

static inline __attribute__((always_inline)) struct T *bar(const char *a) {
  struct T *s;
  s    = baz();
  s->b = 1;
  s->k = -1;
  return s;
}

__attribute__((noipa)) void foo(const char *x, struct T **y) {
  struct T *l = bar(x);
  struct T *m = bar(x);
  y[0]        = l;
  y[1]        = m;
}

int main() {
  struct T *r[2];
  foo("foo", r);
  if (r[0]->e || r[0]->f || r[0]->g || r[0]->h || r[0]->i || r[0]->j ||
      r[0]->k != -1)
    __builtin_abort();
  if (r[1]->e || r[1]->f || r[1]->g || r[1]->h || r[1]->i || r[1]->j ||
      r[1]->k != -1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/93908
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: u16,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     s: T,
// CHECK-NEXT:     c: [i8; 64],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub e: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub g: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub h: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub k: i32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_7: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[S; 2]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn baz() -> *mut () {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { cnt };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         cnt = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut S }.offset([[V_0]] as isize) })
// CHECK-NEXT:         as *mut ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut cnt: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut a: *const i8) -> *mut T {
// CHECK-NEXT:     let mut s: *mut T = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s = baz() as *mut T;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*s).b = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*s).__slate_bits_0.__set_k(-(1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *const i8, mut y: *mut *mut T) {
// CHECK-NEXT:     let mut l: *mut T = bar(x);
// CHECK-NEXT:     let mut m: *mut T = bar(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { y.offset((0 as i32) as isize) } = l;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { y.offset((1 as i32) as isize) } = m;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: __SlateAlign16<[*mut T; 2]> = __SlateAlign16([0 as *mut T; 2]);
// CHECK-NEXT:     foo(
// CHECK-NEXT:         c"foo".as_ptr() as *const i8,
// CHECK-NEXT:         r.0.as_mut_ptr() as *mut *mut T,
// CHECK-NEXT:     );
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_e()
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_f()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_g()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_h()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_i()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_j()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_k()
// CHECK-NEXT:         }) != -(1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_e()
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_f()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_g()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_h()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_i()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_j()
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*unsafe { *unsafe { (r.0.as_mut_ptr() as *mut *mut T).offset((1 as i32) as isize) } })
// CHECK-NEXT:                 .__slate_bits_0
// CHECK-NEXT:                 .__get_k()
// CHECK-NEXT:         }) != -(1 as i32)
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
