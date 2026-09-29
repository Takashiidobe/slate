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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 buf: array<u8, 82>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=96, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u1:[0-9]+]] u1: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_u2:[0-9]+]] u2: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_s1:[0-9]+]] s1: ptr<const u8>, %[[VALUE_s2:[0-9]+]] s2: ptr<const u8>, %[[VALUE_expected:[0-9]+]] expected: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%[[VALUE_s1]]))), pointer_cast<ptr<const i8>, reason=arg>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<const u8>>(%[[VALUE_s2]]))));
// DEFAULT-NEXT:         if logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_value]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_value]]), const<i32>(0)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0)), le<i32>(read<i32>(%[[VALUE_value]]), const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_off1:[0-9]+]] off1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_off2:[0-9]+]] off2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_buf1:[0-9]+]] buf1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_buf2:[0-9]+]] buf2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mod1:[0-9]+]] mod1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mod2:[0-9]+]] mod2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_off1]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_off1]]), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_off1]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_off1]], read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_off2]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%[[VALUE_off2]]), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_off2]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_off2]], read<u64>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<u64>(%[[VALUE_len]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                             condition: lt<u64>(read<u64>(%[[VALUE_len]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE8]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE9]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_p1]], array_decay<ptr<u8>, length=Some(82)>(field0(%[[VALUE_u1]])));
// DEFAULT-NEXT:                                     for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_off1]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE11:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE12:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE11]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE12]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %[[VALUE13:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p1]]);
// DEFAULT-NEXT:                                             let %[[VALUE14:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_p1]], read<ptr<u8>>(%[[VALUE14]]));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%[[VALUE13]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_buf1]], read<ptr<u8>>(%[[VALUE_p1]]));
// DEFAULT-NEXT:                                     for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_len]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE16:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE17:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE16]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE17]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %[[VALUE18:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p1]]);
// DEFAULT-NEXT:                                             let %[[VALUE19:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_p1]], read<ptr<u8>>(%[[VALUE19]]));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%[[VALUE18]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_mod1]], read<ptr<u8>>(%[[VALUE_p1]]));
// DEFAULT-NEXT:                                     for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%[[VALUE_i]]), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE21:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE22:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE21]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE22]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %[[VALUE23:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p1]]);
// DEFAULT-NEXT:                                             let %[[VALUE24:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_p1]], read<ptr<u8>>(%[[VALUE24]]));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%[[VALUE23]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_p2]], array_decay<ptr<u8>, length=Some(82)>(field0(%[[VALUE_u2]])));
// DEFAULT-NEXT:                                     for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_off2]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE26:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE27:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE26]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE27]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %[[VALUE28:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p2]]);
// DEFAULT-NEXT:                                             let %[[VALUE29:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_p2]], read<ptr<u8>>(%[[VALUE29]]));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%[[VALUE28]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_buf2]], read<ptr<u8>>(%[[VALUE_p2]]));
// DEFAULT-NEXT:                                     for %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_len]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE31:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE32:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE31]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE32]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %[[VALUE33:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p2]]);
// DEFAULT-NEXT:                                             let %[[VALUE34:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_p2]], read<ptr<u8>>(%[[VALUE34]]));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%[[VALUE33]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<ptr<u8>>(%[[VALUE_mod2]], read<ptr<u8>>(%[[VALUE_p2]]));
// DEFAULT-NEXT:                                     for %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                         condition: lt<u64>(read<u64>(%[[VALUE_i]]), add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE36:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE37:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE36]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                             write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE37]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             let %[[VALUE38:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p2]]);
// DEFAULT-NEXT:                                             let %[[VALUE39:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<u8>>(%[[VALUE_p2]], read<ptr<u8>>(%[[VALUE39]]));
// DEFAULT-NEXT:                                             write<u8>(deref(read<ptr<u8>>(%[[VALUE38]])), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(120))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), const<i32>(0));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(99))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), const<i32>(1));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-86))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod1]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(0))), reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(-87))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_mod2]]), const<i32>(1))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                                     call<void, signature=fn(ptr<const u8>, ptr<const u8>, i32) -> void>(%[[VALUE_test]], pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf1]])), pointer_cast<ptr<const u8>, reason=arg>(read<ptr<u8>>(%[[VALUE_buf2]])), const<i32>(1));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
