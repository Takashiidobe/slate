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

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn walk() -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return unsafe { g };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     if (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 3;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 0;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         e = 0 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 1;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         d = (((unsafe { d }) as i32) + (1 as i32)) as i16;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if (unsafe { h }) != (0 as i16) {
// CHECK-NEXT:                         __slate_state = 9;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 2;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         e = 0 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 1;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     if (unsafe { f }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 15;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 4;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         f = (unsafe { a }) as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 5;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 7 => {
// CHECK-NEXT:                     if (unsafe { g }) <= (32 as i32) {
// CHECK-NEXT:                         __slate_state = 14;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 6;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 8 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         g = (unsafe { g }) + (1 as i32);
// CHECK-NEXT:                     }
// CHECK-NEXT:                     __slate_state = 7;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 9 => {
// CHECK-NEXT:                     if (unsafe { j }) != (0 as i32) {
// CHECK-NEXT:                         __slate_state = 6;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 8;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 10 => {
// CHECK-NEXT:                     if i < (3 as i32) {
// CHECK-NEXT:                         __slate_state = 12;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 9;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 11 => {
// CHECK-NEXT:                     i += 1 as i32;
// CHECK-NEXT:                     __slate_state = 10;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 12 => {
// CHECK-NEXT:                     if (1 as i32) > ((unsafe { d }) as i32) {
// CHECK-NEXT:                         __slate_state = 13;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 11;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 13 => {
// CHECK-NEXT:                     if (unsafe { c[((unsafe { b }) as i32) as usize] }) != (0 as i8) {
// CHECK-NEXT:                         __slate_state = 11;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 12;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 14 => {
// CHECK-NEXT:                     i = 0 as i32;
// CHECK-NEXT:                     __slate_state = 10;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 15 => {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         g = 0 as i32;
// CHECK-NEXT:                     }
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
