/* Copyright (C) 2002  Free Software Foundation.

   Test strcmp with various combinations of pointer alignments and lengths to
   make sure any optimizations in the library are correct.

   Written by Michael Meissner, March 9, 2002.  */

#include <stddef.h>
#include <string.h>

void abort(void);
void exit(int);

#ifndef MAX_OFFSET
#define MAX_OFFSET (sizeof(long long))
#endif

#ifndef MAX_TEST
#define MAX_TEST (8 * sizeof(long long))
#endif

#ifndef MAX_EXTRA
#define MAX_EXTRA (sizeof(long long))
#endif

#define MAX_LENGTH (MAX_OFFSET + MAX_TEST + MAX_EXTRA + 2)

static union {
  unsigned char buf[MAX_LENGTH];
  long long     align_int;
  long double   align_fp;
} u1, u2;

void test(const unsigned char *s1, const unsigned char *s2, int expected) {
  int value = strcmp((char *)s1, (char *)s2);

  if (expected < 0 && value >= 0)
    abort();
  else if (expected == 0 && value != 0)
    abort();
  else if (expected > 0 && value <= 0)
    abort();
}

int main(void) {
  size_t         off1, off2, len, i;
  unsigned char *buf1, *buf2;
  unsigned char *mod1, *mod2;
  unsigned char *p1, *p2;

  for (off1 = 0; off1 < MAX_OFFSET; off1++)
    for (off2 = 0; off2 < MAX_OFFSET; off2++)
      for (len = 0; len < MAX_TEST; len++) {
        p1 = u1.buf;
        for (i = 0; i < off1; i++)
          *p1++ = '\0';

        buf1 = p1;
        for (i = 0; i < len; i++)
          *p1++ = 'a';

        mod1 = p1;
        for (i = 0; i < MAX_EXTRA + 2; i++)
          *p1++ = 'x';

        p2 = u2.buf;
        for (i = 0; i < off2; i++)
          *p2++ = '\0';

        buf2 = p2;
        for (i = 0; i < len; i++)
          *p2++ = 'a';

        mod2 = p2;
        for (i = 0; i < MAX_EXTRA + 2; i++)
          *p2++ = 'x';

        mod1[0] = '\0';
        mod2[0] = '\0';
        test(buf1, buf2, 0);

        mod1[0] = 'a';
        mod1[1] = '\0';
        mod2[0] = '\0';
        test(buf1, buf2, +1);

        mod1[0] = '\0';
        mod2[0] = 'a';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = 'b';
        mod1[1] = '\0';
        mod2[0] = 'c';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = 'c';
        mod1[1] = '\0';
        mod2[0] = 'b';
        mod2[1] = '\0';
        test(buf1, buf2, +1);

        mod1[0] = 'b';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\251';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = (unsigned char)'\251';
        mod1[1] = '\0';
        mod2[0] = 'b';
        mod2[1] = '\0';
        test(buf1, buf2, +1);

        mod1[0] = (unsigned char)'\251';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\252';
        mod2[1] = '\0';
        test(buf1, buf2, -1);

        mod1[0] = (unsigned char)'\252';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\251';
        mod2[1] = '\0';
        test(buf1, buf2, +1);
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
// DEFAULT-NEXT:         field0 buf: array<u8, 82>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=96, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %5 u1: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %6 u2: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %1 @strcmp(%23 __s1: ptr<const i8>, %24 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%25 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @test(%8 s1: ptr<const u8>, %9 s2: ptr<const u8>, %10 expected: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 value: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%8))), pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%9))));
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%10), const<i32>(0)), ge<i32>(read<i32>(%11), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<i32>(read<i32>(%10), const<i32>(0)), ne<i32>(read<i32>(%11), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if logical_and<bool>(gt<i32>(read<i32>(%10), const<i32>(0)), le<i32>(read<i32>(%11), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 off1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %14 off2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %15 len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %16 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %17 buf1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %18 buf2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %19 mod1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %20 mod2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %21 p1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %22 p2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%13, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%13), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: u64 [synthetic] = read<u64>(%13);
// DEFAULT-NEXT:                 let %36: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%35), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%13, read<u64>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %27
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%14, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%14), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %37: u64 [synthetic] = read<u64>(%14);
// DEFAULT-NEXT:                         let %38: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%37), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%14, read<u64>(%38));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %28
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<u64>(%15, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                             condition: lt<u64>(read<u64>(%15), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %39: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:                                 let %40: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%39), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<u64>(%15, read<u64>(%40));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<u8>>(%21, array_decay<ptr<u8>, length=Some(82)>(field0(%5)));
// DEFAULT-NEXT:                                     for %29
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%16), read<u64>(%13))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %41: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                                             let %42: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%41), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%16, read<u64>(%42));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %43: ptr<u8> [synthetic] = read<ptr<u8>>(%21);
// DEFAULT-NEXT:                                             let %44: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%43), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%21, read<ptr<u8>>(%44));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%43)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%17, read<ptr<u8>>(%21));
// DEFAULT-NEXT:                                     for %30
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%16), read<u64>(%15))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %45: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                                             let %46: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%45), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%16, read<u64>(%46));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %47: ptr<u8> [synthetic] = read<ptr<u8>>(%21);
// DEFAULT-NEXT:                                             let %48: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%47), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%21, read<ptr<u8>>(%48));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%47)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%19, read<ptr<u8>>(%21));
// DEFAULT-NEXT:                                     for %31
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%16), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %49: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                                             let %50: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%49), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%16, read<u64>(%50));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %51: ptr<u8> [synthetic] = read<ptr<u8>>(%21);
// DEFAULT-NEXT:                                             let %52: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%51), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%21, read<ptr<u8>>(%52));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%51)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%22, array_decay<ptr<u8>, length=Some(82)>(field0(%6)));
// DEFAULT-NEXT:                                     for %32
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%16), read<u64>(%14))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %53: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                                             let %54: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%53), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%16, read<u64>(%54));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %55: ptr<u8> [synthetic] = read<ptr<u8>>(%22);
// DEFAULT-NEXT:                                             let %56: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%55), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%22, read<ptr<u8>>(%56));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%55)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%18, read<ptr<u8>>(%22));
// DEFAULT-NEXT:                                     for %33
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%16), read<u64>(%15))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %57: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                                             let %58: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%57), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%16, read<u64>(%58));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %59: ptr<u8> [synthetic] = read<ptr<u8>>(%22);
// DEFAULT-NEXT:                                             let %60: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%59), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%22, read<ptr<u8>>(%60));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%59)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%20, read<ptr<u8>>(%22));
// DEFAULT-NEXT:                                     for %34
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%16), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %61: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                                             let %62: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%61), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%16, read<u64>(%62));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %63: ptr<u8> [synthetic] = read<ptr<u8>>(%22);
// DEFAULT-NEXT:                                             let %64: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%63), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%22, read<ptr<u8>>(%64));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%63)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%19), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%20), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%7, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%17)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%18)), const<i32>(1));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
