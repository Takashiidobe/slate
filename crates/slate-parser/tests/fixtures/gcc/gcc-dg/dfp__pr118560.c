/* PR target/118560 */
/* { dg-do compile } */
/* { dg-options "-O1" } */

struct { _Decimal32 a; } b;
void foo (int, _Decimal32);

#define B(n) \
void				\
bar##n (int, _Decimal32 d)	\
{				\
  foo (n, 1);			\
  b.a = d;			\
}

#define C(n) B(n##0) B(n##1) B(n##2) B(n##3) B(n##4) B(n##5) B(n##6) B(n##7) B(n##8) B(n##9)
C(1) C(2) C(3) C(4) C(5)

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: d32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %1 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%103 <unnamed>: i32, %104 <unnamed>: d32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @bar10(%105 <unnamed>: i32, %4 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(10), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar11(%106 <unnamed>: i32, %6 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(11), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar12(%107 <unnamed>: i32, %8 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(12), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar13(%108 <unnamed>: i32, %10 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(13), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @bar14(%109 <unnamed>: i32, %12 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(14), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar15(%110 <unnamed>: i32, %14 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(15), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @bar16(%111 <unnamed>: i32, %16 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(16), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @bar17(%112 <unnamed>: i32, %18 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(17), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @bar18(%113 <unnamed>: i32, %20 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(18), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @bar19(%114 <unnamed>: i32, %22 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(19), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @bar20(%115 <unnamed>: i32, %24 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(20), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @bar21(%116 <unnamed>: i32, %26 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(21), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @bar22(%117 <unnamed>: i32, %28 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(22), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @bar23(%118 <unnamed>: i32, %30 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(23), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%30));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @bar24(%119 <unnamed>: i32, %32 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(24), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @bar25(%120 <unnamed>: i32, %34 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(25), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @bar26(%121 <unnamed>: i32, %36 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(26), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%36));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @bar27(%122 <unnamed>: i32, %38 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(27), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%38));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @bar28(%123 <unnamed>: i32, %40 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(28), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @bar29(%124 <unnamed>: i32, %42 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(29), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @bar30(%125 <unnamed>: i32, %44 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(30), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%44));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @bar31(%126 <unnamed>: i32, %46 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(31), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%46));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @bar32(%127 <unnamed>: i32, %48 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(32), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%48));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @bar33(%128 <unnamed>: i32, %50 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(33), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%50));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @bar34(%129 <unnamed>: i32, %52 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(34), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%52));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @bar35(%130 <unnamed>: i32, %54 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(35), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%54));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @bar36(%131 <unnamed>: i32, %56 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(36), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%56));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @bar37(%132 <unnamed>: i32, %58 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(37), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%58));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @bar38(%133 <unnamed>: i32, %60 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(38), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%60));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @bar39(%134 <unnamed>: i32, %62 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(39), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%62));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @bar40(%135 <unnamed>: i32, %64 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(40), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @bar41(%136 <unnamed>: i32, %66 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(41), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%66));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %67 @bar42(%137 <unnamed>: i32, %68 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(42), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%68));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @bar43(%138 <unnamed>: i32, %70 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(43), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%70));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @bar44(%139 <unnamed>: i32, %72 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(44), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%72));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %73 @bar45(%140 <unnamed>: i32, %74 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(45), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%74));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @bar46(%141 <unnamed>: i32, %76 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(46), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%76));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @bar47(%142 <unnamed>: i32, %78 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(47), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%78));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %79 @bar48(%143 <unnamed>: i32, %80 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(48), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%80));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @bar49(%144 <unnamed>: i32, %82 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(49), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%82));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @bar50(%145 <unnamed>: i32, %84 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(50), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%84));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @bar51(%146 <unnamed>: i32, %86 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(51), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%86));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @bar52(%147 <unnamed>: i32, %88 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(52), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%88));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %89 @bar53(%148 <unnamed>: i32, %90 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(53), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%90));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @bar54(%149 <unnamed>: i32, %92 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(54), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%92));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %93 @bar55(%150 <unnamed>: i32, %94 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(55), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%94));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %95 @bar56(%151 <unnamed>: i32, %96 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(56), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%96));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %97 @bar57(%152 <unnamed>: i32, %98 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(57), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%98));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %99 @bar58(%153 <unnamed>: i32, %100 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(58), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%100));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @bar59(%154 <unnamed>: i32, %102 d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%2, const<i32>(59), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%1), read<d32>(%102));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
