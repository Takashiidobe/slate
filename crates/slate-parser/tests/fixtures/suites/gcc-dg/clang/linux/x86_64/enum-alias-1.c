/* { dg-do run } */
/* { dg-options "-O2" } */

enum E { E1 = -1, E2 = 0, E3 = 1, MAX = __INT_MAX__ };

typedef int A;
typedef enum E B;

_Static_assert(_Generic((A){ 0 }, B: 1), "");

void* foo(void* a, void *b, A *c, B *d)
{
	*(A**)a = c;
	*(B**)b = d;
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
// DEFAULT-NEXT:     type @type0 E = enum : i32 {
// DEFAULT-NEXT:         %0 E1 = const<i32>(-1);
// DEFAULT-NEXT:         %1 E2 = const<i32>(0);
// DEFAULT-NEXT:         %2 E3 = const<i32>(1);
// DEFAULT-NEXT:         %3 MAX = const<i32>(2147483647);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 A = i32;
// DEFAULT-NEXT:     type @type2 B = @type0;
// DEFAULT-NEXT:     fn %7 @foo(%8 a: ptr<void>, %9 b: ptr<void>, %10 c: ptr<i32>, %11 d: ptr<@type0>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%8))), read<ptr<i32>>(%10));
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(pointer_cast<ptr<ptr<@type0>>, reason=explicit>(read<ptr<void>>(%9))), read<ptr<@type0>>(%11));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(read<ptr<void>>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 a: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %14 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 c: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<i32>>(addr_of<ptr<i32>>(%15), pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<i32>, ptr<@type0>) -> ptr<void>>(%7, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%13)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<i32>>>(%13)), addr_of<ptr<i32>>(%14), pointer_cast<ptr<@type0>, reason=arg>(addr_of<ptr<i32>>(%15)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
