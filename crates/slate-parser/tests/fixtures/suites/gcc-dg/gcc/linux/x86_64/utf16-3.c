/* Contributed by Kris Van Hees <kris.van.hees@oracle.com> */
/* Test concatenation of char16_t* string literals. */
/* { dg-do run } */
/* { dg-options "-std=gnu99 -Wall -Werror" } */

typedef __CHAR16_TYPE__ char16_t;

extern void abort (void);

char16_t	*s0 = u"a" u"b";

char16_t	*s1 = u"a" "b";
char16_t	*s2 = "a" u"b";
char16_t	*s3 = u"a" "\u2029";
char16_t	*s4 = "\u2029" u"b";
char16_t	*s5 = u"a" "\U00064321";
char16_t	*s6 = "\U00064321" u"b";

#define A	0x0061
#define B	0x0062
#define X	0x2029
#define Y1	0xD950
#define Y2	0xDF21

int main ()
{
    if (sizeof ((u"a" u"b")[0]) != sizeof (char16_t))
	abort ();
    if (sizeof ((u"a"  "b")[0]) != sizeof (char16_t))
	abort ();
    if (sizeof (( "a" u"b")[0]) != sizeof (char16_t))
	abort ();

    if (s0[0] != A || s0[1] != B || s0[2] != 0x0000)
	abort ();

    if (s1[0] != A || s1[1] != B || s1[2] != 0x0000)
	abort ();
    if (s2[0] != A || s2[1] != B || s2[2] != 0x0000)
	abort ();
    if (s3[0] != A || s3[1] != X || s3[2] != 0x0000)
	abort ();
    if (s4[0] != X || s4[1] != B || s4[2] != 0x0000)
	abort ();
    if (s5[0] != A || s5[1] != Y1 || s5[2] != Y2 || s5[3] != 0x0000)
	abort ();
    if (s6[0] != Y1 || s6[1] != Y2 || s6[2] != B || s6[3] != 0x0000)
	abort ();
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type[[TYPE_char16_t:[0-9]+]] char16_t = u16;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s0:[0-9]+]] s0: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_str]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_str_2]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_str_3]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([97, 8233, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s3:[0-9]+]] s3: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_str_4]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([8233, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s4:[0-9]+]] s4: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(3)>(%[[VALUE_str_5]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<u16, 4> [storage=static] = code_units<array<u16, 4>>([97, 55632, 57121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s5:[0-9]+]] s5: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(4)>(%[[VALUE_str_6]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<u16, 4> [storage=static] = code_units<array<u16, 4>>([55632, 57121, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s6:[0-9]+]] s6: ptr<u16> [storage=static] = array_decay<ptr<u16>, length=Some(4)>(%[[VALUE_str_7]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s0]]), const<i32>(0)))))), const<i32>(97)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s0]]), const<i32>(1)))))), const<i32>(98))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s0]]), const<i32>(2)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s1]]), const<i32>(0)))))), const<i32>(97)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s1]]), const<i32>(1)))))), const<i32>(98))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s1]]), const<i32>(2)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s2]]), const<i32>(0)))))), const<i32>(97)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s2]]), const<i32>(1)))))), const<i32>(98))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s2]]), const<i32>(2)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s3]]), const<i32>(0)))))), const<i32>(97)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s3]]), const<i32>(1)))))), const<i32>(8233))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s3]]), const<i32>(2)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s4]]), const<i32>(0)))))), const<i32>(8233)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s4]]), const<i32>(1)))))), const<i32>(98))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s4]]), const<i32>(2)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s5]]), const<i32>(0)))))), const<i32>(97)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s5]]), const<i32>(1)))))), const<i32>(55632))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s5]]), const<i32>(2)))))), const<i32>(57121))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s5]]), const<i32>(3)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s6]]), const<i32>(0)))))), const<i32>(55632)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s6]]), const<i32>(1)))))), const<i32>(57121))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s6]]), const<i32>(2)))))), const<i32>(98))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_s6]]), const<i32>(3)))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
