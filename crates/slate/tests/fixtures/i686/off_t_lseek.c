#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    const char *path = "/tmp/slate_off_t_lseek.tmp";
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
        return 1;
    unlink(path);
    if (write(fd, "0123456789", 10) != 10)
        return 2;
    off_t end = lseek(fd, 0, SEEK_END);
    off_t mid = lseek(fd, 4, SEEK_SET);
    char c = 0;
    if (read(fd, &c, 1) != 1)
        return 3;
    printf("%zu %ld %ld %c\n", sizeof(off_t), (long)end, (long)mid, c);
    close(fd);
    return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn open([[ARG_0:arg[0-9]+]]: *const i8, [[ARG_1:arg[0-9]+]]: i32, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::lseek;
// CHECK-NEXT: use slate_support::close;
// CHECK-NEXT: use slate_support::read;
// CHECK-NEXT: use slate_support::write;
// CHECK-NEXT: use slate_support::unlink;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let path: *const i8 = c"/tmp/slate_off_t_lseek.tmp".as_ptr();
// CHECK-NEXT:     let fd: i32 = unsafe {
// CHECK-NEXT:         open(
// CHECK-NEXT:             path,
// CHECK-NEXT:             (slate_support::O_RDWR as i32) | (slate_support::O_CREAT as i32)
// CHECK-NEXT:                 | (slate_support::O_TRUNC as i32),
// CHECK-NEXT:             384i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if fd < 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { unlink(path as *const core::ffi::c_char) as i32 };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         write(
// CHECK-NEXT:             fd as i32,
// CHECK-NEXT:             (b"0123456789\0".as_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             10u32 as usize,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) != 10
// CHECK-NEXT:     {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let end: i32 = unsafe {
// CHECK-NEXT:         lseek(fd as i32, 0i32 as i32, (slate_support::SEEK_END as i32) as i32) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let mid: i32 = unsafe {
// CHECK-NEXT:         lseek(fd as i32, 4i32 as i32, (slate_support::SEEK_SET as i32) as i32) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: i8 = 0;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         read(
// CHECK-NEXT:             fd as i32,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(c) as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             1u32 as usize,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) != 1
// CHECK-NEXT:     {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %ld %ld %c\n".as_ptr(),
// CHECK-NEXT:             std::mem::size_of::<i32>() as u32,
// CHECK-NEXT:             end,
// CHECK-NEXT:             mid,
// CHECK-NEXT:             c as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { close(fd as i32) as i32 };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
