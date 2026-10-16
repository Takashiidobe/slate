#define S (sizeof(int))

unsigned int                   c[624];
void __attribute__((noinline)) bar(void) {
  unsigned int i;
  /* Obfuscated c[i] = c[i-1] * 2.  */
  for (i = 1; i < 624; ++i)
    *(unsigned int *)((void *)c + (__SIZE_TYPE__)i * S) =
        2 * *(unsigned int *)((void *)c +
                              ((__SIZE_TYPE__)i + ((__SIZE_TYPE__)-S) / S) * S);
}
extern void abort(void);
int         main() {
  unsigned int i, j;
  for (i = 0; i < 624; ++i)
    c[i] = 1;
  bar();
  j = 1;
  for (i = 0; i < 624; ++i) {
    if (c[i] != j)
      abort();
    j = j * 2;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: __SlateAlign16<[u32; 624]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     let mut i: u32 = (1 as i32) as u32;
// CHECK-NEXT:     // Obfuscated c[i] = c[i-1] * 2.
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < ((624 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(((unsafe {
// CHECK-NEXT:                 (((unsafe { std::ptr::addr_of_mut!(c.0) as *mut u32 }) as *mut ()) as *const u8)
// CHECK-NEXT:                     .offset((i as u64).wrapping_mul(4 as u64) as isize)
// CHECK-NEXT:             }) as *mut ()) as *mut u32) = ((2 as i32) as u32).wrapping_mul(unsafe {
// CHECK-NEXT:                 *(((unsafe {
// CHECK-NEXT:                     (((unsafe { std::ptr::addr_of_mut!(c.0) as *mut u32 }) as *mut ()) as *const u8)
// CHECK-NEXT:                         .offset(
// CHECK-NEXT:                             (i as u64)
// CHECK-NEXT:                                 .wrapping_add((4 as u64).wrapping_neg() / (4 as u64))
// CHECK-NEXT:                                 .wrapping_mul(4 as u64) as isize,
// CHECK-NEXT:                         )
// CHECK-NEXT:                 }) as *mut ()) as *mut u32)
// CHECK-NEXT:             });
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut j: u32 = (1 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < ((624 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(c.0) as *mut u32 }.offset(i as isize) } =
// CHECK-NEXT:                 (1 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar();
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < ((624 as i32) as u32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(c.0) as *mut u32 }.offset(i as isize) }
// CHECK-NEXT:         }) != j
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         j = j.wrapping_mul((2 as i32) as u32);
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_5]];
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
