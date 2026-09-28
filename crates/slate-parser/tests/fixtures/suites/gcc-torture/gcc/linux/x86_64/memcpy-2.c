/* Copyright (C) 2002  Free Software Foundation.

   Test memcpy with various combinations of pointer alignments and lengths to
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

/* Use a sequence length that is not divisible by two, to make it more
   likely to detect when words are mixed up.  */
#define SEQUENCE_LENGTH 31

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u1, u2;

int main(void) {
  int   off1, off2, len, i;
  char *p, *q, c;

  for (off1 = 0; off1 < MAX_OFFSET; off1++)
    for (off2 = 0; off2 < MAX_OFFSET; off2++)
      for (len = 1; len < MAX_COPY; len++) {
        for (i = 0, c = 'A'; i < MAX_LENGTH; i++, c++) {
          u1.buf[i] = 'a';
          if (c >= 'A' + SEQUENCE_LENGTH)
            c = 'A';
          u2.buf[i] = c;
        }

        p = memcpy(u1.buf + off1, u2.buf + off2, len);
        if (p != u1.buf + off1)
          abort();

        q = u1.buf;
        for (i = 0; i < off1; i++, q++)
          if (*q != 'a')
            abort();

        for (i = 0, c = 'A' + off2; i < len; i++, q++, c++) {
          if (c >= 'A' + SEQUENCE_LENGTH)
            c = 'A';
          if (*q != c)
            abort();
        }

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
// DEFAULT-NEXT:     global %8 u1: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 u2: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %4 @memcpy(%18 __dest: ptr<void> [restrict], %19 __src: ptr<const void> [restrict], %20 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%21 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 off1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 off2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %16 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %17 c: i8 [storage=automatic];
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %23
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%12))), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %31: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%12, read<i32>(%32));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %24
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%13, const<i32>(1));
// DEFAULT-NEXT:                             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %33: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%13, read<i32>(%34));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     for %25
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%17, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8))), const<u64>(8)))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %35: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                                             let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%14, read<i32>(%36));
// DEFAULT-NEXT:                                             let %37: i8 [synthetic] = read<i8>(%17);
// DEFAULT-NEXT:                                             let %38: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%37)), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%17, read<i8>(%38));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%8)), read<i32>(%14))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%17)), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%17, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%9)), read<i32>(%14))), read<i8>(%17));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     write<ptr<i8>>(%15, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%8)), read<i32>(%11))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%9)), read<i32>(%12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%13))))));
// DEFAULT-NEXT:                                     pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%8)), read<i32>(%11))), pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%9)), read<i32>(%12))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%13)))));
// DEFAULT-NEXT:                                     if ne<ptr<i8>>(read<ptr<i8>>(%15), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(field0(%8)), read<i32>(%11)))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                                     write<ptr<i8>>(%16, array_decay<ptr<i8>, length=Some(96)>(field0(%8)));
// DEFAULT-NEXT:                                     for %26
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%14), read<i32>(%11))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %39: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                                             let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%14, read<i32>(%40));
// DEFAULT-NEXT:                                             let %41: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                                             let %42: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%41), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%16, read<ptr<i8>>(%42));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%16)))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                                     for %27
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%17, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(65), read<i32>(%12))));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%14), read<i32>(%13))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %43: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                                             let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%14, read<i32>(%44));
// DEFAULT-NEXT:                                             let %45: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                                             let %46: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%45), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%16, read<ptr<i8>>(%46));
// DEFAULT-NEXT:                                             let %47: i8 [synthetic] = read<i8>(%17);
// DEFAULT-NEXT:                                             let %48: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%47)), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%17, read<i8>(%48));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%17)), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%17, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%16)))), widen<i32, reason=promotion>(read<i8>(%17)))
// DEFAULT-NEXT:                                                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     for %28
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), const<u64>(8))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %49: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                                             let %50: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%14, read<i32>(%50));
// DEFAULT-NEXT:                                             let %51: ptr<i8> [synthetic] = read<ptr<i8>>(%16);
// DEFAULT-NEXT:                                             let %52: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%51), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%16, read<ptr<i8>>(%52));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%16)))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
