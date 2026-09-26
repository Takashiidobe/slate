/* Copyright (C) 2002  Free Software Foundation.

   Test memset with various combinations of pointer alignments and lengths to
   make sure any optimizations in the library are correct.

   Written by Michael Meissner, March 9, 2002.  */

#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_COPY
#define MAX_COPY (10 * sizeof(long long))
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

int main(void) {
  int   off, len, i;
  char *p, *q;

  for (off = 0; off < MAX_OFFSET; off++)
    for (len = 1; len < MAX_COPY; len++) {
      for (i = 0; i < MAX_LENGTH; i++)
        u.buf[i] = 'a';

      p = memset(u.buf + off, '\0', len);
      if (p != u.buf + off)
        abort();

      q = u.buf;
      for (i = 0; i < off; i++, q++)
        if (*q != 'a')
          abort();

      for (i = 0; i < len; i++, q++)
        if (*q != '\0')
          abort();

      for (i = 0; i < MAX_EXTRA; i++, q++)
        if (*q != 'a')
          abort();

      p = memset(u.buf + off, A, len);
      if (p != u.buf + off)
        abort();

      q = u.buf;
      for (i = 0; i < off; i++, q++)
        if (*q != 'a')
          abort();

      for (i = 0; i < len; i++, q++)
        if (*q != 'A')
          abort();

      for (i = 0; i < MAX_EXTRA; i++, q++)
        if (*q != 'a')
          abort();

      p = memset(u.buf + off, 'B', len);
      if (p != u.buf + off)
        abort();

      q = u.buf;
      for (i = 0; i < off; i++, q++)
        if (*q != 'a')
          abort();

      for (i = 0; i < len; i++, q++)
        if (*q != 'B')
          abort();

      for (i = 0; i < MAX_EXTRA; i++, q++)
        if (*q != 'a')
          abort();
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
// DEFAULT-NEXT:         field0 buf: array<i8, 96>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=96, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %5 u: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 A: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(65)) [linkage=external];
// DEFAULT-NEXT:     fn %1 @memset(%13 __s: ptr<void>, %14 __c: i32, %15 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%16 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 off: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %12 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %18
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%9, const<i32>(1));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %31: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%9, read<i32>(%32));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %19
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8))), const<u64>(8)))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %33: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%34));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%10))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                             write<ptr<i8>>(%11, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9))))));
// DEFAULT-NEXT:                             pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9)))));
// DEFAULT-NEXT:                             if ne<ptr<i8>>(read<ptr<i8>>(%11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             write<ptr<i8>>(%12, array_decay<ptr<i8>, length=Some(96)>(field0(%5)));
// DEFAULT-NEXT:                             for %20
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%10), read<i32>(%8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %35: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%36));
// DEFAULT-NEXT:                                     let %37: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %38: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%37), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%38));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             for %21
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%10), read<i32>(%9))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %39: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%40));
// DEFAULT-NEXT:                                     let %41: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %42: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%41), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%42));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(0))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             for %22
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %43: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%44));
// DEFAULT-NEXT:                                     let %45: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %46: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%45), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%46));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             write<ptr<i8>>(%11, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9))))));
// DEFAULT-NEXT:                             pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8))), widen<i32, reason=arg>(read<i8>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9)))));
// DEFAULT-NEXT:                             if ne<ptr<i8>>(read<ptr<i8>>(%11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             write<ptr<i8>>(%12, array_decay<ptr<i8>, length=Some(96)>(field0(%5)));
// DEFAULT-NEXT:                             for %23
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%10), read<i32>(%8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %47: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%48));
// DEFAULT-NEXT:                                     let %49: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %50: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%49), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%50));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             for %24
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%10), read<i32>(%9))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %51: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %52: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%51), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%52));
// DEFAULT-NEXT:                                     let %53: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %54: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%53), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%54));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(65))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             for %25
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %55: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %56: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%55), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%56));
// DEFAULT-NEXT:                                     let %57: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %58: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%57), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%58));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             write<ptr<i8>>(%11, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9))))));
// DEFAULT-NEXT:                             pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8))), const<i32>(66), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%9)))));
// DEFAULT-NEXT:                             if ne<ptr<i8>>(read<ptr<i8>>(%11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%5)), read<i32>(%8)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             write<ptr<i8>>(%12, array_decay<ptr<i8>, length=Some(96)>(field0(%5)));
// DEFAULT-NEXT:                             for %26
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%10), read<i32>(%8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %59: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %60: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%59), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%60));
// DEFAULT-NEXT:                                     let %61: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %62: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%61), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%62));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             for %27
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%10), read<i32>(%9))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %63: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %64: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%63), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%64));
// DEFAULT-NEXT:                                     let %65: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %66: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%65), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%66));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(66))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                             for %28
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %67: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                     let %68: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%67), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%10, read<i32>(%68));
// DEFAULT-NEXT:                                     let %69: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %70: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%69), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%70));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
