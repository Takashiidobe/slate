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
// DEFAULT-NEXT:     global %6 u1: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 u2: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %2 @strcpy(%16 __dest: ptr<i8> [restrict], %17 __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%18 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 off1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 off2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %14 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %15 c: i8 [storage=automatic];
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %20
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%10))), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %28: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%10, read<i32>(%29));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %21
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%11, const<i32>(1));
// DEFAULT-NEXT:                             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%11))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %30: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%11, read<i32>(%31));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     for %22
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%15, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%12))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(8)))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %32: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                                             let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%12, read<i32>(%33));
// DEFAULT-NEXT:                                             let %34: i8 [synthetic] = read<i8>(%15);
// DEFAULT-NEXT:                                             let %35: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%34)), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%15, read<i8>(%35));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%6)), read<i32>(%12))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%15)), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%15, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%7)), read<i32>(%12))), read<i8>(%15));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%7)), add<i32, overflow=ub>(read<i32>(%10), read<i32>(%11)))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%13, call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%2, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%6)), read<i32>(%9)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%7)), read<i32>(%10)))));
// DEFAULT-NEXT:                                     call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%2, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%6)), read<i32>(%9)), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%7)), read<i32>(%10))));
// DEFAULT-NEXT:                                     if ne<ptr<i8>>(read<ptr<i8>>(%13), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%6)), read<i32>(%9)))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                                     write<ptr<i8>>(%14, array_decay<ptr<i8>, length=Some(97)>(field0(%6)));
// DEFAULT-NEXT:                                     for %23
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%12), read<i32>(%9))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %36: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                                             let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%12, read<i32>(%37));
// DEFAULT-NEXT:                                             let %38: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:                                             let %39: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%38), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%14, read<ptr<i8>>(%39));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%14)))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                                     for %24
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%15, truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(65), read<i32>(%10))));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%12), read<i32>(%11))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %40: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                                             let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%12, read<i32>(%41));
// DEFAULT-NEXT:                                             let %42: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:                                             let %43: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%42), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%14, read<ptr<i8>>(%43));
// DEFAULT-NEXT:                                             let %44: i8 [synthetic] = read<i8>(%15);
// DEFAULT-NEXT:                                             let %45: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%44)), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%15, read<i8>(%45));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%15)), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%15, truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%14)))), widen<i32, reason=promotion>(read<i8>(%15)))
// DEFAULT-NEXT:                                                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     let %46: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:                                     let %47: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%46), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%14, read<ptr<i8>>(%47));
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%46)))), const<i32>(0))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                                     for %25
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%12, const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%12))), const<u64>(8))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %48: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                                             let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%12, read<i32>(%49));
// DEFAULT-NEXT:                                             let %50: ptr<i8> [synthetic] = read<ptr<i8>>(%14);
// DEFAULT-NEXT:                                             let %51: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%50), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%14, read<ptr<i8>>(%51));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%14)))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
