/* Verify that constants in memory, referenced only by dead code,
   are not emitted to the object file.
   FIXME: Not presently possible to apply -pedantic to code with
   complex constants in it.  The __extension__ should shut up the
   warning but doesn't.  (Hard to fix -- the lexer is not aware of
   the parser's state.)  */

/* { dg-do compile } */
/* { dg-options "-O2 -std=c99" } */
/* { dg-final { scan-assembler-not "L\\\$?C\[^A-Z\]" } } */

#define I (__extension__ 1.0iF)

struct S { int a; double b[2]; void *c; };

extern void use_str(const char *);
extern void use_S(const struct S *);
extern void use_cplx(__complex__ double);

static inline int
returns_23(void) { return 23; }

void
test1(void)
{
	if (returns_23() == 23)
		return;

	use_str("waltz, nymph, for quick jigs vex bud");
	use_S(&(const struct S){12, {3.1415, 2.1828}, 0 });
	use_cplx(3.1415 + 2.1828*I);
}

void
test2(void)
{
	const char *str = "pack my box with five dozen liquor jugs";
	const struct S S = { 23, { 1.414, 1.618 }, 0 };
	const __complex__ double cplx = 1.414 + 1.618*I;

	if (returns_23() == 23)
		return;

	use_str(str);
	use_S(&S);
	use_cplx(cplx);
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: array<f64, 2>;
// DEFAULT-NEXT:         field2 c: ptr<void>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 24]];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([119, 97, 108, 116, 122, 44, 32, 110, 121, 109, 112, 104, 44, 32, 102, 111, 114, 32, 113, 117, 105, 99, 107, 32, 106, 105, 103, 115, 32, 118, 101, 120, 32, 98, 117, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 40> [storage=static] = code_units<array<i8, 40>>([112, 97, 99, 107, 32, 109, 121, 32, 98, 111, 120, 32, 119, 105, 116, 104, 32, 102, 105, 118, 101, 32, 100, 111, 122, 101, 110, 32, 108, 105, 113, 117, 111, 114, 32, 106, 117, 103, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @use_str(%10 <unnamed>: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @use_S(%11 <unnamed>: ptr<const @type0>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @use_cplx(%12 <unnamed>: complex<f64>) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// DEFAULT-NEXT:     fn %4 @returns_23() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(23))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%13)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%2, addr_of<ptr<const @type0>>(compound_literal %14 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(12), field1 = aggregate<array<f64, 2>, zero_fill=false>(index0 = const<f64>(3.1415), index1 = const<f64>(2.1828)), field2 = null<ptr<void>>)));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.1415), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(2.1828), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 str: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(40)>(%15));
// DEFAULT-NEXT:         let %8 S: @type0 [storage=automatic] [const] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(23), field1 = aggregate<array<f64, 2>, zero_fill=false>(index0 = const<f64>(1.414), index1 = const<f64>(1.618)), field2 = null<ptr<void>>);
// DEFAULT-NEXT:         let %9 cplx: complex<f64> [storage=automatic] [const] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.414), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.618), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(23))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%1, read<ptr<const i8>>(%7));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const @type0>) -> void>(%2, addr_of<ptr<const @type0>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f64>) -> void, abi=sysv64(native_c) -> void>(%3, read<complex<f64>>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
