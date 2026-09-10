#ifndef _SLATE_NETINET_IF_TR_H
#define _SLATE_NETINET_IF_TR_H

#include <features.h>
#include <sys/types.h>
#include <stdint.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<netinet/if_tr.h> is unavailable for this libc profile."
#endif

#define TR_ALEN 6
#define AC 0x10
#define LLC_FRAME 0x40
#define EXTENDED_SAP 0xAA
#define UI_CMD 0x03
struct trh_hdr { uint8_t ac; uint8_t fc; uint8_t daddr[TR_ALEN]; uint8_t saddr[TR_ALEN]; uint16_t rcf; uint16_t rseg[8]; };
struct trllc { uint8_t dsap; uint8_t ssap; uint8_t llc; uint8_t protid[3]; uint16_t ethertype; };
#define TR_HLEN (sizeof(struct trh_hdr) + sizeof(struct trllc))
struct tr_statistics {
  unsigned long rx_packets, tx_packets, rx_bytes, tx_bytes, rx_errors, tx_errors;
  unsigned long rx_dropped, tx_dropped, multicast, transmit_collision;
  unsigned long line_errors, internal_errors, burst_errors, A_C_errors, abort_delimiters;
  unsigned long lost_frames, recv_congest_count, frame_copied_errors, frequency_errors;
  unsigned long token_errors, dummy1;
};
#define TR_RII 0x80
#define TR_RCF_DIR_BIT 0x80
#define TR_RCF_LEN_MASK 0x1f00
#define TR_RCF_BROADCAST 0x8000
#define TR_RCF_LIMITED_BROADCAST 0xC000
#define TR_RCF_FRAME2K 0x20
#define TR_RCF_BROADCAST_MASK 0xC000
#define TR_MAXRIFLEN 18
#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
struct trn_hdr { uint8_t trn_ac; uint8_t trn_fc; uint8_t trn_dhost[TR_ALEN]; uint8_t trn_shost[TR_ALEN]; uint16_t trn_rcf; uint16_t trn_rseg[8]; };
#endif

#endif
