// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

struct s {
  char a;
  int b;
  int arr[4];
  struct { long c; } inner;
};

enum {
  ONE_PAST = (unsigned long)((int *)0 + 1),
  BACK = (long)((int *)8 - (int *)0),
  DIFF_NEG = (long)((short *)0 - (short *)8),
  VOID_STEP = (unsigned long)((void *)0 + 5),
  EQUAL = (int *)0 == (int *)0,
  ORDER = (char *)4 < (char *)8,
  NOT_NULL = !(int *)0,
  NOT_SET = !(int *)8,
  AND = (int *)8 && 1,
  OR = (int *)0 || 0,
  MEMBER = (unsigned long)&((struct s *)0)->b,
  NESTED = (unsigned long)&((struct s *)0)->inner.c,
  ELEMENT = (unsigned long)&((struct s *)0)->arr[2],
  BASED = (unsigned long)&((struct s *)16)->b,
  DEREF_ADDR = (unsigned long)&*(int *)24,
};

struct flags {
  unsigned width : (unsigned long)&((struct s *)0)->b;
};

_Static_assert((unsigned long)&((struct s *)0)->b == 4, "member offset");
_Static_assert((long)((int *)8 - (int *)0) == 2, "difference");

int classify(unsigned long x) {
  switch (x) {
  case (unsigned long)((int *)0 + 1):
    return 1;
  case (unsigned long)&((struct s *)0)->arr[1]:
    return 2;
  }
  return 0;
}

unsigned long table(void) {
  static int slots[(unsigned long)((char *)0 + 3)];
  static int fields[(unsigned long)&((struct s *)0)->b];
  return sizeof slots + sizeof fields;
}

int values(void) {
  return ONE_PAST + BACK + DIFF_NEG + VOID_STEP + EQUAL + ORDER + NOT_NULL + NOT_SET + AND + OR +
         MEMBER + NESTED + ELEMENT + BASED + DEREF_ADDR + sizeof(struct flags);
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
// IR-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 arr: array<i32, 4>;
// IR-NEXT:         field3 inner: @type[[TYPE0:[0-9]+]];
// IR-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 24]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 c: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : i32 {
// IR-NEXT:         %[[VALUE_ONE_PAST:[0-9]+]] ONE_PAST = const<i32>(4);
// IR-NEXT:         %[[VALUE_BACK:[0-9]+]] BACK = const<i32>(2);
// IR-NEXT:         %[[VALUE_DIFF_NEG:[0-9]+]] DIFF_NEG = const<i32>(-4);
// IR-NEXT:         %[[VALUE_VOID_STEP:[0-9]+]] VOID_STEP = const<i32>(5);
// IR-NEXT:         %[[VALUE_EQUAL:[0-9]+]] EQUAL = const<i32>(1);
// IR-NEXT:         %[[VALUE_ORDER:[0-9]+]] ORDER = const<i32>(1);
// IR-NEXT:         %[[VALUE_NOT_NULL:[0-9]+]] NOT_NULL = const<i32>(1);
// IR-NEXT:         %[[VALUE_NOT_SET:[0-9]+]] NOT_SET = const<i32>(0);
// IR-NEXT:         %[[VALUE_AND:[0-9]+]] AND = const<i32>(1);
// IR-NEXT:         %[[VALUE_OR:[0-9]+]] OR = const<i32>(0);
// IR-NEXT:         %[[VALUE_MEMBER:[0-9]+]] MEMBER = const<i32>(4);
// IR-NEXT:         %[[VALUE_NESTED:[0-9]+]] NESTED = const<i32>(24);
// IR-NEXT:         %[[VALUE_ELEMENT:[0-9]+]] ELEMENT = const<i32>(16);
// IR-NEXT:         %[[VALUE_BASED:[0-9]+]] BASED = const<i32>(20);
// IR-NEXT:         %[[VALUE_DEREF_ADDR:[0-9]+]] DEREF_ADDR = const<i32>(24);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_flags:[0-9]+]] flags = struct {
// IR-NEXT:         field0 width: u32 : 4;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     global %[[VALUE_slots:[0-9]+]] slots: array<i32, 3> [storage=static] [linkage=internal];
// IR-NEXT:     global %[[VALUE_fields:[0-9]+]] fields: array<i32, 4> [storage=static] [align=16] [linkage=internal];
// IR-NEXT:     fn %[[VALUE_classify:[0-9]+]] @classify(%[[VALUE_x:[0-9]+]] x: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %[[VALUE0:[0-9]+]] read<u64>(%[[VALUE_x]])
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE0]] const<u64>(4):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 case %[[VALUE0]] const<u64>(12):
// IR-NEXT:                     return const<i32>(2);
// IR-NEXT:             }
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_table:[0-9]+]] @table() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(const<u64>(12), const<u64>(16));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_values:[0-9]+]] @values() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(4), const<i32>(2)), const<i32>(-4)), const<i32>(5)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(0)), const<i32>(1)), const<i32>(0)), const<i32>(4)), const<i32>(24)), const<i32>(16)), const<i32>(20)), const<i32>(24)))), const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
