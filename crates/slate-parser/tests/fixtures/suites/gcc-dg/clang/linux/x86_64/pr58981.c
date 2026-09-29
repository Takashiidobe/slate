/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-minline-all-stringops" { target { i?86-*-* x86_64-*-* } } } */

extern void abort (void);

#define MAX_OFFSET (sizeof (long long))
#define MAX_COPY (8 * sizeof (long long))
#define MAX_EXTRA (sizeof (long long))

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

static union {
  char buf[MAX_LENGTH];
  long long align_int;
  long double align_fp;
} u;

char A[MAX_LENGTH];

int
main ()
{
  int off, len, i;
  char *p, *q;

  for (i = 0; i < MAX_LENGTH; i++)
    A[i] = 'A';

  for (off = 0; off < MAX_OFFSET; off++)
    for (len = 1; len < MAX_COPY; len++)
      {
	for (i = 0; i < MAX_LENGTH; i++)
	  u.buf[i] = 'a';

	p = __builtin_memcpy (u.buf + off, A, len);
	if (p != u.buf + off)
	  abort ();

	q = u.buf;
	for (i = 0; i < off; i++, q++)
	  if (*q != 'a')
	    abort ();

	for (i = 0; i < len; i++, q++)
	  if (*q != 'A')
	    abort ();

	for (i = 0; i < MAX_EXTRA; i++, q++)
	  if (*q != 'a')
	    abort ();
      }

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:         field0 buf: array<i8, 80>;
// DEFAULT-NEXT:         field1 align_int: i64;
// DEFAULT-NEXT:         field2 align_fp: f80;
// DEFAULT-NEXT:     } [size=80, align=16, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_A:[0-9]+]] A: array<i8, 80> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_off:[0-9]+]] off: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_A]]), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=always>(const<i32>(65)));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_off]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_off]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_off]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_len]], const<i32>(1));
// DEFAULT-NEXT:                     condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_len]]))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len]]);
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_len]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), const<u64>(8))), const<u64>(8)))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off]]))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len]]))))));
// DEFAULT-NEXT:                             pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off]]))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(80)>(%[[VALUE_A]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_len]])))));
// DEFAULT-NEXT:                             if ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_p]]), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(80)>(field0(%[[VALUE_u]])), read<i32>(%[[VALUE_off]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             write<ptr<i8>>(%[[VALUE_q]], array_decay<ptr<i8>, length=Some(80)>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:                             for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_off]]))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                                     let %[[VALUE18:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                     let %[[VALUE19:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE19]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                                     let %[[VALUE23:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                     let %[[VALUE24:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE24]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(65))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                             for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                     let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                                     let %[[VALUE28:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_q]]);
// DEFAULT-NEXT:                                     let %[[VALUE29:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<ptr<i8>>(%[[VALUE_q]], read<ptr<i8>>(%[[VALUE29]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_q]])))), const<i32>(97))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
