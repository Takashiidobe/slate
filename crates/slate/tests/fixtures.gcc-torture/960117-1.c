void exit(int);

static char   id_space[2][32 + 1];
typedef short COUNT;

typedef char TEXT;

union T_VALS {
  TEXT *id __attribute__((aligned(2), packed));
};
typedef union T_VALS VALS;

struct T_VAL {
  COUNT pos __attribute__((aligned(2), packed));
  VALS  vals __attribute__((aligned(2), packed));
};
typedef struct T_VAL VAL;

VAL curval = {0};

static short idc = 0;
static int   cur_line;
static int   char_pos;

typedef unsigned short WORD;

WORD get_id(char c) { curval.vals.id[0] = c; }

WORD get_tok() {
  char c         = 'c';
  curval.vals.id = id_space[idc];
  curval.pos     = (cur_line << 10) | char_pos;
  return get_id(c);
}

int main(void) {
  get_tok();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut id_space: __SlateAlign16<{{\[\[}}i8; 33]; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: #[repr(C, packed(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union T_VALS {
// CHECK-NEXT:     id: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T_VAL {
// CHECK-NEXT:     pos: i16,
// CHECK-NEXT:     vals: T_VALS,
// CHECK-NEXT: }
// CHECK-NEXT: static mut curval: T_VAL = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: T_VAL = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].pos = (0 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut idc: i16 = (0 as i32) as i16;
// CHECK-NEXT: static mut cur_line: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut char_pos: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn get_id(mut c: i8) -> u16 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { curval.vals.id }.offset((0 as i32) as isize) } = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn get_tok() -> u16 {
// CHECK-NEXT:     let mut c: i8 = (99 as i32) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         curval.vals.id = unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(id_space.0) as *mut [i8; 33] }
// CHECK-NEXT:                     .offset(((unsafe { idc }) as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 .as_mut_ptr() as *mut i8
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         curval.pos = ((unsafe { cur_line }) << (10 as i32) | unsafe { char_pos }) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     get_id(c)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     get_tok();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
