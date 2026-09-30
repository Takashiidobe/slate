/* { dg-do run } */
/* { dg-options "-O2 -fipa-sra" } */


struct __attribute__((scalar_storage_order("little-endian"))) LE
{
  int i;
  int j;
};

struct __attribute__((scalar_storage_order("big-endian"))) BE
{
  int i;
  int j;
};

struct LE gle;
struct BE gbe;

#define VAL 0x12345678

void __attribute__((noipa))
fill (void)
{
  gle.i = VAL;
  gle.j = 0xdeadbeef;
  gbe.i = VAL;
  gbe.j = 0x11223344;
}

static int __attribute__((noinline))
readLE (struct LE p)
{
  return p.i;
}

static int __attribute__((noinline))
readBE (struct BE p)
{
  return p.i;
}

int
main (int argc, char *argv[])
{
  int r;
  fill ();

  r = readLE (gle);
  if (r != VAL)
    __builtin_abort ();
  r = readBE (gbe);
  if (r != VAL)
    __builtin_abort ();

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
// DEFAULT-NEXT:     type @type[[TYPE_LE:[0-9]+]] LE = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_BE:[0-9]+]] BE = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_gle:[0-9]+]] gle: @type[[TYPE_LE]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gbe:[0-9]+]] gbe: @type[[TYPE_BE]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fill:[0-9]+]] @fill() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_gle]]), const<i32>(305419896));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_gle]]), reinterpret<i32, reason=assign, fits=unknown>(const<u32>(3735928559)));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_gbe]]), const<i32>(305419896));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_gbe]]), const<i32>(287454020));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_readLE:[0-9]+]] @readLE(%[[VALUE_p:[0-9]+]] p: @type[[TYPE_LE]]) -> i32 [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_readBE:[0-9]+]] @readBE(%[[VALUE_p_2:[0-9]+]] p: @type[[TYPE_BE]]) -> i32 [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(field0(%[[VALUE_p_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fill]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(@type[[TYPE_LE]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_readLE]], copy<@type[[TYPE_LE]], reason=arg>(read<@type[[TYPE_LE]]>(%[[VALUE_gle]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(305419896))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn(@type[[TYPE_BE]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_readBE]], copy<@type[[TYPE_BE]], reason=arg>(read<@type[[TYPE_BE]]>(%[[VALUE_gbe]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_r]]), const<i32>(305419896))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
