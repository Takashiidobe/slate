/* PR target/11087
   This testcase was miscompiled on ppc64, because basic_induction_var called
   convert_modes, yet did not expect it to emit any new instructions.
   Those were emitted at the end of the function and destroyed during life
   analysis, while the program used uninitialized pseudos created by
   convert_modes.  */

struct A {
  unsigned short a1;
  unsigned long  a2;
};

struct B {
  int b1, b2, b3, b4, b5;
};

struct C {
  struct B c1[1];
  int      c2, c3;
};

static int foo(int x) { return x < 0 ? -x : x; }

int bar(struct C *x, struct A *y) {
  int                 a = x->c3;
  const int           b = y->a1 >> 9;
  const unsigned long c = y->a2;
  int                 d = a;
  unsigned long       e, f;

  f = foo(c - x->c1[d].b4);
  do {
    if (d <= 0)
      d = x->c2;
    d--;

    e = foo(c - x->c1[d].b4);
    if (e < f)
      a = d;
  } while (d != x->c3);
  x->c1[a].b4 = c + b;
  return a;
}

int main() {
  struct A a;
  struct C b;
  int      c;

  a.a1 = 512;
  a.a2 = 4242;
  __builtin_memset(&b, 0, sizeof(b));
  b.c1[0].b3 = 424242;
  b.c2       = 1;
  c          = bar(&b, &a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/11087
// CHECK-NEXT: //! This testcase was miscompiled on ppc64, because basic_induction_var called
// CHECK-NEXT: //! convert_modes, yet did not expect it to emit any new instructions.
// CHECK-NEXT: //! Those were emitted at the end of the function and destroyed during life
// CHECK-NEXT: //! analysis, while the program used uninitialized pseudos created by
// CHECK-NEXT: //! convert_modes.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     a1: u16,
// CHECK-NEXT:     a2: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     b1: i32,
// CHECK-NEXT:     b2: i32,
// CHECK-NEXT:     b3: i32,
// CHECK-NEXT:     b4: i32,
// CHECK-NEXT:     b5: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C {
// CHECK-NEXT:     c1: [B; 1],
// CHECK-NEXT:     c2: i32,
// CHECK-NEXT:     c3: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) -> i32 {
// CHECK-NEXT:     if x < (0 as i32) { -x } else { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: &mut C, mut y: &A) -> i32 {
// CHECK-NEXT:     let mut a: i32 = unsafe { (*(x as *mut C)).c3 };
// CHECK-NEXT:     let mut b: i32 = (((unsafe { (*(y as *const A)).a1 }) as u32) as i32) >> (9 as i32);
// CHECK-NEXT:     let mut c: u64 = unsafe { (*(y as *const A)).a2 };
// CHECK-NEXT:     let mut d: i32 = a;
// CHECK-NEXT:     let mut e: u64 = 0 as u64;
// CHECK-NEXT:     let mut f: u64 = (foo((c.wrapping_sub(
// CHECK-NEXT:         ((unsafe {
// CHECK-NEXT:             (*unsafe { unsafe { (*(x as *mut C)).c1.as_mut_ptr() as *mut B }.offset(d as isize) })
// CHECK-NEXT:                 .b4
// CHECK-NEXT:         }) as i64) as u64,
// CHECK-NEXT:     ) as u32) as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         if d <= (0 as i32) {
// CHECK-NEXT:             d = unsafe { (*(x as *mut C)).c2 };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = d;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] - (1 as i32);
// CHECK-NEXT:         d = [[V_4]];
// CHECK-NEXT:         e = (foo((c.wrapping_sub(
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*(x as *mut C)).c1.as_mut_ptr() as *mut B }.offset(d as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .b4
// CHECK-NEXT:             }) as i64) as u64,
// CHECK-NEXT:         ) as u32) as i32) as i64) as u64;
// CHECK-NEXT:         if e < f {
// CHECK-NEXT:             a = d;
// CHECK-NEXT:         }
// CHECK-NEXT:         if !(d != unsafe { (*(x as *mut C)).c3 }) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { (*(x as *mut C)).c1.as_mut_ptr() as *mut B }.offset(a as isize) }).b4 =
// CHECK-NEXT:             (c.wrapping_add((b as i64) as u64) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: C = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     a.a1 = ((512 as i32) as i16) as u16;
// CHECK-NEXT:     a.a2 = ((4242 as i32) as i64) as u64;
// CHECK-NEXT:     unsafe { __builtin_memset(std::ptr::addr_of_mut!(b) as *mut (), 0 as i32, 28 as u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (b.c1.as_mut_ptr() as *mut B).offset((0 as i32) as isize) }).b3 = 424242 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b.c2 = 1 as i32;
// CHECK-NEXT:     c = bar(unsafe { &mut (*std::ptr::addr_of_mut!(b)) }, unsafe {
// CHECK-NEXT:         &(*std::ptr::addr_of_mut!(a))
// CHECK-NEXT:     });
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
