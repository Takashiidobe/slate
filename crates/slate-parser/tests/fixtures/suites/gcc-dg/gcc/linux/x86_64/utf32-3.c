/* Contributed by Kris Van Hees <kris.van.hees@oracle.com> */
/* Test concatenation of char32_t* string literals. */
/* { dg-do run } */
/* { dg-options "-std=gnu99 -Wall -Werror" } */

typedef __CHAR32_TYPE__ char32_t;

extern void abort (void);

char32_t	*s0 = U"a" U"b";

char32_t	*s1 = U"a" "b";
char32_t	*s2 = "a" U"b";
char32_t	*s3 = U"a" "\u2029";
char32_t	*s4 = "\u2029" U"b";
char32_t	*s5 = U"a" "\U00064321";
char32_t	*s6 = "\U00064321" U"b";

#define A	0x00000061
#define B	0x00000062
#define X	0x00002029
#define Y	0x00064321

int main ()
{
    if (sizeof ((U"a" U"b")[0]) != sizeof (char32_t))
	abort ();
    if (sizeof ((U"a"  "b")[0]) != sizeof (char32_t))
	abort ();
    if (sizeof (( "a" U"b")[0]) != sizeof (char32_t))
	abort ();

    if (s0[0] != A || s0[1] != B || s0[2] != 0x00000000)
	abort ();

    if (s1[0] != A || s1[1] != B || s1[2] != 0x00000000)
	abort ();
    if (s2[0] != A || s2[1] != B || s2[2] != 0x00000000)
	abort ();
    if (s3[0] != A || s3[1] != X || s3[2] != 0x00000000)
	abort ();
    if (s4[0] != X || s4[1] != B || s4[2] != 0x00000000)
	abort ();
    if (s5[0] != A || s5[1] != Y || s5[2] != 0x00000000)
	abort ();
    if (s6[0] != Y || s6[1] != B || s6[2] != 0x00000000)
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
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s0:[0-9]+]] s0: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str_2]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str_3]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([97, 8233, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s3:[0-9]+]] s3: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str_4]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([8233, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s4:[0-9]+]] s4: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str_5]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([97, 410401, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s5:[0-9]+]] s5: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str_6]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([410401, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s6:[0-9]+]] s6: ptr<u32> [storage=static] = array_decay<ptr<u32>, length=Some(3)>(%[[VALUE_str_7]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s0]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(97))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s0]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(98)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s0]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s1]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(97))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s1]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(98)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s1]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s2]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(97))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s2]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(98)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s2]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s3]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(97))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s3]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8233)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s3]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s4]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8233))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s4]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(98)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s4]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s5]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(97))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s5]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(410401)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s5]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s6]]), const<i32>(0)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(410401))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s6]]), const<i32>(1)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(98)))), ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_s6]]), const<i32>(2)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
