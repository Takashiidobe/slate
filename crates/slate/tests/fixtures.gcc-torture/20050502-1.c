/* PR rtl-optimization/21330 */

extern void abort(void);
extern int  strcmp(const char *, const char *);

int __attribute__((noinline)) bar(const char **x) { return *(*x)++; }

int __attribute__((noinline)) baz(int c) { return c != '@'; }

void __attribute__((noinline)) foo(const char **w, char *x, _Bool y, _Bool z) {
  char c = bar(w);
  int  i = 0;

  while (1) {
    x[i++] = c;
    c      = bar(w);
    if (y && c == '\'')
      break;
    if (z && c == '\"')
      break;
    if (!y && !z && !baz(c))
      break;
  }
  x[i] = 0;
}

int main(void) {
  char        buf[64];
  const char *p;
  p = "abcde'fgh";
  foo(&p, buf, 1, 0);
  if (strcmp(p, "fgh") != 0 || strcmp(buf, "abcde") != 0)
    abort();
  p = "ABCDEFG\"HI";
  foo(&p, buf, 0, 1);
  if (strcmp(p, "HI") != 0 || strcmp(buf, "ABCDEFG") != 0)
    abort();
  p = "abcd\"e'fgh";
  foo(&p, buf, 1, 1);
  if (strcmp(p, "e'fgh") != 0 || strcmp(buf, "abcd") != 0)
    abort();
  p = "ABCDEF'G\"HI";
  foo(&p, buf, 1, 1);
  if (strcmp(p, "G\"HI") != 0 || strcmp(buf, "ABCDEF") != 0)
    abort();
  p = "abcdef@gh";
  foo(&p, buf, 0, 0);
  if (strcmp(p, "gh") != 0 || strcmp(buf, "abcdef") != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/21330
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn strcmp([[V_0:__v[0-9]+]]: *const i8, [[V_1:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &mut *const i8) -> i32 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *const i8 = unsafe { *x };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *const i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *x = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *[[V_2]] }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut c: i32) -> i32 {
// CHECK-NEXT:     (c != (64 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut w: &mut *const i8, mut x: *mut i8, mut y: bool, mut z: bool) {
// CHECK-NEXT:     let mut c: i8 = bar(w) as i8;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { x.offset([[V_4]] as isize) } = c;
// CHECK-NEXT:         }
// CHECK-NEXT:         c = bar(w) as i8;
// CHECK-NEXT:         if y && (c as i32) == (39 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if z && (c as i32) == (34 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:         if !y && !z {
// CHECK-NEXT:             [[V_6]] = !(baz(c as i32) != (0 as i32));
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_6]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_6]] {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset(i as isize) } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16([0 as i8; 64]);
// CHECK-NEXT:     let mut p: *const i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p = c"abcde'fgh".as_ptr() as *const i8;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:         (0 as i32) != (0 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { strcmp(p, c"fgh".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 buf.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 c"abcde".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_7]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     p = c"ABCDEFG\"HI".as_ptr() as *const i8;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         (0 as i32) != (0 as i32),
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { strcmp(p, c"HI".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         [[V_8]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_8]] = (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 buf.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 c"ABCDEFG".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_8]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     p = c"abcd\"e'fgh".as_ptr() as *const i8;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { strcmp(p, c"e'fgh".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         [[V_9]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 buf.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 c"abcd".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_9]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     p = c"ABCDEF'G\"HI".as_ptr() as *const i8;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:         (1 as i32) != (0 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { strcmp(p, c"G\"HI".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 buf.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 c"ABCDEF".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     p = c"abcdef@gh".as_ptr() as *const i8;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(p)) },
// CHECK-NEXT:         buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         (0 as i32) != (0 as i32),
// CHECK-NEXT:         (0 as i32) != (0 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe { strcmp(p, c"gh".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         [[V_11]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_11]] = (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 buf.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 c"abcdef".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_11]] {
// CHECK-NEXT:         unsafe { abort() };
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
