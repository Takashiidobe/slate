/* { dg-xfail-if "kernel strncmp does not perform unsigned comparisons" {
 * vxworks_kernel } } */
/* Copyright (C) 2002  Free Software Foundation.

   Test strncmp with various combinations of pointer alignments and lengths to
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

#define MAX_LENGTH (MAX_OFFSET + MAX_TEST + MAX_EXTRA)

static union {
  unsigned char buf[MAX_LENGTH];
  long long     align_int;
  long double   align_fp;
} u1, u2;

void test(const unsigned char *s1, const unsigned char *s2, size_t len,
          int expected) {
  int value = strncmp((char *)s1, (char *)s2, len);

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
        for (i = 0; i < MAX_EXTRA; i++)
          *p1++ = 'x';

        p2 = u2.buf;
        for (i = 0; i < off2; i++)
          *p2++ = '\0';

        buf2 = p2;
        for (i = 0; i < len; i++)
          *p2++ = 'a';

        mod2 = p2;
        for (i = 0; i < MAX_EXTRA; i++)
          *p2++ = 'x';

        mod1[0] = '\0';
        mod2[0] = '\0';
        test(buf1, buf2, MAX_LENGTH, 0);
        test(buf1, buf2, len, 0);

        mod1[0] = 'a';
        mod1[1] = '\0';
        mod2[0] = '\0';
        test(buf1, buf2, MAX_LENGTH, +1);
        test(buf1, buf2, len, 0);

        mod1[0] = '\0';
        mod2[0] = 'a';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, -1);
        test(buf1, buf2, len, 0);

        mod1[0] = 'b';
        mod1[1] = '\0';
        mod2[0] = 'c';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, -1);
        test(buf1, buf2, len, 0);

        mod1[0] = 'c';
        mod1[1] = '\0';
        mod2[0] = 'b';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, +1);
        test(buf1, buf2, len, 0);

        mod1[0] = 'b';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\251';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, -1);
        test(buf1, buf2, len, 0);

        mod1[0] = (unsigned char)'\251';
        mod1[1] = '\0';
        mod2[0] = 'b';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, +1);
        test(buf1, buf2, len, 0);

        mod1[0] = (unsigned char)'\251';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\252';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, -1);
        test(buf1, buf2, len, 0);

        mod1[0] = (unsigned char)'\252';
        mod1[1] = '\0';
        mod2[0] = (unsigned char)'\251';
        mod2[1] = '\0';
        test(buf1, buf2, MAX_LENGTH, +1);
        test(buf1, buf2, len, 0);
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
// DEFAULT-NEXT:         field0 buf: array<u8, 80>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=80, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %8 u1: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 u2: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %4 @strncmp(%27 __s1: ptr<const i8>, %28 __s2: ptr<const i8>, %29 __n: u64) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @exit(%30 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @test(%11 s1: ptr<const u8>, %12 s2: ptr<const u8>, %13 len: u64, %14 expected: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 value: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%11))), pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%12))), read<u64>(%13));
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%14), const<i32>(0)), ge<i32>(read<i32>(%15), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<i32>(read<i32>(%14), const<i32>(0)), ne<i32>(read<i32>(%15), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if logical_and<bool>(gt<i32>(read<i32>(%14), const<i32>(0)), le<i32>(read<i32>(%15), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 off1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %18 off2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %19 len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %20 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %21 buf1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %22 buf2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %23 mod1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %24 mod2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %25 p1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %26 p2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         for %31
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%17, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%17), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %40: u64 [synthetic] = read<u64>(%17);
// DEFAULT-NEXT:                 let %41: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%40), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%17, read<u64>(%41));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %32
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%18, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%18), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %42: u64 [synthetic] = read<u64>(%18);
// DEFAULT-NEXT:                         let %43: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%42), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%18, read<u64>(%43));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %33
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<u64>(%19, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                             condition: lt<u64>(read<u64>(%19), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %44: u64 [synthetic] = read<u64>(%19);
// DEFAULT-NEXT:                                 let %45: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%44), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<u64>(%19, read<u64>(%45));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<u8>>(%25, array_decay<ptr<u8>, length=Some(80)>(field0(%8)));
// DEFAULT-NEXT:                                     for %34
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%20, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%20), read<u64>(%17))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %46: u64 [synthetic] = read<u64>(%20);
// DEFAULT-NEXT:                                             let %47: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%46), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%20, read<u64>(%47));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %48: ptr<u8> [synthetic] = read<ptr<u8>>(%25);
// DEFAULT-NEXT:                                             let %49: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%48), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%25, read<ptr<u8>>(%49));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%48)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%21, read<ptr<u8>>(%25));
// DEFAULT-NEXT:                                     for %35
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%20, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%20), read<u64>(%19))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %50: u64 [synthetic] = read<u64>(%20);
// DEFAULT-NEXT:                                             let %51: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%50), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%20, read<u64>(%51));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %52: ptr<u8> [synthetic] = read<ptr<u8>>(%25);
// DEFAULT-NEXT:                                             let %53: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%52), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%25, read<ptr<u8>>(%53));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%52)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%23, read<ptr<u8>>(%25));
// DEFAULT-NEXT:                                     for %36
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%20, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%20), const<u64>(8))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %54: u64 [synthetic] = read<u64>(%20);
// DEFAULT-NEXT:                                             let %55: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%54), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%20, read<u64>(%55));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %56: ptr<u8> [synthetic] = read<ptr<u8>>(%25);
// DEFAULT-NEXT:                                             let %57: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%56), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%25, read<ptr<u8>>(%57));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%56)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%26, array_decay<ptr<u8>, length=Some(80)>(field0(%9)));
// DEFAULT-NEXT:                                     for %37
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%20, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%20), read<u64>(%18))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %58: u64 [synthetic] = read<u64>(%20);
// DEFAULT-NEXT:                                             let %59: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%58), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%20, read<u64>(%59));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %60: ptr<u8> [synthetic] = read<ptr<u8>>(%26);
// DEFAULT-NEXT:                                             let %61: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%60), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%26, read<ptr<u8>>(%61));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%60)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%22, read<ptr<u8>>(%26));
// DEFAULT-NEXT:                                     for %38
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%20, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%20), read<u64>(%19))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %62: u64 [synthetic] = read<u64>(%20);
// DEFAULT-NEXT:                                             let %63: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%62), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%20, read<u64>(%63));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %64: ptr<u8> [synthetic] = read<ptr<u8>>(%26);
// DEFAULT-NEXT:                                             let %65: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%64), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%26, read<ptr<u8>>(%65));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%64)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%24, read<ptr<u8>>(%26));
// DEFAULT-NEXT:                                     for %39
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%20, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%20), const<u64>(8))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %66: u64 [synthetic] = read<u64>(%20);
// DEFAULT-NEXT:                                             let %67: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%66), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%20, read<u64>(%67));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %68: ptr<u8> [synthetic] = read<ptr<u8>>(%26);
// DEFAULT-NEXT:                                             let %69: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%68), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%26, read<ptr<u8>>(%69));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%68)), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), const<i32>(0));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), const<i32>(1));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), const<i32>(1));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), const<i32>(1));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%23), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)), const<i32>(1));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, u64, i32) -> void>(%10, pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%21)), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%22)), read<u64>(%19), const<i32>(0));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%6, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
