/* PR tree-optimization/125291 */

char         buf[1111];
char        *archive_le16dec_filename = buf;
unsigned int archive_le16dec_end, archive_le16dec_fn_end,
    archive_le16dec_filename_size, archive_le16dec_offset;
char archive_le16dec_p[] = {21, 0x7f};

[[gnu::noipa]]
void archive_le16dec() {
  archive_le16dec_filename_size = (short)archive_le16dec_filename_size;
  unsigned char flagbits        = 0, flagbyte;
  archive_le16dec_end           = archive_le16dec_filename_size;
  archive_le16dec_fn_end        = archive_le16dec_filename_size * 2;
  archive_le16dec_filename_size = flagbits = 0;
  while (archive_le16dec_offset < archive_le16dec_end &&
         archive_le16dec_filename_size < archive_le16dec_fn_end) {
    if (!flagbits) {
      flagbyte = archive_le16dec_p[archive_le16dec_offset++];
      flagbits = 8;
    }
    flagbits -= 2;
    if (!(flagbyte >> flagbits & 3))
      archive_le16dec_filename_size++;
  }
}

int main() {
  archive_le16dec_filename_size = 2;
  archive_le16dec();
  if (archive_le16dec_filename_size != 1)
    __builtin_trap();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/125291
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.trap"]
// CHECK-NEXT:     fn __slate_intrinsic_46d8fd79c9d1f86c();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[i8; 1111]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut archive_le16dec_filename: *mut i8 = unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut archive_le16dec_end: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut archive_le16dec_fn_end: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut archive_le16dec_filename_size: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut archive_le16dec_offset: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut archive_le16dec_p: [i8; 2] = [(21 as i32) as i8, (127 as i32) as i8];
// CHECK-EMPTY:
// CHECK-NEXT: fn archive_le16dec() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         archive_le16dec_filename_size =
// CHECK-NEXT:             ((((unsafe { archive_le16dec_filename_size }) as u16) as i16) as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut flagbits: u8 = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     let mut flagbyte: u8 = 0 as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         archive_le16dec_end = unsafe { archive_le16dec_filename_size };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         archive_le16dec_fn_end =
// CHECK-NEXT:             unsafe { archive_le16dec_filename_size }.wrapping_mul((2 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         archive_le16dec_filename_size = (((0 as i32) as i8) as u8) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { archive_le16dec_offset }) < unsafe { archive_le16dec_end }
// CHECK-NEXT:         && (unsafe { archive_le16dec_filename_size }) < unsafe { archive_le16dec_fn_end }
// CHECK-NEXT:     {
// CHECK-NEXT:         if !(flagbits != (0 as u8)) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: u32 = unsafe { archive_le16dec_offset };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 archive_le16dec_offset = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:             flagbyte = (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(archive_le16dec_p) as *mut i8 }
// CHECK-NEXT:                         .offset([[V_0]] as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as u8;
// CHECK-NEXT:             flagbits = ((8 as i32) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u8 = flagbits;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u8 = (((([[V_2]] as u32) as i32) - (2 as i32)) as i8) as u8;
// CHECK-NEXT:         flagbits = [[V_3]];
// CHECK-NEXT:         if !(((flagbyte as u32) as i32) >> ((flagbits as u32) as i32) & (3 as i32) != (0 as i32)) {
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: u32 = unsafe { archive_le16dec_filename_size };
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 archive_le16dec_filename_size = [[V_5]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         archive_le16dec_filename_size = (2 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     archive_le16dec();
// CHECK-NEXT:     if (unsafe { archive_le16dec_filename_size }) != ((1 as i32) as u32) {
// CHECK-NEXT:         unsafe { __slate_intrinsic_46d8fd79c9d1f86c() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
