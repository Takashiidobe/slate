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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 foo = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type4 arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type1>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type5 fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type6 foo = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type7 bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type6>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type8 baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type6>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type9 arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type6>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type10 fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type6>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type11 bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type12 baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type13 arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type0>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type14 fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type15 foo = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type16 bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type15>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type17 baz = union {
// DEFAULT-NEXT:         field0 c: ptr<@type15>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type18 arr = struct {
// DEFAULT-NEXT:         field0 c: array<ptr<@type15>, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type19 fun = struct {
// DEFAULT-NEXT:         field0 c: ptr<fn() -> @type15>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type20 bar = struct {
// DEFAULT-NEXT:         field0 c: ptr<@type21>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type21 foo = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @g1(%24 B: ptr<@type11>, %25 Q: ptr<@type11>) -> ptr<@type0> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 t: @type11 [storage=automatic] = copy<@type11, reason=assign>(read<@type11>(deref(read<ptr<@type11>>(%24))));
// DEFAULT-NEXT:         write<@type11>(deref(read<ptr<@type11>>(%24)), copy<@type11, reason=assign>(read<@type11>(deref(read<ptr<@type11>>(%25)))));
// DEFAULT-NEXT:         write<@type11>(deref(read<ptr<@type11>>(%25)), copy<@type11, reason=assign>(read<@type11>(%26)));
// DEFAULT-NEXT:         return read<ptr<@type0>>(field0(deref(read<ptr<@type11>>(%24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @g2(%28 B: ptr<@type12>, %29 Q: ptr<@type12>) -> ptr<@type0> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 t: @type12 [storage=automatic] = copy<@type12, reason=assign>(read<@type12>(deref(read<ptr<@type12>>(%28))));
// DEFAULT-NEXT:         write<@type12>(deref(read<ptr<@type12>>(%28)), copy<@type12, reason=assign>(read<@type12>(deref(read<ptr<@type12>>(%29)))));
// DEFAULT-NEXT:         write<@type12>(deref(read<ptr<@type12>>(%29)), copy<@type12, reason=assign>(read<@type12>(%30)));
// DEFAULT-NEXT:         return read<ptr<@type0>>(field0(deref(read<ptr<@type12>>(%28))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @g3(%34 B: ptr<@type20>, %35 Q: ptr<@type20>) -> ptr<@type0> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %36 t: @type20 [storage=automatic] = copy<@type20, reason=assign>(read<@type20>(deref(read<ptr<@type20>>(%34))));
// DEFAULT-NEXT:         write<@type20>(deref(read<ptr<@type20>>(%34)), copy<@type20, reason=assign>(read<@type20>(deref(read<ptr<@type20>>(%35)))));
// DEFAULT-NEXT:         write<@type20>(deref(read<ptr<@type20>>(%35)), copy<@type20, reason=assign>(read<@type20>(%36)));
// DEFAULT-NEXT:         return pointer_cast<ptr<@type0>, reason=return>(read<ptr<@type21>>(field0(deref(read<ptr<@type20>>(%34)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @g4(%38 B: ptr<@type13>, %39 Q: ptr<@type13>) -> ptr<@type0> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %40 t: @type13 [storage=automatic] = copy<@type13, reason=assign>(read<@type13>(deref(read<ptr<@type13>>(%38))));
// DEFAULT-NEXT:         write<@type13>(deref(read<ptr<@type13>>(%38)), copy<@type13, reason=assign>(read<@type13>(deref(read<ptr<@type13>>(%39)))));
// DEFAULT-NEXT:         write<@type13>(deref(read<ptr<@type13>>(%39)), copy<@type13, reason=assign>(read<@type13>(%40)));
// DEFAULT-NEXT:         return read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(1)>(field0(deref(read<ptr<@type13>>(%38)))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @g5(%42 B: ptr<@type14>, %43 Q: ptr<@type14>) -> ptr<fn() -> @type0> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %44 t: @type14 [storage=automatic] = copy<@type14, reason=assign>(read<@type14>(deref(read<ptr<@type14>>(%42))));
// DEFAULT-NEXT:         write<@type14>(deref(read<ptr<@type14>>(%42)), copy<@type14, reason=assign>(read<@type14>(deref(read<ptr<@type14>>(%43)))));
// DEFAULT-NEXT:         write<@type14>(deref(read<ptr<@type14>>(%43)), copy<@type14, reason=assign>(read<@type14>(%44)));
// DEFAULT-NEXT:         return read<ptr<fn() -> @type0>>(field0(deref(read<ptr<@type14>>(%42))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @Bd() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @Qd() -> @type0 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %47 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %48 Bc: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>();
// DEFAULT-NEXT:         let %49 Qc: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>();
// DEFAULT-NEXT:         let %50 B: @type11 [storage=automatic] = aggregate<@type11, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%48));
// DEFAULT-NEXT:         let %51 Q: @type11 [storage=automatic] = aggregate<@type11, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%49));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn(ptr<@type11>, ptr<@type11>) -> ptr<@type0>>(%23, addr_of<ptr<@type11>>(%50), addr_of<ptr<@type11>>(%51)), addr_of<ptr<@type0>>(%49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:         let %52 Bu: @type12 [storage=automatic] = aggregate<@type12, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%48));
// DEFAULT-NEXT:         let %53 Qu: @type12 [storage=automatic] = aggregate<@type12, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%49));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn(ptr<@type12>, ptr<@type12>) -> ptr<@type0>>(%27, addr_of<ptr<@type12>>(%52), addr_of<ptr<@type12>>(%53)), addr_of<ptr<@type0>>(%49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:         let %54 B2: @type11 [storage=automatic] = aggregate<@type11, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%48));
// DEFAULT-NEXT:         let %55 Q2: @type11 [storage=automatic] = aggregate<@type11, zero_fill=false>(field0 = addr_of<ptr<@type0>>(%49));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn(ptr<@type20>, ptr<@type20>) -> ptr<@type0>>(%31, pointer_cast<ptr<@type20>, reason=arg>(addr_of<ptr<@type11>>(%54)), pointer_cast<ptr<@type20>, reason=arg>(addr_of<ptr<@type11>>(%55))), addr_of<ptr<@type0>>(%49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:         let %56 Ba: @type13 [storage=automatic] = aggregate<@type13, zero_fill=false>(field0 = aggregate<array<ptr<@type0>, 1>, zero_fill=false>(index0 = addr_of<ptr<@type0>>(%48)));
// DEFAULT-NEXT:         let %57 Qa: @type13 [storage=automatic] = aggregate<@type13, zero_fill=false>(field0 = aggregate<array<ptr<@type0>, 1>, zero_fill=false>(index0 = addr_of<ptr<@type0>>(%49)));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn(ptr<@type13>, ptr<@type13>) -> ptr<@type0>>(%37, addr_of<ptr<@type13>>(%56), addr_of<ptr<@type13>>(%57)), addr_of<ptr<@type0>>(%49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%58);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
