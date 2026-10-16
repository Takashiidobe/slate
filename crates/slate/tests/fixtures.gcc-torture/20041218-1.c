/* PR rtl-optimization/16968 */
/* Testcase by Jakub Jelinek  <jakub@redhat.com> */

struct T {
  unsigned int  b, c, *d;
  unsigned char e;
};
struct S {
  unsigned int a;
  struct T     f;
};
struct U {
  struct S g, h;
};
struct V {
  unsigned int i;
  struct U     j;
};

extern void exit(int);
extern void abort(void);

void *dummy1(void *x) { return ""; }

void *dummy2(void *x, void *y) { exit(0); }

struct V *baz(unsigned int x) {
  static struct V v;
  __builtin_memset(&v, 0x55, sizeof(v));
  return &v;
}

int check(void *x, struct S *y) {
  if (y->a || y->f.b || y->f.c || y->f.d || y->f.e)
    abort();
  return 1;
}

static struct V *bar(unsigned int x, void *y) {
  const struct T t = {0, 0, (void *)0, 0};
  struct V      *u;
  void          *v;
  v = dummy1(y);
  if (!v)
    return (void *)0;

  u        = baz(sizeof(struct V));
  u->i     = x;
  u->j.g.a = 0;
  u->j.g.f = t;
  u->j.h.a = 0;
  u->j.h.f = t;

  if (!check(v, &u->j.g) || !check(v, &u->j.h))
    return (void *)0;
  return u;
}

int foo(unsigned int *x, unsigned int y, void **z) {
  void        *v;
  unsigned int i, j;

  *z = v = (void *)0;

  for (i = 0; i < y; i++) {
    struct V *c;

    j = *x;

    switch (j) {
    case 1:
      c = bar(j, x);
      break;
    default:
      c = 0;
      break;
    }
    if (c)
      v = dummy2(v, c);
    else
      return 1;
  }

  *z = v;
  return 0;
}

int main(void) {
  unsigned int one = 1;
  void        *p;
  foo(&one, 1, &p);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/16968
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Jakub Jelinek  <jakub@redhat.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: i32, [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: u32,
// CHECK-NEXT:     d: *mut u32,
// CHECK-NEXT:     e: u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     f: T,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct U {
// CHECK-NEXT:     g: S,
// CHECK-NEXT:     h: S,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct V {
// CHECK-NEXT:     i: u32,
// CHECK-NEXT:     j: U,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dummy1(mut x: *mut ()) -> *mut () {
// CHECK-NEXT:     b"\0".as_ptr() as *mut ()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dummy2(mut x: *mut (), mut y: *mut ()) -> *mut () {
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: u32) -> *mut V {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(v) }) as *mut (),
// CHECK-NEXT:             85 as i32,
// CHECK-NEXT:             72 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(v) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: V = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn check(mut x: *mut (), mut y: *mut S) -> i32 {
// CHECK-NEXT:     if (unsafe { (*y).a }) != (0 as u32)
// CHECK-NEXT:         || (unsafe { (*y).f.b }) != (0 as u32)
// CHECK-NEXT:         || (unsafe { (*y).f.c }) != (0 as u32)
// CHECK-NEXT:         || (unsafe { (*y).f.d }) != std::ptr::null_mut::<u32>()
// CHECK-NEXT:         || (unsafe { (*y).f.e }) != (0 as u8)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u32, mut y: *mut ()) -> *mut V {
// CHECK-NEXT:     let mut t: T = T {
// CHECK-NEXT:         b: (0 as i32) as u32,
// CHECK-NEXT:         c: (0 as i32) as u32,
// CHECK-NEXT:         d: std::ptr::null_mut::<u32>(),
// CHECK-NEXT:         e: ((0 as i32) as i8) as u8,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut u: *mut V = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_4:v_[0-9]+]]: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[V_4]] = dummy1(y);
// CHECK-NEXT:     if !([[V_4]] != std::ptr::null_mut::<()>()) {
// CHECK-NEXT:         return std::ptr::null_mut::<V>();
// CHECK-NEXT:     }
// CHECK-NEXT:     u = baz((72 as u64) as u32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*u).i = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*u).j.g.a = (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*u).j.g.f = t;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*u).j.h.a = (0 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*u).j.h.f = t;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if !(check([[V_4]], unsafe { std::ptr::addr_of_mut!((*u).j.g) }) != (0 as i32)) {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = !(check([[V_4]], unsafe { std::ptr::addr_of_mut!((*u).j.h) }) != (0 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         return std::ptr::null_mut::<V>();
// CHECK-NEXT:     }
// CHECK-NEXT:     u
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &u32, mut y: u32, mut z: &mut *mut ()) -> i32 {
// CHECK-NEXT:     let mut [[V_6:v_[0-9]+]]: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut j: u32 = 0 as u32;
// CHECK-NEXT:     [[V_6]] = std::ptr::null_mut::<()>();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(z as *mut *mut ()) = std::ptr::null_mut::<()>();
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < y {
// CHECK-NEXT:         let mut c: *mut V = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         j = unsafe { *(x as *const u32) };
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:             match j {
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     c = bar(j, (x as *const u32) as *mut ());
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     c = std::ptr::null_mut::<V>();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if c != std::ptr::null_mut::<V>() {
// CHECK-NEXT:             [[V_6]] = dummy2([[V_6]], c as *mut ());
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: u32 = [[V_7]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(z as *mut *mut ()) = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut one: u32 = (1 as i32) as u32;
// CHECK-NEXT:     let mut p: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(one)) },
// CHECK-NEXT:         (1 as i32) as u32,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { abort() };
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
// SLATE-FILECHECK-END
