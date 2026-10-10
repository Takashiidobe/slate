// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
struct S { int a; };
enum E { A };

void rvalues(int c, int x, long l, int *p, struct S *s, enum E e, struct S y) {
    asm("# %0 %1 %2" : : "m"(({ x; })), "m"(({ unsigned v = 3; v; })), "m"(({ *s; })));
    asm("# %0 %1 %2" : : "m"(c ? x : 1), "m"(x ?: c), "m"(c ? *s : y));
    asm("# %0 %1" : : "m"((c, x)), "m"((c, (unsigned)x)));
    asm("# %0 %1 %2 %3" : : "m"((unsigned)x), "m"((long)p), "m"((long long)l), "m"((unsigned)e));
    asm("# %0 %1" : : "m"((int)(unsigned)x), "m"((char *)p));
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
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// IR-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     fn %[[VALUE_rvalues:[0-9]+]] @rvalues(%[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>, %[[VALUE_e:[0-9]+]] e: @type[[TYPE_E]], %[[VALUE_y:[0-9]+]] y: @type[[TYPE_S]]) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             write<i32>(%[[VALUE0]], read<i32>(%[[VALUE_x]]));
// IR-NEXT:         }
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_v:[0-9]+]] v: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(3));
// IR-NEXT:             write<u32>(%[[VALUE1]], read<u32>(%[[VALUE_v]]));
// IR-NEXT:         }
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: @type[[TYPE_S]] [synthetic];
// IR-NEXT:         {
// IR-NEXT:             write<@type[[TYPE_S]]>(%[[VALUE2]], read<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "m" [mem] width 32 read<i32>(%[[VALUE0]]);
// IR-NEXT:             in 1 "m" [mem] width 32 read<u32>(%[[VALUE1]]);
// IR-NEXT:             in 2 "m" [mem] width 32 read<@type[[TYPE_S]]>(%[[VALUE2]]);
// IR-NEXT:         }
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "m" [mem] width 32 conditional<i32>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)), read<i32>(%[[VALUE_x]]), const<i32>(1));
// IR-NEXT:             in 1 "m" [mem] width 32 conditional<i32>(ne<i32>(read<i32>(%[[VALUE3]]), const<i32>(0)), read<i32>(%[[VALUE3]]), read<i32>(%[[VALUE_c]]));
// IR-NEXT:             in 2 "m" [mem] width 32 conditional<@type[[TYPE_S]]>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)), read<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))), read<@type[[TYPE_S]]>(%[[VALUE_y]]));
// IR-NEXT:         }
// IR-NEXT:         read<i32>(%[[VALUE_c]]);
// IR-NEXT:         read<i32>(%[[VALUE_c]]);
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "m" [mem] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "m" [mem] width 32 reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_x]]));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "m" [mem] width 32 reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_x]]));
// IR-NEXT:             in 1 "m" [mem] width 64 ptr_to_int<i64, reason=explicit>(read<ptr<i32>>(%[[VALUE_p]]));
// IR-NEXT:             in 2 "m" [mem] width 64 read<i64>(%[[VALUE_l]]);
// IR-NEXT:             in 3 "m" [mem] width 32 enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e]]));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "m" [mem] width 32 reinterpret<i32, reason=explicit, fits=unknown>(reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_x]])));
// IR-NEXT:             in 1 "m" [mem] width 64 pointer_cast<ptr<i8>, reason=explicit>(read<ptr<i32>>(%[[VALUE_p]]));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
