/* PR tree-optimization/19828 */
typedef __SIZE_TYPE__ size_t;
extern size_t         strlen(const char *s);
extern int            strncmp(const char *s1, const char *s2, size_t n);
extern void           abort(void);

const char *a[16] = {"a", "bc", "de", "fgh"};

int foo(char *x, const char *y, size_t n) {
  size_t i, j = 0;
  for (i = 0; i < n; i++) {
    if (strncmp(x + j, a[i], strlen(a[i])) != 0)
      return 2;
    j += strlen(a[i]);
    if (y)
      j += strlen(y);
  }
  return 0;
}

int main(void) {
  if (foo("abcde", (const char *)0, 3) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strlen(s: *const i8) -> u64;
// CHECK-NEXT:     fn strncmp(s1: *const i8, s2: *const i8, n: u64) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // PR tree-optimization/19828
// CHECK-NEXT: static mut a: __SlateAlign16<[*const i8; 16]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [*const i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (b"a\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]][1] = (b"bc\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]][2] = (b"de\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]][3] = (b"fgh\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut i8, mut y: *const i8, mut n: u64) -> i32 {
// CHECK-NEXT:     let mut i: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     let mut j: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             strncmp(
// CHECK-NEXT:                 (unsafe { x.offset(j as isize) }) as *const i8,
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(a.0) as *mut *const i8 }.offset(i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     strlen(unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(a.0) as *mut *const i8 }
// CHECK-NEXT:                                 .offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     })
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             return 2 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u64 = j;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_add(unsafe {
// CHECK-NEXT:             strlen(unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(a.0) as *mut *const i8 }.offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:         });
// CHECK-NEXT:         j = [[V_1]];
// CHECK-NEXT:         if y != std::ptr::null::<i8>() {
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: u64 = j;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_add(unsafe { strlen(y) });
// CHECK-NEXT:             j = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u64 = [[V_4]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         c"abcde".as_ptr() as *mut i8,
// CHECK-NEXT:         std::ptr::null::<i8>(),
// CHECK-NEXT:         ((3 as i32) as i64) as u64,
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
