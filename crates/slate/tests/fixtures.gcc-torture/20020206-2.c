/* Origin: PR c/5420 from David Mosberger <davidm@hpl.hp.com>.
   This testcase was miscompiled when tail call optimizing, because a
   compound literal initialization was emitted only in the tail call insn
   chain, not in the normal call insn chain.  */

typedef struct {
  unsigned short a;
} A;

extern void abort(void);
extern void exit(int);

void foo(unsigned int x) {
  if (x != 0x800 && x != 0x810)
    abort();
}

int main(int argc, char **argv) {
  int i;
  for (i = 0; i < 2; ++i)
    foo(((A){((!(i >> 4) ? 8 : 64 + (i >> 4)) << 8) + (i << 4)}).a);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: PR c/5420 from David Mosberger <davidm@hpl.hp.com>.
// CHECK-NEXT: //! This testcase was miscompiled when tail call optimizing, because a
// CHECK-NEXT: //! compound literal initialization was emitted only in the tail call insn
// CHECK-NEXT: //! chain, not in the normal call insn chain.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32) {
// CHECK-NEXT:     if x != ((2048 as i32) as u32) && x != ((2064 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         foo((unsafe {
// CHECK-NEXT:             (*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:                     a: ((((if !(i >> (4 as i32) != (0 as i32)) {
// CHECK-NEXT:                         8 as i32
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         (64 as i32) + (i >> (4 as i32))
// CHECK-NEXT:                     }) << (8 as i32))
// CHECK-NEXT:                         + (i << (4 as i32))) as i16) as u16,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             })
// CHECK-NEXT:             .a
// CHECK-NEXT:         }) as u32);
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
