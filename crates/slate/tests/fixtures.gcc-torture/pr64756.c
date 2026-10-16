/* PR rtl-optimization/64756 */

int                   a, *tmp, **c = &tmp;
volatile int          d;
static int *volatile *e = &tmp;
unsigned int          f;

static void fn1(int *p) {
  int g;
  for (; f < 1; f++)
    for (g = 1; g >= 0; g--) {
      d || d;
      *c = p;

      if (tmp != &a)
        __builtin_abort();

      *e = 0;
    }
}

int main() {
  fn1(&a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/64756
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut tmp: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(tmp) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut *mut i32 = (unsafe { std::ptr::addr_of_mut!(tmp) }) as *mut *mut i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1(mut p: *mut i32) {
// CHECK-NEXT:     let mut g: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { f }) < ((1 as i32) as u32) {
// CHECK-NEXT:         g = 1 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while g >= (0 as i32) {
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) }) != (0 as i32)
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) }) != (0 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { c } = p;
// CHECK-NEXT:             }
// CHECK-NEXT:             if (unsafe { tmp }) != unsafe { std::ptr::addr_of_mut!(a) } {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe { std::ptr::write_volatile(unsafe { e }, std::ptr::null_mut::<i32>()) };
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = g;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:             g = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = unsafe { f };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1(unsafe { std::ptr::addr_of_mut!(a) });
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
