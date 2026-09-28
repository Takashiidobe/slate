/* { dg-do run } */
/* { dg-require-effective-target lto } */
/* { dg-options "-O2 -flto" } */

typedef int *A;

void* foo(void* a, void *b, void *c, void *d)
{
	*(A**)a = c;

	typedef enum E *B;
	enum E { E1 = -1, E2 = 0, E3 = 1, MAX = __INT_MAX__ };
	{
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
// DEFAULT-NEXT:     type @type0 A = ptr<i32>;
// DEFAULT-NEXT:     type @type1 E = enum incomplete;
// DEFAULT-NEXT:     type @type2 B = ptr<@type1>;
// DEFAULT-NEXT:     fn %1 @foo(%2 a: ptr<void>, %3 b: ptr<void>, %4 c: ptr<void>, %5 d: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(deref(pointer_cast<ptr<ptr<ptr<i32>>>, reason=explicit>(read<ptr<void>>(%2))), pointer_cast<ptr<ptr<i32>>, reason=assign>(read<ptr<void>>(%4)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<ptr<ptr<@type1>>>(deref(pointer_cast<ptr<ptr<ptr<@type1>>>, reason=explicit>(read<ptr<void>>(%3))), pointer_cast<ptr<ptr<@type1>>, reason=assign>(read<ptr<void>>(%5)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<ptr<i32>>>(deref(pointer_cast<ptr<ptr<ptr<i32>>>, reason=explicit>(read<ptr<void>>(%2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 a: ptr<ptr<i32>> [storage=automatic];
// DEFAULT-NEXT:         let %14 b: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %15 c: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<ptr<i32>>>(addr_of<ptr<ptr<i32>>>(%15), pointer_cast<ptr<ptr<i32>>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<void>, ptr<void>) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<ptr<i32>>>>(%13)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<ptr<i32>>>>(%13)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%14)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%15)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
