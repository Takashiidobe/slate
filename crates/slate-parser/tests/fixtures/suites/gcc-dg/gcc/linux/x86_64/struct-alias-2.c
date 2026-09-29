/* { dg-do run } */
/* { dg-options "-O2 -std=c23" } */


/* Based on the submitted test case for PR123356 but using types with
 * tags and moving the second version into another scope.  */

struct foo { long x; };

void f()
{
	struct foo { };
	struct bar { struct foo *c; };
	union baz { struct foo *c; };
	struct arr { struct foo *c[1]; };
	struct fun { struct foo (*c)(); };
}


void f1()
{
	struct foo { };
	struct bar { struct foo *c; };
	union baz { struct foo *c; };
	struct arr { struct foo *c[1]; };
	struct fun { struct foo (*c)(); };
}

struct bar { struct foo *c; };
union baz { struct foo *c; };
struct arr { struct foo *c[1]; };
struct fun { struct foo (*c)(); };

void f2()
{
	struct foo { int y; };
	struct bar { struct foo *c; };
	union baz { struct foo *c; };
	struct arr { struct foo *c[1]; };
	struct fun { struct foo (*c)(); };
}

__attribute__((noinline))
struct foo * g1(struct bar *B, struct bar *Q)
{
    struct bar t = *B;
    *B = *Q;
    *Q = t;
    return B->c;
}

__attribute__((noinline))
struct foo * g2(union baz *B, union baz *Q)
{
    union baz t = *B;
    *B = *Q;
    *Q = t;
    return B->c;
}

__attribute__((noinline))
struct foo { long x; } * 
	g3(struct bar { struct foo { long x; } *c; } *B,
	   struct bar { struct foo { long x; } *c; } *Q)
{
    struct bar t = *B;
    *B = *Q;
    *Q = t;
    return B->c;
}

__attribute__((noinline))
struct foo * g4(struct arr *B,
	        struct arr *Q)
{
    struct arr t = *B;
    *B = *Q;
    *Q = t;
    return B->c[0];
}

__attribute__((noinline))
struct foo (*g5(struct fun *B,
	        struct fun *Q))()
{
    struct fun t = *B;
    *B = *Q;
    *Q = t;
    return B->c;
}

struct foo Bd() { };
struct foo Qd() { };

int main()
{
    struct foo Bc = { };
    struct foo Qc = { };

    struct bar B = { &Bc };
    struct bar Q = { &Qc };

    if (g1(&B, &Q) != &Qc)
	    __builtin_abort();

    union baz Bu = { &Bc };
    union baz Qu = { &Qc };

    if (g2(&Bu, &Qu) != &Qc)
	    __builtin_abort();

    struct bar B2 = { &Bc };
    struct bar Q2 = { &Qc };

    if (g3(&B2, &Q2) != &Qc)
	    __builtin_abort();

    struct arr Ba = { &Bc };
    struct arr Qa = { &Qc };

    if (g4(&Ba, &Qa) != &Qc)
	    __builtin_abort();
#if 0
    // PR114959
    struct fun Bf = { &Bd };
    struct fun Qf = { &Qd };

    if (g5(&Bf, &Qf) != &Qd)
	    __builtin_abort();
#endif
    return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2:[0-9]+]] foo = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_2]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_baz:[0-9]+]] baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_2]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_arr:[0-9]+]] arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type[[TYPE_foo_2]]>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_fun:[0-9]+]] fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type[[TYPE_foo_2]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_3:[0-9]+]] foo = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_2:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_3]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_baz_2:[0-9]+]] baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_3]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_arr_2:[0-9]+]] arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type[[TYPE_foo_3]]>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_fun_2:[0-9]+]] fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type[[TYPE_foo_3]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_3:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_baz_3:[0-9]+]] baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_arr_3:[0-9]+]] arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type[[TYPE_foo]]>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_fun_3:[0-9]+]] fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type[[TYPE_foo]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_4:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_4:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_4]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_baz_4:[0-9]+]] baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_4]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_arr_4:[0-9]+]] arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type[[TYPE_foo_4]]>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_fun_4:[0-9]+]] fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type[[TYPE_foo_4]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_5:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type[[TYPE_foo_5:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_5]] foo = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g1:[0-9]+]] @g1(%[[VALUE_B:[0-9]+]] B: ptr<@type[[TYPE_bar_3]]>, %[[VALUE_Q:[0-9]+]] Q: ptr<@type[[TYPE_bar_3]]>) -> ptr<@type[[TYPE_foo]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_bar_3]] [storage=automatic] = copy<@type[[TYPE_bar_3]], reason=assign>(read<@type[[TYPE_bar_3]]>(deref(read<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_B]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_bar_3]]>(deref(read<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_B]])), copy<@type[[TYPE_bar_3]], reason=assign>(read<@type[[TYPE_bar_3]]>(deref(read<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_Q]])))));
// DEFAULT-NEXT:         write<@type[[TYPE_bar_3]]>(deref(read<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_Q]])), copy<@type[[TYPE_bar_3]], reason=assign>(read<@type[[TYPE_bar_3]]>(%[[VALUE_t]])));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_foo]]>>(field0(deref(read<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_B]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2(%[[VALUE_B_2:[0-9]+]] B: ptr<@type[[TYPE_baz_3]]>, %[[VALUE_Q_2:[0-9]+]] Q: ptr<@type[[TYPE_baz_3]]>) -> ptr<@type[[TYPE_foo]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: @type[[TYPE_baz_3]] [storage=automatic] = copy<@type[[TYPE_baz_3]], reason=assign>(read<@type[[TYPE_baz_3]]>(deref(read<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_B_2]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_baz_3]]>(deref(read<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_B_2]])), copy<@type[[TYPE_baz_3]], reason=assign>(read<@type[[TYPE_baz_3]]>(deref(read<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_Q_2]])))));
// DEFAULT-NEXT:         write<@type[[TYPE_baz_3]]>(deref(read<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_Q_2]])), copy<@type[[TYPE_baz_3]], reason=assign>(read<@type[[TYPE_baz_3]]>(%[[VALUE_t_2]])));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_foo]]>>(field0(deref(read<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_B_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g3:[0-9]+]] @g3(%[[VALUE_B_3:[0-9]+]] B: ptr<@type[[TYPE_bar_5]]>, %[[VALUE_Q_3:[0-9]+]] Q: ptr<@type[[TYPE_bar_5]]>) -> ptr<@type[[TYPE_foo]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t_3:[0-9]+]] t: @type[[TYPE_bar_5]] [storage=automatic] = copy<@type[[TYPE_bar_5]], reason=assign>(read<@type[[TYPE_bar_5]]>(deref(read<ptr<@type[[TYPE_bar_5]]>>(%[[VALUE_B_3]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_bar_5]]>(deref(read<ptr<@type[[TYPE_bar_5]]>>(%[[VALUE_B_3]])), copy<@type[[TYPE_bar_5]], reason=assign>(read<@type[[TYPE_bar_5]]>(deref(read<ptr<@type[[TYPE_bar_5]]>>(%[[VALUE_Q_3]])))));
// DEFAULT-NEXT:         write<@type[[TYPE_bar_5]]>(deref(read<ptr<@type[[TYPE_bar_5]]>>(%[[VALUE_Q_3]])), copy<@type[[TYPE_bar_5]], reason=assign>(read<@type[[TYPE_bar_5]]>(%[[VALUE_t_3]])));
// DEFAULT-NEXT:         return pointer_cast<ptr<@type[[TYPE_foo]]>, reason=return>(read<ptr<@type[[TYPE_foo_5]]>>(field0(deref(read<ptr<@type[[TYPE_bar_5]]>>(%[[VALUE_B_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g4:[0-9]+]] @g4(%[[VALUE_B_4:[0-9]+]] B: ptr<@type[[TYPE_arr_3]]>, %[[VALUE_Q_4:[0-9]+]] Q: ptr<@type[[TYPE_arr_3]]>) -> ptr<@type[[TYPE_foo]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t_4:[0-9]+]] t: @type[[TYPE_arr_3]] [storage=automatic] = copy<@type[[TYPE_arr_3]], reason=assign>(read<@type[[TYPE_arr_3]]>(deref(read<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_B_4]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_arr_3]]>(deref(read<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_B_4]])), copy<@type[[TYPE_arr_3]], reason=assign>(read<@type[[TYPE_arr_3]]>(deref(read<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_Q_4]])))));
// DEFAULT-NEXT:         write<@type[[TYPE_arr_3]]>(deref(read<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_Q_4]])), copy<@type[[TYPE_arr_3]], reason=assign>(read<@type[[TYPE_arr_3]]>(%[[VALUE_t_4]])));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_foo]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_foo]]>>, subtract=false, element=ptr<@type[[TYPE_foo]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_foo]]>>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_B_4]])))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g5:[0-9]+]] @g5(%[[VALUE_B_5:[0-9]+]] B: ptr<@type[[TYPE_fun_3]]>, %[[VALUE_Q_5:[0-9]+]] Q: ptr<@type[[TYPE_fun_3]]>) -> ptr<fn() -> @type[[TYPE_foo]]> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t_5:[0-9]+]] t: @type[[TYPE_fun_3]] [storage=automatic] = copy<@type[[TYPE_fun_3]], reason=assign>(read<@type[[TYPE_fun_3]]>(deref(read<ptr<@type[[TYPE_fun_3]]>>(%[[VALUE_B_5]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_fun_3]]>(deref(read<ptr<@type[[TYPE_fun_3]]>>(%[[VALUE_B_5]])), copy<@type[[TYPE_fun_3]], reason=assign>(read<@type[[TYPE_fun_3]]>(deref(read<ptr<@type[[TYPE_fun_3]]>>(%[[VALUE_Q_5]])))));
// DEFAULT-NEXT:         write<@type[[TYPE_fun_3]]>(deref(read<ptr<@type[[TYPE_fun_3]]>>(%[[VALUE_Q_5]])), copy<@type[[TYPE_fun_3]], reason=assign>(read<@type[[TYPE_fun_3]]>(%[[VALUE_t_5]])));
// DEFAULT-NEXT:         return read<ptr<fn() -> @type[[TYPE_foo]]>>(field0(deref(read<ptr<@type[[TYPE_fun_3]]>>(%[[VALUE_B_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Bd:[0-9]+]] @Bd() -> @type[[TYPE_foo]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Qd:[0-9]+]] @Qd() -> @type[[TYPE_foo]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_Bc:[0-9]+]] Bc: @type[[TYPE_foo]] [storage=automatic] = aggregate<@type[[TYPE_foo]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_Qc:[0-9]+]] Qc: @type[[TYPE_foo]] [storage=automatic] = aggregate<@type[[TYPE_foo]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_B_6:[0-9]+]] B: @type[[TYPE_bar_3]] [storage=automatic] = aggregate<@type[[TYPE_bar_3]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Bc]]));
// DEFAULT-NEXT:         let %[[VALUE_Q_6:[0-9]+]] Q: @type[[TYPE_bar_3]] [storage=automatic] = aggregate<@type[[TYPE_bar_3]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_foo]]>>(call<ptr<@type[[TYPE_foo]]>, signature=fn(ptr<@type[[TYPE_bar_3]]>, ptr<@type[[TYPE_bar_3]]>) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_g1]], addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_B_6]]), addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_Q_6]])), addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_Bu:[0-9]+]] Bu: @type[[TYPE_baz_3]] [storage=automatic] = aggregate<@type[[TYPE_baz_3]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Bc]]));
// DEFAULT-NEXT:         let %[[VALUE_Qu:[0-9]+]] Qu: @type[[TYPE_baz_3]] [storage=automatic] = aggregate<@type[[TYPE_baz_3]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_foo]]>>(call<ptr<@type[[TYPE_foo]]>, signature=fn(ptr<@type[[TYPE_baz_3]]>, ptr<@type[[TYPE_baz_3]]>) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_g2]], addr_of<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_Bu]]), addr_of<ptr<@type[[TYPE_baz_3]]>>(%[[VALUE_Qu]])), addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_B2:[0-9]+]] B2: @type[[TYPE_bar_3]] [storage=automatic] = aggregate<@type[[TYPE_bar_3]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Bc]]));
// DEFAULT-NEXT:         let %[[VALUE_Q2:[0-9]+]] Q2: @type[[TYPE_bar_3]] [storage=automatic] = aggregate<@type[[TYPE_bar_3]], zero_fill=false>(field0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_foo]]>>(call<ptr<@type[[TYPE_foo]]>, signature=fn(ptr<@type[[TYPE_bar_5]]>, ptr<@type[[TYPE_bar_5]]>) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_g3]], pointer_cast<ptr<@type[[TYPE_bar_5]]>, reason=arg>(addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_B2]])), pointer_cast<ptr<@type[[TYPE_bar_5]]>, reason=arg>(addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_Q2]]))), addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_Ba:[0-9]+]] Ba: @type[[TYPE_arr_3]] [storage=automatic] = aggregate<@type[[TYPE_arr_3]], zero_fill=false>(field0 = aggregate<array<ptr<@type[[TYPE_foo]]>, 1>, zero_fill=false>(index0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Bc]])));
// DEFAULT-NEXT:         let %[[VALUE_Qa:[0-9]+]] Qa: @type[[TYPE_arr_3]] [storage=automatic] = aggregate<@type[[TYPE_arr_3]], zero_fill=false>(field0 = aggregate<array<ptr<@type[[TYPE_foo]]>, 1>, zero_fill=false>(index0 = addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]])));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_foo]]>>(call<ptr<@type[[TYPE_foo]]>, signature=fn(ptr<@type[[TYPE_arr_3]]>, ptr<@type[[TYPE_arr_3]]>) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_g4]], addr_of<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_Ba]]), addr_of<ptr<@type[[TYPE_arr_3]]>>(%[[VALUE_Qa]])), addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_Qc]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
