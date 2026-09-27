/* PR29970, PR91038 */
/* { dg-do run } */
/* { dg-options "-O0 -Wunused-variable" } */

int foo3b(void)   // should not return 0
{
        int n = 0;
        return sizeof *({ n = 10; int x[n]; &x; });
}

int foo4(void)   // should not ICE
{
        return (*({
                        int n = 20;
                        char (*x)[n][n] = __builtin_malloc(n * n);
                        (*x)[12][1] = 1;
                        x;
                }))[12][1];
}

int foo5(void)   // should return 1, returns 0
{
        int n = 0;
        return (*({
                        n = 20;
                        char (*x)[n][n] = __builtin_malloc(n * n);
                        (*x)[12][1] = 1;
                        (*x)[0][1] = 0;
                        x;
                }))[12][1];
}

int foo5c(void)   // should return 400 
{
        int n = 0;
        return sizeof(*({
                        n = 20;
                        char (*x)[n][n] = __builtin_malloc(n * n);
                        (*x)[12][1] = 1;
                        (*x)[0][1] = 0;
                        x;
                }));
}

int foo5b(void)   // should return 1, returns 0
{
	int n = 0;			/* { dg-warning "unused variable" } */
        return (*({
                        int n = 20;
                        char (*x)[n][n] = __builtin_malloc(n * n);
                        (*x)[12][1] = 1;
                        (*x)[0][1] = 0;
                        x;
                }))[12][1];
}

int foo5a(void)   // should return 1, returns 0
{
        return (*({
                        int n = 20;
                        char (*x)[n][n] = __builtin_malloc(n * n);
                        (*x)[12][1] = 1;
                        (*x)[0][1] = 0;
                        x;
                }))[12][1];
}




int main()
{
	if (sizeof(int[10]) != foo3b())
		__builtin_abort();

	if (1 != foo4())
		__builtin_abort();

	if (400 != foo5c())
		__builtin_abort();

	if (1 != foo5a())
		__builtin_abort();

	if (1 != foo5b()) // -O0
		__builtin_abort();

	if (1 != foo5())
		__builtin_abort();

	return 0;
}



// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @foo3b() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %1 n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %34: ptr<vla<i32, %20>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%1, const<i32>(10));
// DEFAULT-NEXT:             let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%1)));
// DEFAULT-NEXT:             let %2 x: vla<i32, %20> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<vla<i32, %20>>>(%34, addr_of<ptr<vla<i32, %20>>>(%2));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%20), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @__builtin_malloc(%23 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35: ptr<vla<vla<i8, %22>, %21>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %4 n: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:             let %21: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:             let %22: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:             let %5 x: ptr<vla<vla<i8, %22>, %21>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %22>, %21>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%4), read<i32>(%4))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %22>>, subtract=false, element=vla<i8, %22>, overflow=ub>(array_decay<ptr<vla<i8, %22>>, length=None>(deref(read<ptr<vla<vla<i8, %22>, %21>>>(%5))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %22>, %21>>>(%35, read<ptr<vla<vla<i8, %22>, %21>>>(%5));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %22>>, subtract=false, element=vla<i8, %22>, overflow=ub>(array_decay<ptr<vla<i8, %22>>, length=None>(deref(read<ptr<vla<vla<i8, %22>, %21>>>(%35))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %36: ptr<vla<vla<i8, %26>, %25>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%7, const<i32>(20));
// DEFAULT-NEXT:             let %25: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// DEFAULT-NEXT:             let %26: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// DEFAULT-NEXT:             let %8 x: ptr<vla<vla<i8, %26>, %25>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %26>, %25>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%7), read<i32>(%7))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %26>>, subtract=false, element=vla<i8, %26>, overflow=ub>(array_decay<ptr<vla<i8, %26>>, length=None>(deref(read<ptr<vla<vla<i8, %26>, %25>>>(%8))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %26>>, subtract=false, element=vla<i8, %26>, overflow=ub>(array_decay<ptr<vla<i8, %26>>, length=None>(deref(read<ptr<vla<vla<i8, %26>, %25>>>(%8))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %26>, %25>>>(%36, read<ptr<vla<vla<i8, %26>, %25>>>(%8));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %26>>, subtract=false, element=vla<i8, %26>, overflow=ub>(array_decay<ptr<vla<i8, %26>>, length=None>(deref(read<ptr<vla<vla<i8, %26>, %25>>>(%36))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @foo5c() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %37: ptr<vla<vla<i8, %28>, %27>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%10, const<i32>(20));
// DEFAULT-NEXT:             let %27: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// DEFAULT-NEXT:             let %28: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// DEFAULT-NEXT:             let %11 x: ptr<vla<vla<i8, %28>, %27>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %28>, %27>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%10), read<i32>(%10))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %28>>, subtract=false, element=vla<i8, %28>, overflow=ub>(array_decay<ptr<vla<i8, %28>>, length=None>(deref(read<ptr<vla<vla<i8, %28>, %27>>>(%11))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %28>>, subtract=false, element=vla<i8, %28>, overflow=ub>(array_decay<ptr<vla<i8, %28>>, length=None>(deref(read<ptr<vla<vla<i8, %28>, %27>>>(%11))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %28>, %27>>>(%37, read<ptr<vla<vla<i8, %28>, %27>>>(%11));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%27), mul<u64, overflow=wrap>(read<u64>(%28), const<u64>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @foo5b() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %38: ptr<vla<vla<i8, %30>, %29>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %14 n: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:             let %29: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%14)));
// DEFAULT-NEXT:             let %30: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%14)));
// DEFAULT-NEXT:             let %15 x: ptr<vla<vla<i8, %30>, %29>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %30>, %29>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%14), read<i32>(%14))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %30>>, subtract=false, element=vla<i8, %30>, overflow=ub>(array_decay<ptr<vla<i8, %30>>, length=None>(deref(read<ptr<vla<vla<i8, %30>, %29>>>(%15))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %30>>, subtract=false, element=vla<i8, %30>, overflow=ub>(array_decay<ptr<vla<i8, %30>>, length=None>(deref(read<ptr<vla<vla<i8, %30>, %29>>>(%15))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %30>, %29>>>(%38, read<ptr<vla<vla<i8, %30>, %29>>>(%15));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %30>>, subtract=false, element=vla<i8, %30>, overflow=ub>(array_decay<ptr<vla<i8, %30>>, length=None>(deref(read<ptr<vla<vla<i8, %30>, %29>>>(%38))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @foo5a() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %39: ptr<vla<vla<i8, %32>, %31>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %17 n: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:             let %31: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%17)));
// DEFAULT-NEXT:             let %32: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%17)));
// DEFAULT-NEXT:             let %18 x: ptr<vla<vla<i8, %32>, %31>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %32>, %31>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%24, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%17), read<i32>(%17))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %32>>, subtract=false, element=vla<i8, %32>, overflow=ub>(array_decay<ptr<vla<i8, %32>>, length=None>(deref(read<ptr<vla<vla<i8, %32>, %31>>>(%18))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %32>>, subtract=false, element=vla<i8, %32>, overflow=ub>(array_decay<ptr<vla<i8, %32>>, length=None>(deref(read<ptr<vla<vla<i8, %32>, %31>>>(%18))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %32>, %31>>>(%39, read<ptr<vla<vla<i8, %32>, %31>>>(%18));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %32>>, subtract=false, element=vla<i8, %32>, overflow=ub>(array_decay<ptr<vla<i8, %32>>, length=None>(deref(read<ptr<vla<vla<i8, %32>, %31>>>(%39))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(40), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(400), call<i32, signature=fn() -> i32>(%9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%33);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
