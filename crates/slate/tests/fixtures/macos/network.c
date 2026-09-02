#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

_Static_assert(sizeof(sa_family_t) == 1, "sa_family_t");
_Static_assert(sizeof(socklen_t) == 4, "socklen_t");
_Static_assert(sizeof(struct sockaddr) == 16, "sockaddr");
_Static_assert(offsetof(struct sockaddr, sa_family) == 1, "sa_family");
_Static_assert(sizeof(struct sockaddr_storage) == 128, "sockaddr_storage");
_Static_assert(_Alignof(struct sockaddr_storage) == 8,
               "sockaddr_storage alignment");
_Static_assert(sizeof(struct sockaddr_in) == 16, "sockaddr_in");
_Static_assert(offsetof(struct sockaddr_in, sin_family) == 1, "sin_family");
_Static_assert(sizeof(struct sockaddr_in6) == 28, "sockaddr_in6");
_Static_assert(offsetof(struct sockaddr_in6, sin6_addr) == 8, "sin6_addr");
_Static_assert(sizeof(struct sockaddr_un) == 106, "sockaddr_un");
_Static_assert(offsetof(struct sockaddr_un, sun_path) == 2, "sun_path");
_Static_assert(sizeof(struct msghdr) == 48, "msghdr");
_Static_assert(offsetof(struct msghdr, msg_iovlen) == 24, "msg_iovlen");
_Static_assert(sizeof(struct cmsghdr) == 12, "cmsghdr");
_Static_assert(CMSG_LEN(1) == 13, "CMSG_LEN");
_Static_assert(CMSG_SPACE(1) == 16, "CMSG_SPACE");
_Static_assert(sizeof(struct addrinfo) == 48, "addrinfo");
_Static_assert(offsetof(struct addrinfo, ai_canonname) == 24, "ai_canonname");
_Static_assert(offsetof(struct addrinfo, ai_addr) == 32, "ai_addr");
_Static_assert(sizeof(struct ifaddrs) == 56, "ifaddrs");
_Static_assert(AF_INET6 == 30, "AF_INET6");
_Static_assert(SOL_SOCKET == 0xffff, "SOL_SOCKET");
_Static_assert(SO_NOSIGPIPE == 0x1022, "SO_NOSIGPIPE");
_Static_assert(AI_NUMERICSERV == 0x1000, "AI_NUMERICSERV");
_Static_assert(NI_NUMERICSCOPE == 0x100, "NI_NUMERICSCOPE");
_Static_assert(EAI_FAMILY == 5, "EAI_FAMILY");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&recvfrom),
                                 ssize_t (*)(int, void *, size_t, int,
                                             struct sockaddr *__restrict,
                                             socklen_t *__restrict)),
    "recvfrom signature");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&getnameinfo),
                                 int (*)(const struct sockaddr *__restrict,
                                         socklen_t, char *__restrict, socklen_t,
                                         char *__restrict, socklen_t, int)),
    "getnameinfo signature");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&inet_ntop),
                                 const char *(*)(int, const void *__restrict,
                                                 char *__restrict, socklen_t)),
    "inet_ntop signature");

#ifdef SOCK_CLOEXEC
#error "Linux SOCK_CLOEXEC leaked into Darwin"
#endif

#ifdef MSG_CMSG_CLOEXEC
#error "Linux MSG_CMSG_CLOEXEC leaked into Darwin"
#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_listen_ipv4(struct sockaddr_in *address) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket_fd < 0)
    return socket_fd;
  address->sin_len         = sizeof(*address);
  address->sin_family      = AF_INET;
  address->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  return bind(socket_fd, (const struct sockaddr *)address, sizeof(*address)) ||
         listen(socket_fd, 8);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_bind_local(int socket_fd, struct sockaddr_un *address,
                     const char *path) {
  address->sun_len    = sizeof(*address);
  address->sun_family = AF_UNIX;
  strcpy(address->sun_path, path);
  return bind(socket_fd, (const struct sockaddr *)address, SUN_LEN(address));
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_resolve(const char *node, struct addrinfo **result) {
  struct addrinfo hints = {0};
  hints.ai_family       = AF_INET6;
  hints.ai_socktype     = SOCK_DGRAM;
  hints.ai_flags        = AI_NUMERICSERV | AI_ADDRCONFIG;
  int status            = getaddrinfo(node, "53", &hints, result);
  if (status == 0)
    freeaddrinfo(*result);
  return status;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
unsigned char *slate_control_data(struct msghdr *message) {
  struct cmsghdr *control = CMSG_FIRSTHDR(message);
  return control == 0 ? 0 : CMSG_DATA(control);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
unsigned int slate_first_interface_flags(void) {
  struct ifaddrs *addresses = 0;
  if (getifaddrs(&addresses) != 0 || addresses == 0)
    return 0;
  unsigned int flags = addresses->ifa_flags;
  freeifaddrs(addresses);
  return flags;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS-DAG: unsafe fn slate_listen_ipv4({{arg[0-9]+}}: *mut sockaddr_in) -> i32 {
// LOWERING-MACOS-DAG:     let mut __retval: i32 = 0;
// LOWERING-MACOS-DAG:     let mut socket_fd: i32 = 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { socket({{_v[0-9]+}} as i32, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:     socket_fd = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = socket_fd;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = socket_fd;
// LOWERING-MACOS-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-MACOS-DAG:             return {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u8 = 16;
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{arg[0-9]+}}).sin_len = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u8 = 2;
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{arg[0-9]+}}).sin_family = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = 2130706433;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = unsafe { htonl({{_v[0-9]+}} as u32) };
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{arg[0-9]+}}).sin_addr.s_addr = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = socket_fd;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut sockaddr = {{arg[0-9]+}} as *mut sockaddr;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = 16;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { bind({{_v[0-9]+}} as i32, {{_v[0-9]+}} as *const sockaddr, {{_v[0-9]+}} as u32) };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = socket_fd;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { listen({{_v[0-9]+}} as i32, {{_v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: unsafe fn slate_bind_local({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut sockaddr_un, {{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u8 = 106;
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{arg[0-9]+}}).sun_len = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u8 = 1;
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         (*{{arg[0-9]+}}).sun_family = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).sun_path) }) as *mut i8;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-MACOS-DAG:         strcpy(
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     }) as *mut i8;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut sockaddr = {{arg[0-9]+}} as *mut sockaddr;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = 106;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = 104;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).sun_path) }) as *mut i8;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { bind({{arg[0-9]+}} as i32, {{_v[0-9]+}} as *const sockaddr, {{_v[0-9]+}} as u32) };
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: unsafe fn slate_resolve({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut addrinfo) -> i32 {
// LOWERING-MACOS-DAG:     let mut result: *mut *mut addrinfo = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut hints: addrinfo = addrinfo {
// LOWERING-MACOS-DAG:         ai_flags: 0,
// LOWERING-MACOS-DAG:         ai_family: 0,
// LOWERING-MACOS-DAG:         ai_socktype: 0,
// LOWERING-MACOS-DAG:         ai_protocol: 0,
// LOWERING-MACOS-DAG:         ai_addrlen: 0,
// LOWERING-MACOS-DAG:         ai_canonname: std::ptr::null_mut(),
// LOWERING-MACOS-DAG:         ai_addr: std::ptr::null_mut(),
// LOWERING-MACOS-DAG:         ai_next: std::ptr::null_mut(),
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let mut status: i32 = 0;
// LOWERING-MACOS-DAG:     result = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     hints = addrinfo {
// LOWERING-MACOS-DAG:         ai_flags: 0,
// LOWERING-MACOS-DAG:         ai_family: 0,
// LOWERING-MACOS-DAG:         ai_socktype: 0,
// LOWERING-MACOS-DAG:         ai_protocol: 0,
// LOWERING-MACOS-DAG:         ai_addrlen: 0,
// LOWERING-MACOS-DAG:         ai_canonname: std::ptr::null_mut(),
// LOWERING-MACOS-DAG:         ai_addr: std::ptr::null_mut(),
// LOWERING-MACOS-DAG:         ai_next: std::ptr::null_mut(),
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 30;
// LOWERING-MACOS-DAG:     hints.ai_family = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-MACOS-DAG:     hints.ai_socktype = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 4096;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 1024;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     hints.ai_flags = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = b"53\0".as_ptr() as *mut i8;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut *mut addrinfo = result;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         getaddrinfo(
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-MACOS-DAG:             std::ptr::addr_of_mut!(hints) as *const addrinfo,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *mut *mut addrinfo,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     status = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = status;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut *mut addrinfo = result;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut addrinfo = unsafe { *{{_v[0-9]+}} };
// LOWERING-MACOS-DAG:             unsafe { freeaddrinfo({{_v[0-9]+}} as *mut addrinfo) };
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = status;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: unsafe fn slate_control_data({{arg[0-9]+}}: *mut msghdr) -> *mut u8 {
// LOWERING-MACOS-DAG:     let mut message: *mut msghdr = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut control: *mut cmsghdr = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     message = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut msghdr = message;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = unsafe { (*{{_v[0-9]+}}).msg_controllen };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<cmsghdr>() as u64;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut cmsghdr = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut msghdr = message;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { (*{{_v[0-9]+}}).msg_control };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut cmsghdr = {{_v[0-9]+}} as *mut cmsghdr;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut cmsghdr = std::ptr::null_mut();
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     control = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut cmsghdr = control;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut cmsghdr = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut u8 = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut cmsghdr = control;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<cmsghdr>() as u64;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: u64 = 3;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: u64 = 18446744073709551612u64;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_first_interface_flags() -> u32 {
// LOWERING-MACOS-DAG:     let mut __retval: u32 = 0;
// LOWERING-MACOS-DAG:     let mut addresses: *mut ifaddrs = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut ifaddrs = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     addresses = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 =
// LOWERING-MACOS-DAG:             unsafe { getifaddrs(std::ptr::addr_of_mut!(addresses) as *mut *mut ifaddrs) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: bool = true;
// LOWERING-MACOS-DAG:             {{_v[0-9]+}}
// LOWERING-MACOS-DAG:         } else {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut ifaddrs = addresses;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut ifaddrs = std::ptr::null_mut();
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             {{_v[0-9]+}}
// LOWERING-MACOS-DAG:         };
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: u32 = 0;
// LOWERING-MACOS-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: u32 = __retval;
// LOWERING-MACOS-DAG:             return {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut ifaddrs = addresses;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = unsafe { (*{{_v[0-9]+}}).ifa_flags };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut ifaddrs = addresses;
// LOWERING-MACOS-DAG:     unsafe { freeifaddrs({{_v[0-9]+}} as *mut ifaddrs) };
// LOWERING-MACOS-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u32 = __retval;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS-DAG: unsafe fn slate_listen_ipv4({{arg[0-9]+}}: *mut sockaddr_in) -> i32 {
// REWRITES-MACOS-DAG:     let mut socket_fd: i32 = unsafe { socket(2 as i32, 1 as i32, 6 as i32) };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = socket_fd < 0;
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         return socket_fd;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{arg[0-9]+}}).sin_len = 16;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{arg[0-9]+}}).sin_family = 2;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u32 = unsafe { htonl(2130706433 as u32) };
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{arg[0-9]+}}).sin_addr.s_addr = {{_v[0-9]+}};
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { bind(socket_fd as i32, {{arg[0-9]+}} as *const sockaddr, 16 as u32) };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { listen(socket_fd as i32, 8 as i32) };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}} as i32
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: unsafe fn slate_bind_local({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut sockaddr_un, {{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{arg[0-9]+}}).sun_len = 106;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         (*{{arg[0-9]+}}).sun_family = 1;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).sun_path) }) as *mut i8;
// REWRITES-MACOS-DAG:     (unsafe {
// REWRITES-MACOS-DAG:         strcpy(
// REWRITES-MACOS-DAG:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *const core::ffi::c_char,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     }) as *mut i8;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut sockaddr = {{arg[0-9]+}} as *mut sockaddr;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u64 = 104;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u64 = 106 - {{_v[0-9]+}};
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{arg[0-9]+}}).sun_path) }) as *mut i8;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-MACOS-DAG:     unsafe { bind({{arg[0-9]+}} as i32, {{_v[0-9]+}} as *const sockaddr, ({{_v[0-9]+}} + {{_v[0-9]+}}) as u32) }
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: unsafe fn slate_resolve({{arg[0-9]+}}: *mut i8, mut result: *mut *mut addrinfo) -> i32 {
// REWRITES-MACOS-DAG:     let mut hints: addrinfo = addrinfo {
// REWRITES-MACOS-DAG:         ai_flags: 0,
// REWRITES-MACOS-DAG:         ai_family: 0,
// REWRITES-MACOS-DAG:         ai_socktype: 0,
// REWRITES-MACOS-DAG:         ai_protocol: 0,
// REWRITES-MACOS-DAG:         ai_addrlen: 0,
// REWRITES-MACOS-DAG:         ai_canonname: std::ptr::null_mut(),
// REWRITES-MACOS-DAG:         ai_addr: std::ptr::null_mut(),
// REWRITES-MACOS-DAG:         ai_next: std::ptr::null_mut(),
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let mut status: i32 = 0;
// REWRITES-MACOS-DAG:     hints.ai_family = 30;
// REWRITES-MACOS-DAG:     hints.ai_socktype = 2;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 1024;
// REWRITES-MACOS-DAG:     hints.ai_flags = 4096 | {{_v[0-9]+}};
// REWRITES-MACOS-DAG:     status = unsafe {
// REWRITES-MACOS-DAG:         getaddrinfo(
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *const core::ffi::c_char,
// REWRITES-MACOS-DAG:             c"53".as_ptr(),
// REWRITES-MACOS-DAG:             std::ptr::addr_of_mut!(hints) as *const addrinfo,
// REWRITES-MACOS-DAG:             result as *mut *mut addrinfo,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = status == 0;
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         unsafe { freeaddrinfo((unsafe { *result }) as *mut addrinfo) };
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     status
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: unsafe fn slate_control_data(mut message: *mut msghdr) -> *mut u8 {
// REWRITES-MACOS-DAG:     let mut control: *mut cmsghdr = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u64 = (unsafe { (*message).msg_controllen }) as u64;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<cmsghdr>() as u64;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut cmsghdr = if {{_v[0-9]+}} >= {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut cmsghdr = (unsafe { (*message).msg_control }) as *mut cmsghdr;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut cmsghdr = std::ptr::null_mut();
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     control = {{_v[0-9]+}};
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = control == std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut u8 = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut u8 = control as *mut u8;
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<cmsghdr>() as u64;
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 3 & 18446744073709551612u64;
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_first_interface_flags() -> u32 {
// REWRITES-MACOS-DAG:     let mut addresses: *mut ifaddrs = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { getifaddrs(std::ptr::addr_of_mut!(addresses) as *mut *mut ifaddrs) };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = true;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = addresses == std::ptr::null_mut();
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         return 0;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: u32 = unsafe { (*addresses).ifa_flags };
// REWRITES-MACOS-DAG:     unsafe { freeifaddrs(addresses as *mut ifaddrs) };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}}
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
