/* Test for GNU extensions to C99 designated initializers */
/* Origin: Jakub Jelinek <jakub@redhat.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

typedef __SIZE_TYPE__ size_t;
extern int            memcmp(const void *, const void *, size_t);
extern void           abort(void);
extern void           exit(int);

int a[][2][4] = {[2 ... 4][0 ... 1][2 ... 3] = 1, [2] = 2, [2][0][2] = 3};
struct E {};
struct F {
  struct E H;
};
struct G {
  int      I;
  struct E J;
  int      K;
};
struct H {
  int      I;
  struct F J;
  int      K;
};
struct G k = {.J = {}, 1};
struct H l = {.J.H = {}, 2};
struct H m = {.J = {}, 3};
struct I {
  int J;
  int K[3];
  int L;
};
struct M {
  int      N;
  struct I O[3];
  int      P;
};
struct M n[] = {[0 ... 5].O[1 ... 2].K[0 ... 1] = 4, 5, 6, 7};
struct M o[] = {
    [0 ... 5].O = {[1 ... 2].K[0 ... 1] = 4}, [5].O[2].K[2] = 5, 6, 7};
struct M p[] = {
    [0 ... 5].O[1 ... 2].K = {[0 ... 1] = 4}, [5].O[2].K[2] = 5, 6, 7};
int q[3][3] = {[0 ... 1] = {[1 ... 2] = 23}, [1][2] = 24};
int r[1]    = {[0 ... 1 - 1] = 27};

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  int x, y, z;

  if (a[2][0][0] != 2 || a[2][0][2] != 3)
    abort();
  a[2][0][0] = 0;
  a[2][0][2] = 1;
  for (x = 0; x <= 4; x++)
    for (y = 0; y <= 1; y++)
      for (z = 0; z <= 3; z++)
        if (a[x][y][z] != (x >= 2 && z >= 2))
          abort();
  if (k.I || l.I || m.I || k.K != 1 || l.K != 2 || m.K != 3)
    abort();
  for (x = 0; x <= 5; x++) {
    if (n[x].N || n[x].O[0].J || n[x].O[0].L)
      abort();
    for (y = 0; y <= 2; y++)
      if (n[x].O[0].K[y])
        abort();
    for (y = 1; y <= 2; y++) {
      if (n[x].O[y].J)
        abort();
      if (n[x].O[y].K[0] != 4)
        abort();
      if (n[x].O[y].K[1] != 4)
        abort();
      if ((x < 5 || y < 2) && (n[x].O[y].K[2] || n[x].O[y].L))
        abort();
    }
    if (x < 5 && n[x].P)
      abort();
  }
  if (n[5].O[2].K[2] != 5 || n[5].O[2].L != 6 || n[5].P != 7)
    abort();
  if (memcmp(n, o, sizeof(n)) || sizeof(n) != sizeof(o))
    abort();
  if (memcmp(n, p, sizeof(n)) || sizeof(n) != sizeof(p))
    abort();
  if (q[0][0] || q[0][1] != 23 || q[0][2] != 23)
    abort();
  if (q[1][0] || q[1][1] != 23 || q[1][2] != 24)
    abort();
  if (q[2][0] || q[2][1] || q[2][2])
    abort();
  if (r[0] != 27)
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut x: i32 = 0;
// LOWERING-DAG:     let mut y: i32 = 0;
// LOWERING-DAG:     let mut z: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:     unsafe {
// LOWERING-X86_64-GNU-DAG:         (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:         a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:     unsafe {
// LOWERING-X86_64-GNU-DAG:         (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:         a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         x = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 y = {{__v[0-9]+}};
// LOWERING-DAG:                 loop {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:                     if !{{__v[0-9]+}} {
// LOWERING-DAG:                         break;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         z = {{__v[0-9]+}};
// LOWERING-DAG:                         loop {
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = z;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:                             if !{{__v[0-9]+}} {
// LOWERING-DAG:                                 break;
// LOWERING-DAG:                             }
// LOWERING-DAG:                             {
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = z;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-DAG:                                     (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)]
// LOWERING-AARCH64-GNU-DAG:                                     a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)]
// LOWERING-DAG:                                 };
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = z;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-DAG:                                     {{__v[0-9]+}}
// LOWERING-DAG:                                 } else {
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                                     {{__v[0-9]+}}
// LOWERING-DAG:                                 };
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                                 if {{__v[0-9]+}} {
// LOWERING-DAG:                                     unsafe { abort() };
// LOWERING-DAG:                                 }
// LOWERING-DAG:                             }
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = z;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:                             z = {{__v[0-9]+}};
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:                     y = {{__v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             x = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { k.I };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { l.I };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { m.I };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { k.K };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { l.K };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { m.K };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         x = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = unsafe { (*n)[({{__v[0-9]+}} as usize)].N };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].J };
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].L };
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     y = {{__v[0-9]+}};
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                         {
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                             let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                                 (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].K[({{__v[0-9]+}} as usize)]
// LOWERING-DAG:                             };
// LOWERING-DAG:                             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                             if {{__v[0-9]+}} {
// LOWERING-DAG:                                 unsafe { abort() };
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:                         y = {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                     y = {{__v[0-9]+}};
// LOWERING-DAG:                     loop {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:                         if !{{__v[0-9]+}} {
// LOWERING-DAG:                             break;
// LOWERING-DAG:                         }
// LOWERING-DAG:                         {
// LOWERING-DAG:                             {
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 =
// LOWERING-DAG:                                     unsafe { (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].J };
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                                 if {{__v[0-9]+}} {
// LOWERING-DAG:                                     unsafe { abort() };
// LOWERING-DAG:                                 }
// LOWERING-DAG:                             }
// LOWERING-DAG:                             {
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                                     (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].K
// LOWERING-DAG:                                         [({{__v[0-9]+}} as usize)]
// LOWERING-DAG:                                 };
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                                 if {{__v[0-9]+}} {
// LOWERING-DAG:                                     unsafe { abort() };
// LOWERING-DAG:                                 }
// LOWERING-DAG:                             }
// LOWERING-DAG:                             {
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                                     (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].K
// LOWERING-DAG:                                         [({{__v[0-9]+}} as usize)]
// LOWERING-DAG:                                 };
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                                 if {{__v[0-9]+}} {
// LOWERING-DAG:                                     unsafe { abort() };
// LOWERING-DAG:                                 }
// LOWERING-DAG:                             }
// LOWERING-DAG:                             {
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                                     {{__v[0-9]+}}
// LOWERING-DAG:                                 } else {
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:                                     {{__v[0-9]+}}
// LOWERING-DAG:                                 };
// LOWERING-DAG:                                 let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                                         (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].K
// LOWERING-DAG:                                             [({{__v[0-9]+}} as usize)]
// LOWERING-DAG:                                     };
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                                         let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                                         {{__v[0-9]+}}
// LOWERING-DAG:                                     } else {
// LOWERING-DAG:                                         let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                         let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                                         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                                             (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].L
// LOWERING-DAG:                                         };
// LOWERING-DAG:                                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                                         {{__v[0-9]+}}
// LOWERING-DAG:                                     };
// LOWERING-DAG:                                     {{__v[0-9]+}}
// LOWERING-DAG:                                 } else {
// LOWERING-DAG:                                     let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                                     {{__v[0-9]+}}
// LOWERING-DAG:                                 };
// LOWERING-DAG:                                 if {{__v[0-9]+}} {
// LOWERING-DAG:                                     unsafe { abort() };
// LOWERING-DAG:                                 }
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = y;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:                         y = {{__v[0-9]+}};
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = unsafe { (*n)[({{__v[0-9]+}} as usize)].P };
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             x = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 5;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 =
// LOWERING-DAG:             unsafe { (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].K[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 5;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*n)[({{__v[0-9]+}} as usize)].O[({{__v[0-9]+}} as usize)].L };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 6;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 5;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*n)[({{__v[0-9]+}} as usize)].P };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 7;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(n).cast::<M>();
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(o).cast::<M>();
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 408;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:             memcmp(
// LOWERING-DAG:                 {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as usize,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = 408;
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = 408;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(n).cast::<M>();
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(p).cast::<M>();
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 408;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:             memcmp(
// LOWERING-DAG:                 {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as usize,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = 408;
// LOWERING-DAG:             let {{__v[0-9]+}}: u64 = 408;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 23;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 23;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 23;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 24;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { (*q)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: i32 = unsafe { q[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { r[({{__v[0-9]+}} as usize)] };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 27;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut x: i32 = 0;
// REWRITES-DAG:     let mut y: i32 = 0;
// REWRITES-DAG:     let mut z: i32 = 0;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = (unsafe { (*a)[2][0][0] }) != 2 || (unsafe { (*a)[2][0][2] }) != 3;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = (unsafe { a[2][0][0] }) != 2 || (unsafe { a[2][0][2] }) != 3;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = 0;
// REWRITES-AARCH64-GNU-DAG:         a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = 0;
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         (*a)[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = 1;
// REWRITES-AARCH64-GNU-DAG:         a[({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)][({{__v[0-9]+}} as usize)] = 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     while x <= 4 {
// REWRITES-DAG:         y = 0;
// REWRITES-DAG:         while y <= 1 {
// REWRITES-DAG:             z = 0;
// REWRITES-DAG:             while z <= 3 {
// REWRITES-DAG:                 let {{__v[0-9]+}}: bool = (unsafe {
// REWRITES-X86_64-GNU-DAG:                     (*a)[((x as i64) as usize)][((y as i64) as usize)][((z as i64) as usize)]
// REWRITES-AARCH64-GNU-DAG:                     a[((x as i64) as usize)][((y as i64) as usize)][((z as i64) as usize)]
// REWRITES-DAG:                 }) != ((x >= 2 && z >= 2) as i32);
// REWRITES-DAG:                 if {{__v[0-9]+}} {
// REWRITES-DAG:                     unsafe { abort() };
// REWRITES-DAG:                 }
// REWRITES-DAG:                 z += 1;
// REWRITES-DAG:             }
// REWRITES-DAG:             y += 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         x += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (unsafe { k.I }) != 0
// REWRITES-DAG:         || (unsafe { l.I }) != 0
// REWRITES-DAG:         || (unsafe { m.I }) != 0
// REWRITES-DAG:         || (unsafe { k.K }) != 1
// REWRITES-DAG:         || (unsafe { l.K }) != 2
// REWRITES-DAG:         || (unsafe { m.K }) != 3;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     x = 0;
// REWRITES-DAG:     while x <= 5 {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = (unsafe { (*n)[((x as i64) as usize)].N }) != 0
// REWRITES-DAG:             || (unsafe { (*n)[((x as i64) as usize)].O[0].J }) != 0
// REWRITES-DAG:             || (unsafe { (*n)[((x as i64) as usize)].O[0].L }) != 0;
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         y = 0;
// REWRITES-DAG:         while y <= 2 {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool =
// REWRITES-DAG:                 (unsafe { (*n)[((x as i64) as usize)].O[0].K[((y as i64) as usize)] }) != 0;
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 unsafe { abort() };
// REWRITES-DAG:             }
// REWRITES-DAG:             y += 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         y = 1;
// REWRITES-DAG:         while y <= 2 {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool =
// REWRITES-DAG:                 (unsafe { (*n)[((x as i64) as usize)].O[((y as i64) as usize)].J }) != 0;
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 unsafe { abort() };
// REWRITES-DAG:             }
// REWRITES-DAG:             let {{__v[0-9]+}}: bool =
// REWRITES-DAG:                 (unsafe { (*n)[((x as i64) as usize)].O[((y as i64) as usize)].K[0] }) != 4;
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 unsafe { abort() };
// REWRITES-DAG:             }
// REWRITES-DAG:             let {{__v[0-9]+}}: bool =
// REWRITES-DAG:                 (unsafe { (*n)[((x as i64) as usize)].O[((y as i64) as usize)].K[1] }) != 4;
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 unsafe { abort() };
// REWRITES-DAG:             }
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = (x < 5 || y < 2)
// REWRITES-DAG:                 && ((unsafe { (*n)[((x as i64) as usize)].O[((y as i64) as usize)].K[2] }) != 0
// REWRITES-DAG:                     || (unsafe { (*n)[((x as i64) as usize)].O[((y as i64) as usize)].L }) != 0);
// REWRITES-DAG:             if {{__v[0-9]+}} {
// REWRITES-DAG:                 unsafe { abort() };
// REWRITES-DAG:             }
// REWRITES-DAG:             y += 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = x < 5 && (unsafe { (*n)[((x as i64) as usize)].P }) != 0;
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         x += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (unsafe { (*n)[5].O[2].K[2] }) != 5
// REWRITES-DAG:         || (unsafe { (*n)[5].O[2].L }) != 6
// REWRITES-DAG:         || (unsafe { (*n)[5].P }) != 7;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(n).cast::<M>();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(o).cast::<M>();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (408 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} != 0 {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: u64 = 408;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = 408 != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(n).cast::<M>();
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut M = std::ptr::addr_of_mut!(p).cast::<M>();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (408 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} != 0 {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: u64 = 408;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = 408 != {{__v[0-9]+}};
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = (unsafe { (*q)[0][0] }) != 0
// REWRITES-X86_64-GNU-DAG:         || (unsafe { (*q)[0][1] }) != 23
// REWRITES-X86_64-GNU-DAG:         || (unsafe { (*q)[0][2] }) != 23;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool =
// REWRITES-AARCH64-GNU-DAG:         (unsafe { q[0][0] }) != 0 || (unsafe { q[0][1] }) != 23 || (unsafe { q[0][2] }) != 23;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = (unsafe { (*q)[1][0] }) != 0
// REWRITES-X86_64-GNU-DAG:         || (unsafe { (*q)[1][1] }) != 23
// REWRITES-X86_64-GNU-DAG:         || (unsafe { (*q)[1][2] }) != 24;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool =
// REWRITES-AARCH64-GNU-DAG:         (unsafe { q[1][0] }) != 0 || (unsafe { q[1][1] }) != 23 || (unsafe { q[1][2] }) != 24;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = (unsafe { (*q)[2][0] }) != 0
// REWRITES-X86_64-GNU-DAG:         || (unsafe { (*q)[2][1] }) != 0
// REWRITES-X86_64-GNU-DAG:         || (unsafe { (*q)[2][2] }) != 0;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool =
// REWRITES-AARCH64-GNU-DAG:         (unsafe { q[2][0] }) != 0 || (unsafe { q[2][1] }) != 0 || (unsafe { q[2][2] }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (unsafe { r[0] }) != 27;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
