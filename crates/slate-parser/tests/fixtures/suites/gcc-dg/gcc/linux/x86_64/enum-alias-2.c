/* { dg-do run } */
/* { dg-options "-O2" } */

typedef int A;

void* foo(void* a, void *b, void *c, void *d)
{
	*(A**)a = c;

	{
		typedef enum E B;
		enum E { E1 = -1, E2 = 0, E3 = 1, MAX = __INT_MAX__ };
		*(B**)b = d;
	}

	return *(A**)a;
}

int main()
{
	A *a, b, c;
	if (&c != (A*)foo(&a, &a, &b, &c))
		__builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = i32;
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_E1:[0-9]+]] E1 = const<i32>(-1);
// DEFAULT-NEXT:         %[[VALUE_E2:[0-9]+]] E2 = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_E3:[0-9]+]] E3 = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_MAX:[0-9]+]] MAX = const<i32>(2147483647);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = @type[[TYPE_E]];
// DEFAULT-NEXT:     fn %[[VALUE_E2]] @foo(%[[VALUE_E3]] a: ptr<void>, %[[VALUE_MAX]] b: ptr<void>, %[[VALUE_c:[0-9]+]] c: ptr<void>, %[[VALUE_d:[0-9]+]] d: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%[[VALUE_E3]]))), pointer_cast<ptr<i32>, reason=assign>(read<ptr<void>>(%[[VALUE_c]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_E]]>>(deref(pointer_cast<ptr<ptr<@type[[TYPE_E]]>>, reason=explicit>(read<ptr<void>>(%[[VALUE_MAX]]))), pointer_cast<ptr<@type[[TYPE_E]]>, reason=assign>(read<ptr<void>>(%[[VALUE_d]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%[[VALUE_E3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_c_2]]), pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<void>, ptr<void>) -> ptr<void>>(%[[VALUE_E2]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%[[VALUE_a]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%[[VALUE_a]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_b]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_c_2]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
