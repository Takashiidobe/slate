/* PR tree-optimization/103255 */

struct H {
  unsigned a;
  unsigned b;
  unsigned c;
};

#if __SIZEOF_POINTER__ >= 4
#define ADDR 0x400000
#else
#define ADDR 0x4000
#endif
#define OFF 0x20

int main() {
  struct H     *h = 0;
  unsigned long o;
  volatile int  t = 1;

  for (o = OFF; o <= OFF; o += 0x1000) {
    struct H *u;
    u = (struct H *)(ADDR + o);
    if (t) {
      h = u;
      break;
    }
  }

  if (h == 0)
    return 0;
  unsigned *tt = &h->b;
  if ((__SIZE_TYPE__)tt != (ADDR + OFF + __builtin_offsetof(struct H, b)))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/103255
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct H {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut h: *mut H = std::ptr::null_mut::<H>();
// CHECK-NEXT:     let mut o: u64 = ((32 as i32) as i64) as u64;
// CHECK-NEXT:     let mut t: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while o <= (((32 as i32) as i64) as u64) {
// CHECK-NEXT:         let mut u: *mut H = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         u = (((4194304 as i32) as i64) as u64).wrapping_add(o) as *mut H;
// CHECK-NEXT:         if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(t)) }) != (0 as i32) {
// CHECK-NEXT:             h = u;
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u64 = o;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_add(((4096 as i32) as i64) as u64);
// CHECK-NEXT:         o = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if h == std::ptr::null_mut::<H>() {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut tt: *mut u32 = unsafe { std::ptr::addr_of_mut!((*h).b) };
// CHECK-NEXT:     if (tt as u64) != ((((4194304 as i32) + (32 as i32)) as i64) as u64).wrapping_add(4 as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
