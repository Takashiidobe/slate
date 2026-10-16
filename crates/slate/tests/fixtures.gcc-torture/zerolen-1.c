extern void abort(void);
extern void exit(int);

union iso_directory_record {
  char carr[4];
  struct {
    unsigned char name_len[1];
    char          name[0];
  } u;
} entry;

void set(union iso_directory_record *);

int main(void) {
  union iso_directory_record *de;

  de = &entry;
  set(de);

  if (de->u.name_len[0] == 1 && de->u.name[0] == 0)
    exit(0);
  else
    abort();
}

void set(union iso_directory_record *p) {
  p->carr[0] = 1;
  p->carr[1] = 0;
  return;
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
// CHECK-NEXT: union iso_directory_record {
// CHECK-NEXT:     carr: [i8; 4],
// CHECK-NEXT:     u: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     name_len: [u8; 1],
// CHECK-NEXT:     name: [i8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: static mut entry: iso_directory_record = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut de: *mut iso_directory_record = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     de = unsafe { std::ptr::addr_of_mut!(entry) };
// CHECK-NEXT:     set(de);
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*de).u.name_len.as_mut_ptr() as *mut u8 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32) == (1 as i32)
// CHECK-NEXT:         && ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*de).u.name.as_mut_ptr() as *mut i8 }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32) == (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn set(mut p: *mut iso_directory_record) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*p).carr.as_mut_ptr() as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = (1 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*p).carr.as_mut_ptr() as *mut i8 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     return;
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
