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
// DEFAULT-NEXT:     fn %[[VALUE_foo3b:[0-9]+]] @foo3b() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<vla<i32, %[[VALUE1:[0-9]+]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], const<i32>(10));
// DEFAULT-NEXT:             let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// DEFAULT-NEXT:             let %[[VALUE_x:[0-9]+]] x: vla<i32, %[[VALUE1]]> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE1]]>>>(%[[VALUE0]], addr_of<ptr<vla<i32, %[[VALUE1]]>>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<vla<vla<i8, %[[VALUE4:[0-9]+]]>, %[[VALUE5:[0-9]+]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_n_2:[0-9]+]] n: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:             let %[[VALUE5]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:             let %[[VALUE4]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// DEFAULT-NEXT:             let %[[VALUE_x_2:[0-9]+]] x: ptr<vla<vla<i8, %[[VALUE4]]>, %[[VALUE5]]>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %[[VALUE4]]>, %[[VALUE5]]>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_n_2]]), read<i32>(%[[VALUE_n_2]]))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE4]]>>, subtract=false, element=vla<i8, %[[VALUE4]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE4]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE4]]>, %[[VALUE5]]>>>(%[[VALUE_x_2]]))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %[[VALUE4]]>, %[[VALUE5]]>>>(%[[VALUE3]], read<ptr<vla<vla<i8, %[[VALUE4]]>, %[[VALUE5]]>>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE4]]>>, subtract=false, element=vla<i8, %[[VALUE4]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE4]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE4]]>, %[[VALUE5]]>>>(%[[VALUE3]]))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_3:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<vla<vla<i8, %[[VALUE7:[0-9]+]]>, %[[VALUE8:[0-9]+]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_3]], const<i32>(20));
// DEFAULT-NEXT:             let %[[VALUE8]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:             let %[[VALUE7]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// DEFAULT-NEXT:             let %[[VALUE_x_3:[0-9]+]] x: ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_n_3]]), read<i32>(%[[VALUE_n_3]]))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE7]]>>, subtract=false, element=vla<i8, %[[VALUE7]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE7]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>>>(%[[VALUE_x_3]]))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE7]]>>, subtract=false, element=vla<i8, %[[VALUE7]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE7]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>>>(%[[VALUE_x_3]]))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>>>(%[[VALUE6]], read<ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>>>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE7]]>>, subtract=false, element=vla<i8, %[[VALUE7]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE7]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE7]]>, %[[VALUE8]]>>>(%[[VALUE6]]))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5c:[0-9]+]] @foo5c() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_4:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: ptr<vla<vla<i8, %[[VALUE10:[0-9]+]]>, %[[VALUE11:[0-9]+]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_4]], const<i32>(20));
// DEFAULT-NEXT:             let %[[VALUE11]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:             let %[[VALUE10]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:             let %[[VALUE_x_4:[0-9]+]] x: ptr<vla<vla<i8, %[[VALUE10]]>, %[[VALUE11]]>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %[[VALUE10]]>, %[[VALUE11]]>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_n_4]]), read<i32>(%[[VALUE_n_4]]))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE10]]>>, subtract=false, element=vla<i8, %[[VALUE10]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE10]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE10]]>, %[[VALUE11]]>>>(%[[VALUE_x_4]]))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE10]]>>, subtract=false, element=vla<i8, %[[VALUE10]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE10]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE10]]>, %[[VALUE11]]>>>(%[[VALUE_x_4]]))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %[[VALUE10]]>, %[[VALUE11]]>>>(%[[VALUE9]], read<ptr<vla<vla<i8, %[[VALUE10]]>, %[[VALUE11]]>>>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE11]]), mul<u64, overflow=wrap>(read<u64>(%[[VALUE10]]), const<u64>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5b:[0-9]+]] @foo5b() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n_5:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<vla<vla<i8, %[[VALUE13:[0-9]+]]>, %[[VALUE14:[0-9]+]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_n_6:[0-9]+]] n: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:             let %[[VALUE14]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:             let %[[VALUE13]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_6]])));
// DEFAULT-NEXT:             let %[[VALUE_x_5:[0-9]+]] x: ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_n_6]]), read<i32>(%[[VALUE_n_6]]))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE13]]>>, subtract=false, element=vla<i8, %[[VALUE13]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE13]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>>>(%[[VALUE_x_5]]))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE13]]>>, subtract=false, element=vla<i8, %[[VALUE13]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE13]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>>>(%[[VALUE_x_5]]))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>>>(%[[VALUE12]], read<ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>>>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE13]]>>, subtract=false, element=vla<i8, %[[VALUE13]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE13]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE13]]>, %[[VALUE14]]>>>(%[[VALUE12]]))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5a:[0-9]+]] @foo5a() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<vla<vla<i8, %[[VALUE16:[0-9]+]]>, %[[VALUE17:[0-9]+]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_n_7:[0-9]+]] n: i32 [storage=automatic] = const<i32>(20);
// DEFAULT-NEXT:             let %[[VALUE17]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:             let %[[VALUE16]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_7]])));
// DEFAULT-NEXT:             let %[[VALUE_x_6:[0-9]+]] x: ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>> [storage=automatic] = pointer_cast<ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_n_7]]), read<i32>(%[[VALUE_n_7]]))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE16]]>>, subtract=false, element=vla<i8, %[[VALUE16]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE16]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>>>(%[[VALUE_x_6]]))), const<i32>(12)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE16]]>>, subtract=false, element=vla<i8, %[[VALUE16]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE16]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>>>(%[[VALUE_x_6]]))), const<i32>(0)))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>>>(%[[VALUE15]], read<ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>>>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(deref(ptr_offset<ptr<vla<i8, %[[VALUE16]]>>, subtract=false, element=vla<i8, %[[VALUE16]]>, overflow=ub>(array_decay<ptr<vla<i8, %[[VALUE16]]>>, length=None>(deref(read<ptr<vla<vla<i8, %[[VALUE16]]>, %[[VALUE17]]>>>(%[[VALUE15]]))), const<i32>(12)))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(40), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn() -> i32>(%[[VALUE_foo3b]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%[[VALUE_foo4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(400), call<i32, signature=fn() -> i32>(%[[VALUE_foo5c]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%[[VALUE_foo5a]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%[[VALUE_foo5b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn() -> i32>(%[[VALUE_foo5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
