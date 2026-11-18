// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct rq;
typedef struct rq rq128 __attribute__((aligned(128)));
extern struct rq requested __attribute__((aligned(64)));
extern rq128 lowered __attribute__((aligned(8)));
extern rq128 inherited;
extern struct rq plain;

_Static_assert(__alignof__(requested) == 64, "");

unsigned long requested_alignment(void) { return __alignof__(requested); }
unsigned long lowered_alignment(void) { return __alignof__(lowered); }
unsigned long inherited_alignment(void) { return __alignof__(inherited); }
unsigned long plain_alignment(void) { return _Alignof(plain); }

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
// IR-NEXT:     type @type[[TYPE_rq:[0-9]+]] rq = struct incomplete;
// IR-NEXT:     type @type[[TYPE_rq128:[0-9]+]] rq128 = @type[[TYPE_rq]];
// IR-NEXT:     extern %[[VALUE_requested:[0-9]+]] requested: @type[[TYPE_rq]] [storage=static] [align=64] [linkage=external];
// IR-NEXT:     extern %[[VALUE_lowered:[0-9]+]] lowered: @type[[TYPE_rq]] [storage=static] [align=128] [linkage=external] [c="rq128"];
// IR-NEXT:     extern %[[VALUE_inherited:[0-9]+]] inherited: @type[[TYPE_rq]] [storage=static] [align=128] [linkage=external] [c="rq128"];
// IR-NEXT:     extern %[[VALUE_plain:[0-9]+]] plain: @type[[TYPE_rq]] [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_requested_alignment:[0-9]+]] @requested_alignment() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<u64>(const<u64>(64) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_lowered_alignment:[0-9]+]] @lowered_alignment() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<u64>(const<u64>(8) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_inherited_alignment:[0-9]+]] @inherited_alignment() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<u64>(const<u64>(128) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain_alignment:[0-9]+]] @plain_alignment() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<u64>(const<u64>(1) [c="__size_t"]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
