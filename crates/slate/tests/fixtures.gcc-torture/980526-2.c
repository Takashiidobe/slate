void abort(void);
void exit(int);

typedef unsigned int dev_t;
typedef unsigned int kdev_t;

static inline kdev_t to_kdev_t(int dev) {
  int major, minor;

  if (sizeof(kdev_t) == 16)
    return (kdev_t)dev;
  major = (dev >> 8);
  minor = (dev & 0xff);
  return (((major) << 22) | (minor));
}

void do_mknod(const char *filename, int mode, kdev_t dev) {
  if (dev == 0x15800078)
    exit(0);
  else
    abort();
}

char *getname(const char *filename) {
  register unsigned int a1, a2, a3, a4, a5, a6, a7, a8, a9;
  a1 = (unsigned int)(filename) * 5 + 1;
  a2 = (unsigned int)(filename) * 6 + 2;
  a3 = (unsigned int)(filename) * 7 + 3;
  a4 = (unsigned int)(filename) * 8 + 4;
  a5 = (unsigned int)(filename) * 9 + 5;
  a6 = (unsigned int)(filename) * 10 + 5;
  a7 = (unsigned int)(filename) * 11 + 5;
  a8 = (unsigned int)(filename) * 12 + 5;
  a9 = (unsigned int)(filename) * 13 + 5;
  return (char *)(a1 * a2 + a3 * a4 + a5 * a6 + a7 * a8 + a9);
}

int sys_mknod(const char *filename, int mode, dev_t dev) {
  int   error;
  char *tmp;

  tmp   = getname(filename);
  error = ((long)(tmp));
  do_mknod(tmp, mode, to_kdev_t(dev));
  return error;
}

int main(void) {
  if (sizeof(int) != 4)
    exit(0);

  return sys_mknod("test", 1, 0x12345678);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn to_kdev_t(mut dev: i32) -> u32 {
// CHECK-NEXT:     let mut major: i32 = 0 as i32;
// CHECK-NEXT:     let mut minor: i32 = 0 as i32;
// CHECK-NEXT:     if (4 as u64) == (((16 as i32) as i64) as u64) {
// CHECK-NEXT:         return dev as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     major = dev >> (8 as i32);
// CHECK-NEXT:     minor = dev & (255 as i32);
// CHECK-NEXT:     (major << (22 as i32) | minor) as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn do_mknod(mut filename: *const i8, mut mode: i32, mut dev: u32) {
// CHECK-NEXT:     if dev == ((360710264 as i32) as u32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn getname(mut filename: *const i8) -> *mut i8 {
// CHECK-NEXT:     let mut a1: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((5 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((1 as i32) as u32);
// CHECK-NEXT:     let mut a2: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((6 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((2 as i32) as u32);
// CHECK-NEXT:     let mut a3: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((7 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((3 as i32) as u32);
// CHECK-NEXT:     let mut a4: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((8 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((4 as i32) as u32);
// CHECK-NEXT:     let mut a5: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((9 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((5 as i32) as u32);
// CHECK-NEXT:     let mut a6: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((10 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((5 as i32) as u32);
// CHECK-NEXT:     let mut a7: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((11 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((5 as i32) as u32);
// CHECK-NEXT:     let mut a8: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((12 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((5 as i32) as u32);
// CHECK-NEXT:     let mut a9: u32 = (filename as u32)
// CHECK-NEXT:         .wrapping_mul((13 as i32) as u32)
// CHECK-NEXT:         .wrapping_add((5 as i32) as u32);
// CHECK-NEXT:     a1
// CHECK-NEXT:         .wrapping_mul(a2)
// CHECK-NEXT:         .wrapping_add(a3.wrapping_mul(a4))
// CHECK-NEXT:         .wrapping_add(a5.wrapping_mul(a6))
// CHECK-NEXT:         .wrapping_add(a7.wrapping_mul(a8))
// CHECK-NEXT:         .wrapping_add(a9) as *mut i8
// CHECK-NEXT: }
// CHECK-NEXT: fn sys_mknod(mut filename: *const i8, mut mode: i32, mut dev: u32) -> i32 {
// CHECK-NEXT:     let mut error: i32 = 0 as i32;
// CHECK-NEXT:     let mut tmp: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     tmp = getname(filename);
// CHECK-NEXT:     error = (tmp as i64) as i32;
// CHECK-NEXT:     do_mknod(tmp as *const i8, mode, to_kdev_t(dev as i32));
// CHECK-NEXT:     error
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     sys_mknod(c"test".as_ptr() as *const i8, 1 as i32, (305419896 as i32) as u32)
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
