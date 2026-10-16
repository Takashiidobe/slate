/* { dg-require-effective-target label_values } */

void abort(void);
void exit(int);

int expect_do1 = 1, expect_do2 = 2;

static int doit(int x) {
  __label__ lbl1;
  __label__ lbl2;
  static int   jtab_init = 0;
  static void *jtab[2];

  if (!jtab_init) {
    jtab[0]   = &&lbl1;
    jtab[1]   = &&lbl2;
    jtab_init = 1;
  }
  goto *jtab[x];
lbl1:
  return 1;
lbl2:
  return 2;
}

static void do1(void) {
  if (doit(0) != expect_do1)
    abort();
}

static void do2(void) {
  if (doit(1) != expect_do2)
    abort();
}

int main(void) { exit(0); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target label_values }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut expect_do1: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut expect_do2: i32 = 2 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn doit(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 4usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 2 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 1 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(jtab.0) as *mut *mut () }
// CHECK-NEXT:                                 .offset(x as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         6 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         7 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(jtab.0) as *mut *mut () }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         } = 6usize as *mut ();
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(jtab.0) as *mut *mut () }
// CHECK-NEXT:                                 .offset((1 as i32) as isize)
// CHECK-NEXT:                         } = 7usize as *mut ();
// CHECK-NEXT:                     }
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         jtab_init = 1 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     continue '__slate_dispatch;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     if !((unsafe { jtab_init }) != (0 as i32)) {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 2usize;
// CHECK-NEXT:                         continue '__slate_dispatch;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut jtab_init: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut jtab: __SlateAlign16<[*mut (); 2]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn do1() {
// CHECK-NEXT:     if doit(0 as i32) != unsafe { expect_do1 } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn do2() {
// CHECK-NEXT:     if doit(1 as i32) != unsafe { expect_do2 } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
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
