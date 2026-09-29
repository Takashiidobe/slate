/* Copyright (C) 2002  Free Software Foundation.

   Test strlen with various combinations of pointer alignments and lengths to
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

#define MAX_LENGTH (MAX_OFFSET + MAX_TEST + MAX_EXTRA + 1)

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u;

int main(void) {
  size_t off, len, len2, i;
  char  *p;

  for (off = 0; off < MAX_OFFSET; off++)
    for (len = 0; len < MAX_TEST; len++) {
      p = u.buf;
      for (i = 0; i < off; i++)
        *p++ = '\0';

      for (i = 0; i < len; i++)
        *p++ = 'a';

      *p++ = '\0';
      for (i = 0; i < MAX_EXTRA; i++)
        *p++ = 'b';

      p    = u.buf + off;
      len2 = strlen(p);
      if (len != len2)
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 81>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=96, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_off:[0-9]+]] off: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len2:[0-9]+]] len2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_off]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_off]]), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_off]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_off]], read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_len]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%[[VALUE_len]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE5]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_len]], read<u64>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_p]], array_decay<ptr<i8>, length=Some(81)>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:                             for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_off]]))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE8:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE9:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE8]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE9]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %[[VALUE10:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                     let %[[VALUE11:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE11]]));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%[[VALUE10]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_len]]))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE13:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE14:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE13]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE14]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %[[VALUE15:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                     let %[[VALUE16:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE16]]));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%[[VALUE15]])), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                             let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                             let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE18]]));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%[[VALUE17]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%[[VALUE_i]]), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE20:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE21:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE20]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE21]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %[[VALUE22:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                                     let %[[VALUE23:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE23]]));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%[[VALUE22]])), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_p]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(81)>(field0(%[[VALUE_u]])), read<u64>(%[[VALUE_off]])));
// DEFAULT-NEXT:                             write<u64>(%[[VALUE_len2]], call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:                             if ne<u64>(read<u64>(%[[VALUE_len]]), read<u64>(%[[VALUE_len2]]))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
