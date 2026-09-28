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
// DEFAULT-NEXT:     global %6 u: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %2 @strlen(%13 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%14 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 off: u64 [storage=automatic];
// DEFAULT-NEXT:         let %9 len: u64 [storage=automatic];
// DEFAULT-NEXT:         let %10 len2: u64 [storage=automatic];
// DEFAULT-NEXT:         let %11 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %12 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%8, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%8), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: u64 [synthetic] = read<u64>(%8);
// DEFAULT-NEXT:                 let %21: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%20), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%8, read<u64>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %16
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u64>(%9, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                     condition: lt<u64>(read<u64>(%9), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %22: u64 [synthetic] = read<u64>(%9);
// DEFAULT-NEXT:                         let %23: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                         write<u64>(%9, read<u64>(%23));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<i8>>(%12, array_decay<ptr<i8>, length=Some(81)>(field0(%6)));
// DEFAULT-NEXT:                             for %17
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%11, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%11), read<u64>(%8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %24: u64 [synthetic] = read<u64>(%11);
// DEFAULT-NEXT:                                     let %25: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%24), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%11, read<u64>(%25));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %26: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %27: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%26), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%27));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%26)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             for %18
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%11, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%11), read<u64>(%9))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %28: u64 [synthetic] = read<u64>(%11);
// DEFAULT-NEXT:                                     let %29: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%28), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%11, read<u64>(%29));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %30: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %31: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%30), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%31));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%30)), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                             let %32: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                             let %33: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%32), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<i8>>(%12, read<ptr<i8>>(%33));
// DEFAULT-NEXT:                             write<i8>(deref(read<ptr<i8>>(%32)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                             for %19
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<u64>(%11, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:                                 condition: lt<u64>(read<u64>(%11), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %34: u64 [synthetic] = read<u64>(%11);
// DEFAULT-NEXT:                                     let %35: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%34), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<u64>(%11, read<u64>(%35));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %36: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:                                     let %37: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%36), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%12, read<ptr<i8>>(%37));
// DEFAULT-NEXT:                                     write<i8>(deref(read<ptr<i8>>(%36)), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:                             write<ptr<i8>>(%12, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(81)>(field0(%6)), read<u64>(%8)));
// DEFAULT-NEXT:                             write<u64>(%10, call<u64, signature=fn(ptr<const i8>) -> u64>(%2, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%12))));
// DEFAULT-NEXT:                             if ne<u64>(read<u64>(%9), read<u64>(%10))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
