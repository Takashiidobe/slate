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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct sdshdr8 {
// CHECK-NEXT:     len: u8,
// CHECK-NEXT:     alloc: u8,
// CHECK-NEXT:     flags: u8,
// CHECK-NEXT:     buf: [i8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct sdshdr16 {
// CHECK-NEXT:     len: u16,
// CHECK-NEXT:     alloc: u16,
// CHECK-NEXT:     flags: u8,
// CHECK-NEXT:     buf: [i8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct sdshdr64 {
// CHECK-NEXT:     len: u64,
// CHECK-NEXT:     alloc: u64,
// CHECK-NEXT:     flags: u8,
// CHECK-NEXT:     buf: [i8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: fn sdslen(s: *mut i8) -> u64 {
// CHECK-NEXT:     match ((unsafe { *s.offset(-1 as isize) }) as i32) & 7 {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return (unsafe { (*(s.offset(-(3 as isize)) as *mut sdshdr8)).len }) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return (unsafe { (*(s.offset(-(5 as isize)) as *mut sdshdr16)).len }) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             return unsafe { (*(s.offset(-(17 as isize)) as *mut sdshdr64)).len };
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn sdsavail(s: *mut i8) -> u64 {
// CHECK-NEXT:     match ((unsafe { *s.offset(-1 as isize) }) as i32) & 7 {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             let sh: *mut sdshdr8 = (unsafe { s.offset(-(3 as isize)) }) as *mut sdshdr8;
// CHECK-NEXT:             return (((((unsafe { (*sh).alloc }) as u32) as i32)
// CHECK-NEXT:                 - (((unsafe { (*sh).len }) as u32) as i32)) as i64) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             let sh: *mut sdshdr16 = (unsafe { s.offset(-(5 as isize)) })
// CHECK-NEXT:                 as *mut sdshdr16;
// CHECK-NEXT:             return (((((unsafe { (*sh).alloc }) as u32) as i32)
// CHECK-NEXT:                 - (((unsafe { (*sh).len }) as u32) as i32)) as i64) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             let sh: *mut sdshdr64 = (unsafe { s.offset(-(17 as isize)) })
// CHECK-NEXT:                 as *mut sdshdr64;
// CHECK-NEXT:             return unsafe { (*sh).alloc }.wrapping_sub(unsafe { (*sh).len });
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn sdsnew(init: *const i8, r#type: i32) -> *mut i8 {
// CHECK-NEXT:     let len: u64 = unsafe { strlen(init) };
// CHECK-NEXT:     let header: u64 = if r#type == 1 { 3 } else { if r#type == 2 { 5 } else { 17 } };
// CHECK-NEXT:     let memory: *mut i8 = (unsafe { malloc(header.wrapping_add(len).wrapping_add(16)) })
// CHECK-NEXT:         as *mut i8;
// CHECK-NEXT:     let s: *mut i8 = unsafe { memory.offset(header as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *s.offset(-1 as isize) = r#type as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match r#type {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 let sh: *mut sdshdr8 = (unsafe { s.offset(-(3 as isize)) })
// CHECK-NEXT:                     as *mut sdshdr8;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*sh).len = len as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*sh).alloc = len.wrapping_add(15) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 let sh: *mut sdshdr16 = (unsafe { s.offset(-(5 as isize)) })
// CHECK-NEXT:                     as *mut sdshdr16;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*sh).len = len as u16;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*sh).alloc = len.wrapping_add(15) as u16;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 let sh: *mut sdshdr64 = (unsafe { s.offset(-(17 as isize)) })
// CHECK-NEXT:                     as *mut sdshdr64;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*sh).len = len;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*sh).alloc = len.wrapping_add(15);
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             init as *const u8,
// CHECK-NEXT:             s as *mut u8,
// CHECK-NEXT:             len.wrapping_add(1) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-NEXT: fn sdsinclen(s: *mut i8, inc: u64) {
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match ((unsafe { *s.offset(-1 as isize) }) as i32) & 7 {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 let [[V_0:__v[0-9]+]]: *mut sdshdr8 = (unsafe { s.offset(-(3 as isize)) })
// CHECK-NEXT:                     as *mut sdshdr8;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*[[V_0]]).len = (((((*[[V_0]]).len as u32) as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_add(inc) as u8;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 let [[V_1:__v[0-9]+]]: *mut sdshdr16 = (unsafe { s.offset(-(5 as isize)) })
// CHECK-NEXT:                     as *mut sdshdr16;
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*[[V_1]]).len = (((((*[[V_1]]).len as u32) as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_add(inc) as u16;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 let [[V_2:__v[0-9]+]]: *mut sdshdr64 = (unsafe { s.offset(-(17 as isize)) })
// CHECK-NEXT:                     as *mut sdshdr64;
// CHECK-NEXT:                 let [[V_3:__v[0-9]+]]: u64 = unsafe { (*[[V_2]]).len };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*[[V_2]]).len = [[V_3]].wrapping_add(1);
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct payloadHeader {
// CHECK-NEXT:     payload_len: u64,
// CHECK-NEXT:     payload_type: u8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union epoll_data {
// CHECK-NEXT:     ptr: *mut (),
// CHECK-NEXT:     fd: i32,
// CHECK-NEXT:     u64: u64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct event {
// CHECK-NEXT:     events: u32,
// CHECK-NEXT:     data: epoll_data,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holder {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     header: payloadHeader,
// CHECK-NEXT:     events: [event; 3],
// CHECK-NEXT:     tail: i16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pack_two {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: u32,
// CHECK-NEXT:     wide: u64,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu\n".as_ptr(), 3u64, 5u64, 17u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu\n".as_ptr(), 2u64, 4u64, 17u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 9u64, 1u64, 12u64, 1u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 48u64, 2u64, 10u64, 46u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 14u64, 2u64, 2u64, 6u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let types: [i32; 3] = [1, 2, 4];
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         let s: *mut i8 = sdsnew(c"hello".as_ptr(), types[i as usize]);
// CHECK-NEXT:         sdsinclen(s, 2u64);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %zu %zu %s\n".as_ptr(),
// CHECK-NEXT:                 types[i as usize],
// CHECK-NEXT:                 sdslen(s),
// CHECK-NEXT:                 sdsavail(s),
// CHECK-NEXT:                 s,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             free(
// CHECK-NEXT:                 s
// CHECK-NEXT:                     .offset(
// CHECK-NEXT:                         -((if types[i as usize] == 1 {
// CHECK-NEXT:                             3u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if types[i as usize] == 2 { 5u64 } else { 17u64 }
// CHECK-NEXT:                         }) as isize),
// CHECK-NEXT:                     ) as *mut (),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[u8; 64]> = __SlateAlign16([0u8; 64]);
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(buffer.0.as_mut_ptr() as *mut u8, 0u8, 64 as usize) };
// CHECK-NEXT:     let mut used: u64 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 1;
// CHECK-NEXT:         while i <= 3 {
// CHECK-NEXT:             let header: *mut payloadHeader = (unsafe {
// CHECK-NEXT:                 (buffer.0.as_mut_ptr() as *mut u8).offset(used as isize)
// CHECK-NEXT:             }) as *mut payloadHeader;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*header).payload_len = ((i * 100) as i64) as u64;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*header).payload_type = (i as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             let [[V_4:__v[0-9]+]]: u64 = unsafe { (*header).payload_len };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*header).payload_len = [[V_4]].wrapping_add(7);
// CHECK-NEXT:             }
// CHECK-NEXT:             used = used.wrapping_add(9u64.wrapping_add((i as i64) as u64));
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut offset: u64 = 0;
// CHECK-NEXT:         let mut i: u64 = 1;
// CHECK-NEXT:         while offset < used {
// CHECK-NEXT:             let mut header: payloadHeader = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::ptr::copy_nonoverlapping(
// CHECK-NEXT:                     ((buffer.0.as_mut_ptr() as *mut u8).offset(offset as isize)
// CHECK-NEXT:                         as *const ()) as *const u8,
// CHECK-NEXT:                     (std::ptr::addr_of_mut!(header) as *mut ()) as *mut u8,
// CHECK-NEXT:                     9 as usize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%zu %zu %d\n".as_ptr(),
// CHECK-NEXT:                     offset,
// CHECK-NEXT:                     header.payload_len,
// CHECK-NEXT:                     (header.payload_type as u32) as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             offset = offset.wrapping_add(9u64.wrapping_add(i));
// CHECK-NEXT:             i = i.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut holder: holder = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(holder) as *mut ()) as *mut u8,
// CHECK-NEXT:             0u8,
// CHECK-NEXT:             48 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     holder.tag = 104;
// CHECK-NEXT:     holder.header.payload_len = 300;
// CHECK-NEXT:     holder.header.payload_type = (holder.header.payload_len / 7) as u8;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         holder.events[i as usize].events = (i + 1) as u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             holder.events[i as usize].data.u64 = 1234605616436508552u64
// CHECK-NEXT:                 .wrapping_add((i as i64) as u64);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         holder.events[1].data.fd = -5;
// CHECK-NEXT:     }
// CHECK-NEXT:     holder.tail = 42;
// CHECK-NEXT:     let copy: event = holder.events[2];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c %zu %u %d %llx %llx %d\n".as_ptr(),
// CHECK-NEXT:             holder.tag as i32,
// CHECK-NEXT:             holder.header.payload_len,
// CHECK-NEXT:             (holder.header.payload_type as u32) as i32,
// CHECK-NEXT:             holder.events[1].data.fd,
// CHECK-NEXT:             holder.events[0].data.u64,
// CHECK-NEXT:             copy.data.u64,
// CHECK-NEXT:             holder.tail as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut two: pack_two = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: pack_two = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = 116i8;
// CHECK-NEXT:         [[T_0]].value = 3735928559u32;
// CHECK-NEXT:         [[T_0]].wide = 72623859790382856u64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     two.value ^= 65535;
// CHECK-NEXT:     two.wide <<= 4i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%c %x %llx\n".as_ptr(), two.tag as i32, two.value, two.wide) };
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
