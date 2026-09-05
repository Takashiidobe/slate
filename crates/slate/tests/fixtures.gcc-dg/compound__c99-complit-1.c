/* Test for compound literals: in C99 only.  Test for valid uses.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do run } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

struct s { int a; int b; };
union u { int c; int d; };

int *i0a = &(int) { 0 };
int *i0b = &(int) { 0 };
int *i1a = &(int) { 1 };
int *i1b = &(int) { 1 };
const int *i0c = &(const int) { 0 };

struct s *s0 = &(struct s) { 1, 2 };
struct s *s1 = &(struct s) { 1, 2 };
const struct s *s2 = &(const struct s) { 1, 2 };

union u *u0 = &(union u) { 3 };
union u *u1 = &(union u) { 3 };
const union u *u2 = &(const union u) { 3 };

int *a0 = (int []) { 1, 2, 3 };
const int *a1 = (const int []) { 1, 2, 3 };

char *p = (char []){ "foo" };

// @lowering-fn-begin
// @rewrite-fn-begin
int
main (void)
{
  if (i0a == i0b || i0a == i0c || i0b == i0c)
    abort ();
  if (i1a == i1b)
    abort ();
  if (*i0a != 0 || *i0b != 0 || *i1a != 1 || *i1b != 1 || *i0c != 0)
    abort ();
  *i0a = 1;
  *i1a = 0;
  if (*i0a != 1 || *i0b != 0 || *i1a != 0 || *i1b != 1 || *i0c != 0)
    abort ();
  if (s0 == s1 || s1 == s2 || s2 == s0)
    abort ();
  if (s0->a != 1 || s0->b != 2 || s1->a != 1 || s1->b != 2
      || s2->a != 1 || s2->b != 2)
    abort ();
  s0->a = 2;
  s1->b = 1;
  if (s0->a != 2 || s0->b != 2 || s1->a != 1 || s1->b != 1
      || s2->a != 1 || s2->b != 2)
    abort ();
  if (u0 == u1 || u1 == u2 || u2 == u0)
    abort ();
  if (u0->c != 3 || u1->c != 3 || u2->c != 3)
    abort ();
  u0->d = 2;
  if (u0->d != 2 || u1->c != 3 || u2->c != 3)
    abort ();
  if (a0 == a1)
    abort ();
  if (a0[0] != 1 || a0[1] != 2 || a0[2] != 3
      || a1[0] != 1 || a1[1] != 2 || a1[2] != 3)
    abort ();
  a0[0] = 3;
  if (a0[0] != 3 || a0[1] != 2 || a0[2] != 3
      || a1[0] != 1 || a1[1] != 2 || a1[2] != 3)
    abort ();
  if (p[0] != 'f' || p[1] != 'o' || p[2] != 'o' || p[3] != 0)
    abort ();
  p[0] = 'g';
  if (p[0] != 'g' || p[1] != 'o' || p[2] != 'o' || p[3] != 0)
    abort ();
  if (sizeof((int []) { 1, 2 ,3 }) != 3 * sizeof(int))
    abort ();
  if (sizeof((int []) { [3] = 4 }) != 4 * sizeof(int))
    abort ();
  struct s *y;
  for (int i = 0; i < 3; i++) {
    struct s *x = &(struct s) { 1, i };
    if (x->a != 1 || x->b != i)
      abort ();
    x->a++;
    x->b--;
    if (x->a != 2 || x->b != i - 1)
      abort ();
    if (i && y != x)
      abort ();
    y = x;
  }
  int *z;
  for (int i = 0; i < 4; i++) {
    int *x = (int []){ 0, i, i + 2, i - 3 };
    if (x[0] != 0 || x[1] != i || x[2] != i + 2 || x[3] != i - 3)
      abort ();
    x[0] = x[1];
    x[1] *= x[2];
    x[2] -= x[3];
    x[3] += 7;
    if (x[0] != i || x[1] != i * (i + 2) || x[2] != 5 || x[3] != i + 4)
      abort ();
    if (i && z != x)
      abort ();
    z = x;
  }
  (int) { 0 } = 1;
  (struct s) { 0, 1 }.a = 3;
  (union u) { 3 }.c = 4;
  (int []){ 1, 2 }[0] = 0;
  exit (0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut y: *mut s = std::ptr::null_mut();
// LOWERING-DAG:     let mut z: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut _compoundliteral2: s = s { a: 0, b: 0 };
// LOWERING-DAG:     let mut _compoundliteral3: u = unsafe { std::mem::zeroed::<u>() };
// LOWERING-DAG:     let mut _compoundliteral4: [i32; 2] = [0; 2];
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i1b };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i1b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i1b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*{{__v[0-9]+}}).a = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*{{__v[0-9]+}}).b = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*{{__v[0-9]+}}).d = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).d };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 102;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i8 = 103;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 103;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 111;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 12;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 3;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 16;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 4;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let mut x: *mut s = std::ptr::null_mut();
// LOWERING-DAG:                 let mut _compoundliteral5: s = s { a: 0, b: 0 };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 _compoundliteral5.a = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 _compoundliteral5.b = {{__v[0-9]+}};
// LOWERING-DAG:                 x = std::ptr::addr_of_mut!(_compoundliteral5);
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     (*{{__v[0-9]+}}).a = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - 1;
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     (*{{__v[0-9]+}}).b = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut s = y;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut s = x;
// LOWERING-DAG:                 y = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let mut i2: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i2 = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let mut x2: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:                 let mut _compoundliteral6: [i32; 4] = [0; 4];
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = _compoundliteral6.as_mut_ptr() as *mut i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = _compoundliteral6.as_mut_ptr() as *mut i32;
// LOWERING-DAG:                 x2 = {{__v[0-9]+}};
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 7;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                 unsafe {
// LOWERING-DAG:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = z;
// LOWERING-DAG:                         let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i32 = x2;
// LOWERING-DAG:                 z = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i2 = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     _compoundliteral2.a = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     _compoundliteral2.b = {{__v[0-9]+}};
// LOWERING-DAG:     _compoundliteral2.a = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         _compoundliteral3.c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         _compoundliteral3.c = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = _compoundliteral4.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     _compoundliteral4[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut y: *mut s = std::ptr::null_mut();
// REWRITES-DAG:     let mut z: *mut i32 = std::ptr::null_mut();
// REWRITES-DAG:     let mut _compoundliteral2: s = s { a: 0, b: 0 };
// REWRITES-DAG:     let mut _compoundliteral3: u = unsafe { std::mem::zeroed::<u>() };
// REWRITES-DAG:     let mut _compoundliteral4: [i32; 2] = [0; 2];
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i1b };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i1b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { i0a };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i1a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i1b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { i0c };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*{{__v[0-9]+}}).a = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*{{__v[0-9]+}}).b = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = unsafe { s2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*{{__v[0-9]+}}).d = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u = unsafe { u0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).d };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut u = unsafe { u2 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).c };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { a1 };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 102;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 111;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 111;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = 103;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 103;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 111;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 111;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { p };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 12;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 16;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     i = {{__v[0-9]+}};
// REWRITES-DAG:     loop {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// REWRITES-DAG:         if !{{__v[0-9]+}} {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:         let mut x: *mut s = std::ptr::null_mut();
// REWRITES-DAG:         let mut _compoundliteral5: s = s { a: 0, b: 0 };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         _compoundliteral5.a = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         _compoundliteral5.b = {{__v[0-9]+}};
// REWRITES-DAG:         x = std::ptr::addr_of_mut!(_compoundliteral5);
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             (*{{__v[0-9]+}}).a = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - 1;
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             (*{{__v[0-9]+}}).b = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).a };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*{{__v[0-9]+}}).b };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut s = y;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut s = x;
// REWRITES-DAG:         y = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// REWRITES-DAG:         i = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let mut i2: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     i2 = {{__v[0-9]+}};
// REWRITES-DAG:     loop {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// REWRITES-DAG:         if !{{__v[0-9]+}} {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:         let mut x2: *mut i32 = std::ptr::null_mut();
// REWRITES-DAG:         let mut _compoundliteral6: [i32; 4] = [0; 4];
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = _compoundliteral6.as_mut_ptr() as *mut i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = _compoundliteral6.as_mut_ptr() as *mut i32;
// REWRITES-DAG:         x2 = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 7;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 5;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = z;
// REWRITES-DAG:             let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i32 = x2;
// REWRITES-DAG:         z = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = i2;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// REWRITES-DAG:         i2 = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     _compoundliteral2.a = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     _compoundliteral2.b = {{__v[0-9]+}};
// REWRITES-DAG:     _compoundliteral2.a = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         _compoundliteral3.c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         _compoundliteral3.c = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = _compoundliteral4.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     _compoundliteral4[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
