typedef struct {
  _Complex double a;
  _Complex double b;
} Scf10;

Scf10 g1s;

void check(Scf10 x, _Complex double y) {
  if (x.a != y)
    __builtin_abort();
}

void init(Scf10 *p, _Complex double y) { p->a = y; }

int main() {
  init(&g1s, (_Complex double)1);
  check(g1s, (_Complex double)1);

  return 0;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: complex<f64>;
// DEFAULT-NEXT:         field1 b: complex<f64>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type1 Scf10 = @type0;
// DEFAULT-NEXT:     global %2 g1s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @check(%4 x: @type0, %5 y: complex<f64>) -> void [linkage=external] [abi=sysv64(native_c, coerce<f64, f64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(field0(%4)), read<complex<f64>>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @init(%7 p: ptr<@type0>, %8 y: complex<f64>) -> void [linkage=external] [abi=sysv64(scalar, coerce<f64, f64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<complex<f64>>(field0(deref(read<ptr<@type0>>(%7))), read<complex<f64>>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, complex<f64>) -> void, abi=sysv64(scalar, coerce<f64, f64>) -> void>(%6, addr_of<ptr<@type0>>(%2), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0, complex<f64>) -> void, abi=sysv64(native_c, coerce<f64, f64>) -> void>(%3, copy<@type0, reason=arg>(read<@type0>(%2)), real_to_complex<complex<f64>, reason=explicit>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
