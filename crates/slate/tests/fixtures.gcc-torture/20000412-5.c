void abort(void);
void exit(int);

int main(void) {
  struct {
    int node;
    int type;
  } lastglob[1] = {{0, 1}};

  if (lastglob[0].node != 0 || lastglob[0].type != 1)
    abort();
  exit(0);
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
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     node: i32,
// CHECK-NEXT:     r#type: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut lastglob: [[[SLATERECORD_0]]; 1] = [
// CHECK-NEXT:         [[SLATERECORD_0]] {
// CHECK-NEXT:             node: 0 as i32,
// CHECK-NEXT:             r#type: 1 as i32,
// CHECK-NEXT:         },
// CHECK-NEXT:     ];
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             (lastglob.as_mut_ptr() as *mut [[SLATERECORD_0]]).offset((0 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .node
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 (lastglob.as_mut_ptr() as *mut [[SLATERECORD_0]])
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 .r#type
// CHECK-NEXT:         }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
