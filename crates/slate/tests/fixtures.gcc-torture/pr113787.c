void foo(int x, int y, int z, int d, int *buf) {
  for (int i = z; i < y - z; ++i)
    for (int j = 0; j < d; ++j)
      /* buf[x(i+1) + j] = buf[x(i+1)-j-1] */
      buf[i * x + (x - z + j)] = buf[i * x + (x - z - 1 - j)];
}

void bar(int x, int y, int z, int d, int *buf) {
  for (int i = 0; i < d; ++i)
    for (int j = z; j < x - z; ++j)
      /* buf[j+(y+i)*x] = buf[j+(y-1-i)*x] */
      buf[j + (y - z + i) * x] = buf[j + (y - z - 1 - i) * x];
}

__attribute__((noipa)) void baz(int x, int y, int d, int *buf) {
  foo(x, y, 0, d, buf);
  bar(x, y, 0, d, buf);
}

int main(void) {
  int a[] = {1, 2, 3};
  baz(1, 2, 1, a);
  /* foo does:
     buf[1] = buf[0];
     buf[2] = buf[1];

     bar does:
     buf[2] = buf[1]; (no-op)
     so we should have { 1, 1, 1 }.  */
  for (int i = 0; i < 3; i++)
    if (a[i] != 1)
      __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: i32, mut d: i32, mut buf: *mut i32) {
// CHECK-NEXT:     let mut i: i32 = z;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < y - z {
// CHECK-NEXT:         let mut j: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < d {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { buf.offset((i * x + (x - z + j)) as isize) } =
// CHECK-NEXT:                     unsafe { *unsafe { buf.offset((i * x + (x - z - (1 as i32) - j)) as isize) } };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // buf[x(i+1) + j] = buf[x(i+1)-j-1]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32, mut y: i32, mut z: i32, mut d: i32, mut buf: *mut i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < d {
// CHECK-NEXT:         let mut j: i32 = z;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while j < x - z {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { buf.offset((j + (y - z + i) * x) as isize) } =
// CHECK-NEXT:                     unsafe { *unsafe { buf.offset((j + (y - z - (1 as i32) - i) * x) as isize) } };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             j = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // buf[j+(y+i)*x] = buf[j+(y-1-i)*x]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: i32, mut y: i32, mut d: i32, mut buf: *mut i32) {
// CHECK-NEXT:     foo(x, y, 0 as i32, d, buf);
// CHECK-NEXT:     bar(x, y, 0 as i32, d, buf);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [i32; 3] = [1 as i32, 2 as i32, 3 as i32];
// CHECK-NEXT:     baz(1 as i32, 2 as i32, 1 as i32, a.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     // foo does:
// CHECK-NEXT:     // buf[1] = buf[0];
// CHECK-NEXT:     // buf[2] = buf[1];
// CHECK-NEXT:     //
// CHECK-NEXT:     // bar does:
// CHECK-NEXT:     // buf[2] = buf[1]; (no-op)
// CHECK-NEXT:     // so we should have { 1, 1, 1 }.
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < (3 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (a.as_mut_ptr() as *mut i32).offset(i as isize) } }) != (1 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         i = [[V_9]];
// CHECK-NEXT:     }
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
