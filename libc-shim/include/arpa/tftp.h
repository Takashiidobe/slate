#ifndef _SLATE_ARPA_TFTP_H
#define _SLATE_ARPA_TFTP_H

#include <features.h>

#define SEGSIZE 512
#define RRQ     01
#define WRQ     02
#define DATA    03
#define ACK     04
#define ERROR   05
struct tftphdr {
#if defined(__SLATE_LIBC_GLIBC)
  short th_opcode;
  union {
    char tu_padding[3];
    struct {
      union {
        unsigned short tu_block;
        short          tu_code;
      } __packed th_u3;
      char tu_data[0];
    } __packed th_u2;
    char tu_stuff[0];
  } __packed th_u1;
#else
  short th_opcode;
  union {
    unsigned short tu_block;
    short          tu_code;
    char           tu_stuff[1];
  } th_u;
  char th_data[1];
#endif
#if defined(__SLATE_LIBC_GLIBC)
} __packed;
#else
};
#endif
#if defined(__SLATE_LIBC_GLIBC)
#define th_block th_u1.th_u2.th_u3.tu_block
#define th_code th_u1.th_u2.th_u3.tu_code
#define th_stuff th_u1.tu_stuff
#define th_data th_u1.th_u2.tu_data
#define th_msg th_u1.th_u2.tu_data
#else
#define th_block  th_u.tu_block
#define th_code   th_u.tu_code
#define th_stuff  th_u.tu_stuff
#define th_msg    th_data
#endif
#define EUNDEF    0
#define ENOTFOUND 1
#define EACCESS   2
#define ENOSPACE  3
#define EBADOP    4
#define EBADID    5
#define EEXISTS   6
#define ENOUSER   7

#endif
