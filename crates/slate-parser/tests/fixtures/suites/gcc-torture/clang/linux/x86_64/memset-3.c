/* Copyright (C) 2002  Free Software Foundation.

   Test memset with various combinations of constant pointer alignments and
   lengths to make sure any optimizations in the compiler are correct.

   Written by Roger Sayle, July 22, 2002.  */

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
  int   len;
  char *p;

  /* off == 0 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf, '\0', len);
    if (p != u.buf)
      abort();
    check(0, len, '\0');

    p = memset(u.buf, A, len);
    if (p != u.buf)
      abort();
    check(0, len, 'A');

    p = memset(u.buf, 'B', len);
    if (p != u.buf)
      abort();
    check(0, len, 'B');
  }

  /* off == 1 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 1, '\0', len);
    if (p != u.buf + 1)
      abort();
    check(1, len, '\0');

    p = memset(u.buf + 1, A, len);
    if (p != u.buf + 1)
      abort();
    check(1, len, 'A');

    p = memset(u.buf + 1, 'B', len);
    if (p != u.buf + 1)
      abort();
    check(1, len, 'B');
  }

  /* off == 2 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 2, '\0', len);
    if (p != u.buf + 2)
      abort();
    check(2, len, '\0');

    p = memset(u.buf + 2, A, len);
    if (p != u.buf + 2)
      abort();
    check(2, len, 'A');

    p = memset(u.buf + 2, 'B', len);
    if (p != u.buf + 2)
      abort();
    check(2, len, 'B');
  }

  /* off == 3 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 3, '\0', len);
    if (p != u.buf + 3)
      abort();
    check(3, len, '\0');

    p = memset(u.buf + 3, A, len);
    if (p != u.buf + 3)
      abort();
    check(3, len, 'A');

    p = memset(u.buf + 3, 'B', len);
    if (p != u.buf + 3)
      abort();
    check(3, len, 'B');
  }

  /* off == 4 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 4, '\0', len);
    if (p != u.buf + 4)
      abort();
    check(4, len, '\0');

    p = memset(u.buf + 4, A, len);
    if (p != u.buf + 4)
      abort();
    check(4, len, 'A');

    p = memset(u.buf + 4, 'B', len);
    if (p != u.buf + 4)
      abort();
    check(4, len, 'B');
  }

  /* off == 5 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 5, '\0', len);
    if (p != u.buf + 5)
      abort();
    check(5, len, '\0');

    p = memset(u.buf + 5, A, len);
    if (p != u.buf + 5)
      abort();
    check(5, len, 'A');

    p = memset(u.buf + 5, 'B', len);
    if (p != u.buf + 5)
      abort();
    check(5, len, 'B');
  }

  /* off == 6 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 6, '\0', len);
    if (p != u.buf + 6)
      abort();
    check(6, len, '\0');

    p = memset(u.buf + 6, A, len);
    if (p != u.buf + 6)
      abort();
    check(6, len, 'A');

    p = memset(u.buf + 6, 'B', len);
    if (p != u.buf + 6)
      abort();
    check(6, len, 'B');
  }

  /* off == 7 */
  for (len = 0; len < MAX_COPY; len++) {
    reset();

    p = memset(u.buf + 7, '\0', len);
    if (p != u.buf + 7)
      abort();
    check(7, len, '\0');

    p = memset(u.buf + 7, A, len);
    if (p != u.buf + 7)
      abort();
    check(7, len, 'A');

    p = memset(u.buf + 7, 'B', len);
    if (p != u.buf + 7)
      abort();
    check(7, len, 'B');
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
// DEFAULT-NEXT:         let %[[VALUE_len_2:[0-9]+]] len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(0), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]]))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(0), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]]))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(0), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(1), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(1), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(1), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(2), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(2), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(2), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(3), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(3), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(3), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(4), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(4), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(4), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(5), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(5), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(5), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(6), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(6), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(6), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_len_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len_2]]);
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_len_2]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_reset]]);
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(7), read<i32>(%[[VALUE_len_2]]), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7))), widen<i32, reason=arg>(read<i8>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(7), read<i32>(%[[VALUE_len_2]]), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]]))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len_2]])))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_u]])), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_check]], const<i32>(7), read<i32>(%[[VALUE_len_2]]), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
