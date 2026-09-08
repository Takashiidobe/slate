#ifndef _SLATE_SYS_RSEQ_H
#define _SLATE_SYS_RSEQ_H

#include <features.h>
#include <stdint.h>

struct rseq_slice_ctrl {
  union {
    uint32_t all;
    struct {
      uint8_t request;
      uint8_t granted;
      uint16_t __reserved;
    };
  };
};

struct rseq_cs {
  uint32_t version;
  uint32_t flags;
  uint64_t start_ip;
  uint64_t post_commit_offset;
  uint64_t abort_ip;
} __attribute__((aligned(32)));

struct rseq {
  uint32_t cpu_id_start;
  uint32_t cpu_id;
  uint64_t rseq_cs;
  uint32_t flags;
#if !defined(__SLATE_ARCH_ARM)
  uint32_t node_id;
  uint32_t mm_cid;
  struct rseq_slice_ctrl slice_ctrl;
  uint8_t __reserved;
  char end[];
#endif
} __attribute__((aligned(32)));

#endif
