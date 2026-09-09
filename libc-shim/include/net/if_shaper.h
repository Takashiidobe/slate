#ifndef _SLATE_NET_IF_SHAPER_H
#define _SLATE_NET_IF_SHAPER_H

#include <net/if.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define SHAPER_QLEN      10
#define SHAPER_LATENCY   (5 * HZ)
#define SHAPER_MAXSLIP   2
#define SHAPER_BURST     (HZ / 50)
#define SHAPER_SET_DEV   0x0001
#define SHAPER_SET_SPEED 0x0002
#define SHAPER_GET_DEV   0x0003
#define SHAPER_GET_SPEED 0x0004

struct shaperconf {
  uint16_t ss_cmd;
  union {
    char     ssu_name[14];
    uint32_t ssu_speed;
  } ss_u;
};

#define ss_speed ss_u.ssu_speed
#define ss_name  ss_u.ssu_name

#endif
