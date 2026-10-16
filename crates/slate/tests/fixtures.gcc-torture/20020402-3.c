/* extracted from gdb sources */

typedef unsigned long long CORE_ADDR;

struct blockvector;

struct symtab {
  struct blockvector *blockvector;
};

struct sec {
  void *unused;
};

struct symbol {
  int   len;
  char *name;
};

struct block {
  CORE_ADDR      startaddr, endaddr;
  struct symbol *function;
  struct block  *superblock;
  unsigned char  gcc_compile_flag;
  int            nsyms;
  struct symbol  syms[1];
};

struct blockvector {
  int           nblocks;
  struct block *block[2];
};

struct blockvector *blockvector_for_pc_sect(register CORE_ADDR pc,
                                            struct symtab     *symtab) {
  register struct block *b;
  register int           bot, top, half;
  struct blockvector    *bl;

  bl = symtab->blockvector;
  b  = bl->block[0];

  bot = 0;
  top = bl->nblocks;

  while (top - bot > 1) {
    half = (top - bot + 1) >> 1;
    b    = bl->block[bot + half];
    if (b->startaddr <= pc)
      bot += half;
    else
      top = bot + half;
  }

  while (bot >= 0) {
    b = bl->block[bot];
    if (b->endaddr > pc) {
      return bl;
    }
    bot--;
  }
  return 0;
}

int main(void) {
  struct block       a  = {0, 0x10000, 0, 0, 1, 20};
  struct block       b  = {0x10000, 0x20000, 0, 0, 1, 20};
  struct blockvector bv = {2, {&a, &b}};
  struct symtab      s  = {&bv};

  struct blockvector *ret;

  ret = blockvector_for_pc_sect(0x500, &s);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! extracted from gdb sources
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct symtab {
// CHECK-NEXT:     blockvector: *mut blockvector,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct symbol {
// CHECK-NEXT:     len: i32,
// CHECK-NEXT:     name: *mut i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct block {
// CHECK-NEXT:     startaddr: u64,
// CHECK-NEXT:     endaddr: u64,
// CHECK-NEXT:     function: *mut symbol,
// CHECK-NEXT:     superblock: *mut block,
// CHECK-NEXT:     gcc_compile_flag: u8,
// CHECK-NEXT:     nsyms: i32,
// CHECK-NEXT:     syms: [symbol; 1],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct blockvector {
// CHECK-NEXT:     nblocks: i32,
// CHECK-NEXT:     block: [*mut block; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn blockvector_for_pc_sect(mut pc: u64, mut symtab: &symtab) -> *mut blockvector {
// CHECK-NEXT:     let mut b: *mut block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut bot: i32 = 0 as i32;
// CHECK-NEXT:     let mut top: i32 = 0 as i32;
// CHECK-NEXT:     let mut half: i32 = 0 as i32;
// CHECK-NEXT:     let mut bl: *mut blockvector = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     bl = unsafe { (*(symtab as *const symtab)).blockvector };
// CHECK-NEXT:     b = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*bl).block.as_mut_ptr() as *mut *mut block }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     top = unsafe { (*bl).nblocks };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while top - bot > (1 as i32) {
// CHECK-NEXT:         half = top - bot + (1 as i32) >> (1 as i32);
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*bl).block.as_mut_ptr() as *mut *mut block }.offset((bot + half) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         if (unsafe { (*b).startaddr }) <= pc {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = bot;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + half;
// CHECK-NEXT:             bot = [[V_1]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             top = bot + half;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while bot >= (0 as i32) {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*bl).block.as_mut_ptr() as *mut *mut block }.offset(bot as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:         if (unsafe { (*b).endaddr }) > pc {
// CHECK-NEXT:             return bl;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = bot;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         bot = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<blockvector>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: block = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].startaddr = ((0 as i32) as i64) as u64;
// CHECK-NEXT:         [[T_0]].endaddr = ((65536 as i32) as i64) as u64;
// CHECK-NEXT:         [[T_0]].function = std::ptr::null_mut::<symbol>();
// CHECK-NEXT:         [[T_0]].superblock = std::ptr::null_mut::<block>();
// CHECK-NEXT:         [[T_0]].gcc_compile_flag = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_0]].nsyms = 20 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: block = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: block = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].startaddr = ((65536 as i32) as i64) as u64;
// CHECK-NEXT:         [[T_1]].endaddr = ((131072 as i32) as i64) as u64;
// CHECK-NEXT:         [[T_1]].function = std::ptr::null_mut::<symbol>();
// CHECK-NEXT:         [[T_1]].superblock = std::ptr::null_mut::<block>();
// CHECK-NEXT:         [[T_1]].gcc_compile_flag = ((1 as i32) as i8) as u8;
// CHECK-NEXT:         [[T_1]].nsyms = 20 as i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut bv: blockvector = blockvector {
// CHECK-NEXT:         nblocks: 2 as i32,
// CHECK-NEXT:         block: [std::ptr::addr_of_mut!(a), std::ptr::addr_of_mut!(b)],
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut s: symtab = symtab {
// CHECK-NEXT:         blockvector: std::ptr::addr_of_mut!(bv),
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut ret: *mut blockvector = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ret = blockvector_for_pc_sect(((1280 as i32) as i64) as u64, unsafe {
// CHECK-NEXT:         &(*std::ptr::addr_of_mut!(s))
// CHECK-NEXT:     });
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
