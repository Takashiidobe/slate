/* { dg-do compile } */
/* { dg-options "-Wc++-compat" } */
enum E1 { A };
enum E2 { B };
int
f1 (int i)
{
  return (int) (i ? A : B);	/* { dg-warning "invalid in C\[+\]\[+\]" } */
}
extern enum E1 f2();
int
f3 (int i)
{
  return (int) (i ? f2 () : B);	/* { dg-warning "invalid in C\[+\]\[+\]" } */
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
// DEFAULT-NEXT:     type @type[[TYPE_E1:[0-9]+]] E1 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_E2:[0-9]+]] E2 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] B = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> @type[[TYPE_E1]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%[[VALUE0]], enum_to_int<u32, reason=promotion>(call<@type[[TYPE_E1]], signature=fn() -> @type[[TYPE_E1]]>(%[[VALUE_f2]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE0]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
