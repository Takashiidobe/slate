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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 81>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=96, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %5 u: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %1 @strlen(%12 __s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%13 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 off: u64 [storage=automatic];
// DEFAULT-NEXT:         let %8 len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %9 len2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %10 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %11 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%7, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%7), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:                 let %20: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%19), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%7, read<u64>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %15
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%8, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %21: u64 [synthetic] = read<u64>(%8);
// DEFAULT-NEXT:                         let %22: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%21), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%8, read<u64>(%22));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<i8>>(%11, array_decay<ptr<i8>, length=Some(81)>(field0(%5)));
// DEFAULT-NEXT:                             for %16
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%10), read<u64>(%7))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %23: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                                     let %24: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%23), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%10, read<u64>(%24));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %25: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:                                     let %26: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%25), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%11, read<ptr<i8>>(%26));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%25)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             for %17
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%10), read<u64>(%8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %27: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                                     let %28: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%27), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%10, read<u64>(%28));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %29: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:                                     let %30: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%29), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%11, read<ptr<i8>>(%30));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%29)), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                             let %31: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:                             let %32: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%31), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%11, read<ptr<i8>>(%32));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%31)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             for %18
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%10), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %33: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                                     let %34: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%33), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%10, read<u64>(%34));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %35: ptr<i8> [synthetic] = read<ptr<i8>>(%11);
// DEFAULT-NEXT:                                     let %36: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%35), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%11, read<ptr<i8>>(%36));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%35)), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:                             write<ptr<i8>>(%11, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(81)>(field0(%5)), read<u64>(%7)));
// DEFAULT-NEXT:                             write<u64>(%9, call<u64, signature=fn(ptr<const i8>) -> u64>(strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%11))));
// DEFAULT-NEXT:                             call<u64, signature=fn(ptr<const i8>) -> u64>(strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%11)));
// DEFAULT-NEXT:                             if ne<u64>(read<u64>(%8), read<u64>(%9))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
