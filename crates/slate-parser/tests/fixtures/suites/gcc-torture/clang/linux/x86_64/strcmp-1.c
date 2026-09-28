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
// DEFAULT-NEXT:     global %7 u1: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %8 u2: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %3 @strcmp(%25 __s1: ptr<const i8>, %26 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @exit(%27 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @test(%10 s1: ptr<const u8>, %11 s2: ptr<const u8>, %12 expected: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 value: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%10))), pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%11))));
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%12), const<i32>(0)), ge<i32>(read<i32>(%13), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<i32>(read<i32>(%12), const<i32>(0)), ne<i32>(read<i32>(%13), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if logical_and<bool>(gt<i32>(read<i32>(%12), const<i32>(0)), le<i32>(read<i32>(%13), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 off1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %16 off2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %17 len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %18 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %19 buf1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %20 buf2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %21 mod1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %22 mod2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %23 p1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %24 p2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%15, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%15), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %37: u64 [synthetic] = read<u64>(%15);
// DEFAULT-NEXT:                 let %38: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%37), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%15, read<u64>(%38));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %29
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%16, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%16), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %39: u64 [synthetic] = read<u64>(%16);
// DEFAULT-NEXT:                         let %40: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%39), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%16, read<u64>(%40));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %30
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<u64>(%17, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                             condition: lt<u64>(read<u64>(%17), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %41: u64 [synthetic] = read<u64>(%17);
// DEFAULT-NEXT:                                 let %42: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%41), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<u64>(%17, read<u64>(%42));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<u8>>(%23, array_decay<ptr<u8>, length=Some(82)>(field0(%7)));
// DEFAULT-NEXT:                                     for %31
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%18), read<u64>(%15))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %43: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                                             let %44: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%43), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%18, read<u64>(%44));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %45: ptr<u8> [synthetic] = read<ptr<u8>>(%23);
// DEFAULT-NEXT:                                             let %46: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%45), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%23, read<ptr<u8>>(%46));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%45)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%19, read<ptr<u8>>(%23));
// DEFAULT-NEXT:                                     for %32
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%18), read<u64>(%17))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %47: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                                             let %48: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%47), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%18, read<u64>(%48));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %49: ptr<u8> [synthetic] = read<ptr<u8>>(%23);
// DEFAULT-NEXT:                                             let %50: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%49), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%23, read<ptr<u8>>(%50));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%49)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%21, read<ptr<u8>>(%23));
// DEFAULT-NEXT:                                     for %33
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%18), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %51: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                                             let %52: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%51), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%18, read<u64>(%52));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %53: ptr<u8> [synthetic] = read<ptr<u8>>(%23);
// DEFAULT-NEXT:                                             let %54: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%53), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%23, read<ptr<u8>>(%54));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%53)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%24, array_decay<ptr<u8>, length=Some(82)>(field0(%8)));
// DEFAULT-NEXT:                                     for %34
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%18), read<u64>(%16))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %55: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                                             let %56: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%55), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%18, read<u64>(%56));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %57: ptr<u8> [synthetic] = read<ptr<u8>>(%24);
// DEFAULT-NEXT:                                             let %58: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%57), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%24, read<ptr<u8>>(%58));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%57)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%20, read<ptr<u8>>(%24));
// DEFAULT-NEXT:                                     for %35
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%18), read<u64>(%17))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %59: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                                             let %60: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%59), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%18, read<u64>(%60));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %61: ptr<u8> [synthetic] = read<ptr<u8>>(%24);
// DEFAULT-NEXT:                                             let %62: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%61), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%24, read<ptr<u8>>(%62));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%61)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%22, read<ptr<u8>>(%24));
// DEFAULT-NEXT:                                     for %36
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%18), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %63: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                                             let %64: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%63), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%18, read<u64>(%64));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %65: ptr<u8> [synthetic] = read<ptr<u8>>(%24);
// DEFAULT-NEXT:                                             let %66: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%65), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%24, read<ptr<u8>>(%66));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%65)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%22), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%9, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%19)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%20)), const<i32>(1));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%5, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
