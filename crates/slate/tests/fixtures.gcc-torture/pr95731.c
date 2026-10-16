/* PR tree-optimization/95731 */

__attribute__((noipa)) int foo(int x, int y, int z, int w, long long u,
                               long long v) {
  return x >= 0 && y >= 0 && z < 0 && u < 0 && w >= 0 && v < 0;
}

__attribute__((noipa)) int bar(int x, int y, int z, int w, long long u,
                               long long v) {
  return u >= 0 && x >= 0 && y >= 0 && v < 0 && z >= 0 && w >= 0;
}

__attribute__((noipa)) int baz(int x, int y, int z, int w, long long u,
                               long long v) {
  return x >= 0 || u < 0 || y >= 0 || v < 0 || z >= 0 || w >= 0;
}

int main() {
  int i;
  for (i = 0; i < 64; i++) {
    int a =
        foo((i & 1) ? -123 : 456, (i & 2) ? -123 : 456, (i & 4) ? -123 : 456,
            (i & 8) ? -123 : 456, (i & 16) ? -123 : 456, (i & 32) ? -123 : 456);
    int b =
        bar((i & 1) ? -123 : 456, (i & 2) ? -123 : 456, (i & 4) ? -123 : 456,
            (i & 8) ? -123 : 456, (i & 16) ? -123 : 456, (i & 32) ? -123 : 456);
    int c =
        baz((i & 1) ? -123 : 456, (i & 2) ? -123 : 456, (i & 4) ? -123 : 456,
            (i & 8) ? -123 : 456, (i & 16) ? -123 : 456, (i & 32) ? -123 : 456);
    if (a != (i == 52) || b != (i == 32) || c != (i != 15))
      __builtin_abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/95731
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: i32, mut w: i32, mut u: i64, mut v: i64) -> i32 {
// CHECK-NEXT:     (x >= (0 as i32)
// CHECK-NEXT:         && y >= (0 as i32)
// CHECK-NEXT:         && z < (0 as i32)
// CHECK-NEXT:         && u < ((0 as i32) as i64)
// CHECK-NEXT:         && w >= (0 as i32)
// CHECK-NEXT:         && v < ((0 as i32) as i64)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32, mut y: i32, mut z: i32, mut w: i32, mut u: i64, mut v: i64) -> i32 {
// CHECK-NEXT:     (u >= ((0 as i32) as i64)
// CHECK-NEXT:         && x >= (0 as i32)
// CHECK-NEXT:         && y >= (0 as i32)
// CHECK-NEXT:         && v < ((0 as i32) as i64)
// CHECK-NEXT:         && z >= (0 as i32)
// CHECK-NEXT:         && w >= (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: i32, mut y: i32, mut z: i32, mut w: i32, mut u: i64, mut v: i64) -> i32 {
// CHECK-NEXT:     (x >= (0 as i32)
// CHECK-NEXT:         || u < ((0 as i32) as i64)
// CHECK-NEXT:         || y >= (0 as i32)
// CHECK-NEXT:         || v < ((0 as i32) as i64)
// CHECK-NEXT:         || z >= (0 as i32)
// CHECK-NEXT:         || w >= (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         let mut a: i32 = foo(
// CHECK-NEXT:             if i & (1 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (2 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (4 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (8 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             (if i & (16 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:             (if i & (32 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut b: i32 = bar(
// CHECK-NEXT:             if i & (1 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (2 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (4 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (8 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             (if i & (16 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:             (if i & (32 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut c: i32 = baz(
// CHECK-NEXT:             if i & (1 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (2 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (4 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             if i & (8 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             },
// CHECK-NEXT:             (if i & (16 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:             (if i & (32 as i32) != (0 as i32) {
// CHECK-NEXT:                 -(123 as i32)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 456 as i32
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if a != ((i == (52 as i32)) as i32)
// CHECK-NEXT:             || b != ((i == (32 as i32)) as i32)
// CHECK-NEXT:             || c != ((i != (15 as i32)) as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
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
