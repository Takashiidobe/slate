/* PR target/70566.  */

#define NULL 0

struct mystruct {
  unsigned int f1 : 1;
  unsigned int f2 : 1;
  unsigned int f3 : 1;
};

__attribute__((noinline)) void myfunc(int a, void *b) {}
__attribute__((noinline)) int  myfunc2(void *a) { return 0; }

static void set_f2(struct mystruct *user, int f2) {
  if (user->f2 != f2)
    myfunc(myfunc2(NULL), NULL);
  else
    __builtin_abort();
}

__attribute__((noinline)) void foo(void *data) {
  struct mystruct *user = data;
  if (!user->f2)
    set_f2(user, 1);
}

int main(void) {
  struct mystruct a;
  a.f1 = 1;
  a.f2 = 0;
  foo(&a);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/70566.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mystruct {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f1: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f2: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub f3: u32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn myfunc(mut a: i32, mut b: *mut ()) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn myfunc2(mut a: *mut ()) -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn set_f2(mut user: &mut mystruct, mut f2: i32) {
// CHECK-NEXT:     if ((unsafe { (*(user as *mut mystruct)).__slate_bits_0.__get_f2() }) as i32) != f2 {
// CHECK-NEXT:         myfunc(
// CHECK-NEXT:             myfunc2(std::ptr::null_mut::<()>()),
// CHECK-NEXT:             std::ptr::null_mut::<()>(),
// CHECK-NEXT:         );
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut data: &mut ()) {
// CHECK-NEXT:     if !(((unsafe {
// CHECK-NEXT:         (*((data as *mut ()) as *mut mystruct))
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_f2()
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         set_f2(
// CHECK-NEXT:             unsafe { &mut (*((data as *mut ()) as *mut mystruct)) },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: mystruct = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a.__slate_bits_0.__set_f1((1 as i32) as u32);
// CHECK-NEXT:     a.__slate_bits_0.__set_f2((0 as i32) as u32);
// CHECK-NEXT:     foo(unsafe { &mut (*(std::ptr::addr_of_mut!(a) as *mut ())) });
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
