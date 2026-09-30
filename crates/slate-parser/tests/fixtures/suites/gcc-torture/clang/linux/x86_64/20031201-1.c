/* Copyright (C) 2003  Free Software Foundation.
   PR target/13256
   STRICT_LOW_PART was handled incorrectly in delay slots.
   Origin: Hans-Peter Nilsson.  */

void abort(void);
void exit(int);

typedef struct {
  unsigned int e0 : 16;
  unsigned int e1 : 16;
} s1;
typedef struct {
  unsigned int e0 : 16;
  unsigned int e1 : 16;
} s2;
typedef struct {
  s1 i12;
  s2 i16;
} io;
static int           test_length = 2;
static io           *i;
static int           m = 1;
static int           d = 1;
static unsigned long test_t0;
static unsigned long test_t1;
void                 test(void) __attribute__((__noinline__));
extern int           f1(void *port) __attribute__((__noinline__));
extern void          f0(void) __attribute__((__noinline__));
int                  f1(void *port) {
  int           fail_count = 0;
  unsigned long tlen;
  s1            x0 = {0};
  s2            x1 = {0};

  i     = port;
  x0.e0 = x1.e0 = 32;
  i->i12        = x0;
  i->i16        = x1;
  do
    f0();
  while (test_t1);
  x0.e0 = x1.e0 = 8;
  i->i12        = x0;
  i->i16        = x1;
  test();
  if (m) {
    unsigned long e = 1000000000 / 460800 * test_length;
    tlen            = test_t1 - test_t0;
    if (((tlen - e) & 0x7FFFFFFF) > 1000)
      f0();
  }
  if (d) {
    unsigned long e = 1000000000 / 460800 * test_length;
    tlen            = test_t1 - test_t0;
    if (((tlen - e) & 0x7FFFFFFF) > 1000)
      f0();
  }
  return fail_count != 0 ? 1 : 0;
}

int main() {
  io io0;
  f1(&io0);
  abort();
}

void test(void) {
  io *iop = i;
  if (iop->i12.e0 != 8 || iop->i16.e0 != 8)
    abort();
  exit(0);
}

void f0(void) {
  static int washere = 0;
  io        *iop     = i;
  if (washere++ || iop->i12.e0 != 32 || iop->i16.e0 != 32)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 e0: u32 : 16;
// DEFAULT-NEXT:         field1 e1: u32 : 16;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 e0: u32 : 16;
// DEFAULT-NEXT:         field1 e1: u32 : 16;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 4)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 i12: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 i16: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_io:[0-9]+]] io = @type[[TYPE2]];
// DEFAULT-NEXT:     global %[[VALUE_test_length:[0-9]+]] test_length: i32 [storage=static] = const<i32>(2) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: ptr<@type[[TYPE2]]> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_test_t0:[0-9]+]] test_t0: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_test_t1:[0-9]+]] test_t1: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_washere:[0-9]+]] washere: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_iop:[0-9]+]] iop: ptr<@type[[TYPE2]]> [storage=automatic] = read<ptr<@type[[TYPE2]]>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_iop]])))))), const<i32>(8)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_iop]])))))), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_port:[0-9]+]] port: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_fail_count:[0-9]+]] fail_count: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_tlen:[0-9]+]] tlen: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x0:[0-9]+]] x0: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_x1:[0-9]+]] x1: @type[[TYPE1]] [storage=automatic] = aggregate<@type[[TYPE1]], zero_fill=true>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE2]]>>(%[[VALUE_i]], pointer_cast<ptr<@type[[TYPE2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_port]])));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_x1]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(32)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_x0]]), reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=assign>(truncate<u16b, reason=assign, fits=unknown>(reinterpret<u32, reason=assign, fits=always>(const<i32>(32)))))));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_i]]))), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_x0]])));
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_i]]))), copy<@type[[TYPE1]], reason=assign>(read<@type[[TYPE1]]>(%[[VALUE_x1]])));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_f0:[0-9]+]]);
// DEFAULT-NEXT:         while ne<u64>(read<u64>(%[[VALUE_test_t1]]), const<u64>(0));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_x1]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_x0]]), reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=assign>(truncate<u16b, reason=assign, fits=unknown>(reinterpret<u32, reason=assign, fits=always>(const<i32>(8)))))));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_i]]))), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_x0]])));
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_i]]))), copy<@type[[TYPE1]], reason=assign>(read<@type[[TYPE1]]>(%[[VALUE_x1]])));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_e:[0-9]+]] e: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(mul<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1000000000), const<i32>(460800)), read<i32>(%[[VALUE_test_length]]))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_tlen]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_test_t1]]), read<u64>(%[[VALUE_test_t0]])));
// DEFAULT-NEXT:                 if gt<u64>(and<u64>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_tlen]]), read<u64>(%[[VALUE_e]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2147483647)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1000))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f0]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_e_2:[0-9]+]] e: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(mul<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1000000000), const<i32>(460800)), read<i32>(%[[VALUE_test_length]]))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_tlen]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_test_t1]]), read<u64>(%[[VALUE_test_t0]])));
// DEFAULT-NEXT:                 if gt<u64>(and<u64>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_tlen]]), read<u64>(%[[VALUE_e_2]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2147483647)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1000))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_f0]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_fail_count]]), const<i32>(0)), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f0]] @f0() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_iop_2:[0-9]+]] iop: ptr<@type[[TYPE2]]> [storage=automatic] = read<ptr<@type[[TYPE2]]>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_washere]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_washere]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE2]]), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(field0(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_iop_2]])))))), const<i32>(32))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(field1(deref(read<ptr<@type[[TYPE2]]>>(%[[VALUE_iop_2]])))))), const<i32>(32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_io0:[0-9]+]] io0: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_f1]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_io0]])));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
