/* PR tree-optimization/20601 */
extern void abort(void);
extern void exit(int);

struct T {
  char  *t1;
  char   t2[4096];
  char **t3;
};

int      a[5];
int      b;
char   **c;
int      d;
char   **e;
struct T t;
char    *f[16];
char    *g[] = {"a", "-u", "b", "c"};

__attribute__((__noreturn__)) void foo(void) {
  while (1)
    ;
}

__attribute__((noinline)) char *bar(char *x, unsigned int y) { return 0; }

static inline char *baz(char *x, unsigned int y) {
  if (sizeof(t.t2) != (unsigned int)-1 && y > sizeof(t.t2))
    foo();
  return bar(x, y);
}

static inline int setup1(int x) {
  char *p;
  int   rval;

  if (!baz(t.t2, sizeof(t.t2)))
    baz(t.t2, sizeof(t.t2));

  if (x & 0x200) {
    char **h, **i = e;

    ++d;
    e = f;
    if (t.t1 && *t.t1)
      e[0] = t.t1;
    else
      abort();

    for (h = e + 1; (*h = *i); ++i, ++h)
      ;
  }
  return 1;
}

static inline int setup2(void) {
  int j = 1;

  e = c + 1;
  d = b - 1;
  while (d > 0 && e[0][0] == '-') {
    if (e[0][1] != '\0' && e[0][2] != '\0')
      abort();

    switch (e[0][1]) {
    case 'u':
      if (!e[1])
        abort();

      t.t3 = &e[1];
      d--;
      e++;
      break;
    case 'P':
      j |= 0x1000;
      break;
    case '-':
      d--;
      e++;
      if (j == 1)
        j |= 0x600;
      return j;
    }
    d--;
    e++;
  }

  if (d > 0 && !(j & 1))
    abort();

  return j;
}

int main(void) {
  int x;
  c    = g;
  b    = 4;
  x    = setup2();
  t.t1 = "/bin/sh";
  setup1(x);
  /* PRE shouldn't transform x into the constant 0x601 here, it's not legal.  */
  if ((x & 0x400) && !a[4])
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR tree-optimization/20601
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     t1: *mut i8,
// CHECK-NEXT:     t2: [i8; 4096],
// CHECK-NEXT:     t3: *mut *mut i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 5]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: T = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: __SlateAlign16<[*mut i8; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: __SlateAlign16<[*mut i8; 4]> = __SlateAlign16([
// CHECK-NEXT:     b"a\0".as_ptr() as *mut i8,
// CHECK-NEXT:     b"-u\0".as_ptr() as *mut i8,
// CHECK-NEXT:     b"b\0".as_ptr() as *mut i8,
// CHECK-NEXT:     b"c\0".as_ptr() as *mut i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: *mut i8, mut y: u32) -> *mut i8 {
// CHECK-NEXT:     std::ptr::null_mut::<i8>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: *mut i8, mut y: u32) -> *mut i8 {
// CHECK-NEXT:     if (4096 as u64) != ((-(1 as i32) as u32) as u64) && (y as u64) > (4096 as u64) {
// CHECK-NEXT:         foo();
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(x, y)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn setup1(mut x: i32) -> i32 {
// CHECK-NEXT:     if !(baz(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(t.t2) as *mut i8 },
// CHECK-NEXT:         (4096 as u64) as u32,
// CHECK-NEXT:     ) != std::ptr::null_mut::<i8>())
// CHECK-NEXT:     {
// CHECK-NEXT:         baz(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(t.t2) as *mut i8 },
// CHECK-NEXT:             (4096 as u64) as u32,
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     if x & (512 as i32) != (0 as i32) {
// CHECK-NEXT:         let mut h: *mut *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut i: *mut *mut i8 = unsafe { e };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = unsafe { std::ptr::addr_of_mut!(f.0) as *mut *mut i8 };
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { t.t1 }) != std::ptr::null_mut::<i8>()
// CHECK-NEXT:             && (unsafe { *unsafe { t.t1 } }) != (0 as i8)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { e }.offset((0 as i32) as isize) } = unsafe { t.t1 };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         h = unsafe { unsafe { e }.offset((1 as i32) as isize) };
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe { *i };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *h = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:             if !([[V_3]] != std::ptr::null_mut::<i8>()) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             {}
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: *mut *mut i8 = i;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: *mut *mut i8 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:             i = [[V_5]];
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: *mut *mut i8 = h;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: *mut *mut i8 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:             h = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn setup2() -> i32 {
// CHECK-NEXT:     let mut j: i32 = 1 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = unsafe { unsafe { c }.offset((1 as i32) as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = (unsafe { b }) - (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { d }) > (0 as i32)
// CHECK-NEXT:         && ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { *unsafe { unsafe { e }.offset((0 as i32) as isize) } }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             == (45 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { *unsafe { unsafe { e }.offset((0 as i32) as isize) } }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:             && ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { *unsafe { unsafe { e }.offset((0 as i32) as isize) } }
// CHECK-NEXT:                         .offset((2 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i32)
// CHECK-NEXT:                 != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: {
// CHECK-NEXT:             match (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { *unsafe { unsafe { e }.offset((0 as i32) as isize) } }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i32
// CHECK-NEXT:             {
// CHECK-NEXT:                 117 => {
// CHECK-NEXT:                     if !((unsafe { *unsafe { unsafe { e }.offset((1 as i32) as isize) } })
// CHECK-NEXT:                         != std::ptr::null_mut::<i8>())
// CHECK-NEXT:                     {
// CHECK-NEXT:                         unsafe { abort() };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         t.t3 = unsafe { unsafe { e }.offset((1 as i32) as isize) };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_8:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:                     let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] - (1 as i32);
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         d = [[V_9]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_10:__v[0-9]+]]: *mut *mut i8 = unsafe { e };
// CHECK-NEXT:                     let mut [[V_11:__v[0-9]+]]: *mut *mut i8 = unsafe { [[V_10]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         e = [[V_11]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 80 => {
// CHECK-NEXT:                     let mut [[V_12:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:                     let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] | (4096 as i32);
// CHECK-NEXT:                     j = [[V_13]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 45 => {
// CHECK-NEXT:                     let mut [[V_14:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:                     let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] - (1 as i32);
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         d = [[V_15]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let mut [[V_16:__v[0-9]+]]: *mut *mut i8 = unsafe { e };
// CHECK-NEXT:                     let mut [[V_17:__v[0-9]+]]: *mut *mut i8 = unsafe { [[V_16]].offset((1 as i32) as isize) };
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         e = [[V_17]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if j == (1 as i32) {
// CHECK-NEXT:                         let mut [[V_18:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:                         let mut [[V_19:__v[0-9]+]]: i32 = [[V_18]] | (1536 as i32);
// CHECK-NEXT:                         j = [[V_19]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     return j;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {}
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_21]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: *mut *mut i8 = unsafe { e };
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: *mut *mut i8 = unsafe { [[V_22]].offset((1 as i32) as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_23]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { d }) > (0 as i32) && !(j & (1 as i32) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     j
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = unsafe { std::ptr::addr_of_mut!(g.0) as *mut *mut i8 };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 4 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     x = setup2();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t.t1 = c"/bin/sh".as_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     setup1(x);
// CHECK-NEXT:     // PRE shouldn't transform x into the constant 0x601 here, it's not legal.
// CHECK-NEXT:     if x & (1024 as i32) != (0 as i32)
// CHECK-NEXT:         && !((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }.offset((4 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
