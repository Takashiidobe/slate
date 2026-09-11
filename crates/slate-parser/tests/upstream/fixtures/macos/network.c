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

int slate_bind_local(int socket_fd, struct sockaddr_un *address,
                     const char *path) {
  address->sun_len    = sizeof(*address);
  address->sun_family = AF_UNIX;
  strcpy(address->sun_path, path);
  return bind(socket_fd, (const struct sockaddr *)address, SUN_LEN(address));
}

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

unsigned char *slate_control_data(struct msghdr *message) {
  struct cmsghdr *control = CMSG_FIRSTHDR(message);
  return control == 0 ? 0 : CMSG_DATA(control);
}

unsigned int slate_first_interface_flags(void) {
  struct ifaddrs *addresses = 0;
  if (getifaddrs(&addresses) != 0 || addresses == 0)
    return 0;
  unsigned int flags = addresses->ifa_flags;
  freeifaddrs(addresses);
  return flags;
}


