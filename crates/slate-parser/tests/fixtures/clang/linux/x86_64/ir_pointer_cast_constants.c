// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

struct siginfo;

#define SEND_SIG_NOINFO ((struct siginfo *) 0)
#define SEND_SIG_PRIV ((struct siginfo *) 1)

int classify(struct siginfo *info) {
  switch ((unsigned long) info) {
  case (unsigned long) SEND_SIG_NOINFO:
    return 1;
  case (unsigned long) SEND_SIG_PRIV:
    return 2;
  }
  return 0;
}

enum {
  NULL_ADDRESS = (int)(unsigned long)(void *)0,
  NARROWED = (char)(int *)0x1ff,
  TRUTH = (_Bool)(int *)8,
  RECAST = (int)(unsigned long)(char *)(const int *)16,
  WRAPPED = (unsigned long long)(int *)-1 == 0xffffffffffffffffull,
};

struct flags {
  unsigned width : (unsigned long)(int *)3;
};

_Static_assert((unsigned long)(int *)8 == 8, "integer to pointer to integer");

unsigned long table(void) {
  static int slots[(unsigned long)(int *)4];
  return sizeof slots;
}

int values(void) {
  return NULL_ADDRESS + NARROWED + TRUTH + RECAST + WRAPPED + sizeof(struct flags);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_siginfo:[0-9]+]] siginfo = struct incomplete;
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : i32 {
// IR-NEXT:         %[[VALUE_NULL_ADDRESS:[0-9]+]] NULL_ADDRESS = const<i32>(0);
// IR-NEXT:         %[[VALUE_NARROWED:[0-9]+]] NARROWED = const<i32>(-1);
// IR-NEXT:         %[[VALUE_TRUTH:[0-9]+]] TRUTH = const<i32>(1);
// IR-NEXT:         %[[VALUE_RECAST:[0-9]+]] RECAST = const<i32>(16);
// IR-NEXT:         %[[VALUE_WRAPPED:[0-9]+]] WRAPPED = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_flags:[0-9]+]] flags = struct {
// IR-NEXT:         field0 width: u32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     global %[[VALUE_slots:[0-9]+]] slots: array<i32, 4> [storage=static] [align=16] [linkage=internal];
// IR-NEXT:     fn %[[VALUE_NARROWED]] @classify(%[[VALUE_TRUTH]] info: ptr<@type[[TYPE_siginfo]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE0:[0-9]+]] ptr_to_int<u64, reason=explicit>(read<ptr<@type[[TYPE_siginfo]]>>(%[[VALUE_TRUTH]]))
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE0]] const<u64>(0):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 case %[[VALUE0]] const<u64>(1):
// IR-NEXT:                     return const<i32>(2);
// IR-NEXT:             }
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_table:[0-9]+]] @table() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_values:[0-9]+]] @values() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(-1)), const<i32>(1)), const<i32>(16)), const<i32>(1)))), const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
