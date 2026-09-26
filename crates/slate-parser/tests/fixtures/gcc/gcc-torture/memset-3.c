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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 31>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %5 u: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 A: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(65)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @memset(%18 __s: ptr<void>, %19 __c: i32, %20 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%21 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @reset() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15)))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), read<i32>(%8))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @check(%10 off: i32, %11 len: i32, %12 ch: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %14 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%13, array_decay<ptr<i8>, length=Some(31)>(field0(%5)));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%37));
// DEFAULT-NEXT:                 let %38: ptr<i8> [synthetic] = read<ptr<i8>>(%13);
// DEFAULT-NEXT:                 let %39: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%13, read<ptr<i8>>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%13)))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%41));
// DEFAULT-NEXT:                 let %42: ptr<i8> [synthetic] = read<ptr<i8>>(%13);
// DEFAULT-NEXT:                 let %43: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%42), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%13, read<ptr<i8>>(%43));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%13)))), read<i32>(%12))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%45));
// DEFAULT-NEXT:                 let %46: ptr<i8> [synthetic] = read<ptr<i8>>(%13);
// DEFAULT-NEXT:                 let %47: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%46), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%13, read<ptr<i8>>(%47));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%13)))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%5))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%5))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), array_decay<ptr<i8>, length=Some(31)>(field0(%5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(0), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%5))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%5))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), array_decay<ptr<i8>, length=Some(31)>(field0(%5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(0), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%5))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(field0(%5))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), array_decay<ptr<i8>, length=Some(31)>(field0(%5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(0), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%51));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(1), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(1), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(1)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(1), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %52: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%53));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(2), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(2), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(2)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(2), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %29
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %54: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%55));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(3), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(3), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(3)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(3), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %56: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%57));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(4), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(4), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(4)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(4), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %31
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%59));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(5), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(5), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(5)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(5), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %32
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %60: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%61));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(6), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(6), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(6)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(6), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %62: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%63));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(7), read<i32>(%16), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(7), read<i32>(%16), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%17, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%17), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%5)), const<i32>(7)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%9, const<i32>(7), read<i32>(%16), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
