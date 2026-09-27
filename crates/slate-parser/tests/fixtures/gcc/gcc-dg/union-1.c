/* PR target/15783 */
/* Origin: Paul Pluzhnikov <ppluzhnikov@charter.net> */

/* This used to ICE on SPARC 64-bit because the back-end was
   returning an invalid construct for the return value of fu2.  */

/* { dg-do compile } */

union u2 {
    struct
    {
        int u2s_a, u2s_b, u2s_c, u2s_d, u2s_e;
    } u2_s;
    double u2_d;
} u2a;

union u2 fu2();

void unions()
{
    u2a = fu2();
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type0 u2 = union {
// DEFAULT-NEXT:         field0 u2_s: @type1;
// DEFAULT-NEXT:         field1 u2_d: f64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 u2s_a: i32;
// DEFAULT-NEXT:         field1 u2s_b: i32;
// DEFAULT-NEXT:         field2 u2s_c: i32;
// DEFAULT-NEXT:         field3 u2s_d: i32;
// DEFAULT-NEXT:         field4 u2s_e: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     global %2 u2a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @fu2(unprototyped) -> @type0 [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %4 @unions(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(%2, copy<@type0, reason=assign>(call<@type0, signature=fn(unprototyped) -> @type0, abi=sysv64() -> native_c>(%3)));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(unprototyped) -> @type0, abi=sysv64() -> native_c>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
