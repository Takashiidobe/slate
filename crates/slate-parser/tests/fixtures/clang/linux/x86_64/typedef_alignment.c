typedef int raised __attribute__((aligned(16)));
typedef int lowered __attribute__((aligned(1)));
typedef raised chained;
typedef raised overridden __attribute__((aligned(4)));
typedef long long packed_long __attribute__((aligned(2)));

struct holds_raised { char c; raised value; };
struct holds_lowered { char c; lowered value; };
struct holds_chain { char c; chained first; overridden second; };
struct holds_array { char c; packed_long values[2]; };

raised global_raised;
lowered global_lowered;
packed_long global_requested __attribute__((aligned(4)));

unsigned long alignments[] = {
    _Alignof(struct holds_raised), _Alignof(struct holds_lowered),
    _Alignof(struct holds_chain),  _Alignof(struct holds_array),
    _Alignof(raised),              _Alignof(lowered),
    _Alignof(chained),             _Alignof(overridden),
    _Alignof(packed_long[3]),      _Alignof(_Atomic raised),
    __alignof__(global_lowered),
};

int locals(void) {
  raised r = 1;
  lowered l = 2;
  return r + l;
}



// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES IR

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
// IR-NEXT:     type @type0 raised = i32;
// IR-NEXT:     type @type1 lowered = i32;
// IR-NEXT:     type @type2 chained = i32;
// IR-NEXT:     type @type3 overridden = i32;
// IR-NEXT:     type @type4 packed_long = i64;
// IR-NEXT:     type @type5 holds_raised = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 value: i32;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type6 holds_lowered = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 value: i32;
// IR-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-NEXT:     type @type7 holds_chain = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 first: i32;
// IR-NEXT:         field2 second: i32;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16, 20]];
// IR-NEXT:     type @type8 holds_array = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 values: array<i64, 2>;
// IR-NEXT:     } [size=18, align=2, offsets=[0, 2]];
// IR-NEXT:     global %9 global_raised: i32 [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %10 global_lowered: i32 [storage=static] [align=1] [linkage=external];
// IR-NEXT:     global %11 global_requested: i64 [storage=static] [align=4] [linkage=external];
// IR-NEXT:     global %12 alignments: array<u64, 11> [storage=static] [align=16] = aggregate<array<u64, 11>, zero_fill=false>(index0 = const<u64>(16), index1 = const<u64>(1), index2 = const<u64>(16), index3 = const<u64>(2), index4 = const<u64>(16), index5 = const<u64>(1), index6 = const<u64>(16), index7 = const<u64>(4), index8 = const<u64>(2), index9 = const<u64>(4), index10 = const<u64>(1)) [linkage=external];
// IR-NEXT:     fn %13 @locals() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %14 r: i32 [storage=automatic] [align=16] = const<i32>(1);
// IR-NEXT:         let %15 l: i32 [storage=automatic] [align=1] = const<i32>(2);
// IR-NEXT:         return add<i32>(read<i32>(%14), read<i32>(%15));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
