/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -Wzero-as-null-pointer-constant" } */

void foo(void*);

void bar()
{
	enum { E = 0 };
	constexpr _BitInt(4) b0 = 0;
	foo(false);			/* { dg-warning "zero as null pointer constant" } */
	foo(b0);			/* { dg-warning "zero as null pointer constant" } */
	foo(E);				/* { dg-warning "zero as null pointer constant" } */

	void *p = false;		/* { dg-warning "zero as null pointer constant" } */
	void *r = b0;			/* { dg-warning "zero as null pointer constant" } */
	void *t = E;			/* { dg-warning "zero as null pointer constant" } */

	1 ? false : p;			/* { dg-warning "zero as null pointer constant" } */
	1 ? p : false;			/* { dg-warning "zero as null pointer constant" } */
	1 ? b0 : p;			/* { dg-warning "zero as null pointer constant" } */
	1 ? p : b0;			/* { dg-warning "zero as null pointer constant" } */
	1 ? E : p;			/* { dg-warning "zero as null pointer constant" } */
	1 ? p : E;			/* { dg-warning "zero as null pointer constant" } */

	if (p == false);		/* { dg-warning "zero as null pointer constant" } */
	if (false == p);		/* { dg-warning "zero as null pointer constant" } */
	if (p == b0);			/* { dg-warning "zero as null pointer constant" } */
	if (b0 == p);			/* { dg-warning "zero as null pointer constant" } */
	if (p == E);			/* { dg-warning "zero as null pointer constant" } */
	if (E == p);			/* { dg-warning "zero as null pointer constant" } */
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E:[0-9]+]] E = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_E]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b0:[0-9]+]] b0: i4b [storage=automatic] [const] [constexpr] = truncate<i4b, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_E]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_E]], null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_E]], null<ptr<void>>);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), null<ptr<void>>, read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), null<ptr<void>>, read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>);
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), null<ptr<void>>, read<ptr<void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>);
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<void>>(null<ptr<void>>, read<ptr<void>>(%[[VALUE_p]]))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<void>>(null<ptr<void>>, read<ptr<void>>(%[[VALUE_p]]))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>)
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if eq<ptr<void>>(null<ptr<void>>, read<ptr<void>>(%[[VALUE_p]]))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
