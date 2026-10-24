// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=gnu11

struct holder {
  char tag;
  long long ll;
};

long long pair[2];

unsigned gnu_type(void) { return __alignof__(long long); }
unsigned gnu_array(void) { return __alignof__(double[3]); }
unsigned gnu_object(void) { return __alignof__(pair); }
unsigned gnu_int(void) { return __alignof__(int); }
unsigned iso_type(void) { return _Alignof(long long); }
unsigned iso_array(void) { return _Alignof(long long[2]); }
unsigned gnu_member(struct holder *p) { return __alignof__(p->ll); }
unsigned gnu_struct(void) { return __alignof__(struct holder); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_holder:[0-9]+]] holder = struct {
// IR-NEXT:         field0 tag: i8;
// IR-NEXT:         field1 ll: i64;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     global %[[VALUE_pair:[0-9]+]] pair: array<i64, 2> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_gnu_type:[0-9]+]] @gnu_type() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(8);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gnu_array:[0-9]+]] @gnu_array() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(8);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gnu_object:[0-9]+]] @gnu_object() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(8);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gnu_int:[0-9]+]] @gnu_int() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(4);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_iso_type:[0-9]+]] @iso_type() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(4);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_iso_array:[0-9]+]] @iso_array() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(4);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gnu_member:[0-9]+]] @gnu_member(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_holder]]>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(4);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gnu_struct:[0-9]+]] @gnu_struct() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(4);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
