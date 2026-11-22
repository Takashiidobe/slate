#include <stdio.h>

char  a;
short b, d = 5, h;
char  c[1];
int   e, f = 4, g, j;

// @check-fn-begin
static int walk(void) {
  int i;
  for (; f; f = a) {
    g = 0;
    for (; g <= 32; ++g) {
      i = 0;
      for (; i < 3; i++)
        while (1 > d)
          if (c[b])
            break;
    L:
      if (j)
        break;
    }
  }
  e = 0;
  for (; e; e = 0) {
    d++;
    for (; h;)
      goto L;
  }
  return g;
}
// @check-fn-end

int main(void) {
  printf("%d %d %d\n", walk(), f, d);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn walk() -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 5usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__loop_5: loop {
// CHECK-NEXT:             if f != 0 {
// CHECK-NEXT:                 g = 0;
// CHECK-NEXT:                 __slate_state = 7;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 e = 0;
// CHECK-NEXT:                 loop {
// CHECK-NEXT:                     if e != 0 {
// CHECK-NEXT:                         d = ((d as i32) + 1) as i16;
// CHECK-NEXT:                         if h != 0 {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             e = 0;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break '__loop_5;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 __slate_state = 9;
// CHECK-NEXT:             }
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 match __slate_state {
// CHECK-NEXT:                     7 => {
// CHECK-NEXT:                         if g <= 32 {
// CHECK-NEXT:                             for i in 0i32..3i32 {
// CHECK-NEXT:                                 while 1 > (d as i32) {
// CHECK-NEXT:                                     if c[(b as i32) as usize] != 0 {
// CHECK-NEXT:                                         break;
// CHECK-NEXT:                                     }
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                             __slate_state = 9;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         if j != 0 {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             g = g + 1;
// CHECK-NEXT:                             __slate_state = 7;
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             f = a as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { g }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
