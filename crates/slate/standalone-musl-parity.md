# Standalone musl vs libc-shim parity audit (API-focused)
Generated: 2026-08-09T22:01:15Z
Comparison: items present in musl headers but missing from libc-shim (macros, functions, typedefs/records, enum constants).
Ignored: include guards, extern "C", #includes, comments, whitespace, enum-vs-define conversion (constants matched across macros).

## aio.h
- Status: 10 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 9 — examples: AIO_ALLDONE, AIO_CANCELED, AIO_NOTCANCELED, LIO_NOP, LIO_NOWAIT, LIO_READ, LIO_WAIT, LIO_WRITE, _AIO_H
- Missing enum/constants: 1 — examples: _AIO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## alloca.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ALLOCA_H
- Missing enum/constants: 1 — examples: _ALLOCA_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## alltypes.h.in
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## ar.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _AR_H
- Missing enum/constants: 1 — examples: _AR_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## arpa/ftp.h
- Status: 28 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 27 — examples: BLK_BYTECOUNT, BLK_EOF, BLK_EOR, BLK_ERRORS, BLK_RESTART, COMPLETE, CONTINUE, ERROR, FORM_C, FORM_N
- Missing enum/constants: 1 — examples: _ARPA_FTP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## arpa/inet.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ARPA_INET_H
- Missing enum/constants: 1 — examples: _ARPA_INET_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## arpa/nameser.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ARPA_NAMESER_H
- Missing enum/constants: 1 — examples: _ARPA_NAMESER_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## arpa/nameser_compat.h
- Status: No API items missing (after filtering).

## arpa/telnet.h
- Status: 23 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 22 — examples: ABORT, AO, AYT, BREAK, DM, DO, DONT, EC, EL, EOR
- Missing enum/constants: 1 — examples: _ARPA_TELNET_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## arpa/tftp.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ARPA_TFTP_H
- Missing enum/constants: 1 — examples: _ARPA_TFTP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## assert.h
- Status: No API items missing (after filtering).

## byteswap.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _BYTESWAP_H
- Missing enum/constants: 1 — examples: _BYTESWAP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## complex.h
- Status: 14 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 7 — examples: _COMPLEX_H, cimag, cimagf, cimagl, creal, crealf, creall
- Missing enum/constants: 7 — examples: _COMPLEX_H, cimag, cimagf, cimagl, creal, crealf, creall
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## cpio.h
- Status: 22 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 21 — examples: C_IRGRP, C_IROTH, C_IRUSR, C_ISBLK, C_ISCHR, C_ISCTG, C_ISDIR, C_ISFIFO, C_ISGID, C_ISLNK
- Missing enum/constants: 1 — examples: _CPIO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## crypt.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _CRYPT_H
- Missing enum/constants: 1 — examples: _CRYPT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## ctype.h
- Status: 22 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 11 — examples: _CTYPE_H, _tolower, _toupper, isalpha, isascii, isdigit, isgraph, islower, isprint, isspace
- Missing enum/constants: 11 — examples: _CTYPE_H, _tolower, _toupper, isalpha, isascii, isdigit, isgraph, islower, isprint, isspace
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## dirent.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _DIRENT_H
- Missing enum/constants: 1 — examples: _DIRENT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## dlfcn.h
- Status: 8 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 7 — examples: RTLD_GLOBAL, RTLD_LAZY, RTLD_LOCAL, RTLD_NODELETE, RTLD_NOLOAD, RTLD_NOW, _DLFCN_H
- Missing enum/constants: 1 — examples: _DLFCN_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## elf.h
- Status: No API items missing (after filtering).

## endian.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ENDIAN_H
- Missing enum/constants: 1 — examples: _ENDIAN_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## err.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ERR_H
- Missing enum/constants: 1 — examples: _ERR_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## errno.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ERRNO_H
- Missing enum/constants: 1 — examples: _ERRNO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## execinfo.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _EXECINFO_H
- Missing enum/constants: 1 — examples: _EXECINFO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## fcntl.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _FCNTL_H
- Missing enum/constants: 1 — examples: _FCNTL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## features.h
- Status: 4 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 2 — examples: _FEATURES_H, _Noreturn
- Missing enum/constants: 2 — examples: _FEATURES_H, _Noreturn
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## fenv.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _FENV_H
- Missing enum/constants: 1 — examples: _FENV_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## float.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## fmtmsg.h
- Status: 17 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 16 — examples: MM_APPL, MM_CONSOLE, MM_ERROR, MM_FIRM, MM_HALT, MM_HARD, MM_INFO, MM_NOSEV, MM_NRECOV, MM_OPSYS
- Missing enum/constants: 1 — examples: _FMTMSG_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## fnmatch.h
- Status: 10 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 9 — examples: FNM_CASEFOLD, FNM_FILE_NAME, FNM_LEADING_DIR, FNM_NOESCAPE, FNM_NOMATCH, FNM_NOSYS, FNM_PATHNAME, FNM_PERIOD, _FNMATCH_H
- Missing enum/constants: 1 — examples: _FNMATCH_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## ftw.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _FTW_H
- Missing enum/constants: 1 — examples: _FTW_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## getopt.h
- Status: 5 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 4 — examples: _GETOPT_H, no_argument, optional_argument, required_argument
- Missing enum/constants: 1 — examples: _GETOPT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## glob.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _GLOB_H
- Missing enum/constants: 1 — examples: _GLOB_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## grp.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _GRP_H
- Missing enum/constants: 1 — examples: _GRP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## iconv.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ICONV_H
- Missing enum/constants: 1 — examples: _ICONV_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## ifaddrs.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _IFADDRS_H
- Missing enum/constants: 1 — examples: _IFADDRS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## inttypes.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _INTTYPES_H
- Missing enum/constants: 1 — examples: _INTTYPES_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## iso646.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ISO646_H
- Missing enum/constants: 1 — examples: _ISO646_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## langinfo.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _LANGINFO_H
- Missing enum/constants: 1 — examples: _LANGINFO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## lastlog.h
- Status: No API items missing (after filtering).

## libgen.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _LIBGEN_H
- Missing enum/constants: 1 — examples: _LIBGEN_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## libintl.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _LIBINTL_H
- Missing enum/constants: 1 — examples: _LIBINTL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## limits.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _LIMITS_H
- Missing enum/constants: 1 — examples: _LIMITS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## link.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _LINK_H
- Missing enum/constants: 1 — examples: _LINK_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## locale.h
- Status: 36 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 16 — examples: GLIBC_LC_ADDRESS, GLIBC_LC_IDENTIFICATION, GLIBC_LC_MEASUREMENT, GLIBC_LC_NAME, GLIBC_LC_PAPER, GLIBC_LC_TELEPHONE, LC_ALL_MASK, LC_COLLATE_MASK, LC_CTYPE_MASK, LC_GLOBAL_LOCALE
- Missing function prototypes: 4 — examples: defined, freelocale, newlocale, uselocale
- Missing enum/constants: 16 — examples: GLIBC_LC_ADDRESS, GLIBC_LC_IDENTIFICATION, GLIBC_LC_MEASUREMENT, GLIBC_LC_NAME, GLIBC_LC_PAPER, GLIBC_LC_TELEPHONE, LC_ALL_MASK, LC_COLLATE_MASK, LC_CTYPE_MASK, LC_GLOBAL_LOCALE
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## malloc.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _MALLOC_H
- Missing enum/constants: 1 — examples: _MALLOC_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## math.h
- Status: 199 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 27 — examples: FP_FAST_FMA, FP_FAST_FMAF, FP_FAST_FMAL, FP_ILOGB0, FP_ILOGBNAN, FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO
- Missing function prototypes: 150 — examples: acosf, acosh, acoshf, acoshl, acosl, asinf, asinh, asinhf, asinhl, asinl
- Missing enum/constants: 22 — examples: FP_FAST_FMA, FP_FAST_FMAF, FP_FAST_FMAL, FP_ILOGB0, FP_ILOGBNAN, HUGE, MAXFLOAT, M_1_PI, M_2_PI, M_2_SQRTPI
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## memory.h
- Status: No API items missing (after filtering).

## mntent.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _MNTENT_H
- Missing enum/constants: 1 — examples: _MNTENT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## monetary.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _MONETARY_H
- Missing enum/constants: 1 — examples: _MONETARY_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## mqueue.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _MQUEUE_H
- Missing enum/constants: 1 — examples: _MQUEUE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## net/ethernet.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NET_ETHERNET_H
- Missing enum/constants: 1 — examples: _NET_ETHERNET_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## net/if.h
- Status: No API items missing (after filtering).

## net/if_arp.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NET_IF_ARP_H
- Missing enum/constants: 1 — examples: _NET_IF_ARP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## net/route.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NET_ROUTE_H
- Missing enum/constants: 1 — examples: _NET_ROUTE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netdb.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETDB_H
- Missing enum/constants: 1 — examples: _NETDB_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/ether.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETINET_ETHER_H
- Missing enum/constants: 1 — examples: _NETINET_ETHER_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/icmp6.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETINET_ICMP6_H
- Missing enum/constants: 1 — examples: _NETINET_ICMP6_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/if_ether.h
- Status: 3 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETINET_IF_ETHER_H
- Missing function prototypes: 1 — examples: while
- Missing enum/constants: 1 — examples: _NETINET_IF_ETHER_H
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/igmp.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETINET_IGMP_H
- Missing enum/constants: 1 — examples: _NETINET_IGMP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/in.h
- Status: No API items missing (after filtering).

## netinet/in_systm.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETINET_IN_SYSTM_H
- Missing enum/constants: 1 — examples: _NETINET_IN_SYSTM_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/ip.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NETINET_IP_H
- Missing enum/constants: 1 — examples: _NETINET_IP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/ip6.h
- Status: No API items missing (after filtering).

## netinet/ip_icmp.h
- Status: No API items missing (after filtering).

## netinet/tcp.h
- Status: 50 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 49 — examples: TCP_CC_INFO, TCP_CLOSE, TCP_CLOSE_WAIT, TCP_CLOSING, TCP_CM_INQ, TCP_CONGESTION, TCP_CORK, TCP_DEFER_ACCEPT, TCP_ESTABLISHED, TCP_FASTOPEN
- Missing enum/constants: 1 — examples: _NETINET_TCP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netinet/udp.h
- Status: 16 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 15 — examples: SOL_UDP, TCP_ENCAP_ESPINTCP, UDP_CORK, UDP_ENCAP, UDP_ENCAP_ESPINUDP, UDP_ENCAP_ESPINUDP_NON_IKE, UDP_ENCAP_GTP0, UDP_ENCAP_GTP1U, UDP_ENCAP_L2TPINUDP, UDP_ENCAP_RXRPC
- Missing enum/constants: 1 — examples: _NETINET_UDP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## netpacket/packet.h
- Status: 35 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 34 — examples: PACKET_ADD_MEMBERSHIP, PACKET_AUXDATA, PACKET_BROADCAST, PACKET_COPY_THRESH, PACKET_DROP_MEMBERSHIP, PACKET_FANOUT, PACKET_FANOUT_DATA, PACKET_FASTROUTE, PACKET_HDRLEN, PACKET_HOST
- Missing enum/constants: 1 — examples: _NETPACKET_PACKET_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## nl_types.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _NL_TYPES_H
- Missing enum/constants: 1 — examples: _NL_TYPES_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## paths.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _PATHS_H
- Missing enum/constants: 1 — examples: _PATHS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## poll.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _POLL_H
- Missing enum/constants: 1 — examples: _POLL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## pthread.h
- Status: 39 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 23 — examples: PTHREAD_CANCEL_MASKED, PTHREAD_CREATE_DETACHED, PTHREAD_CREATE_JOINABLE, PTHREAD_EXPLICIT_SCHED, PTHREAD_INHERIT_SCHED, PTHREAD_MUTEX_DEFAULT, PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_NORMAL, PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ROBUST
- Missing function prototypes: 10 — examples: _pthread_cleanup_pop, _pthread_cleanup_push, pthread_getattr_default_np, pthread_getname_np, pthread_setattr_default_np, pthread_setname_np, pthread_timedjoin_np, pthread_tryjoin_np, void, while
- Missing enum/constants: 6 — examples: PTHREAD_CANCEL_MASKED, PTHREAD_NULL, _PTHREAD_H, pthread_cleanup_pop, pthread_cleanup_push, pthread_equal
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## pty.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _PTY_H
- Missing enum/constants: 1 — examples: _PTY_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## pwd.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _PWD_H
- Missing enum/constants: 1 — examples: _PWD_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## regex.h
- Status: 21 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 21 — examples: REG_BADBR, REG_BADPAT, REG_BADRPT, REG_EBRACE, REG_EBRACK, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_ENOSYS, REG_EPAREN
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## resolv.h
- Status: 30 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 14 — examples: MAXDFLSRCH, MAXDNSRCH, MAXNS, MAXRESOLVSORT, RES_EXHAUSTIVE, RES_F_CONN, RES_F_EDNS0ERR, RES_F_VC, RES_INSECURE1, RES_INSECURE2
- Missing typedefs/records: 2 — examples: in_addr, sockaddr_in6
- Missing enum/constants: 14 — examples: MAXDFLSRCH, MAXDNSRCH, MAXNS, MAXRESOLVSORT, RES_EXHAUSTIVE, RES_F_CONN, RES_F_EDNS0ERR, RES_F_VC, RES_INSECURE1, RES_INSECURE2
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sched.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SCHED_H
- Missing enum/constants: 1 — examples: _SCHED_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## scsi/scsi.h
- Status: 131 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 130 — examples: ABORT, ABORTED_COMMAND, ALLOW_MEDIUM_REMOVAL, BLANK_CHECK, BUSY, BUS_DEVICE_RESET, CHANGE_DEFINITION, CHECK_CONDITION, COMMAND_COMPLETE, COMMAND_TERMINATED
- Missing enum/constants: 1 — examples: _SCSI_SCSI_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## scsi/scsi_ioctl.h
- Status: 10 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 9 — examples: SCSI_IOCTL_BENCHMARK_COMMAND, SCSI_IOCTL_DOORLOCK, SCSI_IOCTL_DOORUNLOCK, SCSI_IOCTL_SEND_COMMAND, SCSI_IOCTL_START_UNIT, SCSI_IOCTL_STOP_UNIT, SCSI_IOCTL_SYNC, SCSI_IOCTL_TEST_UNIT_READY, _SCSI_IOCTL_H
- Missing enum/constants: 1 — examples: _SCSI_IOCTL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## scsi/sg.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SCSI_SG_H
- Missing enum/constants: 1 — examples: _SCSI_SG_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## search.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SEARCH_H
- Missing enum/constants: 1 — examples: _SEARCH_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## semaphore.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SEMAPHORE_H
- Missing enum/constants: 1 — examples: _SEMAPHORE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## setjmp.h
- Status: 10 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 2 — examples: _SETJMP_H, setjmp
- Missing function prototypes: 5 — examples: _longjmp, defined, siglongjmp, sigsetjmp, sizeof
- Missing typedefs/records: 1 — examples: sigjmp_buf
- Missing enum/constants: 2 — examples: _SETJMP_H, setjmp
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## shadow.h
- Status: No API items missing (after filtering).

## signal.h
- Status: 20 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 19 — examples: POLL_ERR, POLL_HUP, POLL_IN, POLL_MSG, POLL_OUT, POLL_PRI, SIG_BLOCK, SIG_SETMASK, SIG_UNBLOCK, SS_AUTODISARM
- Missing enum/constants: 1 — examples: _SIGNAL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## spawn.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SPAWN_H
- Missing enum/constants: 1 — examples: _SPAWN_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stdalign.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## stdarg.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## stdbool.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## stdc-predef.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _STDC_PREDEF_H
- Missing enum/constants: 1 — examples: _STDC_PREDEF_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stddef.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _STDDEF_H
- Missing enum/constants: 1 — examples: _STDDEF_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stdint.h
- Status: 14 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _STDINT_H
- Missing typedefs/records: 12 — examples: int_fast64_t, int_fast8_t, int_least16_t, int_least32_t, int_least64_t, int_least8_t, uint_fast64_t, uint_fast8_t, uint_least16_t, uint_least32_t
- Missing enum/constants: 1 — examples: _STDINT_H
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stdio.h
- Status: 10 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 5 — examples: NULL, RENAME_EXCHANGE, RENAME_NOREPLACE, RENAME_WHITEOUT, _STDIO_H
- Missing enum/constants: 5 — examples: NULL, RENAME_EXCHANGE, RENAME_NOREPLACE, RENAME_WHITEOUT, _STDIO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stdio_ext.h
- Status: 5 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 4 — examples: FSETLOCKING_BYCALLER, FSETLOCKING_INTERNAL, FSETLOCKING_QUERY, _STDIO_EXT_H
- Missing enum/constants: 1 — examples: _STDIO_EXT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stdlib.h
- Status: 25 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 12 — examples: NULL, WCOREDUMP, WEXITSTATUS, WIFCONTINUED, WIFEXITED, WIFSIGNALED, WIFSTOPPED, WNOHANG, WSTOPSIG, WTERMSIG
- Missing function prototypes: 1 — examples: defined
- Missing enum/constants: 12 — examples: NULL, WCOREDUMP, WEXITSTATUS, WIFCONTINUED, WIFEXITED, WIFSIGNALED, WIFSTOPPED, WNOHANG, WSTOPSIG, WTERMSIG
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stdnoreturn.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## string.h
- Status: 8 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 3 — examples: NULL, _STRING_H, strdupa
- Missing function prototypes: 2 — examples: strerrordesc_np, strerrorname_np
- Missing enum/constants: 3 — examples: NULL, _STRING_H, strdupa
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## strings.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _STRINGS_H
- Missing enum/constants: 1 — examples: _STRINGS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## stropts.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## sys/acct.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_ACCT_H
- Missing enum/constants: 1 — examples: _SYS_ACCT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/auxv.h
- Status: No API items missing (after filtering).

## sys/cachectl.h
- Status: No API items missing (after filtering).

## sys/dir.h
- Status: No API items missing (after filtering).

## sys/epoll.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_EPOLL_H
- Missing enum/constants: 1 — examples: _SYS_EPOLL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/errno.h
- Status: No API items missing (after filtering).

## sys/eventfd.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_EVENTFD_H
- Missing enum/constants: 1 — examples: _SYS_EVENTFD_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/fanotify.h
- Status: No API items missing (after filtering).

## sys/fcntl.h
- Status: No API items missing (after filtering).

## sys/file.h
- Status: 9 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 8 — examples: LOCK_EX, LOCK_NB, LOCK_SH, LOCK_UN, L_INCR, L_SET, L_XTND, _SYS_FILE_H
- Missing enum/constants: 1 — examples: _SYS_FILE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/fsuid.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_FSUID_H
- Missing enum/constants: 1 — examples: _SYS_FSUID_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/inotify.h
- Status: No API items missing (after filtering).

## sys/io.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_IO_H
- Missing enum/constants: 1 — examples: _SYS_IO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/ioctl.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_IOCTL_H
- Missing enum/constants: 1 — examples: _SYS_IOCTL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/ipc.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_IPC_H
- Missing enum/constants: 1 — examples: _SYS_IPC_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/kd.h
- Status: No API items missing (after filtering).

## sys/klog.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_KLOG_H
- Missing enum/constants: 1 — examples: _SYS_KLOG_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/membarrier.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## sys/mman.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_MMAN_H
- Missing enum/constants: 1 — examples: _SYS_MMAN_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/mount.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_MOUNT_H
- Missing enum/constants: 1 — examples: _SYS_MOUNT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/msg.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_MSG_H
- Missing enum/constants: 1 — examples: _SYS_MSG_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/mtio.h
- Status: 34 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 17 — examples: MT_ST_ASYNC_WRITES, MT_ST_AUTO_LOCK, MT_ST_BUFFER_WRITES, MT_ST_CAN_BSR, MT_ST_CAN_PARTITIONS, MT_ST_CLEARBOOLEANS, MT_ST_DEBUGGING, MT_ST_DEF_BLKSIZE, MT_ST_DEF_OPTIONS, MT_ST_DEF_WRITES
- Missing enum/constants: 17 — examples: MT_ST_ASYNC_WRITES, MT_ST_AUTO_LOCK, MT_ST_BUFFER_WRITES, MT_ST_CAN_BSR, MT_ST_CAN_PARTITIONS, MT_ST_CLEARBOOLEANS, MT_ST_DEBUGGING, MT_ST_DEF_BLKSIZE, MT_ST_DEF_OPTIONS, MT_ST_DEF_WRITES
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/param.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_PARAM_H
- Missing enum/constants: 1 — examples: _SYS_PARAM_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/personality.h
- Status: No API items missing (after filtering).

## sys/poll.h
- Status: No API items missing (after filtering).

## sys/prctl.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_PRCTL_H
- Missing enum/constants: 1 — examples: _SYS_PRCTL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/procfs.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_PROCFS_H
- Missing enum/constants: 1 — examples: _SYS_PROCFS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/ptrace.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_PTRACE_H
- Missing enum/constants: 1 — examples: _SYS_PTRACE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/quota.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_QUOTA_H
- Missing enum/constants: 1 — examples: _SYS_QUOTA_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/random.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_RANDOM_H
- Missing enum/constants: 1 — examples: _SYS_RANDOM_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/reboot.h
- Status: No API items missing (after filtering).

## sys/reg.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_REG_H
- Missing enum/constants: 1 — examples: _SYS_REG_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/resource.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_RESOURCE_H
- Missing enum/constants: 1 — examples: _SYS_RESOURCE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/select.h
- Status: 14 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 7 — examples: FD_CLR, FD_ISSET, FD_SET, FD_SETSIZE, FD_ZERO, NFDBITS, _SYS_SELECT_H
- Missing function prototypes: 2 — examples: for, while
- Missing enum/constants: 5 — examples: FD_CLR, FD_ISSET, FD_SET, FD_ZERO, _SYS_SELECT_H
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/sem.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SEM_H
- Missing enum/constants: 1 — examples: _SYS_SEM_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/sendfile.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SENDFILE_H
- Missing enum/constants: 1 — examples: _SYS_SENDFILE_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/shm.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SHM_H
- Missing enum/constants: 1 — examples: _SYS_SHM_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/signal.h
- Status: No API items missing (after filtering).

## sys/signalfd.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SIGNALFD_H
- Missing enum/constants: 1 — examples: _SYS_SIGNALFD_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/socket.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SOCKET_H
- Missing enum/constants: 1 — examples: _SYS_SOCKET_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/soundcard.h
- Status: No API items missing (after filtering).

## sys/stat.h
- Status: 27 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 12 — examples: STATX_ATTR_APPEND, STATX_ATTR_AUTOMOUNT, STATX_ATTR_COMPRESSED, STATX_ATTR_DAX, STATX_ATTR_ENCRYPTED, STATX_ATTR_IMMUTABLE, STATX_ATTR_MOUNT_ROOT, STATX_ATTR_NODUMP, STATX_ATTR_VERITY, STATX_ATTR_WRITE_ATOMIC
- Missing function prototypes: 1 — examples: statx
- Missing typedefs/records: 2 — examples: statx, statx_timestamp
- Missing enum/constants: 12 — examples: STATX_ATTR_APPEND, STATX_ATTR_AUTOMOUNT, STATX_ATTR_COMPRESSED, STATX_ATTR_DAX, STATX_ATTR_ENCRYPTED, STATX_ATTR_IMMUTABLE, STATX_ATTR_MOUNT_ROOT, STATX_ATTR_NODUMP, STATX_ATTR_VERITY, STATX_ATTR_WRITE_ATOMIC
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/statfs.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_STATFS_H
- Missing enum/constants: 1 — examples: _SYS_STATFS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/statvfs.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_STATVFS_H
- Missing enum/constants: 1 — examples: _SYS_STATVFS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/stropts.h
- Status: MISSING in libc-shim/include
- Action: consider adding this header or creating a shim that exposes the API below (if any).

## sys/swap.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SWAP_H
- Missing enum/constants: 1 — examples: _SYS_SWAP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/syscall.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SYSCALL_H
- Missing enum/constants: 1 — examples: _SYS_SYSCALL_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/sysinfo.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SYSINFO_H
- Missing enum/constants: 1 — examples: _SYS_SYSINFO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/syslog.h
- Status: No API items missing (after filtering).

## sys/sysmacros.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_SYSMACROS_H
- Missing enum/constants: 1 — examples: _SYS_SYSMACROS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/termios.h
- Status: No API items missing (after filtering).

## sys/time.h
- Status: 19 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 11 — examples: ITIMER_PROF, ITIMER_REAL, ITIMER_VIRTUAL, TIMESPEC_TO_TIMEVAL, TIMEVAL_TO_TIMESPEC, _SYS_TIME_H, timeradd, timerclear, timercmp, timerisset
- Missing enum/constants: 8 — examples: TIMESPEC_TO_TIMEVAL, TIMEVAL_TO_TIMESPEC, _SYS_TIME_H, timeradd, timerclear, timercmp, timerisset, timersub
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/timeb.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_TIMEB_H
- Missing enum/constants: 1 — examples: _SYS_TIMEB_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/timerfd.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_TIMERFD_H
- Missing enum/constants: 1 — examples: _SYS_TIMERFD_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/times.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_TIMES_H
- Missing enum/constants: 1 — examples: _SYS_TIMES_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/timex.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_TIMEX_H
- Missing enum/constants: 1 — examples: _SYS_TIMEX_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/ttydefaults.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_TTYDEFAULTS_H
- Missing enum/constants: 1 — examples: _SYS_TTYDEFAULTS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/types.h
- Status: 22 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 6 — examples: _SYS_TYPES_H, blkcnt64_t, fsblkcnt64_t, fsfilcnt64_t, ino64_t, off64_t
- Missing typedefs/records: 10 — examples: caddr_t, quad_t, u_char, u_int16_t, u_int32_t, u_int8_t, u_quad_t, uint, ulong, ushort
- Missing enum/constants: 6 — examples: _SYS_TYPES_H, blkcnt64_t, fsblkcnt64_t, fsfilcnt64_t, ino64_t, off64_t
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/ucontext.h
- Status: No API items missing (after filtering).

## sys/uio.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_UIO_H
- Missing enum/constants: 1 — examples: _SYS_UIO_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/un.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_UN_H
- Missing enum/constants: 1 — examples: _SYS_UN_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/user.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_USER_H
- Missing enum/constants: 1 — examples: _SYS_USER_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/utsname.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYS_UTSNAME_H
- Missing enum/constants: 1 — examples: _SYS_UTSNAME_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/vfs.h
- Status: No API items missing (after filtering).

## sys/vt.h
- Status: No API items missing (after filtering).

## sys/wait.h
- Status: 18 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 9 — examples: WCOREDUMP, WEXITSTATUS, WIFCONTINUED, WIFEXITED, WIFSIGNALED, WIFSTOPPED, WSTOPSIG, WTERMSIG, _SYS_WAIT_H
- Missing enum/constants: 9 — examples: WCOREDUMP, WEXITSTATUS, WIFCONTINUED, WIFEXITED, WIFSIGNALED, WIFSTOPPED, WSTOPSIG, WTERMSIG, _SYS_WAIT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## sys/xattr.h
- Status: 4 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 3 — examples: XATTR_CREATE, XATTR_REPLACE, _SYS_XATTR_H
- Missing enum/constants: 1 — examples: _SYS_XATTR_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## syscall.h
- Status: No API items missing (after filtering).

## sysexits.h
- Status: 20 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 19 — examples: EX_CANTCREAT, EX_CONFIG, EX_DATAERR, EX_IOERR, EX_NOHOST, EX_NOINPUT, EX_NOPERM, EX_NOUSER, EX_OK, EX_OSERR
- Missing enum/constants: 1 — examples: _SYSEXITS_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## syslog.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _SYSLOG_H
- Missing enum/constants: 1 — examples: _SYSLOG_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## tar.h
- Status: 14 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 13 — examples: TGEXEC, TGREAD, TGWRITE, TOEXEC, TOREAD, TOWRITE, TSGID, TSUID, TSVTX, TUEXEC
- Missing enum/constants: 1 — examples: _TAR_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## termios.h
- Status: 11 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 2 — examples: NCCS, _TERMIOS_H
- Missing function prototypes: 3 — examples: tcgetsid, tcgetwinsize, tcsetwinsize
- Missing typedefs/records: 4 — examples: cc_t, speed_t, tcflag_t, winsize
- Missing enum/constants: 2 — examples: NCCS, _TERMIOS_H
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## tgmath.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _TGMATH_H
- Missing enum/constants: 1 — examples: _TGMATH_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## threads.h
- Status: 4 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 2 — examples: _THREADS_H, thrd_equal
- Missing enum/constants: 2 — examples: _THREADS_H, thrd_equal
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## time.h
- Status: 17 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 15 — examples: CLOCK_BOOTTIME, CLOCK_BOOTTIME_ALARM, CLOCK_MONOTONIC, CLOCK_MONOTONIC_COARSE, CLOCK_MONOTONIC_RAW, CLOCK_PROCESS_CPUTIME_ID, CLOCK_REALTIME, CLOCK_REALTIME_ALARM, CLOCK_REALTIME_COARSE, CLOCK_SGI_CYCLE
- Missing enum/constants: 2 — examples: NULL, _TIME_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## uchar.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _UCHAR_H
- Missing enum/constants: 1 — examples: _UCHAR_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## ucontext.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _UCONTEXT_H
- Missing enum/constants: 1 — examples: _UCONTEXT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## ulimit.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _ULIMIT_H
- Missing enum/constants: 1 — examples: _ULIMIT_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## unistd.h
- Status: 4 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 2 — examples: NULL, _UNISTD_H
- Missing enum/constants: 2 — examples: NULL, _UNISTD_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## utime.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _UTIME_H
- Missing enum/constants: 1 — examples: _UTIME_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## utmp.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _UTMP_H
- Missing enum/constants: 1 — examples: _UTMP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## utmpx.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _UTMPX_H
- Missing enum/constants: 1 — examples: _UTMPX_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## values.h
- Status: 2 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 1 — examples: _VALUES_H
- Missing enum/constants: 1 — examples: _VALUES_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## wait.h
- Status: No API items missing (after filtering).

## wchar.h
- Status: 40 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 5 — examples: NULL, WCHAR_MAX, WCHAR_MIN, _WCHAR_H, iswdigit
- Missing function prototypes: 30 — examples: btowc, defined, fgetws_unlocked, fputwc_unlocked, fputws_unlocked, getwc_unlocked, getwchar_unlocked, iswblank, mbsnrtowcs, putwc_unlocked
- Missing enum/constants: 5 — examples: NULL, WCHAR_MAX, WCHAR_MIN, _WCHAR_H, iswdigit
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## wctype.h
- Status: 5 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 2 — examples: _WCTYPE_H, iswdigit
- Missing typedefs/records: 1 — examples: wctrans_t
- Missing enum/constants: 2 — examples: _WCTYPE_H, iswdigit
- Suggested action: Focus on POSIX types (off_t, ssize_t, ino_t, mode_t) and function prototypes. Add missing macro/constant definitions or map enum constants to #defines for compatibility.

## wordexp.h
- Status: 14 API items in musl missing from libc-shim (categorized below).
- Missing macros (#define): 13 — examples: WRDE_APPEND, WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_DOOFFS, WRDE_NOCMD, WRDE_NOSPACE, WRDE_NOSYS, WRDE_REUSE, WRDE_SHOWERR
- Missing enum/constants: 1 — examples: _WORDEXP_H
- Suggested action: Add missing macro/constant definitions or map enum constants to #defines for compatibility.
