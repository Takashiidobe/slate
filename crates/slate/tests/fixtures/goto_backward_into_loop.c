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
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return g;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     if e != 0 {
// CHECK-NEXT:                         __slate_state = 3;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 0;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     e = 0;
// CHECK-NEXT:                     __slate_state = 1;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     d = ((d as i32) + 1) as i16;
// CHECK-NEXT:                     if h != 0 {
// CHECK-NEXT:                         __slate_state = 9;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 2;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     e = 0;
// CHECK-NEXT:                     __slate_state = 1;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     if f != 0 {
// CHECK-NEXT:                         __slate_state = 15;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 4;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     f = a as i32;
// CHECK-NEXT:                     __slate_state = 5;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 7 => {
// CHECK-NEXT:                     if g <= 32 {
// CHECK-NEXT:                         __slate_state = 14;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 6;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 8 => {
// CHECK-NEXT:                     g = g + 1;
// CHECK-NEXT:                     __slate_state = 7;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 9 => {
// CHECK-NEXT:                     if j != 0 {
// CHECK-NEXT:                         __slate_state = 6;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 8;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 10 => {
// CHECK-NEXT:                     if i < 3 {
// CHECK-NEXT:                         __slate_state = 12;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 9;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 11 => {
// CHECK-NEXT:                     i += 1;
// CHECK-NEXT:                     __slate_state = 10;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 12 => {
// CHECK-NEXT:                     if 1 > (d as i32) {
// CHECK-NEXT:                         __slate_state = 13;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 11;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 13 => {
// CHECK-NEXT:                     if c[(b as i32) as usize] != 0 {
// CHECK-NEXT:                         __slate_state = 11;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 12;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 14 => {
// CHECK-NEXT:                     i = 0;
// CHECK-NEXT:                     __slate_state = 10;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 15 => {
// CHECK-NEXT:                     g = 0;
// CHECK-NEXT:                     __slate_state = 7;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
