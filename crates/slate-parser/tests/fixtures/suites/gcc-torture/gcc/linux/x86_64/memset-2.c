/* Copyright (C) 2002  Free Software Foundation.

   Test memset with various combinations of pointer alignments and constant
   lengths to make sure any optimizations in the compiler are correct.

   Written by Roger Sayle, April 22, 2002.  */

#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_COPY
#define MAX_COPY 15
#endif

#ifndef MAX_EXTRA
#define MAX_EXTRA (sizeof(long long))
#endif

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u;

char A = 'A';

void reset() {
  int i;

  for (i = 0; i < MAX_LENGTH; i++)
    u.buf[i] = 'a';
}

void check(int off, int len, int ch) {
  char *q;
  int   i;

  q = u.buf;
  for (i = 0; i < off; i++, q++)
    if (*q != 'a')
      abort();

  for (i = 0; i < len; i++, q++)
    if (*q != ch)
      abort();

  for (i = 0; i < MAX_EXTRA; i++, q++)
    if (*q != 'a')
      abort();
}

int main() {
  int   off;
  char *p;

  /* len == 1 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 1);
    if (p != u.buf + off)
      abort();
    check(off, 1, '\0');

    p = memset(u.buf + off, A, 1);
    if (p != u.buf + off)
      abort();
    check(off, 1, 'A');

    p = memset(u.buf + off, 'B', 1);
    if (p != u.buf + off)
      abort();
    check(off, 1, 'B');
  }

  /* len == 2 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 2);
    if (p != u.buf + off)
      abort();
    check(off, 2, '\0');

    p = memset(u.buf + off, A, 2);
    if (p != u.buf + off)
      abort();
    check(off, 2, 'A');

    p = memset(u.buf + off, 'B', 2);
    if (p != u.buf + off)
      abort();
    check(off, 2, 'B');
  }

  /* len == 3 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 3);
    if (p != u.buf + off)
      abort();
    check(off, 3, '\0');

    p = memset(u.buf + off, A, 3);
    if (p != u.buf + off)
      abort();
    check(off, 3, 'A');

    p = memset(u.buf + off, 'B', 3);
    if (p != u.buf + off)
      abort();
    check(off, 3, 'B');
  }

  /* len == 4 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 4);
    if (p != u.buf + off)
      abort();
    check(off, 4, '\0');

    p = memset(u.buf + off, A, 4);
    if (p != u.buf + off)
      abort();
    check(off, 4, 'A');

    p = memset(u.buf + off, 'B', 4);
    if (p != u.buf + off)
      abort();
    check(off, 4, 'B');
  }

  /* len == 5 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 5);
    if (p != u.buf + off)
      abort();
    check(off, 5, '\0');

    p = memset(u.buf + off, A, 5);
    if (p != u.buf + off)
      abort();
    check(off, 5, 'A');

    p = memset(u.buf + off, 'B', 5);
    if (p != u.buf + off)
      abort();
    check(off, 5, 'B');
  }

  /* len == 6 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 6);
    if (p != u.buf + off)
      abort();
    check(off, 6, '\0');

    p = memset(u.buf + off, A, 6);
    if (p != u.buf + off)
      abort();
    check(off, 6, 'A');

    p = memset(u.buf + off, 'B', 6);
    if (p != u.buf + off)
      abort();
    check(off, 6, 'B');
  }

  /* len == 7 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 7);
    if (p != u.buf + off)
      abort();
    check(off, 7, '\0');

    p = memset(u.buf + off, A, 7);
    if (p != u.buf + off)
      abort();
    check(off, 7, 'A');

    p = memset(u.buf + off, 'B', 7);
    if (p != u.buf + off)
      abort();
    check(off, 7, 'B');
  }

  /* len == 8 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 8);
    if (p != u.buf + off)
      abort();
    check(off, 8, '\0');

    p = memset(u.buf + off, A, 8);
    if (p != u.buf + off)
      abort();
    check(off, 8, 'A');

    p = memset(u.buf + off, 'B', 8);
    if (p != u.buf + off)
      abort();
    check(off, 8, 'B');
  }

  /* len == 9 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 9);
    if (p != u.buf + off)
      abort();
    check(off, 9, '\0');

    p = memset(u.buf + off, A, 9);
    if (p != u.buf + off)
      abort();
    check(off, 9, 'A');

    p = memset(u.buf + off, 'B', 9);
    if (p != u.buf + off)
      abort();
    check(off, 9, 'B');
  }

  /* len == 10 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 10);
    if (p != u.buf + off)
      abort();
    check(off, 10, '\0');

    p = memset(u.buf + off, A, 10);
    if (p != u.buf + off)
      abort();
    check(off, 10, 'A');

    p = memset(u.buf + off, 'B', 10);
    if (p != u.buf + off)
      abort();
    check(off, 10, 'B');
  }

  /* len == 11 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 11);
    if (p != u.buf + off)
      abort();
    check(off, 11, '\0');

    p = memset(u.buf + off, A, 11);
    if (p != u.buf + off)
      abort();
    check(off, 11, 'A');

    p = memset(u.buf + off, 'B', 11);
    if (p != u.buf + off)
      abort();
    check(off, 11, 'B');
  }

  /* len == 12 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 12);
    if (p != u.buf + off)
      abort();
    check(off, 12, '\0');

    p = memset(u.buf + off, A, 12);
    if (p != u.buf + off)
      abort();
    check(off, 12, 'A');

    p = memset(u.buf + off, 'B', 12);
    if (p != u.buf + off)
      abort();
    check(off, 12, 'B');
  }

  /* len == 13 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 13);
    if (p != u.buf + off)
      abort();
    check(off, 13, '\0');

    p = memset(u.buf + off, A, 13);
    if (p != u.buf + off)
      abort();
    check(off, 13, 'A');

    p = memset(u.buf + off, 'B', 13);
    if (p != u.buf + off)
      abort();
    check(off, 13, 'B');
  }

  /* len == 14 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 14);
    if (p != u.buf + off)
      abort();
    check(off, 14, '\0');

    p = memset(u.buf + off, A, 14);
    if (p != u.buf + off)
      abort();
    check(off, 14, 'A');

    p = memset(u.buf + off, 'B', 14);
    if (p != u.buf + off)
      abort();
    check(off, 14, 'B');
  }

  /* len == 15 */
  for (off = 0; off < MAX_OFFSET; off++) {
    reset();

    p = memset(u.buf + off, '\0', 15);
    if (p != u.buf + off)
      abort();
    check(off, 15, '\0');

    p = memset(u.buf + off, A, 15);
    if (p != u.buf + off)
      abort();
    check(off, 15, 'A');

    p = memset(u.buf + off, 'B', 15);
    if (p != u.buf + off)
      abort();
    check(off, 15, 'B');
  }

  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 31>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_A:[0-9]+]] A: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(65)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_reset:[0-9]+]] @reset() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15)))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_off:[0-9]+]] off: i32, %[[VALUE_len:[0-9]+]] len: i32, %[[VALUE_ch:[0-9]+]] ch: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_q]], array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_off]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE13]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), read<i32>(%[[VALUE_ch]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE18]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_off_2:[0-9]+]] off: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(1), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(1), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(2), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(2), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(3), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(3), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(4), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(4), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(5), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(5), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(5), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(6), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(6), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(6), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(7), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(7), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(7), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(8), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(8), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(8), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(9), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(9), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(9), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE47]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(10), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(10), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE50]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(11), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(11), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(11), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(12), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(12), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(12), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE57:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE56]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE57]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(13), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(13), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(13), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(14), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(14), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(14), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off_2]]);
// DEFAULT-NEXT:                 let %[[VALUE63:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off_2]], read<i32>(%[[VALUE63]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(15), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(15), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], read<i32>(%[[VALUE_off_2]]), const<i32>(15), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
