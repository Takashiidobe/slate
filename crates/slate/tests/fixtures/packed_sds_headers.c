#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *sds;

struct __attribute__((__packed__)) sdshdr8 {
  uint8_t len;
  uint8_t alloc;
  unsigned char flags;
  char buf[];
};

struct __attribute__((__packed__)) sdshdr16 {
  uint16_t len;
  uint16_t alloc;
  unsigned char flags;
  char buf[];
};

struct __attribute__((__packed__)) sdshdr64 {
  uint64_t len;
  uint64_t alloc;
  unsigned char flags;
  char buf[];
};

#define SDS_TYPE_8 1
#define SDS_TYPE_16 2
#define SDS_TYPE_64 4
#define SDS_HDR(T, s) ((struct sdshdr##T *)((s) - (sizeof(struct sdshdr##T))))

static size_t sdslen(const sds s) {
  switch (s[-1] & 7) {
  case SDS_TYPE_8:
    return SDS_HDR(8, s)->len;
  case SDS_TYPE_16:
    return SDS_HDR(16, s)->len;
  case SDS_TYPE_64:
    return SDS_HDR(64, s)->len;
  }
  return 0;
}

static size_t sdsavail(const sds s) {
  switch (s[-1] & 7) {
  case SDS_TYPE_8: {
    struct sdshdr8 *sh = SDS_HDR(8, s);
    return sh->alloc - sh->len;
  }
  case SDS_TYPE_16: {
    struct sdshdr16 *sh = SDS_HDR(16, s);
    return sh->alloc - sh->len;
  }
  case SDS_TYPE_64: {
    struct sdshdr64 *sh = SDS_HDR(64, s);
    return sh->alloc - sh->len;
  }
  }
  return 0;
}

static sds sdsnew(const char *init, int type) {
  size_t len = strlen(init);
  size_t header = type == SDS_TYPE_8    ? sizeof(struct sdshdr8)
                  : type == SDS_TYPE_16 ? sizeof(struct sdshdr16)
                                        : sizeof(struct sdshdr64);
  char *memory = malloc(header + len + 16);
  sds s = memory + header;
  s[-1] = (char)type;
  switch (type) {
  case SDS_TYPE_8: {
    struct sdshdr8 *sh = SDS_HDR(8, s);
    sh->len = (uint8_t)len;
    sh->alloc = (uint8_t)(len + 15);
    break;
  }
  case SDS_TYPE_16: {
    struct sdshdr16 *sh = SDS_HDR(16, s);
    sh->len = (uint16_t)len;
    sh->alloc = (uint16_t)(len + 15);
    break;
  }
  case SDS_TYPE_64: {
    struct sdshdr64 *sh = SDS_HDR(64, s);
    sh->len = len;
    sh->alloc = len + 15;
    break;
  }
  }
  memcpy(s, init, len + 1);
  return s;
}

static void sdsinclen(sds s, size_t inc) {
  switch (s[-1] & 7) {
  case SDS_TYPE_8:
    SDS_HDR(8, s)->len += inc;
    break;
  case SDS_TYPE_16:
    SDS_HDR(16, s)->len += inc;
    break;
  case SDS_TYPE_64:
    SDS_HDR(64, s)->len++;
    break;
  }
}

typedef struct __attribute__((__packed__)) payloadHeader {
  size_t payload_len;
  uint8_t payload_type;
} payloadHeader;

typedef union epoll_data {
  void *ptr;
  int fd;
  uint64_t u64;
} epoll_data_t;

struct __attribute__((__packed__)) event {
  uint32_t events;
  epoll_data_t data;
};

struct holder {
  char tag;
  payloadHeader header;
  struct event events[3];
  short tail;
};

#pragma pack(push, 2)
struct pack_two {
  char tag;
  uint32_t value;
  uint64_t wide;
};
#pragma pack(pop)

int main(void) {
  printf("%zu %zu %zu\n", sizeof(struct sdshdr8), sizeof(struct sdshdr16),
         sizeof(struct sdshdr64));
  printf("%zu %zu %zu\n", offsetof(struct sdshdr16, alloc),
         offsetof(struct sdshdr16, flags), offsetof(struct sdshdr64, buf));
  printf("%zu %zu %zu %zu\n", sizeof(payloadHeader), _Alignof(payloadHeader),
         sizeof(struct event), _Alignof(struct event));
  printf("%zu %zu %zu %zu\n", sizeof(struct holder), _Alignof(struct holder),
         offsetof(struct holder, events), offsetof(struct holder, tail));
  printf("%zu %zu %zu %zu\n", sizeof(struct pack_two),
         _Alignof(struct pack_two), offsetof(struct pack_two, value),
         offsetof(struct pack_two, wide));

  int types[] = {SDS_TYPE_8, SDS_TYPE_16, SDS_TYPE_64};
  for (int i = 0; i < 3; i++) {
    sds s = sdsnew("hello", types[i]);
    sdsinclen(s, 2);
    printf("%d %zu %zu %s\n", types[i], sdslen(s), sdsavail(s), s);
    free(s - (types[i] == SDS_TYPE_8    ? sizeof(struct sdshdr8)
              : types[i] == SDS_TYPE_16 ? sizeof(struct sdshdr16)
                                        : sizeof(struct sdshdr64)));
  }

  unsigned char buffer[64];
  memset(buffer, 0, sizeof buffer);
  size_t used = 0;
  for (int i = 1; i <= 3; i++) {
    payloadHeader *header = (payloadHeader *)(buffer + used);
    header->payload_len = (size_t)(i * 100);
    header->payload_type = (uint8_t)i;
    header->payload_len += 7;
    used += sizeof(payloadHeader) + (size_t)i;
  }
  for (size_t offset = 0, i = 1; offset < used; i++) {
    payloadHeader header;
    memcpy(&header, buffer + offset, sizeof header);
    printf("%zu %zu %d\n", offset, header.payload_len, header.payload_type);
    offset += sizeof(payloadHeader) + i;
  }

  struct holder holder;
  memset(&holder, 0, sizeof holder);
  holder.tag = 'h';
  holder.header.payload_len = 300;
  holder.header.payload_type = (uint8_t)(holder.header.payload_len / 7);
  for (int i = 0; i < 3; i++) {
    holder.events[i].events = (uint32_t)(i + 1);
    holder.events[i].data.u64 = 0x1122334455667788ull + (uint64_t)i;
  }
  holder.events[1].data.fd = -5;
  holder.tail = 42;
  struct event copy = holder.events[2];
  printf("%c %zu %u %d %llx %llx %d\n", holder.tag, holder.header.payload_len,
         holder.header.payload_type, holder.events[1].data.fd,
         (unsigned long long)holder.events[0].data.u64,
         (unsigned long long)copy.data.u64, holder.tail);

  struct pack_two two = {'t', 0xdeadbeef, 0x0102030405060708ull};
  two.value ^= 0xffff;
  two.wide <<= 4;
  printf("%c %x %llx\n", two.tag, two.value, (unsigned long long)two.wide);
  return 0;
}
