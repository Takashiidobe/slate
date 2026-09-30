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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: d32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: d32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar10:[0-9]+]] @bar10(%[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE_d:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(10), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar11:[0-9]+]] @bar11(%[[VALUE3:[0-9]+]] <unnamed>: i32, %[[VALUE_d_2:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(11), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar12:[0-9]+]] @bar12(%[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE_d_3:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(12), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar13:[0-9]+]] @bar13(%[[VALUE5:[0-9]+]] <unnamed>: i32, %[[VALUE_d_4:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(13), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar14:[0-9]+]] @bar14(%[[VALUE6:[0-9]+]] <unnamed>: i32, %[[VALUE_d_5:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(14), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar15:[0-9]+]] @bar15(%[[VALUE7:[0-9]+]] <unnamed>: i32, %[[VALUE_d_6:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(15), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar16:[0-9]+]] @bar16(%[[VALUE8:[0-9]+]] <unnamed>: i32, %[[VALUE_d_7:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(16), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar17:[0-9]+]] @bar17(%[[VALUE9:[0-9]+]] <unnamed>: i32, %[[VALUE_d_8:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(17), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar18:[0-9]+]] @bar18(%[[VALUE10:[0-9]+]] <unnamed>: i32, %[[VALUE_d_9:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(18), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar19:[0-9]+]] @bar19(%[[VALUE11:[0-9]+]] <unnamed>: i32, %[[VALUE_d_10:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(19), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar20:[0-9]+]] @bar20(%[[VALUE12:[0-9]+]] <unnamed>: i32, %[[VALUE_d_11:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(20), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar21:[0-9]+]] @bar21(%[[VALUE13:[0-9]+]] <unnamed>: i32, %[[VALUE_d_12:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(21), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar22:[0-9]+]] @bar22(%[[VALUE14:[0-9]+]] <unnamed>: i32, %[[VALUE_d_13:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(22), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar23:[0-9]+]] @bar23(%[[VALUE15:[0-9]+]] <unnamed>: i32, %[[VALUE_d_14:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(23), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar24:[0-9]+]] @bar24(%[[VALUE16:[0-9]+]] <unnamed>: i32, %[[VALUE_d_15:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(24), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar25:[0-9]+]] @bar25(%[[VALUE17:[0-9]+]] <unnamed>: i32, %[[VALUE_d_16:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(25), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar26:[0-9]+]] @bar26(%[[VALUE18:[0-9]+]] <unnamed>: i32, %[[VALUE_d_17:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(26), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar27:[0-9]+]] @bar27(%[[VALUE19:[0-9]+]] <unnamed>: i32, %[[VALUE_d_18:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(27), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar28:[0-9]+]] @bar28(%[[VALUE20:[0-9]+]] <unnamed>: i32, %[[VALUE_d_19:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(28), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_19]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar29:[0-9]+]] @bar29(%[[VALUE21:[0-9]+]] <unnamed>: i32, %[[VALUE_d_20:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(29), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_20]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar30:[0-9]+]] @bar30(%[[VALUE22:[0-9]+]] <unnamed>: i32, %[[VALUE_d_21:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(30), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar31:[0-9]+]] @bar31(%[[VALUE23:[0-9]+]] <unnamed>: i32, %[[VALUE_d_22:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(31), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_22]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar32:[0-9]+]] @bar32(%[[VALUE24:[0-9]+]] <unnamed>: i32, %[[VALUE_d_23:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(32), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar33:[0-9]+]] @bar33(%[[VALUE25:[0-9]+]] <unnamed>: i32, %[[VALUE_d_24:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(33), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_24]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar34:[0-9]+]] @bar34(%[[VALUE26:[0-9]+]] <unnamed>: i32, %[[VALUE_d_25:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(34), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_25]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar35:[0-9]+]] @bar35(%[[VALUE27:[0-9]+]] <unnamed>: i32, %[[VALUE_d_26:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(35), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_26]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar36:[0-9]+]] @bar36(%[[VALUE28:[0-9]+]] <unnamed>: i32, %[[VALUE_d_27:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(36), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar37:[0-9]+]] @bar37(%[[VALUE29:[0-9]+]] <unnamed>: i32, %[[VALUE_d_28:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(37), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_28]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar38:[0-9]+]] @bar38(%[[VALUE30:[0-9]+]] <unnamed>: i32, %[[VALUE_d_29:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(38), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_29]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar39:[0-9]+]] @bar39(%[[VALUE31:[0-9]+]] <unnamed>: i32, %[[VALUE_d_30:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(39), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_30]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar40:[0-9]+]] @bar40(%[[VALUE32:[0-9]+]] <unnamed>: i32, %[[VALUE_d_31:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(40), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_31]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar41:[0-9]+]] @bar41(%[[VALUE33:[0-9]+]] <unnamed>: i32, %[[VALUE_d_32:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(41), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_32]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar42:[0-9]+]] @bar42(%[[VALUE34:[0-9]+]] <unnamed>: i32, %[[VALUE_d_33:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(42), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_33]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar43:[0-9]+]] @bar43(%[[VALUE35:[0-9]+]] <unnamed>: i32, %[[VALUE_d_34:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(43), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_34]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar44:[0-9]+]] @bar44(%[[VALUE36:[0-9]+]] <unnamed>: i32, %[[VALUE_d_35:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(44), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_35]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar45:[0-9]+]] @bar45(%[[VALUE37:[0-9]+]] <unnamed>: i32, %[[VALUE_d_36:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(45), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_36]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar46:[0-9]+]] @bar46(%[[VALUE38:[0-9]+]] <unnamed>: i32, %[[VALUE_d_37:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(46), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_37]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar47:[0-9]+]] @bar47(%[[VALUE39:[0-9]+]] <unnamed>: i32, %[[VALUE_d_38:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(47), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_38]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar48:[0-9]+]] @bar48(%[[VALUE40:[0-9]+]] <unnamed>: i32, %[[VALUE_d_39:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(48), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_39]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar49:[0-9]+]] @bar49(%[[VALUE41:[0-9]+]] <unnamed>: i32, %[[VALUE_d_40:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(49), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_40]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar50:[0-9]+]] @bar50(%[[VALUE42:[0-9]+]] <unnamed>: i32, %[[VALUE_d_41:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(50), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_41]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar51:[0-9]+]] @bar51(%[[VALUE43:[0-9]+]] <unnamed>: i32, %[[VALUE_d_42:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(51), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_42]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar52:[0-9]+]] @bar52(%[[VALUE44:[0-9]+]] <unnamed>: i32, %[[VALUE_d_43:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(52), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_43]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar53:[0-9]+]] @bar53(%[[VALUE45:[0-9]+]] <unnamed>: i32, %[[VALUE_d_44:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(53), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_44]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar54:[0-9]+]] @bar54(%[[VALUE46:[0-9]+]] <unnamed>: i32, %[[VALUE_d_45:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(54), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_45]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar55:[0-9]+]] @bar55(%[[VALUE47:[0-9]+]] <unnamed>: i32, %[[VALUE_d_46:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(55), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_46]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar56:[0-9]+]] @bar56(%[[VALUE48:[0-9]+]] <unnamed>: i32, %[[VALUE_d_47:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(56), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_47]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar57:[0-9]+]] @bar57(%[[VALUE49:[0-9]+]] <unnamed>: i32, %[[VALUE_d_48:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(57), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_48]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar58:[0-9]+]] @bar58(%[[VALUE50:[0-9]+]] <unnamed>: i32, %[[VALUE_d_49:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(58), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_49]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar59:[0-9]+]] @bar59(%[[VALUE51:[0-9]+]] <unnamed>: i32, %[[VALUE_d_50:[0-9]+]] d: d32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, d32) -> void>(%[[VALUE_foo]], const<i32>(59), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<d32>(field0(%[[VALUE_b]]), read<d32>(%[[VALUE_d_50]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
