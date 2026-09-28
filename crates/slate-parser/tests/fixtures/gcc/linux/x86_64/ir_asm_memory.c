// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
typedef int v4 __attribute__((vector_size(16)));
struct Pair { int b : 3; int w; };

void unaddressable(struct Pair *s, v4 v) {
    register int r = 1;
    asm("# %0 %1 %2" : : "rm"(s->b), "m"(v[1]), "rm"(r));
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
// IR-NEXT:     type @type0 v4 = vector<i32, 4>;
// IR-NEXT:     type @type1 Pair = struct {
// IR-NEXT:         field0 b: i32 : 3;
// IR-NEXT:         field1 w: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// IR-NEXT:     fn %2 @unaddressable(%3 s: ptr<@type1>, %4 v: vector<i32, 4>) -> void [linkage=external] [abi=sysv64(scalar, direct) -> void] [fallthrough=ret_void] {
// IR-NEXT:         let %5 r: i32 [storage=automatic] = const<i32>(1);
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "rm" [reg | mem] -> reg width 32 read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(deref(read<ptr<@type1>>(%3))));
// IR-NEXT:             in 1 "m" [mem] width 32 read<i32>(lane(%4, const<i32>(1)));
// IR-NEXT:             in 2 "rm" [reg | mem] -> reg width 32 read<i32>(%5);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
