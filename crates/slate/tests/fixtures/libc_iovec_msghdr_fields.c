#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

static size_t total_length(const struct iovec *vec, int count) {
  size_t total = 0;
  for (int i = 0; i < count; i++)
    total += vec[i].iov_len;
  return total;
}

int main(void) {
  char          first[]  = "hello ";
  char          second[] = "world";
  struct iovec  out[2];
  struct iovec  in[2];
  char          a[8] = {0};
  char          b[8] = {0};
  struct msghdr send_message;
  struct msghdr receive_message;
  int           pair[2];

  out[0].iov_base = first;
  out[0].iov_len  = strlen(first);
  out[1].iov_base = second;
  out[1].iov_len  = strlen(second);

  in[0].iov_base = a;
  in[0].iov_len  = 6;
  in[1].iov_base = b;
  in[1].iov_len  = 5;

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) != 0)
    return 1;

  memset(&send_message, 0, sizeof send_message);
  send_message.msg_iov    = out;
  send_message.msg_iovlen = 2;
  if (sendmsg(pair[0], &send_message, 0) != 11)
    return 2;

  memset(&receive_message, 0, sizeof receive_message);
  receive_message.msg_iov    = in;
  receive_message.msg_iovlen = 2;
  if (recvmsg(pair[1], &receive_message, 0) != 11)
    return 3;

  printf("%zu %zu %s%s %d\n", total_length(out, 2), total_length(in, 2), a, b,
         (int)receive_message.msg_iovlen);
  close(pair[0]);
  close(pair[1]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-NEXT: use slate_support::socketpair;
// CHECK-NEXT: use slate_support::sendmsg;
// CHECK-NEXT: use slate_support::recvmsg;
// CHECK-NEXT: use slate_support::close;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn total_length(vec: &[slate_support::iovec]) -> u64 {
// CHECK-NEXT:     let mut total: u64 = 0;
// CHECK-NEXT:     for i in 0..vec.len() as i32 {
// CHECK-NEXT:         total = total.wrapping_add(unsafe { vec[i as usize].iov_len as u64 });
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut first: [i8; 7] = [104, 101, 108, 108, 111, 32, 0];
// CHECK-NEXT:     let mut second: [i8; 6] = [119, 111, 114, 108, 100, 0];
// CHECK-NEXT:     let mut out: __SlateAlign16<[slate_support::iovec; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut r#in: __SlateAlign16<[slate_support::iovec; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut a: [i8; 8] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: [i8; 8] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [i8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]][0] = 0i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut send_message: slate_support::msghdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut receive_message: slate_support::msghdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pair: [i32; 2] = [0i32; 2];
// CHECK-NEXT:     out.0[0].iov_base = (first.as_mut_ptr() as *mut ()) as _;
// CHECK-NEXT:     out.0[0].iov_len = (unsafe {
// CHECK-NEXT:         strlen((first.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as u64
// CHECK-NEXT:     }) as _;
// CHECK-NEXT:     out.0[1].iov_base = (second.as_mut_ptr() as *mut ()) as _;
// CHECK-NEXT:     out.0[1].iov_len = (unsafe {
// CHECK-NEXT:         strlen((second.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as u64
// CHECK-NEXT:     }) as _;
// CHECK-NEXT:     r#in.0[0].iov_base = (a.as_mut_ptr() as *mut ()) as _;
// CHECK-NEXT:     r#in.0[0].iov_len = 6u64 as _;
// CHECK-NEXT:     r#in.0[1].iov_base = (b.as_mut_ptr() as *mut ()) as _;
// CHECK-NEXT:     r#in.0[1].iov_len = 5u64 as _;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         socketpair(
// CHECK-NEXT:             1i32 as i32,
// CHECK-NEXT:             (slate_support::SOCK_STREAM as i32) as i32,
// CHECK-NEXT:             0i32 as i32,
// CHECK-NEXT:             (pair.as_mut_ptr() as *mut i32) as *mut i32,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(send_message) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             ((std::mem::size_of::<slate_support::msghdr>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     send_message.msg_iov = out.0.as_mut_ptr() as *mut slate_support::iovec;
// CHECK-NEXT:     send_message.msg_iovlen = 2u64 as _;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         sendmsg(
// CHECK-NEXT:             pair[0] as i32,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(send_message) as *const slate_support::msghdr)
// CHECK-NEXT:                 as *const slate_support::msghdr,
// CHECK-NEXT:             0i32 as i32,
// CHECK-NEXT:         ) as i64
// CHECK-NEXT:     }) != 11
// CHECK-NEXT:     {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(receive_message) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             ((std::mem::size_of::<slate_support::msghdr>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     receive_message.msg_iov = r#in.0.as_mut_ptr() as *mut slate_support::iovec;
// CHECK-NEXT:     receive_message.msg_iovlen = 2u64 as _;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         recvmsg(
// CHECK-NEXT:             pair[1] as i32,
// CHECK-NEXT:             std::ptr::addr_of_mut!(receive_message) as *mut slate_support::msghdr,
// CHECK-NEXT:             0i32 as i32,
// CHECK-NEXT:         ) as i64
// CHECK-NEXT:     }) != 11
// CHECK-NEXT:     {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %zu %s%s %d\n".as_ptr(),
// CHECK-NEXT:             total_length(&out.0),
// CHECK-NEXT:             total_length(&r#in.0),
// CHECK-NEXT:             a.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             b.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             (receive_message.msg_iovlen as u64) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { close(pair[0] as i32) as i32 };
// CHECK-NEXT:     unsafe { close(pair[1] as i32) as i32 };
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
