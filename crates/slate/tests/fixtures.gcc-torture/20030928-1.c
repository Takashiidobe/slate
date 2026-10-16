#include <limits.h>

void abort(void);
void exit(int);

#if INT_MAX <= 32767
int main() { exit(0); }
#else
void get_addrs(const char **x, int *y) {
  x[0] = "a1111" + (y[0] - 0x10000) * 2;
  x[1] = "a1112" + (y[1] - 0x20000) * 2;
  x[2] = "a1113" + (y[2] - 0x30000) * 2;
  x[3] = "a1114" + (y[3] - 0x40000) * 2;
  x[4] = "a1115" + (y[4] - 0x50000) * 2;
  x[5] = "a1116" + (y[5] - 0x60000) * 2;
  x[6] = "a1117" + (y[6] - 0x70000) * 2;
  x[7] = "a1118" + (y[7] - 0x80000) * 2;
}

int main() {
  const char *x[8];
  int         y[8];
  int         i;

  for (i = 0; i < 8; i++)
    y[i] = 0x10000 * (i + 1);
  get_addrs(x, y);
  for (i = 0; i < 8; i++)
    if (*x[i] != 'a')
      abort();
  exit(0);
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn get_addrs(mut x: *mut *const i8, mut y: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((0 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1111".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((0 as i32) as isize) } })
// CHECK-NEXT:                         - (65536 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((1 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1112".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((1 as i32) as isize) } })
// CHECK-NEXT:                         - (131072 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((2 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1113".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((2 as i32) as isize) } })
// CHECK-NEXT:                         - (196608 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((3 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1114".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((3 as i32) as isize) } })
// CHECK-NEXT:                         - (262144 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((4 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1115".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((4 as i32) as isize) } })
// CHECK-NEXT:                         - (327680 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((5 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1116".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((5 as i32) as isize) } })
// CHECK-NEXT:                         - (393216 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((6 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1117".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((6 as i32) as isize) } })
// CHECK-NEXT:                         - (458752 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((7 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             (c"a1118".as_ptr() as *mut i8)
// CHECK-NEXT:                 .offset(
// CHECK-NEXT:                     (((unsafe { *unsafe { y.offset((7 as i32) as isize) } })
// CHECK-NEXT:                         - (524288 as i32)) * (2 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[*const i8; 8]> = __SlateAlign16([0 as *const i8; 8]);
// CHECK-NEXT:     let mut y: __SlateAlign16<[i32; 8]> = __SlateAlign16([0 as i32; 8]);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (y.0.as_mut_ptr() as *mut i32).offset(i as isize) } = (65536
// CHECK-NEXT:                 as i32) * (i + (1 as i32));
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     get_addrs(x.0.as_mut_ptr() as *mut *const i8, y.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 *unsafe { (x.0.as_mut_ptr() as *mut *const i8).offset(i as isize) }
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32) != (97 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
