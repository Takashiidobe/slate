/* Make sure that we don't free any temp stack slots associated with
   initializing marker before we're finished with them.  */

extern void abort();

typedef struct {
  short v16;
} __attribute__((packed)) jint16_t;

struct node {
  jint16_t magic;
  jint16_t nodetype;
  int      totlen;
} __attribute__((packed));

struct node node, *node_p = &node;

int main() {
  struct node marker = {.magic    = (jint16_t){0x1985},
                        .nodetype = (jint16_t){0x2003},
                        .totlen   = node_p->totlen};
  if (marker.magic.v16 != 0x1985)
    abort();
  if (marker.nodetype.v16 != 0x2003)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Make sure that we don't free any temp stack slots associated with
// CHECK-NEXT: //! initializing marker before we're finished with them.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v16: i16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct node {
// CHECK-NEXT:     magic: [[SLATERECORD_0]],
// CHECK-NEXT:     nodetype: [[SLATERECORD_0]],
// CHECK-NEXT:     totlen: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut node: node = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut node_p: *mut node = unsafe { std::ptr::addr_of_mut!(node) };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut marker: node = node {
// CHECK-NEXT:         magic: unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:                     v16: (6533 as i32) as i16,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         nodetype: unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = [[SLATERECORD_0]] {
// CHECK-NEXT:                     v16: (8195 as i32) as i16,
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         totlen: unsafe { (*unsafe { node_p }).totlen },
// CHECK-NEXT:     };
// CHECK-NEXT:     if (marker.magic.v16 as i32) != (6533 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (marker.nodetype.v16 as i32) != (8195 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
