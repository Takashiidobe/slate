/* { dg-require-stack-size "0x10000" } */

/* When generating o32 MIPS PIC, main's $gp save slot was out of range
   of a single load instruction.  */
struct big {
  int i[sizeof(int) >= 4 && sizeof(void *) >= 4 ? 0x4000 : 4];
};
struct big gb;
int        foo(struct big b, int x) { return b.i[x]; }
int        main(void) { return foo(gb, 0) + foo(gb, 1); }


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
// DEFAULT-NEXT:     type @type[[TYPE_big:[0-9]+]] big = struct {
// DEFAULT-NEXT:         field0 i: array<i32, 16384>;
// DEFAULT-NEXT:     } [size=65536, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_gb:[0-9]+]] gb: @type[[TYPE_big]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_b:[0-9]+]] b: @type[[TYPE_big]], %[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16384)>(field0(%[[VALUE_b]])), read<i32>(%[[VALUE_x]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(@type[[TYPE_big]], i32) -> i32, abi=sysv64(native_c, scalar) -> scalar>(%[[VALUE_foo]], copy<@type[[TYPE_big]], reason=arg>(read<@type[[TYPE_big]]>(%[[VALUE_gb]])), const<i32>(0)), call<i32, signature=fn(@type[[TYPE_big]], i32) -> i32, abi=sysv64(native_c, scalar) -> scalar>(%[[VALUE_foo]], copy<@type[[TYPE_big]], reason=arg>(read<@type[[TYPE_big]]>(%[[VALUE_gb]])), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
