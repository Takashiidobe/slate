typedef unsigned int u32;

static const u32 deadfish = 0xdeadf155;

static const u32 cfb_tab8_be[] = {
    0x00000000, 0x000000ff, 0x0000ff00, 0x0000ffff, 0x00ff0000, 0x00ff00ff,
    0x00ffff00, 0x00ffffff, 0xff000000, 0xff0000ff, 0xff00ff00, 0xff00ffff,
    0xffff0000, 0xffff00ff, 0xffffff00, 0xffffffff};

static const u32 cfb_tab8_le[] = {
    0x00000000, 0xff000000, 0x00ff0000, 0xffff0000, 0x0000ff00, 0xff00ff00,
    0x00ffff00, 0xffffff00, 0x000000ff, 0xff0000ff, 0x00ff00ff, 0xffff00ff,
    0x0000ffff, 0xff00ffff, 0x00ffffff, 0xffffffff};

static const u32 cfb_tab16_be[] = {0x00000000, 0x0000ffff, 0xffff0000,
                                   0xffffffff};

static const u32 cfb_tab16_le[] = {0x00000000, 0xffff0000, 0x0000ffff,
                                   0xffffffff};

static const u32 cfb_tab32[] = {0x00000000, 0xffffffff};

const u32 *xxx(int bpp) {
  const u32 *tab;

  if (0)
    return &deadfish;

  switch (bpp) {
  case 8:
    tab = cfb_tab8_be;
    break;
  case 16:
    tab = cfb_tab16_be;
    break;
  case 32:
  default:
    tab = cfb_tab32;
    break;
  }

  return tab;
}

int main(void) {
  const u32 *a = xxx(8);
  int        b = a[0];
  if (b != cfb_tab8_be[0])
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut deadfish: u32 = 3735941461 as u32;
// CHECK-NEXT: static mut cfb_tab8_be: __SlateAlign16<[u32; 16]> = __SlateAlign16([
// CHECK-NEXT:     (0 as i32) as u32,
// CHECK-NEXT:     (255 as i32) as u32,
// CHECK-NEXT:     (65280 as i32) as u32,
// CHECK-NEXT:     (65535 as i32) as u32,
// CHECK-NEXT:     (16711680 as i32) as u32,
// CHECK-NEXT:     (16711935 as i32) as u32,
// CHECK-NEXT:     (16776960 as i32) as u32,
// CHECK-NEXT:     (16777215 as i32) as u32,
// CHECK-NEXT:     4278190080 as u32,
// CHECK-NEXT:     4278190335 as u32,
// CHECK-NEXT:     4278255360 as u32,
// CHECK-NEXT:     4278255615 as u32,
// CHECK-NEXT:     4294901760 as u32,
// CHECK-NEXT:     4294902015 as u32,
// CHECK-NEXT:     4294967040 as u32,
// CHECK-NEXT:     4294967295 as u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut cfb_tab8_le: __SlateAlign16<[u32; 16]> = __SlateAlign16([
// CHECK-NEXT:     (0 as i32) as u32,
// CHECK-NEXT:     4278190080 as u32,
// CHECK-NEXT:     (16711680 as i32) as u32,
// CHECK-NEXT:     4294901760 as u32,
// CHECK-NEXT:     (65280 as i32) as u32,
// CHECK-NEXT:     4278255360 as u32,
// CHECK-NEXT:     (16776960 as i32) as u32,
// CHECK-NEXT:     4294967040 as u32,
// CHECK-NEXT:     (255 as i32) as u32,
// CHECK-NEXT:     4278190335 as u32,
// CHECK-NEXT:     (16711935 as i32) as u32,
// CHECK-NEXT:     4294902015 as u32,
// CHECK-NEXT:     (65535 as i32) as u32,
// CHECK-NEXT:     4278255615 as u32,
// CHECK-NEXT:     (16777215 as i32) as u32,
// CHECK-NEXT:     4294967295 as u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut cfb_tab16_be: __SlateAlign16<[u32; 4]> = __SlateAlign16([
// CHECK-NEXT:     (0 as i32) as u32,
// CHECK-NEXT:     (65535 as i32) as u32,
// CHECK-NEXT:     4294901760 as u32,
// CHECK-NEXT:     4294967295 as u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut cfb_tab16_le: __SlateAlign16<[u32; 4]> = __SlateAlign16([
// CHECK-NEXT:     (0 as i32) as u32,
// CHECK-NEXT:     4294901760 as u32,
// CHECK-NEXT:     (65535 as i32) as u32,
// CHECK-NEXT:     4294967295 as u32,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut cfb_tab32: [u32; 2] = [(0 as i32) as u32, 4294967295 as u32];
// CHECK-NEXT: fn xxx(mut bpp: i32) -> *const u32 {
// CHECK-NEXT:     let mut tab: *const u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (0 as i32) != (0 as i32) {
// CHECK-NEXT:         return unsafe { std::ptr::addr_of!(deadfish) };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match bpp {
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 tab = unsafe { std::ptr::addr_of!(cfb_tab8_be.0) as *const u32 };
// CHECK-NEXT:             }
// CHECK-NEXT:             16 => {
// CHECK-NEXT:                 tab = unsafe { std::ptr::addr_of!(cfb_tab16_be.0) as *const u32 };
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 tab = unsafe { std::ptr::addr_of!(cfb_tab32) as *const u32 };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     tab
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: *const u32 = xxx(8 as i32);
// CHECK-NEXT:     let mut b: i32 = (unsafe { *unsafe { a.offset((0 as i32) as isize) } }) as i32;
// CHECK-NEXT:     if (b as u32)
// CHECK-NEXT:         != unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of!(cfb_tab8_be.0) as *const u32 }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
