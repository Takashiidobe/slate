/* Test builtin-memcpy (which may emit different code for different N).  */
#include <string.h>

void abort(void);

#define TESTSIZE 80

char src[TESTSIZE] __attribute__((aligned));
char dst[TESTSIZE] __attribute__((aligned));

void check(char *test, char *match, int n) {
  if (memcmp(test, match, n))
    abort();
}

#define TN(n)                                                                  \
  {                                                                            \
    memset(dst, 0, n);                                                         \
    memcpy(dst, src, n);                                                       \
    check(dst, src, n);                                                        \
  }
#define T(n)                                                                   \
  TN(n)                                                                        \
  TN((n) + 1)                                                                  \
  TN((n) + 2)                                                                  \
  TN((n) + 3)

int main(void) {
  int i, j;

  for (i = 0; i < sizeof(src); ++i)
    src[i] = 'a' + i % 26;

  T(0);
  T(4);
  T(8);
  T(12);
  T(16);
  T(20);
  T(24);
  T(28);
  T(32);
  T(36);
  T(40);
  T(44);
  T(48);
  T(52);
  T(56);
  T(60);
  T(64);
  T(68);
  T(72);
  T(76);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test builtin-memcpy (which may emit different code for different N).
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn memcmp(__s1: *const (), __s2: *const (), __n: u64) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut src: __SlateAlign16<[i8; 80]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut dst: __SlateAlign16<[i8; 80]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn check(mut test: *mut i8, mut r#match: *mut i8, mut n: i32) {
// CHECK-NEXT:     if (unsafe { memcmp(test as *const (), r#match as *const (), (n as i64) as u64) }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (80 as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }.offset(i as isize) } =
// CHECK-NEXT:                 ((97 as i32) + i % (26 as i32)) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((0 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((0 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((0 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((0 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (0 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((0 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((0 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (0 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((0 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((0 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (0 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((4 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((4 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (4 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((4 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((4 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (4 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((4 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((4 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (4 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((8 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((8 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (8 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((8 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((8 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (8 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((8 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((8 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (8 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((12 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((12 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         12 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((12 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((12 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (12 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((12 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((12 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (12 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((12 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((12 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (12 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((16 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((16 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         16 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((16 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((16 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (16 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((16 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((16 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (16 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((16 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((16 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (16 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((20 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((20 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         20 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((20 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((20 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (20 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((20 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((20 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (20 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((20 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((20 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (20 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((24 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((24 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         24 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((24 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((24 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (24 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((24 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((24 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (24 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((24 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((24 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (24 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((28 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((28 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         28 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((28 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((28 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (28 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((28 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((28 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (28 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((28 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((28 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (28 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((32 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((32 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         32 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((32 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((32 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (32 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((32 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((32 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (32 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((32 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((32 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (32 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((36 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((36 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         36 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((36 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((36 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (36 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((36 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((36 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (36 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((36 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((36 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (36 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((40 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((40 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         40 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((40 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((40 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (40 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((40 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((40 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (40 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((40 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((40 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (40 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((44 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((44 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         44 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((44 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((44 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (44 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((44 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((44 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (44 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((44 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((44 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (44 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((48 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((48 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         48 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((48 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((48 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (48 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((48 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((48 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (48 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((48 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((48 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (48 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((52 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((52 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         52 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((52 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((52 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (52 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((52 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((52 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (52 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((52 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((52 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (52 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((56 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((56 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         56 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((56 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((56 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (56 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((56 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((56 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (56 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((56 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((56 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (56 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((60 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((60 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         60 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((60 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((60 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (60 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((60 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((60 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (60 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((60 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((60 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (60 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((64 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((64 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         64 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((64 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((64 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (64 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((64 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((64 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (64 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((64 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((64 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (64 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((68 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((68 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         68 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((68 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((68 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (68 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((68 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((68 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (68 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((68 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((68 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (68 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((72 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((72 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         72 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((72 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((72 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (72 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((72 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((72 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (72 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((72 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((72 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (72 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((76 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((76 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         76 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((76 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((76 as i32) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (76 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((76 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((76 as i32) + (2 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (76 as i32) + (2 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((76 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 }) as *const (),
// CHECK-NEXT:             (((76 as i32) + (3 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     check(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(dst.0) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(src.0) as *mut i8 },
// CHECK-NEXT:         (76 as i32) + (3 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     {}
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
