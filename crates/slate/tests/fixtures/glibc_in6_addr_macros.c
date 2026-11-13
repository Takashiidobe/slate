#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

static int classify(const struct sockaddr_in6 *address) {
  if (IN6_IS_ADDR_UNSPECIFIED(&address->sin6_addr)) {
    return 0;
  }
  if (IN6_IS_ADDR_V4MAPPED(&address->sin6_addr)) {
    return 4;
  }
  return 6;
}

int main(void) {
  const char         *inputs[] = {"::", "::ffff:10.0.0.1", "2001:db8::1"};
  struct sockaddr_in6 address;
  unsigned            sum = 0;

  for (int i = 0; i < 3; i++) {
    memset(&address, 0, sizeof address);
    address.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, inputs[i], &address.sin6_addr) != 1) {
      return 1;
    }
    for (int byte = 0; byte < 16; byte++) {
      sum += address.sin6_addr.s6_addr[byte];
    }
    printf("%s: %d\n", inputs[i], classify(&address));
  }
  address.sin6_addr.s6_addr[15] = 7;
  printf("sum: %u last: %u\n", sum, address.sin6_addr.s6_addr[15]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn htonl([[ARG_0:arg[0-9]+]]: u32) -> u32;
// CHECK-NEXT:     fn inet_pton([[ARG_0]]: i32, [[ARG_1:arg[0-9]+]]: *const i8, [[ARG_2:arg[0-9]+]]: *mut ()) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct in6_addr {
// CHECK-NEXT:     __in6_u: __SlateRecord_in_h_225,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_in_h_225 {
// CHECK-NEXT:     __u6_addr8: [u8; 16],
// CHECK-NEXT:     __u6_addr16: [u16; 8],
// CHECK-NEXT:     __u6_addr32: [u32; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct sockaddr_in6 {
// CHECK-NEXT:     sin6_family: u16,
// CHECK-NEXT:     sin6_port: u16,
// CHECK-NEXT:     sin6_flowinfo: u32,
// CHECK-NEXT:     sin6_addr: in6_addr,
// CHECK-NEXT:     sin6_scope_id: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: fn classify(address: &sockaddr_in6) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __a: *const in6_addr = unsafe {
// CHECK-NEXT:             std::ptr::addr_of!((* (address as * const sockaddr_in6)).sin6_addr)
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(0) })
// CHECK-NEXT:             == 0
// CHECK-NEXT:             && (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(1) })
// CHECK-NEXT:                 == 0
// CHECK-NEXT:             && (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(2) })
// CHECK-NEXT:                 == 0
// CHECK-NEXT:             && (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(3) })
// CHECK-NEXT:                 == 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     let __a: *const in6_addr = unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (address as * const sockaddr_in6)).sin6_addr)
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_1]] = (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(0) }) == 0
// CHECK-NEXT:         && (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(1) }) == 0
// CHECK-NEXT:         && (unsafe { *((*__a).__in6_u.__u6_addr32.as_ptr() as *const u32).add(2) })
// CHECK-NEXT:             == unsafe { htonl(65535u32) };
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     6
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let inputs: __SlateAlign16<[*const i8; 3]> = __SlateAlign16([
// CHECK-NEXT:         c"::".as_ptr(),
// CHECK-NEXT:         c"::ffff:10.0.0.1".as_ptr(),
// CHECK-NEXT:         c"2001:db8::1".as_ptr(),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut address: sockaddr_in6 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut sum: u32 = 0;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_bytes(
// CHECK-NEXT:                 ((std::ptr::addr_of_mut!(address) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                     as *mut u8,
// CHECK-NEXT:                 (0i32 as i32) as u8,
// CHECK-NEXT:                 ((std::mem::size_of::<sockaddr_in6>() as u64) as usize) as usize,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         address.sin6_family = 10;
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             inet_pton(
// CHECK-NEXT:                 10i32,
// CHECK-NEXT:                 inputs.0[i as usize],
// CHECK-NEXT:                 std::ptr::addr_of_mut!(address.sin6_addr) as *mut (),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != 1
// CHECK-NEXT:         {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         for byte in 0i32..16i32 {
// CHECK-NEXT:             sum = sum
// CHECK-NEXT:                 .wrapping_add(
// CHECK-NEXT:                     ((unsafe { address.sin6_addr.__in6_u.__u6_addr8[byte as usize] })
// CHECK-NEXT:                         as i32) as u32,
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%s: %d\n".as_ptr(),
// CHECK-NEXT:                 inputs.0[i as usize],
// CHECK-NEXT:                 classify(&(*(std::ptr::addr_of_mut!(address) as *const sockaddr_in6))),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         address.sin6_addr.__in6_u.__u6_addr8[15] = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"sum: %u last: %u\n".as_ptr(),
// CHECK-NEXT:             sum,
// CHECK-NEXT:             address.sin6_addr.__in6_u.__u6_addr8[15] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
