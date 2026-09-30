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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 97>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=112, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u1:[0-9]+]] u1: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_u2:[0-9]+]] u2: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<i8> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const i8> [restrict]) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_off1:[0-9]+]] off1: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_off2:[0-9]+]] off2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off1]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off1]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off1]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off1]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_off2]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off2]]))), const<u64>(8))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off2]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_off2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_len]], const<i32>(1));
// DEFAULT-NEXT:                             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_len]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8)))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len]]);
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_len]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                             write<i8>(%[[VALUE_c]], truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))), const<u64>(8))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), const<u64>(8)))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                                             let %[[VALUE13:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_c]]);
// DEFAULT-NEXT:                                             let %[[VALUE14:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE13]])), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE14]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u1]])), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%[[VALUE_c]], truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u2]])), read<i32>(%[[VALUE_i]]))), read<i8>(%[[VALUE_c]]));
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u2]])), add<i32, overflow=ub>(read<i32>(%[[VALUE_off2]]), read<i32>(%[[VALUE_len]])))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_p]], call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u1]])), read<i32>(%[[VALUE_off1]])), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u2]])), read<i32>(%[[VALUE_off2]])))));
// DEFAULT-NEXT:                                     if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u1]])), read<i32>(%[[VALUE_off1]])))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_q]], array_decay<ptr<i8>, length=Some(97)>(field0(%[[VALUE_u1]])));
// DEFAULT-NEXT:                                     for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_off1]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                                             let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                             let %[[VALUE19:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE19]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                     for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                             let %[[VALUE21:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(const<i32>(65), read<i32>(%[[VALUE_off2]])));
// DEFAULT-NEXT:                                             write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE21]]));
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                                             let %[[VALUE24:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                             let %[[VALUE25:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE25]]));
// DEFAULT-NEXT:                                             let %[[VALUE26:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_c]]);
// DEFAULT-NEXT:                                             let %[[VALUE27:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE26]])), const<i32>(1)));
// DEFAULT-NEXT:                                             write<i8>(%[[VALUE_c]], read<i8>(%[[VALUE27]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), add<i32, overflow=ub>(const<i32>(65), const<i32>(31)))
// DEFAULT-NEXT:                                                     write<i8>(%[[VALUE_c]], truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:                                                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])))
// DEFAULT-NEXT:                                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     let %[[VALUE28:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                     let %[[VALUE29:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE29]]));
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE28]])))), const<i32>(0))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                     for %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                         condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(8))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:                                             let %[[VALUE33:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                             let %[[VALUE34:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE33]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE34]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(97))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
