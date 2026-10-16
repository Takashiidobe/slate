void abort(void);
void exit(int);

struct inode {
  long long           i_size;
  struct super_block *i_sb;
};

struct file {
  long long f_pos;
};

struct super_block {
  int           s_blocksize;
  unsigned char s_blocksize_bits;
  int           s_hs;
};

static char *isofs_bread(unsigned int block) {
  if (block)
    abort();
  exit(0);
}

static int do_isofs_readdir(struct inode *inode, struct file *filp) {
  int           bufsize = inode->i_sb->s_blocksize;
  unsigned char bufbits = inode->i_sb->s_blocksize_bits;
  unsigned int  block, offset;
  char         *bh = 0;
  int           hs;

  if (filp->f_pos >= inode->i_size)
    return 0;

  offset = filp->f_pos & (bufsize - 1);
  block  = filp->f_pos >> bufbits;
  hs     = inode->i_sb->s_hs;

  while (filp->f_pos < inode->i_size) {
    if (!bh)
      bh = isofs_bread(block);

    hs += block << bufbits;

    if (hs == 0)
      filp->f_pos++;

    if (offset >= bufsize)
      offset &= bufsize - 1;

    if (*bh)
      filp->f_pos++;

    filp->f_pos++;
  }
  return 0;
}

struct super_block s;
struct inode       i;
struct file        f;

int main(int argc, char **argv) {
  s.s_blocksize      = 512;
  s.s_blocksize_bits = 9;
  i.i_size           = 2048;
  i.i_sb             = &s;
  f.f_pos            = 0;

  do_isofs_readdir(&i, &f);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct inode {
// CHECK-NEXT:     i_size: i64,
// CHECK-NEXT:     i_sb: *mut super_block,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct file {
// CHECK-NEXT:     f_pos: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct super_block {
// CHECK-NEXT:     s_blocksize: i32,
// CHECK-NEXT:     s_blocksize_bits: u8,
// CHECK-NEXT:     s_hs: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn isofs_bread(mut block: u32) -> *mut i8 {
// CHECK-NEXT:     if block != (0 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn do_isofs_readdir(mut inode: *mut inode, mut filp: *mut file) -> i32 {
// CHECK-NEXT:     let mut bufsize: i32 = unsafe { (*unsafe { (*inode).i_sb }).s_blocksize };
// CHECK-NEXT:     let mut bufbits: u8 = unsafe { (*unsafe { (*inode).i_sb }).s_blocksize_bits };
// CHECK-NEXT:     let mut block: u32 = 0 as u32;
// CHECK-NEXT:     let mut offset: u32 = 0 as u32;
// CHECK-NEXT:     let mut bh: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let mut hs: i32 = 0 as i32;
// CHECK-NEXT:     if (unsafe { (*filp).f_pos }) >= unsafe { (*inode).i_size } {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     offset = (((unsafe { (*filp).f_pos }) & ((bufsize - (1 as i32)) as i64)) as i32)
// CHECK-NEXT:         as u32;
// CHECK-NEXT:     block = (((unsafe { (*filp).f_pos }) >> ((bufbits as u32) as i32)) as i32) as u32;
// CHECK-NEXT:     hs = unsafe { (*unsafe { (*inode).i_sb }).s_hs };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { (*filp).f_pos }) < unsafe { (*inode).i_size } {
// CHECK-NEXT:         if !(bh != std::ptr::null_mut::<i8>()) {
// CHECK-NEXT:             bh = isofs_bread(block);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = hs;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = ([[V_1]] as u32)
// CHECK-NEXT:             .wrapping_add(block << ((bufbits as u32) as i32)) as i32;
// CHECK-NEXT:         hs = [[V_2]];
// CHECK-NEXT:         if hs == (0 as i32) {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i64 = unsafe { (*filp).f_pos };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] + ((1 as i32) as i64);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*filp).f_pos = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if offset >= (bufsize as u32) {
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: u32 = offset;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: u32 = [[V_5]] & ((bufsize - (1 as i32)) as u32);
// CHECK-NEXT:             offset = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { *bh }) != (0 as i8) {
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i64 = unsafe { (*filp).f_pos };
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i64 = [[V_7]] + ((1 as i32) as i64);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*filp).f_pos = [[V_8]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i64 = unsafe { (*filp).f_pos };
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i64 = [[V_9]] + ((1 as i32) as i64);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*filp).f_pos = [[V_10]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: static mut s: super_block = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut i: inode = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut f: file = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.s_blocksize = 512 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.s_blocksize_bits = ((9 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i.i_size = (2048 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i.i_sb = unsafe { std::ptr::addr_of_mut!(s) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f.f_pos = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     do_isofs_readdir(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(i) },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(f) },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT:     0
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
