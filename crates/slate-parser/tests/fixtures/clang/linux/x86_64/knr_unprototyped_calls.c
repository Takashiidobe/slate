typedef char C;
int unproto();
int knr(a, b, t) char a; float b; C t; { return sizeof(a) + a + b + t; }
int (*fp)();
int use(void) {
  unproto(1.0f, (char)2);
  knr(1.5, 2, 3);
  knr(1);
  knr(1, 2, 3, 4);
  return fp(1);
}
int proto_later();
int proto_later(int x);

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-WARNING DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wdeprecated-non-prototype
// DEFAULT: ⚠ a function definition without a prototype is deprecated in all versions of
// DEFAULT: │ C and is not supported in C23
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/knr_unprototyped_calls.c:3:1]
// DEFAULT: 2 │ int unproto();
// DEFAULT: 3 │ int knr(a, b, t) char a; float b; C t; { return sizeof(a) + a + b + t; }
// DEFAULT: · ────────────────────────────────────────────────────────────────────────
// DEFAULT: 4 │ int (*fp)();
// DEFAULT: ╰────
// DEFAULT: -Wdeprecated-non-prototype
// DEFAULT: ⚠ passing arguments to 'unproto' without a prototype is deprecated in all
// DEFAULT: │ versions of C and is not supported in C23
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/knr_unprototyped_calls.c:6:3]
// DEFAULT: 5 │ int use(void) {
// DEFAULT: 6 │   unproto(1.0f, (char)2);
// DEFAULT: ·   ──────────────────────
// DEFAULT: 7 │   knr(1.5, 2, 3);
// DEFAULT: ╰────
// DEFAULT: -Wdeprecated-non-prototype
// DEFAULT: ⚠ passing arguments to a function without a prototype is deprecated in all
// DEFAULT: │ versions of C and is not supported in C23
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/knr_unprototyped_calls.c:10:10]
// DEFAULT: 9 │   knr(1, 2, 3, 4);
// DEFAULT: 10 │   return fp(1);
// DEFAULT: ·          ─────
// DEFAULT: 11 │ }
// DEFAULT: ╰────
// DEFAULT: -Wdeprecated-non-prototype
// DEFAULT: ⚠ a function declaration without a prototype is deprecated in all versions
// DEFAULT: │ of C and is treated as a zero-parameter prototype in C23, conflicting with
// DEFAULT: │ a subsequent declaration
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/knr_unprototyped_calls.c:12:5]
// DEFAULT: 11 │ }
// DEFAULT: 12 │ int proto_later();
// DEFAULT: ·     ─────────────
// DEFAULT: 13 │ int proto_later(int x);
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f80;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f80 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = i8;
// IR-DEFAULT-NEXT:     global %[[VALUE_fp:[0-9]+]] fp: ptr<fn(unprototyped) -> i32> [storage=static] [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_unproto:[0-9]+]] @unproto(unprototyped) -> i32 [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_knr:[0-9]+]] @knr(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_t:[0-9]+]] t: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i8 [storage=automatic] = truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_a]]));
// IR-DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: f32 [storage=automatic] = float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f64>(%[[VALUE_b]]));
// IR-DEFAULT-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: i8 [storage=automatic] = truncate<i8, reason=arg, fits=unknown>(read<i32>(%[[VALUE_t]]));
// IR-DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(add<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_a_2]])))))), read<f32>(%[[VALUE_b_2]])), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_t_2]])))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_unproto]], float_widen<f64, reason=vararg>(const<f32>(1.0)), widen<i32, reason=vararg>(truncate<i8, reason=explicit, fits=always>(const<i32>(2))));
// IR-DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_knr]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(1.5)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(3));
// IR-DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_knr]], const<i32>(1));
// IR-DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_knr]], const<i32>(1), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(3), const<i32>(4));
// IR-DEFAULT-NEXT:         return call<i32, signature=fn(unprototyped) -> i32>(read<ptr<fn(unprototyped) -> i32>>(%[[VALUE_fp]]), const<i32>(1));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_proto_later:[0-9]+]] @proto_later(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
