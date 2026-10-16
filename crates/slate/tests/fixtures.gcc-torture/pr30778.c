extern void *memset(void *, int, __SIZE_TYPE__);
extern void  abort(void);

struct reg_stat {
  void *last_death;
  void *last_set;
  void *last_set_value;
  int   last_set_label;
  char  last_set_sign_bit_copies;
  int   last_set_mode : 8;
  char  last_set_invalid;
  char  sign_bit_copies;
  long  nonzero_bits;
};

static struct reg_stat *reg_stat;

void __attribute__((noinline)) init_reg_last(void) {
  memset(reg_stat, 0, __builtin_offsetof(struct reg_stat, sign_bit_copies));
}

int main(void) {
  struct reg_stat r;

  reg_stat       = &r;
  r.nonzero_bits = -1;
  init_reg_last();
  if (r.nonzero_bits != -1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct reg_stat {
// CHECK-NEXT:     last_death: *mut (),
// CHECK-NEXT:     last_set: *mut (),
// CHECK-NEXT:     last_set_value: *mut (),
// CHECK-NEXT:     last_set_label: i32,
// CHECK-NEXT:     last_set_sign_bit_copies: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     last_set_invalid: i8,
// CHECK-NEXT:     sign_bit_copies: i8,
// CHECK-NEXT:     nonzero_bits: i64,
// CHECK-NEXT: }
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
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub last_set_mode: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut reg_stat: *mut reg_stat = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn init_reg_last() {
// CHECK-NEXT:     unsafe { memset((unsafe { reg_stat }) as *mut (), 0 as i32, 31 as u64) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: reg_stat = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         reg_stat = std::ptr::addr_of_mut!(r);
// CHECK-NEXT:     }
// CHECK-NEXT:     r.nonzero_bits = -(1 as i32) as i64;
// CHECK-NEXT:     init_reg_last();
// CHECK-NEXT:     if r.nonzero_bits != (-(1 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
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
