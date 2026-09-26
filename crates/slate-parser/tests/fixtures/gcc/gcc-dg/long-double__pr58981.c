/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-minline-all-stringops" { target { i?86-*-* x86_64-*-* } } } */

extern void abort(void);

#define MAX_OFFSET (sizeof(long long))
#define MAX_COPY   (8 * sizeof(long long))
#define MAX_EXTRA  (sizeof(long long))

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

static union {
  char        buf[MAX_LENGTH];
  long long   align_int;
  long double align_fp;
} u;

char A[MAX_LENGTH];

int main() {
  int   off, len, i;
  char *p, *q;

  for (i = 0; i < MAX_LENGTH; i++)
    A[i] = 'A';

  for (off = 0; off < MAX_OFFSET; off++)
    for (len = 1; len < MAX_COPY; len++) {
      for (i = 0; i < MAX_LENGTH; i++)
        u.buf[i] = 'a';

      p = __builtin_memcpy(u.buf + off, A, len);
      if (p != u.buf + off)
        abort();

      q = u.buf;
      for (i = 0; i < off; i++, q++)
        if (*q != 'a')
          abort();

      for (i = 0; i < len; i++, q++)
        if (*q != 'A')
          abort();

      for (i = 0; i < MAX_EXTRA; i++, q++)
        if (*q != 'a')
          abort();
    }

  return 0;
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
// DEFAULT-NEXT:         field0 buf: array<i8, 80>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=80, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %2 u: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 A: array<i8, 80> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %17 @__builtin_memcpy(%14 <unnamed>: ptr<void>, %15 <unnamed>: ptr<const void>, %16 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 off: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %9 q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(%3), read<i32>(%7))), truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%5))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %12
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%6))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                         let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%6, read<i32>(%26));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %13
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %27: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                                     let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%7, read<i32>(%28));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%2)), read<i32>(%7))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                             write<ptr<i8>>(%8, pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%17, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%2)), read<i32>(%5))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%6))))));
// DEFAULT-NEXT:                             pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%17, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%2)), read<i32>(%5))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%6)))));
// DEFAULT-NEXT:                             if ne<ptr<i8>>(read<ptr<i8>>(%8), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%2)), read<i32>(%5)))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             write<ptr<i8>>(%9, array_decay<ptr<i8>, length=Some(80)>(field0(%2)));
// DEFAULT-NEXT:                             for %18
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %29: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                                     let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%7, read<i32>(%30));
// DEFAULT-NEXT:                                     let %31: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                                     let %32: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%31), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%9, read<ptr<i8>>(%32));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             for %19
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %33: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                                     let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%7, read<i32>(%34));
// DEFAULT-NEXT:                                     let %35: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                                     let %36: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%35), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%9, read<ptr<i8>>(%36));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(65))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             for %20
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7))), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %37: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                                     let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%7, read<i32>(%38));
// DEFAULT-NEXT:                                     let %39: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:                                     let %40: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%39), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%9, read<ptr<i8>>(%40));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
