/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-O -fgcse -fno-split-wide-types" } */

extern void abort(void);

typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef struct
{
  uint16_t thread;
  uint16_t phase;
} s32;

typedef union
{
  uint32_t i;
  s32 s;
} u32;

typedef union
{
  uint64_t i;
  u32 u;
} u64;

static __attribute__((noinline))
void foo(int val)
{
  u64 data;
  uint32_t thread;

  data.u.i = 0x10000L;
  thread = data.u.s.thread;
  if (val)
    abort ();
  if (thread)
    abort ();
}

int main(void)
{
  foo (0);
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
// DEFAULT-NEXT:     type @type0 uint16_t = u16;
// DEFAULT-NEXT:     type @type1 uint32_t = u32;
// DEFAULT-NEXT:     type @type2 uint64_t = u64;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 thread: u16;
// DEFAULT-NEXT:         field1 phase: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type4 s32 = @type3;
// DEFAULT-NEXT:     type @type5 = union {
// DEFAULT-NEXT:         field0 i: u32;
// DEFAULT-NEXT:         field1 s: @type3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type6 u32 = @type5;
// DEFAULT-NEXT:     type @type7 = union {
// DEFAULT-NEXT:         field0 i: u64;
// DEFAULT-NEXT:         field1 u: @type5;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type8 u64 = @type7;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @foo(%11 val: i32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 data: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %13 thread: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(field1(%12)), reinterpret<u32, reason=assign, fits=unknown>(truncate<i32, reason=assign, fits=always>(const<i64>(65536))));
// DEFAULT-NEXT:         write<u32>(%13, widen<u32, reason=assign>(read<u16>(field0(field1(field1(%12))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%13), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%10, const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
