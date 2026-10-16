
/* Origin: Kaveh Ghazi <ghazi@caip.rutgers.edu> 2002-05-27.  */

/* Use a different function for each test so the link failures
   indicate which one is broken.  */
extern void link_error0(void);
extern void link_error1(void);
extern void link_error2(void);
extern void link_error3(void);
extern void link_error4(void);
extern void link_error5(void);
extern void link_error6(void);
extern void link_error7(void);

extern int i;

extern int func0(int) __attribute__((__pure__));
extern int func1(int) __attribute__((__const__));

/* GCC should automatically detect attributes for these functions.
   At -O3 They'll be inlined, but that's ok.  */
static int func2(int a) { return i + a; }        /* pure */
static int func3(int a) { return a * 3; }        /* const */
static int func4(int a) { return func0(a) + a; } /* pure */
static int func5(int a) { return a + func1(a); } /* const */
static int func6(int a) { return func2(a) + a; } /* pure */
static int func7(int a) { return a + func3(a); } /* const */

int main() {
  int i[10], r;

  i[0] = 0;
  r    = func0(0);
  if (i[0])
    link_error0();

  i[1] = 0;
  r    = func1(0);
  if (i[1])
    link_error1();

  i[2] = 0;
  r    = func2(0);
  if (i[2])
    link_error2();

  i[3] = 0;
  r    = func3(0);
  if (i[3])
    link_error3();

  i[4] = 0;
  r    = func4(0);
  if (i[4])
    link_error4();

  i[5] = 0;
  r    = func5(0);
  if (i[5])
    link_error5();

  i[6] = 0;
  r    = func6(0);
  if (i[6])
    link_error6();

  i[7] = 0;
  r    = func7(0);
  if (i[7])
    link_error7();

  return r;
}

int func0(int a) { return a - i; } /* pure */
int func1(int a) { return a - a; } /* const */

int i = 2;

#ifndef __OPTIMIZE__
/* Avoid link failures when not optimizing. */
void link_error0() {}
void link_error1() {}
void link_error2() {}
void link_error3() {}
void link_error4() {}
void link_error5() {}
void link_error6() {}
void link_error7() {}
#endif /* ! __OPTIMIZE__ */

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: Kaveh Ghazi <ghazi@caip.rutgers.edu> 2002-05-27.
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: /// GCC should automatically detect attributes for these functions.
// CHECK-NEXT: /// At -O3 They'll be inlined, but that's ok.
// CHECK-NEXT: /// pure
// CHECK-NEXT: fn func2(mut a: i32) -> i32 {
// CHECK-NEXT:     (unsafe { i }) + a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// const
// CHECK-NEXT: fn func3(mut a: i32) -> i32 {
// CHECK-NEXT:     a * (3 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// pure
// CHECK-NEXT: fn func4(mut a: i32) -> i32 {
// CHECK-NEXT:     func0(a) + a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// const
// CHECK-NEXT: fn func5(mut a: i32) -> i32 {
// CHECK-NEXT:     a + func1(a)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// pure
// CHECK-NEXT: fn func6(mut a: i32) -> i32 {
// CHECK-NEXT:     func2(a) + a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// const
// CHECK-NEXT: fn func7(mut a: i32) -> i32 {
// CHECK-NEXT:     a + func3(a)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[I_0:i_[0-9]+]]: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func0(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func1(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func2(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((2 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error2();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func3(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error3();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func4(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((4 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error4();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func5(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((5 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error5();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func6(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((6 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error6();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = func7(0 as i32);
// CHECK-NEXT:     if (unsafe { *unsafe { ([[I_0]].0.as_mut_ptr() as *mut i32).offset((7 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         link_error7();
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// pure
// CHECK-NEXT: fn func0(mut a: i32) -> i32 {
// CHECK-NEXT:     a - unsafe { i }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// const
// CHECK-NEXT: fn func1(mut a: i32) -> i32 {
// CHECK-NEXT:     a - a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = 2 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: /// Avoid link failures when not optimizing.
// CHECK-NEXT: fn link_error0() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error1() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error2() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error3() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error4() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error5() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error6() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error7() {}
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
