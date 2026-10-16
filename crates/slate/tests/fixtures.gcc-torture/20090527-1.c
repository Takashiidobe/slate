typedef enum { POSITION_ASIS, POSITION_UNSPECIFIED } unit_position;

typedef enum { STATUS_UNKNOWN, STATUS_UNSPECIFIED } unit_status;

typedef struct {
  unit_position position;
  unit_status   status;
} unit_flags;

extern void abort(void);

void new_unit(unit_flags *flags) {
  if (flags->status == STATUS_UNSPECIFIED)
    flags->status = STATUS_UNKNOWN;

  if (flags->position == POSITION_UNSPECIFIED)
    flags->position = POSITION_ASIS;

  switch (flags->status) {
  case STATUS_UNKNOWN:
    break;

  default:
    abort();
  }
}

int main() {
  unit_flags f;
  f.status = STATUS_UNSPECIFIED;
  new_unit(&f);
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
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     position: u32,
// CHECK-NEXT:     status: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn new_unit(mut flags: &mut [[SLATERECORD_0]]) {
// CHECK-NEXT:     if ((unsafe { (*(flags as *mut [[SLATERECORD_0]])).status }) as u32)
// CHECK-NEXT:         == ((1 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(flags as *mut [[SLATERECORD_0]])).status = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*(flags as *mut [[SLATERECORD_0]])).position }) as u32)
// CHECK-NEXT:         == ((1 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(flags as *mut [[SLATERECORD_0]])).position = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match (unsafe { (*(flags as *mut [[SLATERECORD_0]])).status }) as u32 {
// CHECK-NEXT:             0 => {}
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f.status = (1 as i32) as u32;
// CHECK-NEXT:     new_unit(unsafe { &mut (*std::ptr::addr_of_mut!(f)) });
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
