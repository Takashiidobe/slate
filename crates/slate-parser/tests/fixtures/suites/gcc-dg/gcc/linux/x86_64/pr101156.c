/* { dg-do compile } */
/* { dg-options "-fchecking" } */
/* { dg-require-effective-target nonlocal_goto } */

struct S { int i; };
void baz(struct S *p)
{
  __builtin_setjmp(p--);
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_setjmp:[0-9]+]] @__builtin_setjmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_S]]>, subtract=true, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE___builtin_setjmp]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
