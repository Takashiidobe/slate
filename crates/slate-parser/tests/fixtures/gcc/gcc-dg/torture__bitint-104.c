/* PR tree-optimization/126490 */
/* { dg-do run { target bitint } } */

typedef unsigned _BitInt(1) T;

[[gnu::noipa]] int foo(T a, T b) { return ((a & b) == (a ^ b)) + 1; }

[[gnu::noipa]] int bar(T a, T b) { return ((a & b) == (a ^ b)) != 0; }

[[gnu::noipa]] int baz(T a, T b) { return ((a & b) == (a ^ b)) == 0; }

[[gnu::noipa]] int qux(T a, T b) { return ((a & b) == (a ^ b)) < 1; }

[[gnu::noipa]] int corge(int a, int b) { return (a & b) == (a ^ b); }

[[gnu::noipa]] int garply(T a, T b) { return ((a & b) ^ (a == b)) + 1; }

[[gnu::noipa]] int fred(T a, T b) { return ((a & b) ^ (a == b)) != 0; }

[[gnu::noipa]] int xyzzy(T a, T b) { return ((a & b) ^ (a == b)) == 0; }

[[gnu::noipa]] int waldo(T a, T b) { return ((a & b) ^ (a == b)) < 1; }

int main() {
  for (int i = 0; i < 4; ++i) {
    int a = i & 1;
    int b = i >> 1;
    int c = corge(a, b);
    if (foo(a, b) != c + 1 || bar(a, b) != (c != 0) || baz(a, b) != (c == 0) ||
        qux(a, b) != (c < 1) || garply(a, b) != c + 1 ||
        fred(a, b) != (c != 0) || xyzzy(a, b) != (c == 0) ||
        waldo(a, b) != (c < 1))
      __builtin_abort();
  }
}



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
// DEFAULT-NEXT:     type @type0 T = u1b;
// DEFAULT-NEXT:     fn %1 @foo(%2 a: u1b, %3 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<u1b>(and<u1b>(read<u1b>(%2), read<u1b>(%3)), xor<u1b>(read<u1b>(%2), read<u1b>(%3)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 a: u1b, %6 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(from_bool<i32, reason=promotion>(eq<u1b>(and<u1b>(read<u1b>(%5), read<u1b>(%6)), xor<u1b>(read<u1b>(%5), read<u1b>(%6)))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz(%8 a: u1b, %9 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(from_bool<i32, reason=promotion>(eq<u1b>(and<u1b>(read<u1b>(%8), read<u1b>(%9)), xor<u1b>(read<u1b>(%8), read<u1b>(%9)))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @qux(%11 a: u1b, %12 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(from_bool<i32, reason=promotion>(eq<u1b>(and<u1b>(read<u1b>(%11), read<u1b>(%12)), xor<u1b>(read<u1b>(%11), read<u1b>(%12)))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @corge(%14 a: i32, %15 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(and<i32>(read<i32>(%14), read<i32>(%15)), xor<i32>(read<i32>(%14), read<i32>(%15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @garply(%17 a: u1b, %18 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(xor<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(and<u1b>(read<u1b>(%17), read<u1b>(%18)))), from_bool<i32, reason=promotion>(eq<u1b>(read<u1b>(%17), read<u1b>(%18)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @fred(%20 a: u1b, %21 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(xor<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(and<u1b>(read<u1b>(%20), read<u1b>(%21)))), from_bool<i32, reason=promotion>(eq<u1b>(read<u1b>(%20), read<u1b>(%21)))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @xyzzy(%23 a: u1b, %24 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(xor<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(and<u1b>(read<u1b>(%23), read<u1b>(%24)))), from_bool<i32, reason=promotion>(eq<u1b>(read<u1b>(%23), read<u1b>(%24)))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @waldo(%26 a: u1b, %27 b: u1b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(xor<i32>(reinterpret<i32, reason=usual_arith, fits=unknown>(widen<u32, reason=usual_arith>(and<u1b>(read<u1b>(%26), read<u1b>(%27)))), from_bool<i32, reason=promotion>(eq<u1b>(read<u1b>(%26), read<u1b>(%27)))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %29 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%29), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %30 a: i32 [storage=automatic] = and<i32>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                     let %31 b: i32 [storage=automatic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                     let %32 c: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%13, read<i32>(%30), read<i32>(%31));
// DEFAULT-NEXT:                     let %37: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%1, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), add<i32, overflow=ub>(read<i32>(%32), const<i32>(1)))
// DEFAULT-NEXT:                         write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%37, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%4, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%32), const<i32>(0)))));
// DEFAULT-NEXT:                     let %38: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%37)
// DEFAULT-NEXT:                         write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%38, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%7, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%32), const<i32>(0)))));
// DEFAULT-NEXT:                     let %39: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%38)
// DEFAULT-NEXT:                         write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%39, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%10, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%32), const<i32>(1)))));
// DEFAULT-NEXT:                     let %40: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%39)
// DEFAULT-NEXT:                         write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%40, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%16, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), add<i32, overflow=ub>(read<i32>(%32), const<i32>(1))));
// DEFAULT-NEXT:                     let %41: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%40)
// DEFAULT-NEXT:                         write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%41, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%19, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%32), const<i32>(0)))));
// DEFAULT-NEXT:                     let %42: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%41)
// DEFAULT-NEXT:                         write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%42, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%22, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%32), const<i32>(0)))));
// DEFAULT-NEXT:                     let %43: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%42)
// DEFAULT-NEXT:                         write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%43, ne<i32>(call<i32, signature=fn(u1b, u1b) -> i32>(%25, reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%30))), reinterpret<u1b, reason=arg, fits=unknown>(truncate<i1b, reason=arg, fits=unknown>(read<i32>(%31)))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%32), const<i32>(1)))));
// DEFAULT-NEXT:                     if read<bool>(%43)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%34);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
