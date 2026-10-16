struct s {
  unsigned char a[256];
};
union u {
  struct {
    struct s b;
    int      c;
  } d;
  struct {
    int      c;
    struct s b;
  } e;
};

static union u   v;
static struct s *p = &v.d.b;
static struct s *q = &v.e.b;

static struct s __attribute__((noinline)) rp(void) { return *p; }

static void qp(void) { *q = rp(); }

int main() {
  int i;
  for (i = 0; i < 256; i++)
    p->a[i] = i;
  qp();
  for (i = 0; i < 256; i++)
    if (q->a[i] != i)
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     a: [u8; 256],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union u {
// CHECK-NEXT:     d: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     e: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     b: s,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     b: s,
// CHECK-NEXT: }
// CHECK-NEXT: static mut v: u = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut p: *mut s = unsafe { std::ptr::addr_of_mut!(v.d.b) };
// CHECK-NEXT: static mut q: *mut s = unsafe { std::ptr::addr_of_mut!(v.e.b) };
// CHECK-NEXT: fn rp() -> s {
// CHECK-NEXT:     return unsafe { *unsafe { p } };
// CHECK-NEXT: }
// CHECK-NEXT: fn qp() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { q } = rp();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { p }).a.as_mut_ptr() as *mut u8 }.offset(i as isize)
// CHECK-NEXT:             } = (i as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     qp();
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         if (((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*unsafe { q }).a.as_mut_ptr() as *mut u8 }.offset(i as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32) as i32) != i
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
