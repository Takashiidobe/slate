#include <stdio.h>

int limit = 3;
static int bounds;

static int bound(void) {
  bounds++;
  return 3;
}

static void take(int *pointer) {
  if (*pointer == 1) *pointer = 2;
}

// @check-fn-begin
static int rejected(int *p, int n, volatile int vn) {
  int s = 0;
  for (int a = 0; a < n; a++) {
    a += p[a];
  }
  for (int b = 0; b < n; b++) {
    take(&b);
    s += b;
  }
  int c;
  for (c = 0; c < n; c++) {
    s += p[c];
  }
  s += c;
  for (int d = 0; d < limit; d++) {
    s += p[d];
  }
  int m = n;
  for (int e = 0; e < m; e++) {
    m--;
    s += e;
  }
  for (unsigned f = 0; f <= 4294967295u; f++) {
    if (f == 3) {
      break;
    }
    s += f;
  }
  for (int g = 0; g <= n - 1; g++) {
    s += p[g];
  }
  for (volatile int h = 0; h < n; h++) {
    s += p[h];
  }
  for (int i = 0; i < vn; i++) {
    s += p[i];
  }
  for (char j = 0; j < 4; j++) {
    s += p[j];
  }
  for (int k = 0; k < n; k += 2) {
    s += p[k];
  }
  for (int l = 0; l != n; l++) {
    s += p[l];
  }
  for (int q = 0; q < bound(); q++) {
    s += p[q];
  }
  for (int r = n - 1; r > 0; r--) {
    s += p[r];
  }
  int taken = 1;
  take(&taken);
  for (int t = 0; t < taken; t++) {
    s += p[t];
  }
  for (int u = 0; u < n; u++) {
    __asm__ volatile("" ::: "memory");
    s += u;
  }
  for (int v = 0; v < v + 5; v++) {
    if (v > 3) {
      break;
    }
    s += v;
  }
  return s;
}
// @check-fn-end

// @check-fn-begin
static int jumps(int *p, int n) {
  int s = 0;
  for (int i = 0; i < n; i++) {
    if (i == 1) goto inside;
    s += 10;
  inside:
    s += 1;
  }
  return s;
}
// @check-fn-end

int main(void) {
  int values[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  printf("%d\n", rejected(values, 4, 4));
  printf("%d %d\n", jumps(values, 4), bounds);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn rejected(p: &[i32], n: i32, vn: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     let mut a: i32 = 0;
// CHECK-NEXT:     while a < n {
// CHECK-NEXT:         a += p[a as usize];
// CHECK-NEXT:         a += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b: i32 = 0;
// CHECK-NEXT:     while b < n {
// CHECK-NEXT:         take(&mut b);
// CHECK-NEXT:         s += b;
// CHECK-NEXT:         b += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: i32 = 0;
// CHECK-NEXT:     while c < n {
// CHECK-NEXT:         s += p[c as usize];
// CHECK-NEXT:         c += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     s += c;
// CHECK-NEXT:     let mut d: i32 = 0;
// CHECK-NEXT:     while d < unsafe { limit } {
// CHECK-NEXT:         s += p[d as usize];
// CHECK-NEXT:         d += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut m: i32 = n;
// CHECK-NEXT:     let mut e: i32 = 0;
// CHECK-NEXT:     while e < m {
// CHECK-NEXT:         m -= 1;
// CHECK-NEXT:         s += e;
// CHECK-NEXT:         e += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut f: u32 = 0;
// CHECK-NEXT:     while f <= 4294967295 {
// CHECK-NEXT:         if f == 3 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         s = (s as u32).wrapping_add(f) as i32;
// CHECK-NEXT:         f = f.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut g: i32 = 0;
// CHECK-NEXT:     while g <= n - 1 {
// CHECK-NEXT:         s += p[g as usize];
// CHECK-NEXT:         g += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut h: i32 = 0;
// CHECK-NEXT:     while (unsafe { core::ptr::read_volatile(core::ptr::addr_of!(h)) }) < n {
// CHECK-NEXT:         s += p[(unsafe { core::ptr::read_volatile(core::ptr::addr_of!(h)) }) as usize];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::ptr::write_volatile(
// CHECK-NEXT:                 core::ptr::addr_of_mut!(h),
// CHECK-NEXT:                 core::ptr::read_volatile(core::ptr::addr_of!(h)) + 1,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < unsafe { core::ptr::read_volatile(core::ptr::addr_of!(vn)) } {
// CHECK-NEXT:         s += p[i as usize];
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut j: i8 = 0;
// CHECK-NEXT:     while (j as i32) < 4 {
// CHECK-NEXT:         s += p[(j as i32) as usize];
// CHECK-NEXT:         j = ((j as i32) + 1) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut k: i32 = 0;
// CHECK-NEXT:     while k < n {
// CHECK-NEXT:         s += p[k as usize];
// CHECK-NEXT:         k += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut l: i32 = 0;
// CHECK-NEXT:     while l != n {
// CHECK-NEXT:         s += p[l as usize];
// CHECK-NEXT:         l += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut q: i32 = 0;
// CHECK-NEXT:     while q < bound() {
// CHECK-NEXT:         s += p[q as usize];
// CHECK-NEXT:         q += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut r: i32 = n - 1;
// CHECK-NEXT:     while r > 0 {
// CHECK-NEXT:         s += p[r as usize];
// CHECK-NEXT:         r -= 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut taken: i32 = 1;
// CHECK-NEXT:     take(&mut taken);
// CHECK-NEXT:     let mut t: i32 = 0;
// CHECK-NEXT:     while t < taken {
// CHECK-NEXT:         s += p[t as usize];
// CHECK-NEXT:         t += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     for u in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:         }
// CHECK-NEXT:         s += u;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut v: i32 = 0;
// CHECK-NEXT:     while v < v + 5 {
// CHECK-NEXT:         if v > 3 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         s += v;
// CHECK-NEXT:         v += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn jumps(p: &i32, n: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut s: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s = 0;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         while i < n {
// CHECK-NEXT:             if i != 1 {
// CHECK-NEXT:                 s += 10;
// CHECK-NEXT:             }
// CHECK-NEXT:             s += 1;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         return s;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
