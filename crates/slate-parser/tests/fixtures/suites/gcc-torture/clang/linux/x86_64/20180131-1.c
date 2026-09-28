/* PR rtl-optimization/84071 */
/* Reported by Wilco <wilco@gcc.gnu.org> */

extern void abort(void);

typedef union {
  signed short   ss;
  unsigned short us;
  int            x;
} U;

int f(int x, int y, int z, int a, U u) __attribute__((noclone, noinline));

int f(int x, int y, int z, int a, U u) { return (u.ss <= 0) + u.us; }

int main(void) {
  U u = {.ss = -1};

  if (f(0, 0, 0, 0, u) != (1 << sizeof(short) * 8))
    abort();

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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 ss: i16;
// DEFAULT-NEXT:         field1 us: u16;
// DEFAULT-NEXT:         field2 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type1 U = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @f(%9 x: i32, %10 y: i32, %11 z: i32, %12 a: i32, %13 u: @type0) -> i32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(from_bool<i32, reason=promotion>(le<i32>(widen<i32, reason=promotion>(read<i16>(field0(%13))), const<i32>(0))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(%13)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 u: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, i32, i32, @type0) -> i32, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> scalar>(%8, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type0, reason=arg>(read<@type0>(%15))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
