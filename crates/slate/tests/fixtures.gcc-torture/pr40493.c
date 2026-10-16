extern void abort(void);

typedef union i386_operand_type {
  struct {
    unsigned int reg8         : 1;
    unsigned int reg16        : 1;
    unsigned int reg32        : 1;
    unsigned int reg64        : 1;
    unsigned int floatreg     : 1;
    unsigned int regmmx       : 1;
    unsigned int regxmm       : 1;
    unsigned int regymm       : 1;
    unsigned int control      : 1;
    unsigned int debug        : 1;
    unsigned int test         : 1;
    unsigned int sreg2        : 1;
    unsigned int sreg3        : 1;
    unsigned int imm1         : 1;
    unsigned int imm8         : 1;
    unsigned int imm8s        : 1;
    unsigned int imm16        : 1;
    unsigned int imm32        : 1;
    unsigned int imm32s       : 1;
    unsigned int imm64        : 1;
    unsigned int disp8        : 1;
    unsigned int disp16       : 1;
    unsigned int disp32       : 1;
    unsigned int disp32s      : 1;
    unsigned int disp64       : 1;
    unsigned int acc          : 1;
    unsigned int floatacc     : 1;
    unsigned int baseindex    : 1;
    unsigned int inoutportreg : 1;
    unsigned int shiftcount   : 1;
    unsigned int jumpabsolute : 1;
    unsigned int esseg        : 1;
    unsigned int regmem       : 1;
    unsigned int mem          : 1;
    unsigned int byte         : 1;
    unsigned int word         : 1;
    unsigned int dword        : 1;
    unsigned int fword        : 1;
    unsigned int qword        : 1;
    unsigned int tbyte        : 1;
    unsigned int xmmword      : 1;
    unsigned int ymmword      : 1;
    unsigned int unspecified  : 1;
    unsigned int anysize      : 1;
  } bitfield;
  unsigned int array[2];
} i386_operand_type;

unsigned int x00, x01, y00, y01;

int main(int argc, char *argv[]) {
  i386_operand_type a, b, c, d;

  a.bitfield.reg16 = 1;
  a.bitfield.imm16 = 0;
  a.array[1]       = 22;

  b   = a;
  x00 = b.array[0];
  x01 = b.array[1];

  c   = b;
  y00 = c.array[0];
  y01 = c.array[1];

  d = c;
  if (d.bitfield.reg16 != 1)
    abort();
  if (d.bitfield.imm16 != 0)
    abort();
  if (d.array[1] != 22)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union i386_operand_type {
// CHECK-NEXT:     bitfield: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     array: [u32; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;6],
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub reg8: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub reg16: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub reg32: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub reg64: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub floatreg: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub regmmx: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub regxmm: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub regymm: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub control: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub debug: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub test: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub sreg2: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub sreg3: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm1: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm8: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm8s: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm16: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm32: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm32s: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub imm64: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub disp8: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub disp16: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub disp32: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub disp32s: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub disp64: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub acc: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub floatacc: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub baseindex: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub inoutportreg: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub shiftcount: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub jumpabsolute: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub esseg: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub regmem: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub mem: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub byte: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub word: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub dword: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub fword: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub qword: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub tbyte: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub xmmword: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub ymmword: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub unspecified: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub anysize: u32,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_44: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut x00: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut x01: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut y00: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut y01: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut a: i386_operand_type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: i386_operand_type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: i386_operand_type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut d: i386_operand_type = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.bitfield.__slate_bits_0.__set_reg16((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.bitfield.__slate_bits_0.__set_imm16((0 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { a.array.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = (22 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b = a;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x00 = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { b.array.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x01 = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { b.array.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     c = b;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y00 = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { c.array.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y01 = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { c.array.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     d = c;
// CHECK-NEXT:     if ((unsafe { d.bitfield.__slate_bits_0.__get_reg16() }) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { d.bitfield.__slate_bits_0.__get_imm16() }) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { d.array.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != ((22 as i32) as u32)
// CHECK-NEXT:     {
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
