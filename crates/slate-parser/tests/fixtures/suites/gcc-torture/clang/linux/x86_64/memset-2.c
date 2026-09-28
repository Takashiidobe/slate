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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 31>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %8 u: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 A: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(65)) [linkage=external];
// DEFAULT-NEXT:     fn %4 @memset(%21 __s: ptr<void>, %22 __c: i32, %23 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%24 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @reset() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15)))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%45));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%11))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @check(%13 off: i32, %14 len: i32, %15 ch: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(31)>(field0(%8)));
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), read<i32>(%13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%47));
// DEFAULT-NEXT:                 let %48: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                 let %49: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%48), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%16, read<ptr<i8>>(%49));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%16)))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), read<i32>(%14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%51));
// DEFAULT-NEXT:                 let %52: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                 let %53: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%52), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%16, read<ptr<i8>>(%53));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%16)))), read<i32>(%15))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%17))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %54: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%55));
// DEFAULT-NEXT:                 let %56: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                 let %57: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%56), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%16, read<ptr<i8>>(%57));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%16)))), const<i32>(97))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %19 off: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %29
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%59));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(1), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(1), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %60: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%61));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(2), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(2), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %31
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %62: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%63));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(3), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(3), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(3), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %32
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %64: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %65: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%65));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(4), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(4), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %66: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %67: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%66), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%67));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(5), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(5), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(5), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %68: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %69: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%69));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(6), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(6), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(6), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %35
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %70: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%71));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(7), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(7), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(7), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %72: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%72), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%73));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(8), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(8), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(8), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %37
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %74: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%75));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(9), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(9), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(9), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %38
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %76: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %77: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%76), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%77));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(10), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(10), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(10), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %39
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %78: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%78), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%79));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(11), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(11), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(11), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %80: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %81: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%80), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%81));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(12), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(12), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(12), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %41
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %82: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %83: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%82), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%83));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(13), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(13), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(13)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(13), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %42
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %84: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %85: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%84), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%85));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(14), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(14), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(14)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(14), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %43
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%19))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %86: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %87: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%87));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(15), const<i32>(0));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), widen<i32, reason=arg>(read<i8>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(15), const<i32>(65));
// DEFAULT-NEXT:                     write<ptr<i8>>(%20, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15))))));
// DEFAULT-NEXT:                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(15)))));
// DEFAULT-NEXT:                     if ne<ptr<i8>>(read<ptr<i8>>(%20), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%8)), read<i32>(%19)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, i32) -> void>(%12, read<i32>(%19), const<i32>(15), const<i32>(66));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
