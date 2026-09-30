/* PR 24255 */
/* { dg-do run } */
/* { dg-options "-O" } */

extern void abort (void);

union wait { int w_status; };

typedef union
{
  union wait *uptr;
  int *iptr;
} WAIT_STATUS __attribute__ ((__transparent_union__));

int status;
union wait wstatus;

void __attribute__((noinline))
test1 (WAIT_STATUS s)
{
  if (s.iptr != &status)
    abort ();
}

void __attribute__((noinline))
test2 (WAIT_STATUS s)
{
  if (s.uptr != &wstatus)
    abort ();
}

int main()
{
  test1 (&status);
  test2 (&wstatus);
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
// DEFAULT-NEXT:     type @type[[TYPE_wait:[0-9]+]] wait = union {
// DEFAULT-NEXT:         field0 w_status: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 uptr: ptr<@type[[TYPE_wait]]>;
// DEFAULT-NEXT:         field1 iptr: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_WAIT_STATUS:[0-9]+]] WAIT_STATUS = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_status:[0-9]+]] status: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wstatus:[0-9]+]] wstatus: @type[[TYPE_wait]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_s:[0-9]+]] s: @type[[TYPE0]]) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field1(%[[VALUE_s]])), addr_of<ptr<i32>>(%[[VALUE_status]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_s_2:[0-9]+]] s: @type[[TYPE0]]) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_wait]]>>(read<ptr<@type[[TYPE_wait]]>>(field0(%[[VALUE_s_2]])), addr_of<ptr<@type[[TYPE_wait]]>>(%[[VALUE_wstatus]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_test1]], aggregate<@type[[TYPE0]], zero_fill=false>(field1 = addr_of<ptr<i32>>(%[[VALUE_status]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE0]]) -> void>(%[[VALUE_test2]], aggregate<@type[[TYPE0]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_wait]]>>(%[[VALUE_wstatus]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
