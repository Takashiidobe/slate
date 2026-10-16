void abort(void);
void exit(int);

unsigned bug(unsigned short value, unsigned short *buffer,
             unsigned short *bufend);

unsigned short buf[] = {1, 4, 16, 64, 256};
int            main() {
  if (bug(512, buf, buf + 3) != 491)
    abort();

  exit(0);
}

unsigned bug(unsigned short value, unsigned short *buffer,
             unsigned short *bufend) {
  unsigned short *tmp;

  for (tmp = buffer; tmp < bufend; tmp++)
    value -= *tmp;

  return value;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut buf: [u16; 5] = [
// CHECK-NEXT:     ((1 as i32) as i16) as u16,
// CHECK-NEXT:     ((4 as i32) as i16) as u16,
// CHECK-NEXT:     ((16 as i32) as i16) as u16,
// CHECK-NEXT:     ((64 as i32) as i16) as u16,
// CHECK-NEXT:     ((256 as i32) as i16) as u16,
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if bug(
// CHECK-NEXT:         ((512 as i32) as i16) as u16,
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(buf) as *mut u16 },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(buf) as *mut u16 }
// CHECK-NEXT:                 .offset((3 as i32) as isize)
// CHECK-NEXT:         },
// CHECK-NEXT:     ) != ((491 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn bug(mut value: u16, mut buffer: *mut u16, mut bufend: *mut u16) -> u32 {
// CHECK-NEXT:     let mut tmp: *mut u16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     tmp = buffer;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while tmp < bufend {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u16 = value;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u16 = (((([[V_1]] as u32) as i32)
// CHECK-NEXT:             - (((unsafe { *tmp }) as u32) as i32)) as i16) as u16;
// CHECK-NEXT:         value = [[V_2]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut u16 = tmp;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut u16 = unsafe { [[V_3]].offset((1 as i32) as isize) };
// CHECK-NEXT:         tmp = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     value as u32
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
