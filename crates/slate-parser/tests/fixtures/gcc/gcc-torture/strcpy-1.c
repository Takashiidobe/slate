/* Copyright (C) 2002  Free Software Foundation.

   Test strcpy with various combinations of pointer alignments and lengths to
   make sure any optimizations in the library are correct.  */

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

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + 1 + MAX_EXTRA)

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
        u2.buf[off2 + len] = '\0';

        p = strcpy(u1.buf + off1, u2.buf + off2);
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

        if (*q++ != '\0')
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 97>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=112, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %4 u1: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 u2: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @strcpy(%14 __dest: ptr<i8> [restrict], %15 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 off1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 off2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %12 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %13 c: i8 [storage=automatic];
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %18
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8))), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %26: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%8, read<i32>(%27));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %19
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%9, const<i32>(1));
// DEFAULT-NEXT:                             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %28: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%9, read<i32>(%29));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     for %20
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%13, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(8)))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %30: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                             let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%10, read<i32>(%31));
// DEFAULT-NEXT:                                             let %32: i8 [synthetic] = read<i8>(%13);
// DEFAULT-NEXT:                                             let %33: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%32)), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%13, read<i8>(%33));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%4)), read<i32>(%10))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%13)), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%13, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%5)), read<i32>(%10))), read<i8>(%13));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%5)), add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9)))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%11, call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%0, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%4)), read<i32>(%7)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%5)), read<i32>(%8)))));
// DEFAULT-NEXT:                                     call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%0, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%4)), read<i32>(%7)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%5)), read<i32>(%8))));
// DEFAULT-NEXT:                                     if ne<ptr<i8>>(read<ptr<i8>>(%11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%4)), read<i32>(%7)))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, array_decay<ptr<i8>, length=Some(97)>(field0(%4)));
// DEFAULT-NEXT:                                     for %21
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%10), read<i32>(%7))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %34: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                             let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%10, read<i32>(%35));
// DEFAULT-NEXT:                                             let %36: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                             let %37: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%36), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%12, read<ptr<i8>>(%37));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                                     for %22
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%13, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(65), read<i32>(%8))));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%10), read<i32>(%9))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %38: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                             let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%10, read<i32>(%39));
// DEFAULT-NEXT:                                             let %40: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                             let %41: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%40), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%12, read<ptr<i8>>(%41));
// DEFAULT-NEXT:                                             let %42: i8 [synthetic] = read<i8>(%13);
// DEFAULT-NEXT:                                             let %43: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%42)), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%13, read<i8>(%43));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%13)), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%13, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), widen<i32, reason=promotion>(read<i8>(%13)))
// DEFAULT-NEXT:                                                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     let %44: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %45: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%44), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%45));
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%44)))), const<i32>(0))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                                     for %23
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), const<u64>(8))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %46: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                                             let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%10, read<i32>(%47));
// DEFAULT-NEXT:                                             let %48: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                             let %49: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%48), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%12, read<ptr<i8>>(%49));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%12)))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
