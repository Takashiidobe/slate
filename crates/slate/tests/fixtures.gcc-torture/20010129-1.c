/* { dg-options "-mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } }
 */

extern void abort(void);
extern void exit(int);

long baz1(void *a) {
  static long l;
  return l++;
}

int baz2(const char *a) { return 0; }

int baz3(int i) {
  if (!i)
    abort();
  return 1;
}

void **bar;

int foo(void *a, long b, int c) {
  int    d = 0, e, f = 0, i;
  char   g[256];
  void **h;

  g[0] = '\n';
  g[1] = 0;

  while (baz1(a) < b) {
    if (g[0] != ' ' && g[0] != '\t') {
      f = 1;
      e = 0;
      if (!d && baz2(g) == 0) {
        if ((c & 0x10) == 0)
          continue;
        e = d = 1;
      }
      if (!((c & 0x10) && (c & 0x4000) && e) && (c & 2))
        continue;
      if ((c & 0x2000) && baz2(g) == 0)
        continue;
      if ((c & 0x1408) && baz2(g) == 0)
        continue;
      if ((c & 0x200) && baz2(g) == 0)
        continue;
      if (c & 0x80) {
        for (h = bar, i = 0; h; h = (void **)*h, i++)
          if (baz3(i))
            break;
      }
      f = 0;
    }
  }
  return 0;
}

int main() {
  void *n = 0;
  bar     = &n;
  foo(&n, 1, 0xc811);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn baz1(mut a: &()) -> i64 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = unsafe { l };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] + ((1 as i32) as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         l = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn baz2(mut a: *const i8) -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz3(mut i: i32) -> i32 {
// CHECK-NEXT:     if !(i != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut bar: *mut *mut () = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: &(), mut b: i64, mut c: i32) -> i32 {
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     let mut f: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut g: __SlateAlign16<[i8; 256]> = __SlateAlign16([0 as i8; 256]);
// CHECK-NEXT:     let mut h: *mut *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (g.0.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } = (10 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (g.0.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while baz1(a) < b {
// CHECK-NEXT:         '[[SLATE_CONTINUE_0:__slate_continue_[0-9]+]]: {
// CHECK-NEXT:             if ((unsafe { *unsafe { (g.0.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) } })
// CHECK-NEXT:                 as i32)
// CHECK-NEXT:                 != (32 as i32)
// CHECK-NEXT:                 && ((unsafe {
// CHECK-NEXT:                     *unsafe { (g.0.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) }
// CHECK-NEXT:                 }) as i32)
// CHECK-NEXT:                     != (9 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 f = 1 as i32;
// CHECK-NEXT:                 e = 0 as i32;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:                 if !(d != (0 as i32)) {
// CHECK-NEXT:                     [[V_3]] = baz2(g.0.as_mut_ptr() as *const i8) == (0 as i32);
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     [[V_3]] = false as bool;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if [[V_3]] {
// CHECK-NEXT:                     if c & (16 as i32) == (0 as i32) {
// CHECK-NEXT:                         break '[[SLATE_CONTINUE_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     d = 1 as i32;
// CHECK-NEXT:                     e = 1 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !(c & (16 as i32) != (0 as i32)
// CHECK-NEXT:                     && c & (16384 as i32) != (0 as i32)
// CHECK-NEXT:                     && e != (0 as i32))
// CHECK-NEXT:                     && c & (2 as i32) != (0 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:                     if c & (8192 as i32) != (0 as i32) {
// CHECK-NEXT:                         [[V_4]] = baz2(g.0.as_mut_ptr() as *const i8) == (0 as i32);
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         [[V_4]] = false as bool;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if [[V_4]] {
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:                         if c & (5128 as i32) != (0 as i32) {
// CHECK-NEXT:                             [[V_5]] = baz2(g.0.as_mut_ptr() as *const i8) == (0 as i32);
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             [[V_5]] = false as bool;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         if [[V_5]] {
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:                             if c & (512 as i32) != (0 as i32) {
// CHECK-NEXT:                                 [[V_6]] = baz2(g.0.as_mut_ptr() as *const i8) == (0 as i32);
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 [[V_6]] = false as bool;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if [[V_6]] {
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if c & (128 as i32) != (0 as i32) {
// CHECK-NEXT:                                     h = unsafe { bar };
// CHECK-NEXT:                                     i = 0 as i32;
// CHECK-NEXT:                                     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while h != std::ptr::null_mut::<*mut ()>() {
// CHECK-NEXT:                                         if baz3(i) != (0 as i32) {
// CHECK-NEXT:                                             break '[[SLATE_BREAK_1]];
// CHECK-NEXT:                                         }
// CHECK-NEXT:                                         h = (unsafe { *h }) as *mut *mut ();
// CHECK-NEXT:                                         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                                         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:                                         i = [[V_8]];
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 f = 0 as i32;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut n: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         bar = std::ptr::addr_of_mut!(n);
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(n) as *mut ())) },
// CHECK-NEXT:         (1 as i32) as i64,
// CHECK-NEXT:         51217 as i32,
// CHECK-NEXT:     );
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
